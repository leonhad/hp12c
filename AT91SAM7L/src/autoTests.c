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
file NOT created by Cyrille de Brebisson! I Take NO responsability for that ugly "code"!
File part of the 12c+ project
*/
#include "system.h"
#include "main.h"
#include "board.h"
#include "lcd.h"
#include "keyboard.h"
#include "board.h"
#include "hp12c.h"
#include "autoTests.h"
#include "graphics.h"


#define VERSION_INFO "2 0 0 8 - 0 1 - 2 8 " // TODO: get from build.h

  // display copyright - " C 2008 hP" 
#define COPYRIGHT_INFO "  C   2 0 0 8   h p "

// transform an int in a string in hex format...
char *inttohexstr(int i, char *s, int j)
{
//  for (int j= 7; j>=0; j--)
  for (; j>=0; j--)
  {
    if (((i>>(j<<2))&0xf)>=0xa) *s++= (((i>>(j<<2))&0xf)-0xa)+'a'; else *s++= ((i>>(j<<2))&0xf)+'0';
  }
  return s;
}


static void keyDisplayTest(TCalc *c)
{
  unsigned long long allOutKeys = 0;
  Lcd_Enable(0);
  c->ScreenOff= false;

  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
//  while (KeyboardMap == 0LL);

    // LCD Display - All the status indicators should turn on (Full display on)
    AT91C_SLCDC_MEM[0]= 0xffffffff;
    AT91C_SLCDC_MEM[1]= 0x0;
    AT91C_SLCDC_MEM[2]= 0xffffffff;
    AT91C_SLCDC_MEM[3]= 0x0;
    AT91C_SLCDC_MEM[4]= 0xffffffff;
    AT91C_SLCDC_MEM[5]= 0x0;
    AT91C_SLCDC_MEM[6]= 0xffffffff;
    AT91C_SLCDC_MEM[7]= 0x0;
    wait(200000);
    
 do
  {
    
    while (KeyBuffEmpty()) Watchdog();
    u32 k= KeyBuffGetKey(); 
    
     allOutKeys  |=  (1LL << k);
    
    switch (k)
    {
    case 0: // 'n', remove f1(c1,s2), a1 (c1,s3)
    displaySeg(segment_f1, 0);
    displaySeg(segment_a1, 0);
      break;
    case 1: // 'i', remove b1, f2, a2 
    displaySeg(segment_b1, 0);
    displaySeg(segment_f2, 0);
    displaySeg(segment_a2, 0);
      break;
    case 2: // 'PV', remove b2, f3, a3
    displaySeg(segment_b2, 0);
    displaySeg(segment_f3, 0);
    displaySeg(segment_a3, 0);
      break;
    case 3: // 'PMT', remove b3, f4, a4
    displaySeg(segment_b3, 0);
    displaySeg(segment_f4, 0);
    displaySeg(segment_a4, 0);
      break;
    case 4: // 'FV', remove b4, f5, a5
    displaySeg(segment_b4, 0);
    displaySeg(segment_f5, 0);
    displaySeg(segment_a5, 0);
      break;
    case 5: // 'CHS', remove b5, f6, a6
    displaySeg(segment_b5, 0);
    displaySeg(segment_f6, 0);
    displaySeg(segment_a6, 0);
      break;
    case 6: // '7', remove b6, f7, a7
    displaySeg(segment_b6, 0);
    displaySeg(segment_f7, 0);
    displaySeg(segment_a7, 0);
      break;
    case 7: // '8', remove b7, f8, a8
    displaySeg(segment_b7, 0);
    displaySeg(segment_f8, 0);
    displaySeg(segment_a8, 0);
      break;
    case 8: // '9', remove b8, f9, a9
    displaySeg(segment_b8, 0);
    displaySeg(segment_f9, 0);
    displaySeg(segment_a9, 0);
      break;
    case 9: // '/', remove b9, f10, a10
    displaySeg(segment_b9, 0);
    displaySeg(segment_f10, 0);
    displaySeg(segment_a10, 0);
      break;
    case 10: // 'Y power x', remove '-', e1, g1
    displaySeg(segment_minus, 0);
    displaySeg(segment_e1, 0);
    displaySeg(segment_g1, 0);
      break;
    case 11: // '1/X', remove e2, g2
    displaySeg(segment_e2, 0);
    displaySeg(segment_g2, 0);
      break;
    case 12: // '%T', remove e3, g3
    displaySeg(segment_e3, 0);
    displaySeg(segment_g3, 0);
      break;
    case 13: // 'Delta %', remove e4, g4
    displaySeg(segment_e4, 0);
    displaySeg(segment_g4, 0);
      break;
    case 14: // '%', remove e5, g5
    displaySeg(segment_e5, 0);
    displaySeg(segment_g5, 0);
      break;
    case 15: // 'EEX', remove e6, g6
    displaySeg(segment_e6, 0);
    displaySeg(segment_g6, 0);
      break;
    case 16: // '4', remove e7, g7
    displaySeg(segment_e7, 0);
    displaySeg(segment_g7, 0);
      break;
    case 17: // '5', remove e8, g8
    displaySeg(segment_e8, 0);
    displaySeg(segment_g8, 0);
      break;
    case 18: // '6', remove e9, g9
    displaySeg(segment_e9, 0);
    displaySeg(segment_g9, 0);
      break;
    case 19: // 'x', remove e10, g10
    displaySeg(segment_e10, 0);
    displaySeg(segment_g10, 0);
      break;
    case 20: // 'R/S', remove d1, c1, h1, i1
    displaySeg(segment_d1, 0);
    displaySeg(segment_c1, 0);
    displaySeg(segment_h1, 0);
    displaySeg(segment_i1, 0);
      break;
    case 21: // 'SST', remove d2, c2, h2, i2
    displaySeg(segment_d2, 0);
    displaySeg(segment_c2, 0);
    displaySeg(segment_h2, 0);
    displaySeg(segment_i2, 0);
      break;
    case 22: // 'RDown', remove d3, c3, h3, i3
    displaySeg(segment_d3, 0);
    displaySeg(segment_c3, 0);
    displaySeg(segment_h3, 0);
    displaySeg(segment_i3, 0);
      break;
    case 23: // 'X<>y', remove d4, c4, h4, i4
    displaySeg(segment_d4, 0);
    displaySeg(segment_c4, 0);
    displaySeg(segment_h4, 0);
    displaySeg(segment_i4, 0);
      break;
    case 24: // 'CLx', remove d5, c5, h5, i5
    displaySeg(segment_d5, 0);
    displaySeg(segment_c5, 0);
    displaySeg(segment_h5, 0);
    displaySeg(segment_i5, 0);
      break;
    case 25: // 'ENTER', remove d6, c6, h6, i6
    displaySeg(segment_d6, 0);
    displaySeg(segment_c6, 0);
    displaySeg(segment_h6, 0);
    displaySeg(segment_i6, 0);
    allOutKeys  |=  (1LL << 35); // also covers 35 (for enter)
      break;
    case 26: // '1', remove d7, c7, h7, i7
    displaySeg(segment_d7, 0);
    displaySeg(segment_c7, 0);
    displaySeg(segment_h7, 0);
    displaySeg(segment_i7, 0);
      break;
    case 27: // '2', remove d8, c8, h8, i8
    displaySeg(segment_d8, 0);
    displaySeg(segment_c8, 0);
    displaySeg(segment_h8, 0);
    displaySeg(segment_i8, 0);
      break;
    case 28: // '3', remove d9, c9, h9, i9
    displaySeg(segment_d9, 0);
    displaySeg(segment_c9, 0);
    displaySeg(segment_h9, 0);
    displaySeg(segment_i9, 0);
      break;
    case 29: // '-', remove d10, c10, h10, i10
    displaySeg(segment_d10, 0);
    displaySeg(segment_c10, 0);
    displaySeg(segment_h10, 0);
    displaySeg(segment_i10, 0);
      break;
    case 30: // 'ON', remove b10, battery (*)
    displaySeg(segment_b10, 0);
    displaySeg(segment_battery, 0);
      break;
    case 31: // 'f', remove user
    displaySeg(segment_user, 0);
      break;
    case 32: // 'g', remove f
    displaySeg(segment_f, 0);
      break;
    case 33: // 'STO', remove g, begin
    displaySeg(segment_g, 0);
    displaySeg(segment_begin, 0);
      break;
    case 34: // 'RCL', remove G
    displaySeg(segment_G, 0);
      break;
    case 35: // Not used
      break;
    case 36: // '0', remove RAD
    displaySeg(segment_rad, 0);
      break;
    case 37: // '.', remove D.MY
    displaySeg(segment_d_my, 0);
      break;
    case 38: // 'SIG +', remove C 
    displaySeg(segment_c, 0);
      break;
    case 39: // '+', remove prgm
    displaySeg(segment_prgm, 0);
      break;
    default:
      ;
   }
  } while ( allOutKeys !=  0xFFFFFFFFFF);
 
  while (System.KeyboardMap!=0LL); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
  while (System.KeyboardMap == 0LL) Watchdog();

}


