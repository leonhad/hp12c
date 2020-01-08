/*
Copyright (c) 2007 Hewlett-Packard development company llc
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  - Redistributions of source code must retain the above copyright notice,
//  this list of conditions and the disclaimer below.
//
//  - Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the disclaimer below in the documentation and/or
//  other materials provided with the distribution. Unless a specific license is granted
//  to te licencee
//
//  - This software can only be used on a HP hardware. You are not permited to leaverage
//  any part of this software for products/project that do not work on hardware sold by HP
//
//  HP's name may not be used to endorse or promote products derived from
//  this software without specific prior written permission.
//
//  DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY HP "AS IS" AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
//  DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
//  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  ----------------------------------------------------------------------------
file created by Cyrille de Brebisson
*/

#include "board.h"
#include "main.h"
#include "lcd.h"
#include "keyboard.h"
#include "rtc.h"
#include "dbgu.h"
#include "HP12c.h"
#include "graphics.h"
#include "timers.h"
#include "system.h"
#include "autotests.h"
#include <string.h> // memset, sprintf

// add hms times in dcb format with 24 hour modulo
// used to calculate next 5 minute wakup...
static int hmsadd(int h1, int h2)
{
  int r=0;
  // seconds
  r= (h1&0xf) + (h2&0xf);
  if (r >= 0xA) r= r - 0xA + 0x10;
  r+= (h1&0xf0) + (h2&0xf0);
  if (r >= 0x60) r= r - 0x60 + 0x100;
  //minutes
  r+= (h1&0xf00) + (h2&0xf00);
  if (r >= 0xA00) r= r - 0xA00 + 0x1000;
  r+= (h1&0xf000) + (h2&0xf000);
  if (r >= 0x6000) r= r - 0x6000 + 0x10000;
  // hours...
  r+= (h1&0xf0000) + (h2&0xf0000);
  if (r >= 0xA0000) r= r - 0xA0000 + 0x100000;
  r+= (h1&0xf00000) + (h2&0xf00000);
  if (r >= 0x240000) r= r - 0x240000;

  return r;
}

#ifdef debugmessage
// transform a time register in a string that is the time and can be "printed"...
static char *timestr(int t, char *s)
{
  s[2]= s[5]= ':';
  s[8]= 0;
  s[0]= ((t>>20)&0xf)+'0'; s[1]= ((t>>16)&0xf)+'0';
  s[3]= ((t>>12)&0xf)+'0'; s[4]= ((t>>8)&0xf)+'0';
  s[6]= ((t>>4)&0xf)+'0'; s[7]= ((t>>0)&0xf)+'0';
  return s;
}
#endif

// setup RTC timer 5 minutes past now....
// used for auto power off...
// time is given in hms format encoded in hex: 0x12141 is 1 hour, 21 minutes, 41 seconds...
void SetRTCAlarm(unsigned int time)
{
  AT91F_RTC_Close(); // need else you get issues with the open
  AT91F_RTC_Open(); // setup RTC
  // Program RTC Wake Up at 5 minutes...
  Printk("  set alarm RTC\r\n");
  u32 t=AT91F_RTC_Get_Time();

  debugexec(char s[9]; Printk("  time is "); Printk(timestr(t, s)););
  u32 t2= hmsadd(t, time); // add 5 minutes to time... for alarm

  AT91F_RTC_Set_Time_Alarm(t2&0xff, (t2>>8)&0xff, (t2>>16)&0xff, AT91C_RTC_SECEN|AT91C_RTC_MINEN|AT91C_RTC_HOUREN);

  Printk("\r\n  enable wakup RTC at ");
  Printk(timestr(t2, s));
  Printk("\r\n");
}


