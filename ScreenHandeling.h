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
* file created by Cyrille
*/

/*! \ScreenHandeling.h Euro LCD emulator
* 
*	This file contains the required constants for emulator's LCD  
*	
*	
*
*
*/

//#define VIRTUAL_LCD_COLOR	RGB(109,118,93)//RGB(71,165,137)
#ifdef C12
#define VIRTUAL_LCD_COLOR	RGB(170, 165, 136)//RGB(71,165,137)
#endif
#ifdef C15
#define VIRTUAL_LCD_COLOR	RGB(154, 154, 142)//RGB(71,165,137)
#endif

#define CLPWidth	1566		// Clipboard bitmap width
#define CLPHeight	308			// Clipboard bitmap height
#define LCDWidth	313			// LCD bitmap width
#define LCDHeight	57 		// LCD bitmap height


void On0p5secondTimer();
bool On0p2firstTimer();
void WindowsBlink(bool blink);
void UpdateScreenContent();
void WindowsSwapBuffers();
