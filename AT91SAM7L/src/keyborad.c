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
#include "keyboard.h"
#include "board.h"
#include "system.h"
#include "timers.h"
#include "dbgu.h"
#include "main.h"
#include "graphics.h"
#include "HP12c.h"

// *****************************************************************************
// keyboard (PIOC) stuff

static void EnableKeyboardInterupt();
static int ScanKeyboard();

static void DisableKeyboardInterupt()
{
  AT91C_BASE_AIC->AIC_IDCR = AT91C_BASE_AIC->AIC_ICCR = 0x1 << AT91C_ID_PIOC; // disable IT
  AT91C_BASE_PIOC->PIO_IDR= AllKeyboardInputs;
}

/*! \fn KeyboardInterupt
 *\brief  Handle keyboard interrupt.
 * acknoledge interrupt whether needed or not.
 * handle keyboard interupt -
 *  -# scan keyboard and add keys in buffer using ScanKeyboard()
 *  -# if keys down, then disable keyboard interrupt and use the timer(EnableTimerInterupt()) to do pooling! every 60ms.
 *  -# if no keys down, then re-enable the keyboard interrupts
 *
 **/

static void TimerInterupt2()
{
  Printk("<Timer 0 interupt>");
  AcknoledgeTimerInterupt(0);

  if (ScanKeyboard())
  {
    Printk("<Stop Timer 1 interupt>");
    StopTimer(0);
    StopTimer(1);
    EnableKeyboardInterupt();
  } else
    EnableTimerInterupt(0, 60, TimerInterupt2);
}

static void KeyboardInterupt()
{
  Printk("<Keyboard Interupt>");
  // acknoledge interrupt if needed (even if not!)
  unsigned int dummy= AT91C_BASE_PIOC->PIO_ISR; dummy= dummy;

  // handle keyboard interupt
  if (!ScanKeyboard()) // scan keyboard and add keys in buffer..
  { // if keys down...
    DisableKeyboardInterupt(); // disable keyboard interrupt as
    EnableTimerInterupt(0, 60, TimerInterupt2);
  } else
    EnableKeyboardInterupt();  // if no keys down, then re-enable the keyboard interrupts...
}

/*! \fn EnableKeyboardInterupt
 *\brief  enables keyboard interrupt.
 * -# configure PIO C interrupt handler
 * -# enable interupt generation on all inputs from the keyboard
 * -# set all rows to 0
 * -# enable PIOC interrupts in the AIC controler
 **/
static void EnableKeyboardInterupt()
{
  //* configure PIO C interrupt handler
   // Disable the interrupt on the interrupt controller
   AT91C_BASE_AIC->AIC_IDCR = 0x1 <<AT91C_ID_PIOC ;
   // Save the interrupt handler routine pointer and the interrupt priority
   AT91C_BASE_AIC->AIC_SVR[AT91C_ID_PIOC] = (unsigned int) KeyboardInterupt ;
   // Store the Source Mode Register
   AT91C_BASE_AIC->AIC_SMR[AT91C_ID_PIOC] = AT91C_AIC_SRCTYPE_EXT_LOW_LEVEL | 1  ;
   // Clear the interrupt on the interrupt controller
   AT91C_BASE_AIC->AIC_ICCR = 0x1 << AT91C_ID_PIOC;

  // enable interupt generation on all inputs from the keyboard. disable on the rest...
  AT91C_BASE_PIOC->PIO_IDR= ~AllKeyboardInputs; //InterruptDisable
  AT91C_BASE_PIOC->PIO_IER= AllKeyboardInputs; //Interup enable
  // set all rows to 0
  AT91C_BASE_PIOC->PIO_CODR= AllKeyboardRows; // set output to 0
  // enable PIOC interrupts in the AIC controler
  AT91C_BASE_AIC->AIC_IECR = 0x1 << AT91C_ID_PIOC; // enable IT
}

void Keyboard_Init()
{
  // init keyboard buffer
  initKeyBuffer();
  // Enable PIOC Clocking this allows to use inputs!
  AT91C_BASE_PMC->PMC_PCER= (unsigned int) 1 << AT91C_ID_PIOC; // enable PIOC clock
  // Disable keybaord rows Pull-up enable pullup on all the other inputs...
  AT91C_BASE_PIOC->PIO_PPUDR= (AllKeyboardRows);
  AT91C_BASE_PIOC->PIO_PPUER= ~(AllKeyboardRows);
  // Configure columns as Inputs
  AT91C_BASE_PIOC->PIO_PER= AT91C_BASE_PIOC->PIO_ODR= AllKeyboardInputs; // set as inputs
  // set rows state as 0 (before the config as output to avoid glitches)
  AT91C_BASE_PIOC->PIO_CODR= AllKeyboardRows; // set outputs to 0
  // Configure rows as Output
  AT91C_BASE_PIOC->PIO_PER= AT91C_BASE_PIOC->PIO_OER=  AllKeyboardRows; // set as outputs
  KeyboardInterupt(); // if there is a key press, handle it, else setup keyboard interupt...
}