//************************ low power functions (ie: turn cpu off) **************
// calc in low power mode,
// screen and everything off (except RAM),
// wait for ON key only...
static void TurnCalcOff()
{
  SetCPUSpeed(BaseCpuSpeed);
  AT91C_BASE_AIC->AIC_IDCR= ~0; // disable all interrupts...

  Printk("put calc in OFF mode, wait for FWUP\r\n");
  // turn off LCD (can be needed in case of very low battery)
  Lcd_Disable();

  int i = 0;
  while (i < 1000) { Watchdog(); if ((AT91C_BASE_SUPC->SUPC_SR & (1 << 12)) == 0) i = 0; else i++; }
  
  // stop BOD
  AT91C_BASE_SUPC->SUPC_BOMR = 0;

  // kill all timers
  StopTimer(0);
  StopTimer(1);
  StopTimer(2);

  // put all io lines inputs and pullups... and stops keyboard from scanning...
  Printk("  config PIOC\r\n");
  Printk("  enter off mode\r\n");
//  AT91F_PIO_CfgInput(AT91C_BASE_PIOC, 0xffffffff);
//  AT91F_PIO_CfgPullup(AT91C_BASE_PIOC, 0xffffffff);
  AT91C_BASE_PIOC->PIO_PER= AT91C_BASE_PIOC->PIO_ODR= AT91C_BASE_PIOC->PIO_PPUER= 0xffffffff; // input, pullup no filter
  Wait(1);
  AT91C_BASE_PMC->PMC_PCDR= 1 << AT91C_ID_PIOC; // disable PIOC clock

  // Enable SRAM Backup
  AT91C_BASE_SUPC->SUPC_MR = (AT91C_BASE_SUPC->SUPC_MR)|(0xA5UL << 24)|AT91C_SUPC_SRAMON;

  Printk("  turn off RTC\r\n");
  AT91F_RTC_Close();
  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)&(~AT91C_SUPC_RTCEN); // disable RTC wakup

  Printk("  config Wakup source\r\n");
  // Program Wake-Up Source debouncer
  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)& ~(0x7 <<12);
  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)& ~(0x7 <<8);
  // enable ON key
  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)|AT91C_SUPC_FWUPEN;
  // disable wakup on anything else...
  AT91C_BASE_SUPC->SUPC_WUIR = (AT91C_BASE_SUPC->SUPC_WUIR)&(~0xffff);

  // Enter Backup Mode
  AT91C_BASE_SUPC->SUPC_CR = (0xA5UL << 24)|AT91C_SUPC_VROFF;
  for (;;);
}

// calc in low power mode,
// everything is off except RAM, screen, RTC (for 5 minutes auto power off) and keyboard,
// wait for any key or RTC timer...
static void PutCalcIdle()
{
  AT91C_BASE_AIC->AIC_IDCR= ~0; // disable all interrupts...
  Printk("put calc to sleep, wait for keypress or RTC\r\n");
  // stop BOD
  AT91C_BASE_SUPC->SUPC_BOMR = 0;
  // kill all timers
  StopTimer(0);
  StopTimer(1);
  StopTimer(2);

  // Enable SRAM Backup
  AT91C_BASE_SUPC->SUPC_MR = (AT91C_BASE_SUPC->SUPC_MR)|(0xA5UL << 24)|AT91C_SUPC_SRAMON;

  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)|AT91C_SUPC_RTCEN; // make sure we wakeup with RTC...

  Printk("  config Wakup source\r\n");
  // Program Wake-Up Source debouncer
  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)& ~(0x7 <<12);
  // enable FWUP/ON key
  AT91C_BASE_SUPC->SUPC_WUMR = (AT91C_BASE_SUPC->SUPC_WUMR)|AT91C_SUPC_FWUPEN;
  // enable wakup on high to low on all keys...
  AT91C_BASE_SUPC->SUPC_WUIR = AllKeyboardWU;

  Printk("  enter backup mode\r\n");

  // Enter Backup Mode
  AT91C_BASE_SUPC->SUPC_CR = (0xA5 << 24)|AT91C_SUPC_VROFF;
  for (;;);
}

