//  Copyright (c) 2007 Hewlett-Packard development company llc
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
/*
* file created by Anuj Garg
*/

/*! \HP12CButton.h HP12C LCD emulator
* 
*
*	File contains macros to create the imaginary rectangles around the buttons. 
*	File also contains the class which is responsible for the functionality of each individual button.
*	
*/
#pragma once
#include "stdafx.h"

// macros
//row1
#ifdef C12
#define button(r, c) {r*10+c, { { c*48+35, r*58+127 }, { c*48+35+40, r*58+127+36 } } }
#define buttonEnter(r, c) {r*10+c, { { c*48+35, r*56+130 }, { c*48+35+40, r*56+130+94 } } }
// HP Logo 
#define HP_Logo_Area	{ -1, {  {468, 27 }, {512, 70}		} }
#endif
#ifdef C15
#define button(r, c) {r*10+c, { { c*48+35, r*56+130 }, { c*48+35+40, r*56+130+35 } } }
#define buttonEnter(r, c) {r*10+c, { { c*48+35, r*56+130 }, { c*48+35+40, r*56+130+94 } } }
// HP Logo 
#define HP_Logo_Area	{ -1, {  {468, 29 }, {509, 74}		} }
#endif


#define NUM_BUTTONS		40
 
typedef struct { int keyPadNum; POINT points[2]; } TButtonRect  ;

class CHP12CButton 
{
public:
	
	CHP12CButton();
	 ~CHP12CButton();

	HRGN hpGetKeyRegion(CPoint *pPtMouse, short *pKeyPadNum);	
	HRGN hpGetKeyRegion(short nKeyPadNum);
	bool hpHitTest(CPoint *pPtMouse);
};


