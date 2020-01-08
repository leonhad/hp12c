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

#include "afxwin.h"
#if !defined(AFX_HP12C_CDLG_H__DE0A6FBC_9615_4253_8A3B_03A98221A272__INCLUDED_)
#define AFX_HP12C_CDLG_H__DE0A6FBC_9615_4253_8A3B_03A98221A272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHP12C_cDlg dialog

#include "VirtualLCD.h"
#include <list>

using namespace std;

class CHP12C_cDlg : public CDialog
{
// Construction
public:
	CHP12C_cDlg(CWnd* pParent = NULL);	// standard constructor
	~CHP12C_cDlg();
	void keypress(int a);
	void SendKeyToPort( int keyPadNum ) ;
	// Dialog Data
	//{{AFX_DATA(CHP12C_cDlg)
	enum { IDD = IDD_HP20b_C_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHP12C_cDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	void scriptGenPipeComm();
	//virtual void OnClose();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CHP12C_cDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHP12COnOFF();
	afx_msg void OnHP12COnC();
	afx_msg void OnHP12COnD();
	afx_msg void OnHP12COnMinus();
	afx_msg void OnHP12COnPlus();
	afx_msg void OnHP12CResetState();
	afx_msg void OnHP12CCopytoclipboard();
	afx_msg LPARAM OnMouseLeave(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHP12CPccalc1();
	afx_msg void OnMenu();
	afx_msg void OnHP12CPccalc3();
	afx_msg void OnHP12CPccalc4();
	afx_msg void OnHP12CPccalc5();
	afx_msg void OnHP12CExit();
	afx_msg void OnHP12CShowTitlebar();
	afx_msg void OnHelpAboutbox();
	afx_msg void OnHelpHp12Cbusinessconsultant();
	afx_msg void OnBuy();
	afx_msg void OnEditCopyNumber();
	afx_msg void OnEditPasteNumber();
	afx_msg void OnHP12CShowcaptionMenu();
	afx_msg void OnMove(int x, int y);
	afx_msg LPARAM OnDisplayChange(WPARAM x, LPARAM y);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void MakeTestSystemReady();
	void ShowHP12CTitlebar();
	void SaveHP12CState();
	void ReadHP12CState();
	void HP12CKeyDown(WPARAM wKeyCode);
	void HP12CKeyUp	(WPARAM wKeyCode);
	void ForceHP12CKeyUp(WPARAM wKeyCode);
	void ReadRegistry();
	void WriteToRegistry();
	void RemoveTestSystemItem();
	void UpdateTestbenchMenu(CMenu *pShortCutMenu = NULL);
	void ShrinkCalculator();

	// Contains resource id for the selected test bench menu item
	UINT m_nCurrentTestbenchId;
	// True if user presses the ESC key
	BOOL	m_keyEscPressed;	
	// Object of LCD Virtual screen 
	CVirtualLCD m_VirtualLCD;
	// Object for the background image of the calculator 
	CStatic m_Background;
	// Contains the virtual key code of last key pressed using keyboard
	WPARAM m_wLastVirtualKey;
	// True if the user presses shift key
	bool	m_bShiftKeyPressed;
	// Contains the current key pad number of the selected button image,  
	// when the user selects using keybaord.
	short	m_nCurKeyPadNum;
	// Contains the current key pad number of the selected button image,  
	// when the user selects using keybaord.
	int		m_nLastKeyNum;
	// Region of the selected button image
	HRGN	m_rgnPressedButton;
	// Contains ANSI char code of the key, when a user preses the key
	UINT	m_nHP12CKeyDown;
	// Enumeration to handle whether user is using keyboard or mouse.
	typedef enum { NONE, KEYBOARD, MOUSE }; UINT	m_Touch_Base;
	// Contains the key code of each key down 
	list <WPARAM> m_listKeyCode;
	//True if title bar is hidden
	bool m_bHideTitlebar;
	// Contains the data from the last time when clicked the "Copy Number" menu
	char m_CopyString[24];
	// True if user presses Shift Calc key
	bool m_bReadyToCloseCalc;
	// True if calculator image is shrinked because of screen resolution change;
	bool m_bImgShrinked;
	// Bounding rectangle of Calc
	RECT m_rcWindowRect;
	
	
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnStnClickedStaticBg();
  afx_msg void OnCalculatorAssignasdefaulthpcalculator();
  afx_msg void OnCalculatorManagehpcalculatoremulators();
  afx_msg void OnHelpAdvancedhelp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HP12C_CDLG_H__DE0A6FBC_9615_4253_8A3B_03A98221A272__INCLUDED_)
