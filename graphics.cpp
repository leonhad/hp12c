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

#include "graphics.h"
#ifdef _ARM_
#include "board.h"
#endif

struct seg7table {
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
  int h;
  int i;
};

/**
 *    -a-
 *  f|   |b
 *    -g-
 *  e|   |c
 *    -d-  .h
 *         ,i
 */
struct seg7table const seg7[10] = {  
  {segment_a1,segment_b1, segment_c1, segment_d1, segment_e1, segment_f1, segment_g1, segment_h1, segment_i1},
  {segment_a2,segment_b2, segment_c2, segment_d2, segment_e2, segment_f2, segment_g2, segment_h2, segment_i2},
  {segment_a3,segment_b3, segment_c3, segment_d3, segment_e3, segment_f3, segment_g3, segment_h3, segment_i3},
  {segment_a4,segment_b4, segment_c4, segment_d4, segment_e4, segment_f4, segment_g4, segment_h4, segment_i4},
  {segment_a5,segment_b5, segment_c5, segment_d5, segment_e5, segment_f5, segment_g5, segment_h5, segment_i5},
  {segment_a6,segment_b6, segment_c6, segment_d6, segment_e6, segment_f6, segment_g6, segment_h6, segment_i6},
  {segment_a7,segment_b7, segment_c7, segment_d7, segment_e7, segment_f7, segment_g7, segment_h7, segment_i7},
  {segment_a8,segment_b8, segment_c8, segment_d8, segment_e8, segment_f8, segment_g8, segment_h8, segment_i8},
  {segment_a9,segment_b9, segment_c9, segment_d9, segment_e9, segment_f9, segment_g9, segment_h9, segment_i9},
  {segment_a10,segment_b10, segment_c10, segment_d10, segment_e10, segment_f10, segment_g10, segment_h10, segment_i10}
};

void displayTable(int pos, char ch) {

    switch (ch) {
    case 'A':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'b':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'c':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'C':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 0);
      break;

    case 'd':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'e':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'f':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'h':

      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'H':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'E':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'i':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 0);
      break;

    case 'L':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 0);
      break;

    case 'n':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 0);
      break;
      
    case 'o':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;


    case 'R': // R -> to display 'r' on lower segment
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;
      
    case 'r':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 0);
      break;

    case 'u':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case '-':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case 'p':
    case 'P':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case '.':
      displaySeg(seg7[pos/2].h, 1);
      break;

    case ',':
      displaySeg(seg7[pos/2].i, 1);
      break;

    case '1':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 0);
      break;

    case '2':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;
      
    case '3':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 1);
      break;

    case '4':
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;
      
    case '5':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;
    case '6':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;
      
    case '7':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 0);
      break;
      
    case '8':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;
      
    case '9':
    case 'g':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 1);
      break;
      
    case '0':
      displaySeg(seg7[pos/2].a, 1);
      displaySeg(seg7[pos/2].b, 1);
      displaySeg(seg7[pos/2].c, 1);
      displaySeg(seg7[pos/2].d, 1);
      displaySeg(seg7[pos/2].e, 1);
      displaySeg(seg7[pos/2].f, 1);
      displaySeg(seg7[pos/2].g, 0);
      break;
      
    case ' ': // Blank and default case to turn off all the 7 segments.
    default:
      
      if (pos%2) {
      displaySeg(seg7[pos/2].h, 0);
      displaySeg(seg7[pos/2].i, 0);
      }
      else {
      displaySeg(seg7[pos/2].a, 0);
      displaySeg(seg7[pos/2].b, 0);
      displaySeg(seg7[pos/2].c, 0);
      displaySeg(seg7[pos/2].d, 0);
      displaySeg(seg7[pos/2].e, 0);
      displaySeg(seg7[pos/2].f, 0);
      displaySeg(seg7[pos/2].g, 0);
      }

      break;
      
    } // end of switch ch

}

void displayData(char pDD[20]) 
{
  for (int i=0; i<20; i++)
  {
    displayTable(i, pDD[i]);
  }
}
#define C1 0
#define C2 2
#define C3 4
#define C4 6

#define SEGMENT01	0x00000001
#define SEGMENT02	0x00000002
#define SEGMENT03	0x00000004
#define SEGMENT04	0x00000008
#define SEGMENT05	0x00000010
#define SEGMENT06	0x00000020
#define SEGMENT07	0x00000040
#define SEGMENT08	0x00000080
#define SEGMENT09	0x00000100
#define SEGMENT10	0x00000200
#define SEGMENT11	0x00000400
#define SEGMENT12	0x00000800
#define SEGMENT13	0x00001000
#define SEGMENT14	0x00002000
#define SEGMENT15	0x00004000
#define SEGMENT16	0x00008000
#define SEGMENT17	0x00010000
#define SEGMENT18	0x00020000
#define SEGMENT19	0x00040000
#define SEGMENT20	0x00080000
#define SEGMENT21	0x00100000
#define SEGMENT22	0x00200000
#define SEGMENT23	0x00400000
#define SEGMENT24	0x00800000
#define SEGMENT25	0x01000000
#define SEGMENT26	0x02000000
#define TOTAL_SEGMENTS 26

void displaySeg(int seg, int enable)
{

  if (enable) {
    if (seg >= 1 && seg <= 26)
      AT91C_SLCDC_MEM[C1] |= ( 1 << ((seg-1)%TOTAL_SEGMENTS) );
    else if (seg >= 27 && seg <= 52)
      AT91C_SLCDC_MEM[C2] |= ( 1 << ((seg-1)%TOTAL_SEGMENTS) );
    else if (seg >= 52 && seg <= 78)
      AT91C_SLCDC_MEM[C3] |= ( 1 << ((seg-1)%TOTAL_SEGMENTS) );
    else if (seg >= 79 && seg <= 104)
      AT91C_SLCDC_MEM[C4] |= ( 1 << ((seg-1)%TOTAL_SEGMENTS) );
  }
  else {
    if (seg >= 1 && seg <= 26)
      AT91C_SLCDC_MEM[C1] &= ~( 1 << ((seg-1)%TOTAL_SEGMENTS) );
    else if (seg >= 27 && seg <= 52)
      AT91C_SLCDC_MEM[C2] &= ~( 1 << ((seg-1)%TOTAL_SEGMENTS) );
    else if (seg >= 52 && seg <= 78)
      AT91C_SLCDC_MEM[C3] &= ~( 1 << ((seg-1)%TOTAL_SEGMENTS) );
    else if (seg >= 79 && seg <= 104)
      AT91C_SLCDC_MEM[C4] &= ~( 1 << ((seg-1)%TOTAL_SEGMENTS) );
  }
}