__no_init bool TestSystem; // if true, CPU speed is ALWAYS at 36Mhz
__no_init int SlowClock; // slow clock speed
__no_init int CpuSpeed;     // current CPU speed
__no_init int BaseCpuSpeed; // internal RC speed, mesured using oscilator
__no_init int RealCpuSpeed; // real speed the CPU is set at, canbe different from the "freq" parametter due to discreete frequency range
void SetCPUSpeed(int freq) // set the cpu frequency, input is in Mhz and should be <30Mhz...
                           // frequency of <= 0 reset base frequency.
                           // the minimum frequency is around 40Khz
{
  if (freq<=0) freq= BaseCpuSpeed;
  if (freq>36000000 || TestSystem) freq= 36000000;
  if (freq==CpuSpeed) return; // if no changes, return...
  int oldRealSpeed= RealCpuSpeed;
  CpuSpeed= freq;
  if (freq>BaseCpuSpeed) // do we need to use the PLL?
  { // yes
    int presc= 0;
    while (freq<18000000) { freq*=2; presc++; }
    if (presc>6) presc= 6;
    freq= freq/SlowClock-1;
    if (freq>2047) freq= 2047; // maxes out at 2047
    RealCpuSpeed= (freq*SlowClock)>>presc;
    // set flash wait state (1 ws if > 15Mhz)
    AT91C_BASE_MC->MC_FMR= AT91C_BASE_MC->MC_FMR&~AT91C_MC_FWS | (RealCpuSpeed>15000000)?AT91C_MC_FWS_1FWS:AT91C_MC_FWS_0FWS;
    debugexec(char s[100]; sprintf(s, "PLL prog: cpu=%d mul=%d prescaler=%d\r\n", RealCpuSpeed, freq, presc); Printk(s););
    // program the PLL
    AT91C_BASE_PMC->PMC_PLLR = (0<<29) | AT91C_CKGR_OUT_2 | AT91C_CKGR_PLLCOUNT |
                               (AT91C_CKGR_MUL & (freq << 16)) | (AT91C_CKGR_DIV & 1);
    // Wait for PLL stabilization
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK) );
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // first, select pre-scaler
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~(7<<2)) | (presc<<2);
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // then Select the Processor clock source as PLL (need to do both operations separately)
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR&~3) | AT91C_PMC_CSS_PLL_CLK ;
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // disable 2Mhz clock
//    AT91C_BASE_PMC->CKGR_MOR=  (AT91C_BASE_PMC->CKGR_MOR&~((1<<24) | (1<<0))) | AT91C_CKGR_KEY;
//    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MAINSELS) == AT91C_PMC_MAINSELS);
  } else if (freq>SlowClock) {
    // enable 2Mhz clock
//    AT91C_BASE_PMC->CKGR_MOR=  (AT91C_BASE_PMC->CKGR_MOR&~(1<<24)) | (1<<0) | AT91C_CKGR_KEY;
//    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MAINSELS) == AT91C_PMC_MAINSELS);
    int presc= 0;
    while (freq<BaseCpuSpeed) { freq*=2; presc++; }
    if (presc>6) presc= 6;
    RealCpuSpeed= BaseCpuSpeed>>presc;
    debugexec(char s[100]; sprintf(s, "2Mhz prog: cpu=%d prescaler=%d\r\n", RealCpuSpeed, presc); Printk(s););

    // Wait until the main clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // first Select the Processor clock source
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR&~0x3) | AT91C_PMC_CSS_MAIN_CLK ;
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // then, select pre-scaler
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~(7<<2)) | (presc<<2);
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = (0<<29) | AT91C_CKGR_OUT_2 | AT91C_CKGR_PLLCOUNT |
                               (AT91C_CKGR_MUL & 0) | (AT91C_CKGR_DIV & 1);
    // set 0 flash wait state
    AT91C_BASE_MC->MC_FMR= AT91C_BASE_MC->MC_FMR&~AT91C_MC_FWS | AT91C_MC_FWS_0FWS;
  } else {
    int presc= 0;
    while (freq<SlowClock) { freq*=2; presc++; }
    if (presc>6) presc= 6;
    RealCpuSpeed= SlowClock>>presc;
    debugexec(char s[100]; sprintf(s, "Slow Clock prog: cpu=%d prescaler=%d\r\n", RealCpuSpeed, presc); Printk(s););

    // first Select the Processor clock source
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR&~0x3) | AT91C_PMC_CSS_SLOW_CLK ;
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // then, select pre-scaler
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~(7<<2)) | (presc<<2);
    // Wait until the master clock is established
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) );
    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = (0<<29) | AT91C_CKGR_OUT_2 | AT91C_CKGR_PLLCOUNT |
                               (AT91C_CKGR_MUL & 0) | (AT91C_CKGR_DIV & 1);
    // disable 2Mhz clock