static void displayCheckSum(TCalc *c, int checksum){
  Lcd_Enable(0);
  c->ScreenOff= false;
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released

  char chSum;
  char stringChSum[21] = "C h E - - - -     h ", st[9];
  chSum = TestChecksum();
  inttohexstr(chSum, st, 7);
  stringChSum[10] = st[4];
  stringChSum[12] = st[5];
  stringChSum[14] = st[6];
  stringChSum[16] = st[7];
  displayData(stringChSum);
  
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
  while (System.KeyboardMap == 0LL) Watchdog();
  
  // display version info/Build Date UEr 02 or 2007-12-20
  displayData(VERSION_INFO);
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
  while (System.KeyboardMap == 0LL) Watchdog();
  
  // display copyright - " C 2007 hP" 
  displayData(COPYRIGHT_INFO);
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
  while (System.KeyboardMap == 0LL) Watchdog();
}

static void displayTest(TCalc *c)
{
  unsigned int s=0;  // key no. 22 -> RDown
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  Lcd_Enable(0);
  c->ScreenOff= false;
  
  
  do
  {
    // LCD Display - All the status indicators should turn on (Full display on)
    AT91C_SLCDC_MEM[0]= 0xffffffff;
    AT91C_SLCDC_MEM[1]= 0x0;
    AT91C_SLCDC_MEM[2]= 0xffffffff;
    AT91C_SLCDC_MEM[3]= 0x0;
    AT91C_SLCDC_MEM[4]= 0xffffffff;
    AT91C_SLCDC_MEM[5]= 0x0;
    AT91C_SLCDC_MEM[6]= 0xffffffff;
    AT91C_SLCDC_MEM[7]= 0x0;
    wait(200000);
    
    while (System.KeyboardMap == 0LL) Watchdog();
    if (System.KeyboardMap != (1LL<<22))
    {
    return;
    }

    // LCD Display - odd segment number display
    s=0;
    for(int i=1; i< 26; i+=2)
    s |= 1 << i;
    
    AT91C_SLCDC_MEM[0]= s;
    AT91C_SLCDC_MEM[2]= s;
    AT91C_SLCDC_MEM[4]= s;
    AT91C_SLCDC_MEM[6]= s;

    wait(200000);
    while (System.KeyboardMap == 0LL) Watchdog();
    if (System.KeyboardMap != (1LL<<22))
    {
    return;
    }

    // LCD Display - even segment number display
    s=0;
    for(int i=0; i< 26; i+=2)
    s |= 1 << i;
    
    AT91C_SLCDC_MEM[0]= s;
    AT91C_SLCDC_MEM[2]= s;
    AT91C_SLCDC_MEM[4]= s;
    AT91C_SLCDC_MEM[6]= s;

    wait(200000);
    while (System.KeyboardMap == 0LL) Watchdog();
    if (System.KeyboardMap != (1LL<<22))
    {
    return;
    }

    
  } while ( (System.KeyboardMap==0LL )|| (System.KeyboardMap == (1LL<<22)));
  
  KeyboardBufferClear();
}

