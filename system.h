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
typedef unsigned long long int u64;
typedef unsigned long u32;
typedef signed long i32;
typedef unsigned char u8;
typedef signed char i8;
typedef int Boolean;
typedef unsigned short int const r16;

extern int CpuSpeed;   // contains the current CPU speed in hz
extern int RealCpuSpeed; // real speed the CPU is set at, canbe different from the "freq" parametter due to discreete frequency range
extern int BaseCpuSpeed; // internal RC speed, mesured using oscilator
void SetCPUSpeed(int freq); // set the cpu frequency, input is in Mhz and should be <30Mhz...

typedef enum { //TODO
  KNKeyOn= 71
} TKeyNumbers;

#define MAXKEYBUFSIZE 16              // keyboard buffer

typedef struct {
  volatile u64 KeyboardMap;
  volatile signed char LastKeyPress;
  volatile char ONPlusTask;
  volatile char BufRead;
  volatile char BufWrite;
  volatile char KeyBuffer[MAXKEYBUFSIZE];
  int RS232Speed;
} TSystem;

// mask for keys in the keyboard map
u64 const Key11InKeyMap= 1LL<<0;
u64 const Key12InKeyMap= 1LL<<1;
u64 const Key13InKeyMap= 1LL<<2;
u64 const Key14InKeyMap= 1LL<<3;
u64 const Key16InKeyMap= 1LL<<5;
u64 const KeyMinusInBuffer= 1LL<<(5*6+4);
u64 const KeyPlusInBuffer= 1LL<<(6*6+4);
u64 const OnKeyInKeyMap= 1LL<<30;
// returns the bit in eyboard map that correspond to a key code...
u64 KeyCodeToKeyMap(int key);

extern TSystem System;
int KeyBuffGetKey();         //!<  returns -1 if there is no keys in the buffer
int KeyBuffPeek();           //!<  returns -1 if there is no keys in the buffer, else return the key, but does not remove it from buffer
int KeyBuffEmpty();          //!< returns true/false if there is a key in the buffer or not...
/*! \fn void KeyboardBufferClear()
    \brief remove all keys from keyboard buffer
*/
void KeyboardBufferClear();
/*! \fn void AddKeyInBuffer(int k)
    \brief puts a key in the keyboard buffer.

     Can be used to "simulate" a key press.
    \param k specifies the key to be added to the keyboard buffer
*/
void AddKeyInBuffer(int k);

void initKeyBuffer();

#ifdef _ARM_
#define IncludeChecksum
#endif
#ifdef IncludeChecksum
extern "C" char const __checksum;
char TestChecksum();
#else
#define __checksum 0
#define TestChecksum() 0
#endif

// return the current VBAT voltage in 1/10 of volts (34 -> 3.4V)
// perform testing for voltages between min and max. mornally, voltage bellow 2.1V should never hapen
// as the calculator should turn off as soon as you try to turn it on/
// voltages above 3V should never hapen also as lithum battery are 3v.
// mesures can be taken between 1.9 and 3.4V...
int TestVoltage(int min, int max);

// Wait wait for n mili seconds...
#ifdef _ARM_
#define Wait(a) wait(RealCpuSpeed*a/1024/4)
#else
#undef Wait
#define Wait(a) Sleep(a)
#endif
void UpdateRS232();
void InitRS323(int speed);
