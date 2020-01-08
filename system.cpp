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

#include "system.h"

// return true if the buffer is empty
int KeyBuffEmpty()
{
  char r= System.BufRead;
  char w= System.BufWrite;
  return r==w;
}

// reset the keyboard buffer
void KeyboardBufferClear()
{
  System.BufRead= System.BufWrite= 0;
}

// return a key from the key buffer. return -1 if buffer empty. does not remove the key from the buffer...
int KeyBuffPeek()
{
  if (KeyBuffEmpty()) return -1;
  return System.KeyBuffer[System.BufRead];
}

// return a key from the key buffer. return -1 if buffer empty
int KeyBuffGetKey()
{
  char r= System.BufRead;
  if (r==System.BufWrite) return -1;
  char key= System.KeyBuffer[r++];
  if (r==MAXKEYBUFSIZE) r=0;
  System.BufRead= r;
  return key;
}

// *****************************************************************************
// Keyboard buffer management

void AddKeyInBuffer(int k)
{
  char r= System.BufRead;
  char w= System.BufWrite;
  if (w!=r-1 && (w!=MAXKEYBUFSIZE || r!=0)) // if space in buffer
  {
    System.KeyBuffer[w]= k;                       // place key in buffer
    int t= w+1; if (t==MAXKEYBUFSIZE) t=0; // update write pointer
    System.BufWrite= t;
  }
}

void InitRS323(int speed)
{
  System.RS232Speed= speed;
  UpdateRS232();
}

#ifdef _ARM_
#include "board.h"
#include "dbgu.h"
__no_init TSystem System;
void UpdateRS232()
{
  if (System.RS232Speed==0)
  { //TODO: kill RS232...
  } else
    AT91F_Init_DBGU(RealCpuSpeed, System.RS232Speed);
}

i32 GetChar()
{
  return ((AT91C_BASE_DBGU->DBGU_CSR&1)!=0)?(int)AT91C_BASE_DBGU->DBGU_RHR&0xff:-1;
}

i32 GetChar(u32 timeout)
{
  timeout= (CpuSpeed/1024/32)*timeout;
  int r;
  while (timeout-->0)
  {
    r= GetChar();
    if (r!=-1) return r;  
  }
  return -1;
}

void SendChar(u8 a)
{
  while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
  AT91C_BASE_DBGU->DBGU_THR = a;
}
void SendChars(u8 const *p, u32 size, bool ForseSend)
{
  while (size--) SendChar(*p++);
}
u32 GetChars(u8 *p, u32 size, u32 timeoutms)
{
  u32 r=0;
  int c;
  while (((c=GetChar(timeoutms))!=-1) && size--) { *p++= c; r++; }
  return r;
}

#else
void UpdateRS232() { }
TSystem System;
void SetCPUSpeed(int freq)
{
}
int BaseCpuSpeed= 0;
#endif

void initKeyBuffer()
{
  // init variables
  System.KeyboardMap= 0LL;
  System.BufRead= System.BufWrite= 0;
  System.LastKeyPress= -1;
  System.ONPlusTask= 0;
  System.RS232Speed= 0;
}

unsigned long long KeyCodeToKeyMap(int key)
{
  if ((key<0)||(key>39)) return 0LL;
  return 1LL<<key;
}

