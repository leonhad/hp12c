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

/*! \VirtualLCD.cpp HP12C LCD emulator
* 
*
*	Implementation of CVirtualLCD class
*	
*
*/

#include "stdafx.h"
#include "HP12c_c.h"
#include "VirtualLCD.h"
#include "ScreenHandeling.h"

// CVirtualLCD

IMPLEMENT_DYNAMIC(CVirtualLCD, CStatic)

CVirtualLCD::CVirtualLCD()
{
	hpInitBitmap();
	m_nDefTimerScrollLines = m_nDefTimerScrollLines  = TIMER_DISABLED;

}

CVirtualLCD::~CVirtualLCD()
{
	
	hpDestroyBitmap();
}


BEGIN_MESSAGE_MAP(CVirtualLCD, CStatic)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


/***************************************************************
** 
** Responsible to create the compatible bitmap of resource bitmap. 
** It is also responsible to create memory device context and brush 
** to paint he bitmap.  
**
***************************************************************/
//
void CVirtualLCD::hpInitBitmap(void)
{	
	HDC hdc= ::GetDC(m_hWnd);
	m_hBitmap= CreateCompatibleBitmap(hdc, CLPWidth, CLPHeight);
	m_hMyBitmap		=	CreateCompatibleBitmap(hdc, LCDWidth,LCDHeight);
	
	::ReleaseDC(m_hWnd, hdc);
	
	if(!m_hBitmap)
		return ;
	if (!m_hMyBitmap)	return ;


	m_hMemDC	= CreateCompatibleDC (::GetDC(m_hWnd)) ;
	if(!m_hMemDC)
		return;

	m_hMyDC	= CreateCompatibleDC (::GetDC(m_hWnd)) ;
	if(!m_hMyDC)
		return;

	SelectObject (m_hMemDC, m_hBitmap);
	SelectObject (m_hMyDC,		m_hMyBitmap);
	

	m_hBrush	= CreateSolidBrush(0);
	if(!m_hBrush)
		return;

	SelectObject (m_hMemDC, m_hBrush);
	SelectObject (m_hMyDC, m_hBrush);
	
	m_hBrushRepaint = CreateSolidBrush(VIRTUAL_LCD_COLOR);


}

/***************************************************************
** 
** Responsible to destroy the loaded bitmap and memory device 
** context.
**
***************************************************************/
//
void CVirtualLCD::hpDestroyBitmap(void)
{
	DeleteObject(m_hBrushRepaint);	m_hBrushRepaint = NULL;
	DeleteObject(m_hBrush);			m_hBrush		= NULL;
	DeleteObject(m_hBitmap);		m_hBitmap		= NULL;
	DeleteDC	(m_hMemDC) ;		m_hMemDC		= NULL;
	DeleteObject(m_hMyBitmap);		m_hMyBitmap		= NULL;
	DeleteDC	(m_hMyDC) ;			m_hMyDC		= NULL;	
}

/***************************************************************
** 
** Responsible to display the bitmap on the Static box.
**
***************************************************************/
//
void CVirtualLCD::hpFinish(void)
{
	HDC hdc= ::GetDC(m_hWnd);
	//StretchBlt(hdc, 0, 0, LCDWidth, LCDHeight, m_hMemDC, 0, 0, CLPWidth, CLPHeight, SRCCOPY);
	//StretchBlt(hdc, 0, 0, 330, 88, m_hMyDC, 0, 0, 330, 88, SRCCOPY);
	BitBlt(hdc, 0,0, LCDWidth,LCDHeight, m_hMyDC, 0,0, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);
}

/***************************************************************
** 
** Responsible to copy bitmap from memory device context to 
** clipboard in Bitmap format
**
***************************************************************/
//
void CVirtualLCD::hpCopyToClipboard(void)
{
	// opens the clipboard for examination and prevents other applications 
	// from modifying the clipboard content
	if ( !OpenClipboard() ){
		AfxMessageBox("Cannot open the Clipboard",MB_OK | MB_ICONINFORMATION );
		return;
	}
	// empties the clipboard and frees handles to data in the clipboard
	if( !EmptyClipboard() ){
		AfxMessageBox( "Cannot empty the Clipboard",MB_OK | MB_ICONINFORMATION );
		return;
	}
	// places data on the clipboard in a bitmap clipboard format
	if ( ::SetClipboardData( CF_BITMAP, m_hBitmap ) == NULL ){
		AfxMessageBox( "Unable to set Clipboard data" ,MB_OK | MB_ICONINFORMATION );
		CloseClipboard();
		return;
	}
	// closes the clipboard
	CloseClipboard();
}

