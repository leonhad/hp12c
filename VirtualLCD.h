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

/*! \VirtualLCD.h HP12C LCD emulator
* 
*
*	This class is responsible to simulate the functionality of LCD 
*	through a static box.
*
*
*/

#pragma once
#include "resource.h"
#include "stdafx.h"


#define TIME_SCROLLING		600 //ms
#define TIME_SCROLL_LINES	100 //ms

// CVirtualLCD
class CVirtualLCD : public CStatic
{
	DECLARE_DYNAMIC(CVirtualLCD)

public:
	CVirtualLCD();
	virtual ~CVirtualLCD();

protected:
	//{{AFX_MSG(CVirtualLCD)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// Handle of the bitmap 
	HBITMAP m_hBitmap;
	// Handle of the memory device context
	HDC		m_hMemDC;
	// handle of the Brush
	HBRUSH	m_hBrush;
	HBRUSH	m_hBrushRepaint;
	// handle of the device context for Virtual LCD
	HDC		m_hMyDC;
	// handle of the bitmap for Virtual LCD 
	HBITMAP	m_hMyBitmap;
	// All four corrdinates of Virtual LCD
	RECT	m_rcOrgLCDPos;

	void hpFinish(void);
	void hpCopyToClipboard(void);
	void hpCopyToClipboard(CString text);
	CString hpCopyToHP12C(void);

	void hpStartTimerBlinkCur(void);
	void hpStopTimerBlinkCur(void);

	void hpStartTimerScrollLines(int ms);
	void hpStopTimerScrollLines();
	
	int	 hpGetXPos();
	int  hpGetYPos();
	int	 hpGetWidth();
	int  hpGetHeight();

private:
	
	#define DEF_TIMER_BLINK_CUR		500			// 500 ms
	#define DEF_TIMER_SCRL_LINES	200			// 200 ms
	typedef enum TIMER_ID { TIMER_DISABLED = 0, TIMER_ID_BLINK_CUR, TIMER_ID_SCROLL_LINES } ;
	UINT m_nDefTimerBlinkCur	;				//Timer
	UINT m_nDefTimerScrollLines	;				//Timer


	void hpInitBitmap(void);
	void hpDestroyBitmap(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