// called after x ms (max bounce time) to disable bounce checking on the last key pressed
static void ResetLastKeyPress()
{
  Printk("<Timer 1 interupt>");
  AcknoledgeTimerInterupt(1);
  System.LastKeyPress= -1;
  StopTimer(1);
}

/*! \fn ScanKeyboard
 *\brief  scans the keyboard
 * -#  update the key map
 * -#  update LastKeyPress if a new key was pressed
 * -#  debounce stuff
 * -#  and place stuff in the key buffer if it's a new key press..
 * -#  a key consumer program should call GetKey() to read a key from the buffer
 * -#  return true if no key is pressed..
 **/
static int ScanKeyboard()
{
  unsigned long long NewKeyMap= ~0LL;

  // read the 4 lines of the keyboard and create a 64 bit structure
  // 10 bit per line to hold the keyup/down information
  // 1 bit per key...
  AT91C_BASE_PIOC->PIO_ODR= AllKeyboardRows; // set as inputs
  unsigned int t;
  for (int line=3; line>=0; line--)
  {
    AT91C_BASE_PIOC->PIO_OER=  1<<(line+26); // set as output
    wait(100);
    t= AT91C_BASE_PIOC->PIO_PDSR; // aquire the keys...
    // column order
    // Original   C1, C0, C9, X1, X2, X2, X3, C8, C7, C6, C5, C4, C3, C2
    // rearrange  X1, X1, X1, X1, C9, C8, C7, C6, C5, C4, C3, C2, C1, C0
    // 
    t=  (((t&0x7f)<<2)) | (((t&0x800)>>2)) | ((t&0x3000) >> 12); // pack the keys (bit 0-6 and 11-13) are used as they map on the WKUP pins...
    NewKeyMap= (NewKeyMap<<10)+(unsigned long long)t;
    AT91C_BASE_PIOC->PIO_ODR= 1<<(line+26); // back to input
  }
  AT91C_BASE_PIOC->PIO_OER=  AllKeyboardRows; // all outputs...
  // invert the result as we are working with inverted logic
  NewKeyMap= ~NewKeyMap;
//  printkeymask(NewKeyMap);
  if ((AT91C_BASE_PIOC->PIO_PDSR&(1<<10))==0) NewKeyMap|= OnKeyInKeyMap; // ON Key

  // new key press is the list of the keys that are pressed now, but were not
  // pressed last time...
  unsigned long long NewKeyPress= NewKeyMap & ~System.KeyboardMap;

  // key released are the keys that are not pressed now, but were presssed last time!
  // in this case, we are not using that information because the software
  // does not do anything on key release... so it's just commented out
  // unsigned long long KeyReleased= KeyboardMap & ~NewKeyMap;

  // save the current key map as the key map
  System.KeyboardMap= NewKeyMap;
//  char buf[]= "KEYMAP 0123456789\r\n";
//  for (int i=0; i<10; i++) buf[7+i]= "0123456789ABCDEF"[(System.KeyboardMap>>((9-i)*4))&0xf];
//    AT91F_DBGU_Printk(buf);
  
  // handle the case of the ON key. if the ON key is down, handle it first and then look at other keys...
  if ((NewKeyPress&OnKeyInKeyMap)!=0LL)
  {
    AddKeyInBuffer(30); // add on key in buffer
    NewKeyPress= NewKeyPress&~OnKeyInKeyMap; // remove on key from list of new key pressed...
  }

  // is there a new key down?
  if (NewKeyPress==0LL)
    return System.KeyboardMap==0LL; // no, just return true if no key is down at all...
  
  // isolate the first new key down..
  for (int r=0; r<40; r+=10)
    for (int c=0; c<10; c++)
    {
      if (NewKeyPress&1!=0) // is that key down?
      {
        if (NewKeyPress>>1==0) // if this is not the only new key down, chances are this is a ghost effect
                               // or the user is mighty good at pressing 2 keys exactly at the same time (give or take
                               // timer delay...) in all cases, if this is a double press, then we can not know which key was pressed first
                               // and should ignore it, and if it's a ghost, we can not know which key is actually pressed and
                               // should also ignore the key press...
        { // ok, so, this is a valid key press... but is it a bounce?
          int key= r+c;
          if (System.LastKeyPress!=key) // is it a bounce? if yes, ignore key...
          {
            System.LastKeyPress= key; // not a bounce, save the key for alter bounce detection!
            AddKeyInBuffer(key);      // send key to the system
            Printk("<Setup Timer 1 interupt>");
            EnableTimerInterupt(1, 50, ResetLastKeyPress); // program timer for end of bounce time detection
          }
        }
        return 0;  // return, and there is at least one key down...
      }
      NewKeyPress>>=1; // next key.. this is of course inefficient, but hey... good enough for now... and on an arm it's not too bad anyway...
    }
  return 0; // put here to prevent a warning, but can not happen...
}