void TestKeyboard(TCalc *Calc)
{
  Printk("Start keyboard test\r\n");
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  Lcd_Enable(0);
  Calc->ScreenOff= false;
  for (int i=0; i<40; i++)
  {
    for (int j=0; j<7; j++) AT91C_SLCDC_MEM[j*2]= 0x15<<(i*2-j*26);
    while (KeyBuffEmpty()) Watchdog();
    u32 k= KeyBuffGetKey(); if (i==35 && k==25) k=35; // special treatement for ENTER key...
    if (k==i) continue;
    displayData("  E R R o R   9     "); // R -> to display 'r' on lower segment
    KeyboardBufferClear();
    return;
  }
  AT91C_SLCDC_MEM[0]= 1<<6 | 1<<5;
  AT91C_SLCDC_MEM[1]= 0x0;
  AT91C_SLCDC_MEM[2]= 1<<6 | 1<<5 | 1<<20;
  AT91C_SLCDC_MEM[3]= 0x0;
  AT91C_SLCDC_MEM[4]= 1<<7 | 1<<6;
  AT91C_SLCDC_MEM[5]= 0x0;
  AT91C_SLCDC_MEM[6]= 0x0;
  AT91C_SLCDC_MEM[7]= 0x0;
}
void factoryTestMain(TCalc *c){
  
  Lcd_Enable(0);
  c->ScreenOff= false;
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released

  do {
  // display 1.L2.C3.H (all 10 digits)
  displayData("1.L 2.C 3.H         ");
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
  while (System.KeyboardMap == 0LL) Watchdog();
  
  switch(System.KeyboardMap) 
  {
  case (1LL<<26): // key '1'
    displayTest(c); 
    // set screen to 0...
    AT91C_SLCDC_MEM[0]= AT91C_SLCDC_MEM[1]= AT91C_SLCDC_MEM[2]= AT91C_SLCDC_MEM[3]= AT91C_SLCDC_MEM[4]= AT91C_SLCDC_MEM[5]= AT91C_SLCDC_MEM[6]= AT91C_SLCDC_MEM[7]= 0;
    break;

  case (1LL<<27): // key '2'
    displayCheckSum(c, 1234);
    break;

  case (1LL<<28): // key '3'
    keyDisplayTest(c);
    break;

  case (1LL<<30): // key ON/OFF
//    KeyboardBufferClear();
    // set screen to 0...
    AT91C_SLCDC_MEM[0]= AT91C_SLCDC_MEM[1]= AT91C_SLCDC_MEM[2]= AT91C_SLCDC_MEM[3]= AT91C_SLCDC_MEM[4]= AT91C_SLCDC_MEM[5]= AT91C_SLCDC_MEM[6]= AT91C_SLCDC_MEM[7]= 0;
    return;
    
  default:
//    return;
    ;
  }
  
  }while (1);
  
}

