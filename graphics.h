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
#ifdef WIN32
#define __noinit
void UpdateDlgScreen();
void CopyBitmapToClipboard();
void WindowsBlink(bool blink);
void WindowsSwapBuffers();
extern unsigned int LcdData[20];
unsigned int * const AT91C_SLCDC_MEM=LcdData;
void inline Lcd_Enable(int contrast) { }
void inline Lcd_Disable() { }
void UpdateScreenContent();
#else
#define WindowsSwapBuffers()
#define UpdateDlgScreen()
#define UpdateScreenContent()
#endif

enum displaySegment {
  segment_none = 0,  segment_none1,  segment_f1,  segment_a1,  segment_a2,  segment_f2,  segment_b2,  segment_f3,
  segment_a3,  segment_a4,  segment_f4,  segment_b4,  segment_f5,  segment_a5,  segment_a6,  segment_f6,  segment_b6,
  segment_f7,  segment_a7,  segment_a8,  segment_f8,  segment_b8,  segment_f9,  segment_a9,  segment_a10,  segment_f10,
  segment_b10,  segment_none2,  segment_e1,  segment_g1,   segment_b1,  segment_e2,  segment_g2,  segment_e3,  segment_g3,
  segment_b3,  segment_e4,  segment_g4,  segment_e5,  segment_g5,  segment_b5,  segment_e6,  segment_g6,  segment_e7,  segment_g7,
  segment_b7,  segment_e8,  segment_g8,  segment_e9,  segment_g9,  segment_b9,  segment_e10,  segment_g10,  segment_none3,
  segment_d1,  segment_c1,  segment_d2,  segment_c2,  segment_h2, // dot2
  segment_d3,  segment_c3,  segment_d4,    segment_c4,  segment_h4, // dot4
  segment_d5,  segment_c5,  segment_d6,  segment_c6,  segment_h6, // dot6
  segment_d7,  segment_c7,  segment_d8,  segment_c8,  segment_h8, // dot8
  segment_d9,  segment_c9,  segment_d10,  segment_c10,  segment_h10, // dot10
  segment_battery,  segment_minus,  segment_i1, // comma1
  segment_h1, // dot1
  segment_user,  segment_i2, // comma2
  segment_f,  segment_i3, // comma3
  segment_h3, // dot3
  segment_g,  segment_i4, // comma4
  segment_begin,  segment_i5, // comma5
  segment_h5, // dot5
  segment_G,  segment_i6, // comma6
  segment_rad,  segment_i7, // comma7
  segment_h7, // dot7
  segment_d_my,  segment_i8, // comma8
  segment_c,  segment_i9, // comma9
  segment_h9, // dot9  
  segment_prgm,  segment_i10 // comma10
};

void displaySeg(int seg, int enable);
void displayData(char pDD[20]);