//    AT91C_BASE_PMC->CKGR_MOR=  (AT91C_BASE_PMC->CKGR_MOR&~((1<<24) | (1<<0))) | AT91C_CKGR_KEY;
//    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MAINSELS) == AT91C_PMC_MAINSELS);
    // set 0 flash wait state
    AT91C_BASE_MC->MC_FMR= AT91C_BASE_MC->MC_FMR&~AT91C_MC_FWS | AT91C_MC_FWS_0FWS;
  }
  // init RS232. used only in debug or test mode (#defined out automaticaly else)
  UpdateRS232();
  debugexec(char s[80]; sprintf(s, "Cpu speed: real:%d required:%d\r\n", RealCpuSpeed, CpuSpeed); Printk(s); );
  ReprogramTimers(oldRealSpeed);
}

/*! \fn TestChecksum()
 * \brief TestChecksum. return the checksum of the code.
 * for this function to work, you need to have the "filled unused code" in the processing tab of the linker options
 * checked and set at 0x00, and select 1 byte arithmetic sum checksum, with no complement, alignement 1, MSB first and initial value of 0x00
 * upno function return, you can either display the checksum or/and test it against the __checksum variable 
 * as selecting the "fill unused code" option forces the linker to generate 128KB of code,
 * during development, I recomend disabled the function in order to avoid wasting lots of time in downloads...
 **/
#ifdef IncludeChecksum
char TestChecksum()
{
    u32 *mem= (u32*)0x100000;
    int crc=0;
    for (int i=131072/4; i!=0; i--)
    {
      u32 t= *mem++;
      crc+= t + (t>>8) + (t>>16) + (t>>24);
    }
    return (crc-__checksum)&0xff;
}
#endif

#ifdef executeAnalysis
// array that records rom "execution" and can be used for coverage analysis...
unsigned int romcoverage[romcoveragesize];
struct sBadCalculationTable badCalcTable[MAX_BAD_CALCULATION];
#endif

// returns current VBat in deci volts...
// only test up to max voltage
int TestVoltage(int min, int max)
{
  min= min-19;
  max= max-19;
  if (max>16) max=16;
  if (min<0) min= 0;
  for (; min<=max; min++)
  {
    AT91C_BASE_SUPC->SUPC_BOMR = AT91C_SUPC_BODSMPL_CONTINUOUS|(min);
    Wait(1/20); // 0.05ms pause...
    if ((AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_BROWNOUT )!=0) // if lower than current BOD value
    {
      AT91C_BASE_SUPC->SUPC_BOMR = 0;  // stop BOD
      return min+19;
    }
  }
  AT91C_BASE_SUPC->SUPC_BOMR = 0;  // stop BOD
  return 35;
}

void LowPower()
{
  // very low battery testing... start BOD, 2.1V detection. 
  int v= TestVoltage(21, 25);
  if (v<=21) // if lower than 2.1V turn calc off
  { 
    Printk("Power < 2.1V, auto off\r\n"); 
    TurnCalcOff(); 
  } else {
     // counts the number of consecutive readings >= 2.4V and <2.4V, if more than 100 in a row, change
     // the low power flag
    if (calc->LowPower)
    {
      if (v>23) calc->LowPowerCount++; else calc->LowPowerCount= 0;
      if (calc->LowPowerCount>100) { calc->LowPower= false; calc->LowPowerCount= 0; }
    } else {
      if (v<=23) calc->LowPowerCount++; else calc->LowPowerCount= 0;
      if (calc->LowPowerCount>100) { calc->LowPower= true; calc->LowPowerCount= 0; }
    }
  }
}

//************************ Main function ***************************************
#ifdef debugmessage
static char const * const MainResetCause[6]= {"Power Up", "Normal WakeUp", "Watchdog", "SW Reset", "NRST pin", "Brown Out"};
#endif

/*! \fn main()
 * \brief Main function
 **/
