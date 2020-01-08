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

#include "stdafx.h"
#include "HP12c_c.h"
#include "HP12c_cDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHP12C_cApp

BEGIN_MESSAGE_MAP(CHP12C_cApp, CWinApp)
	//{{AFX_MSG_MAP(CHP12C_cApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)
  ON_COMMAND(ID_CALCULATOR_MANAGEHPCALCULATOREMULATORS, &CHP12C_cApp::OnCalculatorManagehpcalculatoremulators)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHP12C_cApp construction

CHP12C_cApp::CHP12C_cApp()
{
	// Place all significant initialization in InitInstance
	m_hwndDialog = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHP12C_cApp object

CHP12C_cApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHP12C_cApp initialization

BOOL CHP12C_cApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	//First free the string allocated by MFC at CWinApp startup.
	//The string is allocated before InitInstance is called.
	free((void*)m_pszAppName);
	//Change the name of the application file.
	//The CWinApp destructor will free the memory.
	m_pszAppName = _tcsdup("HP 12C Business Consultant");
	

	CHP12C_cDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CHP12C_cApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	if (m_hwndDialog != NULL)
	{
		if ((lpMsg->hwnd == m_hwndDialog) || ::IsChild(m_hwndDialog, lpMsg->hwnd))
		{
			if (lpMsg->message == WM_KEYDOWN)
				::SendMessage(m_hwndDialog, WM_KEYDOWN, lpMsg->wParam, lpMsg->lParam);
			if (lpMsg->message == WM_KEYUP)
				::SendMessage(m_hwndDialog, WM_KEYUP, lpMsg->wParam, lpMsg->lParam);
		}
	}
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

void CHP12C_cApp::OnCalculatorManagehpcalculatoremulators()
{
  // TODO: Add your command handler code here
}