void TestAll(TCalc *c, int stop)
{
  Printk("Start all tests\r\n");
  Lcd_Enable(0);
  c->ScreenOff= false;
  while (System.KeyboardMap!=0LL); // wait until all key released
  do
  {
    displayData("  r u n n i n g     ");
    wait(100000);

    AT91C_SLCDC_MEM[0]= AT91C_SLCDC_MEM[1]= AT91C_SLCDC_MEM[2]= AT91C_SLCDC_MEM[3]= AT91C_SLCDC_MEM[4]= AT91C_SLCDC_MEM[5]= AT91C_SLCDC_MEM[6]= AT91C_SLCDC_MEM[7]= 0;
    wait(250000);
    Watchdog();
  } while (stop && System.KeyboardMap==0LL);
  
  KeyboardBufferClear();
}

void displayLastResetState(TCalc *c){
  
  Lcd_Enable(0);
  c->ScreenOff= false;
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  
  char resetType;
  char stringresetType[21] = "R E - - - - - -   h ", st[9];
  
//  #define AT91C_RSTC_RSR  (AT91_CAST(AT91_REG *) 	0xFFFFFD04) // (RSTC) Reset Status Register
//  resetType = *AT91C_RSTC_RSR; -- for Testing
/*
  RSTTYP

rE------Xh

X holds a value between 0 to 5, which indicates one of the following state

POWERUP       0  Power-up Reset. VDDCORE rising.
WAKEUP        1  (RSTC) WakeUp Reset. VDDCORE rising.
Watchdog      2  (RSTC) Watchdog Reset. Watchdog overflow occured.
SOFTWARE      3  (RSTC) Software Reset. Processor reset required by the software.
USER          4  (RSTC) User Reset. NRST pin detected low.
BROWNOUT      5  (RSTC) Brownout Reset occured.
*/
  resetType = ((*AT91C_RSTC_RSR) & 0x00000700) >> 8; 
  
  inttohexstr(resetType, st, 7);
  
//  stringresetType[10] = st[4];
//  stringresetType[12] = st[5];
//  stringresetType[14] = st[6];
  stringresetType[16] = st[7];
  displayData(stringresetType);
  
  while (System.KeyboardMap!=0LL) Watchdog(); // wait until all key released
  KeyboardBufferClear();
  wait(200000);
  while (System.KeyboardMap == 0LL) Watchdog(); // wait for any key press to Exit
  
  
}
