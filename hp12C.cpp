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
#include "hp12C.h"
#include "system.h"
#include "graphics.h"

void SendBinary(u8 code, u32 size, u8 const *d)
{
  u32 t= code+ (size<<8);
  SendChars((u8*)&t, 3, false);
  SendChars(d, size, true); 
}


void CheckCommunication()
{ 
  int c= GetChar();
  if (c==-1) return;
  if (c<40)  // keypress (<40)
  {
    AddKeyInBuffer(c);
#ifdef WIN32
extern HANDLE KeyEvent;
    SetEvent(KeyEvent); 
#endif
  } else if (c=='Q' || c=='q')    // send screen binary: 'Q' 2 bytes size (little endian) + data
    SendBinary(c, 32, (u8*)AT91C_SLCDC_MEM); // code for screen
  else if (c=='s')
  {
    SendBinary('s', 8, (u8*)&BuildDate);
    return;
  }
}

void updatescreen(TCalc * calc)
{
  char buf[30]="                      ";
  if (!calc->ScreenOff)
  {
    i64 r;
    if (calc->entering) r= calc->entry; else r= calc->stack[0];
    if (r<0) buf[0]='-', r=-r;
    for (int i=9; i>=0; i--) 
    { 
      char mod= (char)(r%10);
      buf[i*2]= '0'+mod; 
      r/= 10; 
      if (r==0) break; 
    }
  }
  displayData(buf);
  UpdateDlgScreen();
}

void init(TCalc * calc)
{
  calc->LowPower= false;
  calc->LowPowerCount= 0;
  calc->ScreenOff= false;
  calc->no_ResetMark= 0x12BC67D9FE3954AFLL;
  for (int i=0; i<4; i++) calc->stack[i]= 0LL;
  calc->entering= false;
}

static void push(TCalc *calc, i64 a)
{
  for (int i=2; i>=0; i--) calc->stack[i+1]= calc->stack[i];
  if (a>=10000000000) a= 9999999999;
  if (a<=-10000000000) a= -9999999999;
  calc->stack[0]= a;
}

static void enter(TCalc *calc)
{
  push(calc, calc->stack[0]); 
  if (calc->entering) 
  { 
    calc->stack[0]= calc->entry; 
    calc->entering= false; 
  }
}

static i64 pop(TCalc *calc)
{
  if (calc->entering) enter(calc);
  i64 a= calc->stack[0];
  for (int i=0; i<3; i++) calc->stack[i]= calc->stack[i+1];
  return a;
}

static void addnumber(TCalc *calc, int a)
{
  if (!calc->entering)
  {
    calc->entering= true;
    calc->nbentrychr= 0;
    calc->entry= 0LL;
  }
  if (calc->nbentrychr==10) return;
  calc->entry= calc->entry*10+a;
  calc->nbentrychr++;
}

void execute(TCalc * calc)
{
  int k= KeyBuffGetKey();
  if (k==-1) return;
  i64 a, b;
  switch (k)
  {
  case 6: addnumber(calc, 7); break; // key 7 pressed
  case 7: addnumber(calc, 8); break; // key 8 pressed
  case 8: addnumber(calc, 9); break; // key 9 pressed
  case 16: addnumber(calc, 4); break; // key 4 pressed
  case 17: addnumber(calc, 5); break; // key 5 pressed
  case 18: addnumber(calc, 6); break; // key 6 pressed
  case 26: addnumber(calc, 1); break; // key 1 pressed
  case 27: addnumber(calc, 2); break; // key 2 pressed
  case 28: addnumber(calc, 3); break; // key 3 pressed
  case 36: addnumber(calc, 0); break; // key 0 pressed
  case 25: enter(calc); break;
  case 24: if (calc->entering) calc->entering= false; else calc->stack[0]= 0LL; break; // clx
  case 30: calc->ScreenOff= ! calc->ScreenOff; break; // ON
  case 23: if (calc->entering) enter(calc);  // swap key
           a= calc->stack[0]; calc->stack[0]= calc->stack[1]; calc->stack[1]= a; 
           break;
  case 22: a= calc->stack[0]; // Rdown key
           for (int i=0; i<3; i++) calc->stack[i]= calc->stack[i+1];
           calc->stack[3]= a;
           break;
  case 9: a= pop(calc); b= pop(calc); push(calc, b/a); break; // / key
  case 19: a= pop(calc); b= pop(calc); push(calc, b*a); break; // / key
  case 29: a= pop(calc); b= pop(calc); push(calc, b-a); break; // / key
  case 39: a= pop(calc); b= pop(calc); push(calc, b+a); break; // / key
  }
  updatescreen(calc);
}

