/*
Copyright (c) 2007 Hewlett-Packard development company llc
file created by Cyrille de Brebisson
File part of the 12c+ project

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
*/




// define the following if you want the debug messages going through the debug rs232 port...
//#define debugmessage
#ifdef debugmessage
#define INITDBRS232(a, b) AT91F_Init_DBGU(a, b)
#define Printk(a) AT91F_DBGU_Printk(a)
#define debugexec(a) a
#else
#define INITDBRS232(a, b)
#define Printk(a)
#define debugexec(a)
#endif

#ifndef debugmessage
/*! \def TestSystem
    \brief A macro that enables test system

    \detail
// define the following to compile with the test system.
// the test system allows a PC to send keypresses request through RS232 and to receive in return
// the screen data...
// note that testSystem and debugmessage are mutually exclusive...
*/

#endif
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long long u64;
typedef signed long long i64;
typedef unsigned long u32;
typedef long i32;
typedef unsigned short int const r16;

#ifdef TestSystem
  #undef INITDBRS232
  #define INITDBRS232(a, b) AT91F_Init_DBGU(a, b)
#endif

i32 GetChar();
i32 GetChar(u32 timeout);
void SendChar(u8 a);
void SendChars(u8 const *p, u32 size, bool ForseSend);
u32 GetChars(u8 *p, u32 size, u32 timeoutms);

typedef struct
{
  u64 no_ResetMark; //!< this is used to "Detect" if the structure needs to be re-initialized...
  bool LowPower, ScreenOff;
  unsigned char LowPowerCount;
  i64 stack[4], entry;
  bool entering;
  unsigned char nbentrychr;
} TCalc;

/*! \fn main()
 * \brief Main function
 * - main entry point
 * - runs the system
 * - never ending loop
 **/
int main();                   //

/*! \fn execute(TCalc *calc)
 * \brief execute funtion
 * defined in execute1 or execute2.c
 * this is the main entry point for the nut emualtor
 * it returns as soon as the old firmware puts the calcualtor back in @b "standby" mode
 **/
void execute(TCalc *calc);
void updatescreen(TCalc * calc);


/*! \fn wait(unsigned int time)
 * \brief wait - used for small waits
 * \Details
 * loops time times (ie: 4 cycles per number)
**/

#ifdef WIN32
#include "stdafx.h"
#define wait(t)  Sleep(t)
#else
extern "C" void wait(unsigned int time);
#endif

#ifdef WIN32
inline void Watchdog() { }
extern TCalc CALC;
extern TCalc *calc;
#else
#include "board.h"
// restart the watchdog
inline void Watchdog() { AT91C_BASE_WDTC->WDTC_WDCR= 0xA5000001; }
TCalc *const calc= (TCalc*)0x300000;
#endif
void init(TCalc *Calc);

extern unsigned long long const BuildDate;
void CheckCommunication();
