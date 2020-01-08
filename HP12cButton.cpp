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

/*! \HP12CButton.cpp HP12C LCD emulator
* 
*
*	Implementation of CHP12CButton class
*	
*
*/
#include "stdafx.h"
#include "HP12c_c.h"
#include "HP12cButton.h"


// Table defining the sizes of all the buttons
#define buttons(r) button(r, 0), button(r, 1), button(r, 2), button(r, 3), button(r, 4), button(r, 5), button(r, 6), button(r, 7), button(r, 8), button(r, 9)

TButtonRect Button_Table[NUM_BUTTONS]=
{
buttons(0), buttons(1),
button(2, 0), button(2, 1), button(2, 2), button(2, 3), button(2, 4), buttonEnter(2, 5), button(2, 6), button(2, 7), button(2, 8), button(2, 9), 
button(3, 0), button(3, 1), button(3, 2), button(3, 3), button(3, 4),               button(3, 6), button(3, 7), button(3, 8), button(3, 9),
HP_Logo_Area };


CHP12CButton::CHP12CButton()
{
}

CHP12CButton::~CHP12CButton()
{
}


/***************************************************************
** 
**  Retrieve the region and keypad number of the selected button 
**  depending upon the input point
**
***************************************************************/
//
HRGN CHP12CButton::hpGetKeyRegion(CPoint *pPtMouse, short *pKeyPadNum )
{
	HRGN hRgn		= NULL;
	bool bFound		= false;
	{
		for (int n = 0; n < NUM_BUTTONS; n++){
			hRgn = CreateRoundRectRgn(Button_Table[n].points[0].x, Button_Table[n].points[0].y, Button_Table[n].points[1].x, Button_Table[n].points[1].y, 6,12);	
			if(NULL != hRgn){ 
				if(PtInRegion(hRgn, pPtMouse->x, pPtMouse->y)) {  *pKeyPadNum = Button_Table[n].keyPadNum; bFound = true; break;  } 
			}
		}
	}
	if(bFound) return hRgn ; 
	*pKeyPadNum = 0;
	return NULL;
}


/***************************************************************
** 
**   Retrieve the region of the selected button depending upon the 
**	 input keypad number 
**
***************************************************************/
//
HRGN CHP12CButton::hpGetKeyRegion(short nKeyPadNum)
{
	HRGN hRgn		= NULL;
	{
		for (int n = 0; n < NUM_BUTTONS; n++){
			if(Button_Table[n].keyPadNum == nKeyPadNum)
			{
				hRgn = CreateRoundRectRgn(Button_Table[n].points[0].x, Button_Table[n].points[0].y, 
					Button_Table[n].points[1].x, Button_Table[n].points[1].y, 6,12);	
				if(NULL != hRgn) 
					return hRgn; 
			}
		}
	}
	return NULL;
}

/***************************************************************
** 
**   To test whether the point lies in the required region or not.
**
***************************************************************/
//
bool CHP12CButton::hpHitTest(CPoint *pPtMouse)
{
	HRGN hRgn		= NULL;
	{
		TButtonRect bt_area[1] = { HP_Logo_Area };
		hRgn = CreateRectRgn(bt_area[0].points[0].x, bt_area[0].points[0].y, bt_area[0].points[1].x, bt_area[0].points[1].y);	
	}
	
	if(NULL != hRgn) 
		if(PtInRegion(hRgn, pPtMouse->x, pPtMouse->y))
			return true;
	
	return false;
}