/***************************************************************
** 
** Responsible to copy bitmap from memory device context to 
** clipboard in Text format
**
***************************************************************/
//
void CVirtualLCD::hpCopyToClipboard(CString text)
{
	// opens the clipboard for examination and prevents other applications 
	// from modifying the clipboard content
	if ( !OpenClipboard() ){
		AfxMessageBox("Cannot open the Clipboard", MB_OK | MB_ICONINFORMATION );
		return;
	}
	// empties the clipboard and frees handles to data in the clipboard
	if( !EmptyClipboard() ){
		AfxMessageBox( "Cannot empty the Clipboard",MB_OK | MB_ICONINFORMATION );
		return;
	}

	LPTSTR  lptstrCopy	= NULL; 
	HGLOBAL hglbCopy	= NULL; 
	
	// Allocate a global memory object for the text. 
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, text.GetLength()+1); 
	if (hglbCopy == NULL) { 
		CloseClipboard(); 
		return ; 
	} 

	// Lock the handle and copy the text to the buffer. 
	lptstrCopy = (LPTSTR  )GlobalLock(hglbCopy); 
	memset(lptstrCopy, 0, text.GetLength()); 
	memcpy(lptstrCopy, text.GetBuffer(text.GetLength()), text.GetLength()); 
	GlobalUnlock(hglbCopy); 

	// Place the handle on the clipboard. 
	if ( ::SetClipboardData( CF_TEXT, hglbCopy ) == NULL ){
		AfxMessageBox( "Unable to set Clipboard data",MB_OK | MB_ICONINFORMATION );
		GlobalFree(hglbCopy);
		CloseClipboard();
		return;
	}
	GlobalFree(hglbCopy);
		
	// closes the clipboard
	CloseClipboard();
}

/***************************************************************
** 
** Responsible to reterive text from clipboard  
**
***************************************************************/
//
CString CVirtualLCD::hpCopyToHP12C(void)
{
	// opens the clipboard for examination and prevents other applications 
	// from modifying the clipboard content
	if ( !OpenClipboard() ){
		AfxMessageBox("Cannot open the Clipboard", MB_OK | MB_ICONINFORMATION );
		return " ";
	}
	
	LPTSTR  lptstrCopy	= NULL; 
	HGLOBAL hglbCopy	= NULL; 

	hglbCopy = ::GetClipboardData(CF_TEXT); 
	if (hglbCopy != NULL) 
	{ 
		// Lock the handle and copy the text to the buffer. 
		lptstrCopy = (LPTSTR  )GlobalLock(hglbCopy); 
		if (lptstrCopy != NULL) 
		{
			GlobalUnlock(hglbCopy); 
	        CloseClipboard(); 
			return lptstrCopy;
		}
	}
	// closes the clipboard
	CloseClipboard();
	return " ";
}

/***************************************************************
** 
** The framework calls this member function after each interval 
** specified in the SetTimer member function used to install a timer
**
***************************************************************/
//
void CVirtualLCD::OnTimer(UINT_PTR nIDEvent)
{
	
	if(m_nDefTimerBlinkCur == TIMER_ID_BLINK_CUR)
		On0p5secondTimer();	

		CStatic::OnTimer(nIDEvent);
}

/***************************************************************
** 
** Start timer for Cursor Blinking 
**
***************************************************************/
//
void CVirtualLCD::hpStartTimerBlinkCur()
{
	if(!m_nDefTimerBlinkCur) 	m_nDefTimerBlinkCur		=	SetTimer(TIMER_ID_BLINK_CUR,DEF_TIMER_BLINK_CUR,0);
	
}

/***************************************************************
** 
** Stop timer for Cursor Blinking
**
***************************************************************/
//
void CVirtualLCD::hpStopTimerBlinkCur()
{
	if(m_nDefTimerBlinkCur)		{	KillTimer(m_nDefTimerBlinkCur);		m_nDefTimerBlinkCur    = TIMER_DISABLED; }
	
}

/***************************************************************
** 
** Start timer for Scrolling
**
***************************************************************/
//
void CVirtualLCD::hpStartTimerScrollLines(int ms)
{
	if(!m_nDefTimerScrollLines) m_nDefTimerScrollLines  =	SetTimer(TIMER_ID_SCROLL_LINES,ms,0);
}

/***************************************************************
** 
** Stop timer for Scrolling
**
***************************************************************/
//
void CVirtualLCD::hpStopTimerScrollLines()
{
	if(m_nDefTimerScrollLines)	{	KillTimer(m_nDefTimerScrollLines);	m_nDefTimerScrollLines = TIMER_DISABLED; }
}


/***************************************************************
** 
** The framework calls this function when Windows or an application 
** repaint static box
**
***************************************************************/
//
void CVirtualLCD::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RECT r={0, 0, LCDWidth, LCDHeight};
	if(m_hBrushRepaint){
		FillRect(dc, &r,m_hBrushRepaint);
		UpdateScreenContent();
	}
}
/***************************************************************
** 
** Responsible to reterive text from clipboard  
**
***************************************************************/
//
void CVirtualLCD::OnLButtonUp(UINT nFlags, CPoint point)
{
	hpCopyToClipboard();
	CStatic::OnLButtonUp(nFlags, point);
}
/***************************************************************
** 
** Responsible to reterive current left (x) position
**
***************************************************************/
//
int CVirtualLCD::hpGetXPos()
{
	RECT rc;
	GetWindowRect(&rc);
	return rc.left;	
}
/***************************************************************
** 
** Responsible to reterive current top (y) position
**
***************************************************************/
//
int CVirtualLCD::hpGetYPos()
{
	RECT rc;
	GetWindowRect(&rc);
	return rc.top;	
}
/***************************************************************
** 
** Responsible to reterive current width
**
***************************************************************/
//
int CVirtualLCD::hpGetWidth()
{
	RECT rc;
	GetWindowRect(&rc);
	return (rc.right- rc.left);	
}
/***************************************************************
** 
** Responsible to reterive current Height 
**
***************************************************************/
//
int CVirtualLCD::hpGetHeight()
{
	RECT rc;
	GetWindowRect(&rc);
	return (rc.bottom-rc.top);	
}