int main()
{
  
  // calculate CPU speed accuracy is low due to the abscence of xtal, so
  // we are using a constant for the moment...
  SlowClock= 28500;
//#define XTAL // uncomment if you are using an external cristal to handle SlowClock
#ifdef XTAL
  AT91C_BASE_SUPC->SUPC_CR = (0xA5 << 24)|AT91C_SUPC_XTALSEL;
  while ((AT91C_BASE_PMC->PMC_MCFR&(1<<16))==0);
  unsigned int mcfr= AT91C_BASE_PMC->PMC_MCFR;
  BaseCpuSpeed= CpuSpeed= (mcfr&0xffff)*SlowClock/16;
#else
  RealCpuSpeed= BaseCpuSpeed= CpuSpeed= 1900000;
#endif

  // init keyboard and PIOC and timers as needed
  Keyboard_Init();                        

  // print reboot information...
  Printk("Boot: ");
  Printk(MainResetCause[(AT91C_BASE_RSTC->RSTC_RSR>>8)&0x7]);
  Printk("\r\n");

  LowPower();
  
  // reset of calc if needed...
//  SetCPUSpeed(36000000); AT91F_Init_DBGU(RealCpuSpeed, 115200);
  if (((AT91C_BASE_RSTC->RSTC_RSR>>8)&7)==0) 
    calc->no_ResetMark= 0LL; // if master reset pressed, force full logical reset
  if (calc->no_ResetMark!=0x12BC67D9FE3954AFLL) // if the "no reset mark" is valid in the struct, assume that this is a
    init(calc);                                // soft reset or reboot and do not reinitialize the calc...


  // Autotests?
  TestSystem= false;
  if (System.KeyboardMap==((1LL<<30)|(1LL<<32)|(1LL<<25)))  factoryTestMain(calc); // g - key no. 32 and 25 for , 'e', // ENTER
  else if (System.KeyboardMap==((1LL<<30)|(1LL<<32)))  displayLastResetState(calc); // g and ON key
  else if (System.KeyboardMap==((1LL<<30)|(1LL<<31))) // f and ON together start the test system... do a reset to turn it off...
  {
    TestSystem= true;
    SetCPUSpeed(36000000);
    InitRS323(115200);
  }
  
  for (;;) // loop endlessly looking for 1 of 3 events: key in buffer, 5 minute timer or calculator turned off
           // or in test mode for a "virtual" key press (or a real one)...
  {
    while (!KeyBuffEmpty()) //if key in buffer
    { // execute key
      Watchdog();
      int k= KeyBuffPeek();
      int spd= 17000000;                // by default, we use a 17mhz speed, lower than 18Mhz to avoid having to set the flash to 2 cycles...
      if (k<=4 || k==10 || k==11 || k==15 || k==20) spd=36000000; // any TVM key, bond, delta days or program key, we set full speed..
      SetCPUSpeed(spd);     
      execute(calc);                      // execute calcultor emualtor to handle keypress!
      SetCPUSpeed(BaseCpuSpeed);          // reset to internal oscilator 2mhz...
      LowPower();
      SetRTCAlarm(0x000500); // setup 5 minutes timer...
    }
    Watchdog();

    if (TestSystem)
      CheckCommunication();
    else {
      if (AT91F_RTC_AlarmOccured())    // 5 minute timer: auto power off
      {
        Printk("RTC Event! go off\r\n");
        calc->ScreenOff= true; // if 5 minute timer, turn calc off
        updatescreen(calc);
      }
      if (calc->ScreenOff) TurnCalcOff();  // if screen if off, turn calculator off... only wakeup is ON! turn PIO off... note that it does not return from this function.... it restarts at 0 (ie: reboots)
      if (System.KeyboardMap==0LL) PutCalcIdle(); // if no keys are down, idle the calc waiting for keypress or 5 minute timer... note that it does not return from this function.... it restarts at 0 (ie: reboots)
      // go in wait mode, waiting for timer to tick at 60ms...
      Printk("Entering wait mode... ");
  //    SetCPUSpeed(SlowClock);
      // stop CPU clock...
      AT91C_BASE_PMC->PMC_SCDR= 1;
  //    SetCPUSpeed(BaseCpuSpeed);
      Printk("Waking up!\r\n");
    }
  }
}

