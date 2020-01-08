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
#include "HP12c.h"
#include "system.h"
#include "ScreenHandeling.h"// for LCD data
#include "HP12cButton.h"
#include "graphics.h"

#include "atlimage.h"

// The compiler encountered a function that was marked with deprecated. 
// The function may no longer be supported in a future release. 
// The warning has been disabled for fopen(...) function
#pragma warning(disable : 4996)
#pragma warning(disable : 4800)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

extern CVirtualLCD *g_pLCD;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString GetVersionInfo();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

/***************************************************************
** 
** This member function is called in response to the WM_INITDIALOG message.
** 
***************************************************************/
//
BOOL CAboutDlg::OnInitDialog()
{
#ifdef C12
  SetWindowText("About HP12C");
  SetDlgItemText(IDC_STATIC, CString("PC version of the HP12C calculator"));
#endif
#ifdef C15
  SetWindowText("About HP15C");
  SetDlgItemText(IDC_STATIC4, CString("PC version of the HP15C calculator"));
#endif
	SetDlgItemText(IDC_STATIC_VER_INFO, GetVersionInfo());
	return TRUE;
}
/***************************************************************
** 
**  Function to extract the build date from globally defined 
**  variable BuildDate in Build.h 
** 
***************************************************************/
//
CString CAboutDlg::GetVersionInfo()
{
	CString ret_val;
  u64 b= BuildDate&0xfffffffffffffffLL;
  if ((b&0xfff)!=0) b<<=4;
	int m,d,y;
	m	= (int)((b & 0xff00000000000000LL) >> 56);
	d	= (int)((b & 0x00ff000000000000LL) >> 48);
	y	= (int)((b & 0x0000ffff00000000LL) >> 32);
	ret_val.Format("Version %x %x %x",m ,d ,y );
	return ret_val;
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHP12C_cDlg dialog
CHP12C_cDlg::CHP12C_cDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHP12C_cDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHP12C_cDlg)
	m_nHP12CKeyDown		= 0;
	m_Touch_Base		= NONE;
	m_bShiftKeyPressed	= false;
	m_bHideTitlebar		= false;
	m_bImgShrinked        = false;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CHP12C_cDlg::~CHP12C_cDlg()
{

}

void CHP12C_cDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHP12C_cDlg)
	DDX_Control(pDX, IDC_VIRTUAL_LCD, m_VirtualLCD);
	DDX_Control(pDX, IDC_STATIC_BG, m_Background);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHP12C_cDlg, CDialog)
	//{{AFX_MSG_MAP(CHP12C_cDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CHAR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE,   OnMouseLeave)
	ON_MESSAGE(WM_DISPLAYCHANGE,   OnDisplayChange)
	ON_COMMAND(ID_HP20b_ONAF, &CHP12C_cDlg::OnHP12COnOFF)
	ON_COMMAND(ID_HP20b_ONC, &CHP12C_cDlg::OnHP12COnC)
	ON_COMMAND(ID_HP20b_OND, &CHP12C_cDlg::OnHP12COnD)
	ON_COMMAND(ID_HP20b_ONMINUS, &CHP12C_cDlg::OnHP12COnMinus)
	ON_COMMAND(ID_HP20b_ONPLUS, &CHP12C_cDlg::OnHP12COnPlus)
	ON_COMMAND(ID_HP20b_RESETSTATE, &CHP12C_cDlg::OnHP12CResetState)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HP20b_COPYTOCLIPBOARD, &CHP12C_cDlg::OnHP12CCopytoclipboard)
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_HP20b_EXIT, &CHP12C_cDlg::OnHP12CExit)
	ON_COMMAND(ID_HP20b_SHOWCAPTION, &CHP12C_cDlg::OnHP12CShowTitlebar)
	//ON_WM_NCLBUTTONUP()
	ON_COMMAND(ID_HELP_ABOUTBOX, &CHP12C_cDlg::OnHelpAboutbox)
	ON_COMMAND(ID_HELP_HP20BBUSINESSCONSULTANT, &CHP12C_cDlg::OnHelpHp12Cbusinessconsultant)
	ON_COMMAND(ID_BUY, &CHP12C_cDlg::OnBuy)
	ON_COMMAND(ID_EDIT_COPY_NUMBER, &CHP12C_cDlg::OnEditCopyNumber)
	ON_COMMAND(ID_EDIT_PASTE_NUMBER, &CHP12C_cDlg::OnEditPasteNumber)	
	ON_COMMAND(ID_HP20b_SHOWCAPTION_MENU, &CHP12C_cDlg::OnHP12CShowcaptionMenu)
	ON_WM_MOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
  ON_STN_CLICKED(IDC_STATIC_BG, &CHP12C_cDlg::OnStnClickedStaticBg)
  ON_COMMAND(ID_CALCULATOR_ASSIGNASDEFAULTHPCALCULATOR, &CHP12C_cDlg::OnCalculatorAssignasdefaulthpcalculator)
  ON_COMMAND(ID_CALCULATOR_MANAGEHPCALCULATOREMULATORS, &CHP12C_cDlg::OnCalculatorManagehpcalculatoremulators)
  ON_COMMAND(ID_HELP_ADVANCEDHELP, &CHP12C_cDlg::OnHelpAdvancedhelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHP12C_cDlg message handlers

TCalc CALC;
CRITICAL_SECTION criticalsection;

void KeyPressMutex(TCalc *calc)
{
  EnterCriticalSection(&criticalsection);
  int k= KeyBuffPeek();
  if (k>=0) { SendChar(k); execute(calc); }
  LeaveCriticalSection(&criticalsection);
}

int TestVoltage(int min, int max)
{
  return 30;
}

HANDLE Pipe= 0;
CHP12C_cDlg *dlg;
TCalc *calc= &CALC;
#ifdef executeAnalysis
unsigned int romcoverage[romcoveragesize];
struct sBadCalculationTable badCalcTable[MAX_BAD_CALCULATION];
#endif
int CpuSpeed = 4000;

bool CommunicationPipeConnected= false;
HANDLE ComunicationNamedPipe;
unsigned long __stdcall CommunicationThread(void *p)
{
  while (true)
  {
    if (CommunicationPipeConnected)
    {
      if (!PeekNamedPipe(ComunicationNamedPipe, NULL, NULL, NULL, NULL, NULL))
      {
        DWORD er= GetLastError();
        if (er=109) CommunicationPipeConnected= false;
      }
    }
    if (!CommunicationPipeConnected)
    {
      CommunicationPipeConnected= ConnectNamedPipe(ComunicationNamedPipe, NULL);
      if (!CommunicationPipeConnected)
      {
        DWORD er= GetLastError();
        if (er==ERROR_PIPE_CONNECTED) CommunicationPipeConnected= true;
        if (er==ERROR_NO_DATA) DisconnectNamedPipe(ComunicationNamedPipe);
      }
    }
    if (!CommunicationPipeConnected) { Sleep(100); continue; }
    CheckCommunication();
  }
}

u32 GetChars(u8 *b, u32 nb, u32 timeout)
{
  while (nb!=0)
  {
    int c= GetChar(300);
    if (c==-1) return nb;
    *b++= c;
    nb--;
  }
  return 0;
}

void  SendChars(u8 const *d, u32 size, bool ForceSend)
{
  while (size)
  {
    if (!CommunicationPipeConnected) return;
    DWORD w;
    WriteFile(ComunicationNamedPipe, d, size, &w, NULL);
    d+= w;
    size-= w;
  }
}

void SendChar(unsigned char c)
{
  if (!CommunicationPipeConnected) return;
  DWORD w;
  WriteFile(ComunicationNamedPipe, &c, 1, &w, NULL);
}

i32 GetChar()
{
  if (!CommunicationPipeConnected) return -1;
  DWORD bytes, w;
  unsigned char c;
  if (!PeekNamedPipe(ComunicationNamedPipe, &c, 1, &bytes, NULL, NULL)) return -1;
  if (bytes==0) return -1;
  if (!ReadFile(ComunicationNamedPipe, &c, 1, &w, NULL)) return -1;
  if (w==0) return -1;
  return c;
}

i32 GetChar(u32 timeout)
{
  int c= GetChar();
  if (c==-1) { Sleep(timeout); c= GetChar(); }
  return c;
}

HANDLE KeyEvent;

void UpdateDlgScreen()
{
  SendMessage(dlg->m_hWnd, WM_CHAR, '[', 0); //used to force a screen refresh...	
}

unsigned long __stdcall CalculationThread(void *p)
{
  while (1)
  {
    WaitForSingleObject(KeyEvent, INFINITE);
    while (!KeyBuffEmpty()) KeyPressMutex(calc);
  }
}

#ifdef C12
#define PRODUCT_CODE 31
#endif
#ifdef C15
#define PRODUCT_CODE 32
#endif


BOOL CHP12C_cDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

  // IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	dlg= this;

	((CHP12C_cApp *)AfxGetApp())->m_hwndDialog = m_hWnd;
	g_pLCD = &m_VirtualLCD;

	InitializeCriticalSection(&criticalsection);
	initKeyBuffer();
	init(calc);
	ReadHP12CState(); // load saved state file if exists ....

	updatescreen(calc);

	KeyEvent= CreateEvent(NULL, false, false, "");

	unsigned long id;
	CreateThread(NULL, 0, CalculationThread, NULL, 0, &id);

  int i= 0;
  char name[50];
  do {
#ifdef C12
    if (i==0) strcpy(name, "\\\\.\\pipe\\hp12c"); else sprintf(name, "\\\\.\\pipe\\hp12c_%d", i);
#endif
#ifdef C15
    if (i==0) strcpy(name, "\\\\.\\pipe\\hp15c"); else sprintf(name, "\\\\.\\pipe\\hp15c_%d", i);
#endif
    ComunicationNamedPipe= CreateNamedPipe(name, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT	,1, 8192, 8192, 100, NULL);
    i++;
  } while (INVALID_HANDLE_VALUE==ComunicationNamedPipe);
  CreateThread(NULL, 0, CommunicationThread, NULL, 0, &id);
	
	::GetWindowRect(m_VirtualLCD.m_hWnd, &m_VirtualLCD.m_rcOrgLCDPos);
	ScreenToClient(&m_VirtualLCD.m_rcOrgLCDPos);

//	if(::GetSystemMetrics(SM_CXSCREEN)<=800){
//		ShrinkCalculator();
//	}
	
	#ifdef C12
	m_Background.SetBitmap(::LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP2)) );
	#endif
	#ifdef C15
	m_Background.SetBitmap(::LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
	#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/***************************************************************
** 
** Function is responsible to remove Test System related menus  
** 
***************************************************************/
//

void CHP12C_cDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} 
	else
	{
		// If the title bar is hidden, do not accept ALT key
		if(m_bHideTitlebar)
			if ( nID == SC_KEYMENU )
				return;

		CDialog::OnSysCommand(nID, lParam);
	}
}

/***************************************************************
** 
** Override function to filter VK_F10 keystroke. Pressing F10 doesn't generate WM_KEYDOWN.
** F10 is a special key used to activate menu bar.
**
***************************************************************/
//
BOOL CHP12C_cDlg::PreTranslateMessage( MSG* pMsg )
{
	if((pMsg->wParam - VK_F1 + 1) == 10/*VK_F10*/) {
		if (pMsg->message == WM_SYSKEYDOWN) 
			HP12CKeyDown(VK_F10);
		else if (pMsg->message == WM_SYSKEYUP) 
			HP12CKeyUp(VK_F10);
		return 1;
	}
	if (pMsg->message == WM_KEYDOWN) { 	HP12CKeyDown(pMsg->wParam); 	return 1;  }
	else if (pMsg->message == WM_KEYUP) { 	HP12CKeyUp(pMsg->wParam); 	return 1; }

	return CDialog::PreTranslateMessage(pMsg);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CHP12C_cDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHP12C_cDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHP12C_cDlg::keypress(int a)
{
  System.KeyboardMap|= KeyCodeToKeyMap(a);
  AddKeyInBuffer(a);
  ATLTRACE2("KEY %d added in buffer (buf is %d keys)\r\n", a, (System.BufWrite-System.BufRead) % 16);
  SetEvent(KeyEvent); 
}

void CHP12C_cDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar!='[')
    keypress(nChar); 
  else {
    UpdateScreenContent(); // copy the graphics on the PC screen
  }
}

/***************************************************************
** 
** Responsible to add the virtual key code of the current pressed key,
** and fire a WM_KEYUP message for the previous key which the user has 
** not released.
**
***************************************************************/
//
void CHP12C_cDlg::ForceHP12CKeyUp(WPARAM wKeyCode)
{
	if(m_listKeyCode.size() > 0)
	{
		list<WPARAM>::iterator iter;
		bool already_exists = false;
		for (iter = m_listKeyCode.begin(); iter != m_listKeyCode.end(); ++iter){
			if(*iter == wKeyCode){	already_exists = true; break ; }
		}
		if(!already_exists)	{		
			for (iter = m_listKeyCode.begin(); iter != m_listKeyCode.end(); ){
				HP12CKeyUp(*iter);	
				iter = m_listKeyCode.erase(iter);
			}
			m_listKeyCode.push_back(wKeyCode);
		}
	}
	else
		m_listKeyCode.push_back(wKeyCode);
}

/***************************************************************
** 
** Responsible to trap keystrokes for non-system keys. Once trapped, 
** calls the keypress(...) function
**
***************************************************************/
//
void CHP12C_cDlg::HP12CKeyDown(WPARAM wKeyCode)
{
	ForceHP12CKeyUp(wKeyCode);
	
	if(m_Touch_Base == NONE)
	{
		if(m_wLastVirtualKey != wKeyCode)
		{
			short key_num = 0;
			switch (wKeyCode){
				// shift key 
				case VK_LSHIFT:
				case VK_RSHIFT:
				case VK_SHIFT:	m_bShiftKeyPressed = true; break;

				// row1
				case 78		:																// key N		
				case VK_F1	:	keypress(key_num = 11);		break;							// key F1
				case 89		:																// key Y
				case VK_F2	:	keypress(key_num = 12);		break;							// key F2
				case 80		: 																// key P
				case VK_F3	:	keypress(key_num = 13);		break;							// key F3
				case 77		:																// key M
				case VK_F4	:	keypress(key_num = 14);		break;							// key F4
				case 70		:																// key F
				case VK_F5	:	keypress(key_num = 15);		break;							// key F5
				case 65		:																// key A
				case VK_F6	:	keypress(key_num = 16);		break;							// key F6

				//row2
				case 67		:																// key C
				case VK_F7	:	keypress(key_num = 21);		break;							// key F7
				case 73		:																// key I
				case VK_F8	:	keypress(key_num = 22);		break;							// key F8
				case 86		:																// key V
				case VK_F9	:	keypress(key_num = 23);		break;							// key F9
				case 66		:																// key B
				case VK_F10	:	keypress(key_num = 24);		break;							// key F10
				case 53		:	if(!m_bShiftKeyPressed)		keypress(key_num = 53);			// key 5
								else 						keypress(key_num = 25);			// key %
								break;
				case VK_F11	:	keypress(key_num = 25);		break;							// key F11
				case 82		:																// key R			
				case VK_F12	:	keypress(key_num = 26);		break;							// key F12

				//row3
				case 13		:	keypress(key_num = 31);		break;
				case 57		:	if(!m_bShiftKeyPressed)		keypress(key_num = 44);			// key 9
								else 						keypress(key_num = 32);			// key (
								break;
				case 48		:	if(!m_bShiftKeyPressed)		keypress(key_num = 72);			// key 0
								else						keypress(key_num = 33);			// key )
								break;
				case 87		:	keypress(key_num = 34);		break;							//+/-
				case 8		:	keypress(key_num = 35);		break;

				//row4
				case 38		:	keypress(key_num = 41);		break;							// key Up Arrow
				case 103	:	keypress(key_num = 42);		break;							// key Numpad(7)
				case 55		:	if (!m_bShiftKeyPressed)	keypress(key_num = 42); break;	// key (7)
				case 104	:
				case 56		:	if(!m_bShiftKeyPressed)		keypress(key_num = 43);			// key Numpad(8)
								else						keypress(key_num = 55);			// key *
								break;
				case 105	:	keypress(key_num = 44);		break;							// key Numpad(9)
				case 111	:	keypress(key_num = 45);		break;							// key Numpad(/)
				case 191	:   if (!m_bShiftKeyPressed)	keypress(key_num = 45); break;	// key (/)
				
				//row5
				case 40		:	keypress(key_num = 51);		break;							// key Down Arrow
				case 100	:	keypress(key_num = 52);		break;							// key Numpad(4)
				case 52		:	if (!m_bShiftKeyPressed)	keypress(key_num = 52); break;	// key (4)
				case 101	:	keypress(key_num = 53);		break;							// key Numpad(5)
				case 102	:	keypress(key_num = 54);		break;							// key Numpad(6)
				case 54		:	if (!m_bShiftKeyPressed)	keypress(key_num = 54); break;	// key (6)
				case 106	:	keypress(key_num = 55);		break;							// key Numpad(*)
				
					
				//row6
				case 17		:	keypress(key_num = 61);		break;							// key CTRL
				case 97		:	keypress(key_num = 62);		break;							// key Numpad(1)
				case 49		:	if (!m_bShiftKeyPressed)	keypress(key_num = 62); break;	// key (1)
				case 98		:	keypress(key_num = 63);		break;							// key Numpad(2)
				case 50		:	if (!m_bShiftKeyPressed)	keypress(key_num = 63); break;	// key (2)
				case 99		:	keypress(key_num = 64);		break;							// key Numpad(3)
				case 51		:	if (!m_bShiftKeyPressed)	keypress(key_num = 64); break;	// key (3)
				case 109	:	
				case 189	:	keypress(key_num = 65);		break;							// key Numpad(-)

				//row7
				case 27		:	keypress(key_num = 71);		break;							// key Esc
				case 96		:	keypress(key_num = 72);		break;							// key Numpad(0)
				case 110	:	keypress(key_num = 73);		break;							// key Numpad(.)
				case 190	:	if (!m_bShiftKeyPressed)	keypress(key_num = 73); break;	// key (.)
				case 187	:	if(!m_bShiftKeyPressed)		keypress(key_num = 74);			// key =
								else						keypress(key_num = 75);			// key +
								break;
				case 107	:	keypress(key_num = 75);		break;							// key Numpad(+)
		 
			}
			
			if(key_num > 0){
				m_nHP12CKeyDown = key_num;
				CHP12CButton HP12C_btn;
				m_rgnPressedButton = HP12C_btn.hpGetKeyRegion(m_nHP12CKeyDown);
				HDC hDC = ::GetDC(m_Background.m_hWnd);
				InvertRgn(hDC, m_rgnPressedButton);
				::ReleaseDC(m_Background.m_hWnd, hDC);
				m_wLastVirtualKey = wKeyCode;
				m_Touch_Base = KEYBOARD;

			}
		}
	}
}



/***************************************************************
** 
** Responsible to trap keystrokes for non-system keys. Once trapped, 
** calls the keypress(...) function
**
***************************************************************/
//
void CHP12C_cDlg::HP12CKeyUp(WPARAM wKeyCode)
{
	if(	wKeyCode == VK_LSHIFT || wKeyCode == VK_RSHIFT || wKeyCode == VK_SHIFT)
		m_bShiftKeyPressed = false; 
	
	if(m_nHP12CKeyDown > 0){
	//	TRACE1("\n m_nHP12CKeyDown = %d", m_nHP12CKeyDown);
	//	System.KeyboardMap&= ~KeyCodeToKeyMap(m_nHP12CKeyDown);
		CHP12CButton HP12C_btn;
		m_rgnPressedButton = HP12C_btn.hpGetKeyRegion(m_nHP12CKeyDown);
		HDC hDC = ::GetDC(m_Background.m_hWnd);
		InvertRgn(hDC, m_rgnPressedButton);
		::ReleaseDC(m_Background.m_hWnd, hDC);
	
		System.KeyboardMap&= ~KeyCodeToKeyMap(m_nHP12CKeyDown);

		m_nHP12CKeyDown = 0;
		m_wLastVirtualKey = 0;
		m_Touch_Base = NONE;
	}
}

/***************************************************************
** 
**  Dump the current HP12C State to the file.
**
***************************************************************/

char *GetSAVEFILE(char *b)
{
  GetEnvironmentVariable("APPDATA", b, 256);
#ifdef C12
  strcat(b, "\\HP12CState.bin");
#endif
#ifdef C15
  strcat(b, "\\HP15CState.bin");
#endif
  return b;
}

void CHP12C_cDlg::SaveHP12CState()
{
  char buf [300];
	FILE *fp;
	fp = fopen(GetSAVEFILE(buf), "wb");
	if (!fp)
		return ;
	fwrite(calc, sizeof(CALC),1,fp);
	fclose(fp);
}
/***************************************************************
** 
**  Responsible to read the last saved state
**
***************************************************************/
//
void CHP12C_cDlg::ReadHP12CState()
{
	FILE *fp;
  char buf [300];
	fp = fopen(GetSAVEFILE(buf), "rb");
	if (!fp)
		return ;
	fread(calc, sizeof(CALC),1,fp);
  fclose(fp);
  UpdateDlgScreen();
  ReadRegistry();
}

/***************************************************************
** 
**  Called to inform the CWnd object that it is being destroyed.
**
***************************************************************/
//
void CHP12C_cDlg::OnDestroy()
{
	CDialog::OnDestroy();
	SaveHP12CState();
	WriteToRegistry();
	if(Pipe!= NULL) { CloseHandle(Pipe); Pipe = NULL; }
}

/***************************************************************
** 
** Called when the user releases the left mouse button on the button image 
** displayed on the calculator. Function is responsible to invert the color 
** of the selected button image.
**
***************************************************************/
//
void CHP12C_cDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(NULL != m_rgnPressedButton && MOUSE == m_Touch_Base){
		HDC hDC = ::GetDC(m_Background.m_hWnd);
		InvertRgn(hDC, m_rgnPressedButton);
		::ReleaseDC(m_Background.m_hWnd, hDC);
		
		System.KeyboardMap&= ~KeyCodeToKeyMap(m_nCurKeyPadNum);

		m_rgnPressedButton = NULL; m_nCurKeyPadNum = 0;
	}
	if(MOUSE == m_Touch_Base)
		m_Touch_Base = NONE;

	CDialog::OnLButtonUp(nFlags, point);
}

/***************************************************************
** 
**  Called when the user presses  the left mouse button on the button image 
**  displayed on the calculator. Function is responsible to invert the color 
**  of the selected button image.
**
***************************************************************/
//
void CHP12C_cDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_Touch_Base == NONE) {
		CHP12CButton btn;
		m_rgnPressedButton =  btn.hpGetKeyRegion(&point, &m_nCurKeyPadNum);
		if(NULL != m_rgnPressedButton){

			keypress(m_nCurKeyPadNum);
			HDC hDC = ::GetDC(m_Background.m_hWnd);
			InvertRgn(hDC, m_rgnPressedButton);
			::ReleaseDC(m_Background.m_hWnd, hDC);
			
			//track when the mouse pointer leaves a window
			TRACKMOUSEEVENT tme; 
			tme.cbSize = sizeof(tme); tme.hwndTrack = m_hWnd; tme.dwFlags = TME_LEAVE;
			_TrackMouseEvent(&tme);
		}
		m_Touch_Base = MOUSE;
	}

	if(NULL == m_rgnPressedButton){
		//To move HP12C without title bar
		m_Touch_Base = NONE;
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	CDialog::OnLButtonDown(nFlags, point);
}


/***************************************************************
** 
**  Called when the user releases the right mouse button on HP-Logo.  
**  This function is responsible to display the short cut menu.
**
***************************************************************/
//
void CHP12C_cDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CHP12CButton btn;
	if(btn.hpHitTest(&point)) {
		CMenu *pMenu = new CMenu;
		pMenu->LoadMenuA(MAKEINTRESOURCE(IDR_MENU3));		
		// check/uncheck the 'test System' menu item
		if(m_bHideTitlebar)
			pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
		else
			pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);
		
		ClientToScreen(&point);
		pMenu->GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
			
		delete pMenu;
	} 

	CDialog::OnRButtonUp(nFlags, point);
}


/***************************************************************
** 
**  Called when the user presses the right mouse button on GUI 
**  except LCD Area.  
**  This function is responsible to sned SHIFT key to calc firmware.
**
***************************************************************/
//
void CHP12C_cDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CHP12CButton btn;
	if(!btn.hpHitTest(&point))
	{
		// Forcing to send SHIFT key to calculator firmware
    keypress(61);
//		HP12CKeyDown(VK_CONTROL );
	}
	CDialog::OnRButtonDown(nFlags, point);
}

/***************************************************************
** 
**  Called when the user presses the right mouse button on GUI 
**  except LCD Area.  
**  This function is responsible to sned SHIFT key to calc firmware.
**
***************************************************************/
//
void CHP12C_cDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CHP12CButton btn;
	if(!btn.hpHitTest(&point))
	{
		// Forcing to send SHIFT key to calculator firmware
		HP12CKeyDown(VK_CONTROL );
		HP12CKeyUp(VK_CONTROL );
	}
	CDialog::OnRButtonDblClk(nFlags, point);
}

/***************************************************************
** 
**  Called when the mouse pointer leaves a window or hovers over a window for a specified amount of time
**
***************************************************************/
//
 
LPARAM CHP12C_cDlg::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	SendMessage(WM_LBUTTONUP, 0,0);
	return 0;
}


/***************************************************************
** 
**  Called when the user presses 'On OFF' sub-menu
**
***************************************************************/
//
void CHP12C_cDlg::OnHP12COnOFF()
{
	keypress(85);
}

/***************************************************************
** 
**  Called when the user presses 'On C' sub-menu
**
***************************************************************/
//
void CHP12C_cDlg::OnHP12COnC()
{
}

/***************************************************************
** 
**  Called when the user presses 'On D' sub-menu
**
***************************************************************/
//
void CHP12C_cDlg::OnHP12COnD()
{
}

/***************************************************************
** 
**  Called when the user presses 'On -' sub-menu
**
***************************************************************/
//
void CHP12C_cDlg::OnHP12COnMinus()
{
}

/***************************************************************
** 
**  Called when the user presses 'On +' sub-menu
**
***************************************************************/
//
void CHP12C_cDlg::OnHP12COnPlus()
{
}

/***************************************************************
** 
**  Called when the user presses 'CopyToClipboard' sub-menu
**
***************************************************************/
//
void CHP12C_cDlg::OnHP12CCopytoclipboard()
{
	m_VirtualLCD.hpCopyToClipboard();
}

/***************************************************************
** 
**  Called when the user presses 'Reset State' sub-menu
**
***************************************************************/
void CHP12C_cDlg::OnHP12CResetState()
{
	if(AfxMessageBox("Are you sure that you want to reset the calculator?", MB_YESNO | MB_ICONINFORMATION) == IDYES){
		init(calc);
		updatescreen(calc);
	}
}

/***************************************************************
** 
**  Called when the user double-clicks the left mouse button on the button image 
**  displayed on the calculator. Function is responsible to invert the color 
**  of the selected button image.
**
***************************************************************/
//
void CHP12C_cDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CHP12CButton btn;
	m_rgnPressedButton =  btn.hpGetKeyRegion(&point, &m_nCurKeyPadNum);
	if(NULL != m_rgnPressedButton)
	{
		m_Touch_Base = MOUSE;
  		keypress(m_nCurKeyPadNum);
		HDC hDC = ::GetDC(m_Background.m_hWnd);
		InvertRgn(hDC, m_rgnPressedButton);
		::ReleaseDC(m_Background.m_hWnd, hDC);
		System.KeyboardMap&= ~KeyCodeToKeyMap(m_nCurKeyPadNum);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

/***************************************************************
** 
**  Called when the user presses 'Exit' sub-menu
**  
***************************************************************/
void CHP12C_cDlg::OnHP12CExit()
{
	SendMessage(WM_CLOSE, 0, 0);
}

/***************************************************************
** 
**  Called when the user presses 'Hide Titlebar' sub-menu
**  
***************************************************************/
void CHP12C_cDlg::OnHP12CShowTitlebar()
{
	ShowHP12CTitlebar();

	// check/uncheck main menu
	CMenu *pMenu = GetMenu();
	if(m_bHideTitlebar)
		pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
	else
		pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);

}

/***************************************************************
** 
**  To show/hide the Title bar of the application window.     
**  This is also responsible to set the window region.   
**  
***************************************************************/
//
void CHP12C_cDlg::ShowHP12CTitlebar()
{
	if(m_bHideTitlebar){
		// Display title bar
		SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)|WS_CAPTION); 
		SetWindowRgn(NULL, true);
	}
	else
	{
		// Remove title bar	
		HRGN rgn;
		if(true) //(::GetSystemMetrics(SM_CXSCREEN)>800 && ::GetSystemMetrics(SM_CYSCREEN) >600)
		{
      static POINT const table[4]= { {5, 22}, {543, 22}, {543, 384}, {5, 384}  };
			rgn = CreatePolygonRgn (table, 4, ALTERNATE);	
			SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)&~WS_CAPTION); 
			SetWindowRgn(rgn, TRUE);
			
		}
		else
		{
			POINT table[4]= { {7, 0+22 }, {7, 557+29 }, {418, 557+29 }, {418, 0+22 }}; 
			rgn = CreatePolygonRgn (table, 4, ALTERNATE);	
			SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)&~WS_CAPTION); 
			SetWindowRgn(rgn, TRUE);
		}
	}
	m_bHideTitlebar = !m_bHideTitlebar;	
}

/***************************************************************
** 
**  Called when the user presses 'About' sub-menu
**  
***************************************************************/
//
void CHP12C_cDlg::OnHelpAboutbox()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

/***************************************************************
** 
**  Called when the user presses 'HP 12C Business Consultant Help' sub-menu
**  
***************************************************************/
//
void CHP12C_cDlg::OnHelpHp12Cbusinessconsultant()
{
#ifdef C12
	HINSTANCE h = ShellExecute(NULL, "open", "hp12c.pdf", NULL, NULL, SW_SHOWNORMAL);
	// Returns a value greater than 32 if successful, or an error value 
	// that is less than or equal to 32 otherwise
	if( (int)h <= 32 )
		AfxMessageBox("The help file hp12c.pdf was not found", MB_OK | MB_ICONINFORMATION);
#endif
#ifdef C15
	HINSTANCE h = ShellExecute(NULL, "open", "hp15c.pdf", NULL, NULL, SW_SHOWNORMAL);
	// Returns a value greater than 32 if successful, or an error value 
	// that is less than or equal to 32 otherwise
	if( (int)h <= 32 )
		AfxMessageBox("The help file hp15c.pdf was not found", MB_OK | MB_ICONINFORMATION);
#endif
}

/***************************************************************
** 
**  Called when the user presses 'Buy' sub-menu
**  
***************************************************************/
//
void CHP12C_cDlg::OnBuy()
{
	// Open default web browser
	ShellExecute(NULL, "open", "http://www.hp.com/calculators", NULL, NULL, SW_SHOWNORMAL);
}

/***************************************************************
** 
**  Called when the user presses 'Copy Number' sub-menu
**  
***************************************************************/
//
void CHP12C_cDlg::OnEditCopyNumber()
{
}

/***************************************************************
** 
**  Called when the user presses 'Paste Number' sub-menu
**  
***************************************************************/
// TODO change for 15C!!!!
struct { char c; unsigned int keys; } const keydefs[]=
{ { '0', 36 }, 
{ '1', 26 }, {'2', 27}, {'3', 28}, 
{ '4', 16 }, {'5', 17}, {'6', 18}, 
{ '7', 6 }, {'8', 7}, {'9', 8},
{ 'e', 15}, { 'E', 15}, { '-', 5}, {'.', 37},
{ '+', 39}, {'*', 19}, {'/', 9}, 
{0, 0}};
void CHP12C_cDlg::OnEditPasteNumber()
{
	// reterive clipboard data 
	CString val = m_VirtualLCD.hpCopyToHP12C();
	if(!val.IsEmpty())
	{
		// fire keyboard events 
		for(int i=0; i < val.GetLength(); i++){
			int oo =val.GetAt(i);
			int j=0; while (keydefs[j].c!=0 && keydefs[j].c!=oo) j++;
			if (keydefs[j].c!=0)
      {
        AddKeyInBuffer(keydefs[j].keys);
        SetEvent(KeyEvent);
      }
		}
	}
}

/***************************************************************
** 
**  Resonsible to create the registry key and write the window's current 
**  coordinates and title bar visibility status to the registry.
**  
***************************************************************/
//
void CHP12C_cDlg::WriteToRegistry()
{
	RECT rect;
	HKEY hKey, hkResult;
    char pos[8];
	
	GetWindowRect(&rect);
	// Open/Create required registry and write values
	if( RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE",0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){
		if(RegCreateKeyEx(hKey, "Hewlett-Packard" , 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, NULL) == ERROR_SUCCESS){
			HKEY hkResult1;
#ifdef C12
			if( RegCreateKeyEx(hkResult, "hp12c" , 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult1, NULL)  == ERROR_SUCCESS){
#endif
#ifdef C15
			if( RegCreateKeyEx(hkResult, "hp15c" , 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult1, NULL)  == ERROR_SUCCESS){
#endif
				itoa(rect.left, pos, 10);
				if( RegSetValueEx(hkResult1, "left", 0, REG_SZ, (BYTE*)pos, lstrlen(pos)+1) == ERROR_SUCCESS){	
					itoa(rect.top, pos, 10);
					if( RegSetValueEx(hkResult1, "top", 0, REG_SZ, (BYTE*)pos, lstrlen(pos)+1) == ERROR_SUCCESS){
						sprintf(pos, "%d", m_bHideTitlebar);
						RegSetValueEx(hkResult1, "Titlebar", 0, REG_SZ, (BYTE*)pos, lstrlen(pos)+1);
					}
				}
			}
			RegCloseKey(hkResult1);
		}
	}
	
	RegCloseKey(hkResult);
	RegCloseKey(hKey);
}

/***************************************************************
** 
** Responsible to read retrieve left, top coordinates of the application 
** window and title bar visibility status from the registry.
**  
***************************************************************/
//
void CHP12C_cDlg::ReadRegistry()
{
	int  left, top;
	HKEY hKey;
	TCHAR data[8];
	DWORD dwBufLen = 8; // 8 Bytes
	
	// read registy and reterive values
#ifdef C12
	if( RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Hewlett-Packard\\hp12C", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){
#endif
#ifdef C15
	if( RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Hewlett-Packard\\hp15C", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){
#endif
		if( RegQueryValueEx(hKey, "left",  NULL, NULL, (LPBYTE)data, &dwBufLen) == ERROR_SUCCESS)
			left = atoi(data); else left = 0;
	 	dwBufLen = 8;
		if(RegQueryValueEx(hKey, "top", NULL, NULL, (LPBYTE)data, &dwBufLen) == ERROR_SUCCESS)
			top = atoi(data); else	 top = 0;
		
		dwBufLen = 8;
		if(RegQueryValueEx(hKey, "Titlebar", NULL, NULL, (LPBYTE)data, &dwBufLen) == ERROR_SUCCESS)
			m_bHideTitlebar = atoi(data); else	m_bHideTitlebar = 0;
		RegCloseKey(hKey); hKey  = NULL;
	}
	
	RECT rect; GetClientRect(&rect);
	if(left < -(rect.right-rect.left)) left = 0;
	if(top < -(rect.bottom -rect.top)) top = 0;

	// change the window postion
	SetWindowPos(NULL, left, top, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	m_bHideTitlebar  = !m_bHideTitlebar;
	ShowHP12CTitlebar();

	//check/uncheck main menu
	CMenu *pMenu = GetMenu();
	if(m_bHideTitlebar)
		pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
	else
		pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);
}


/***************************************************************
** 
**  Called when the user chooses 'Hide Titlebar' sub-menu from the main menu
**  
***************************************************************/
//
void CHP12C_cDlg::OnHP12CShowcaptionMenu()
{
	ShowHP12CTitlebar();

	// check/uncheck main menu
	CMenu *pMenu = GetMenu();
	if(m_bHideTitlebar)
		pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
	else
		pMenu->CheckMenuItem(ID_HP20b_SHOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);

	AfxMessageBox("Right click on the HP logo and unckeck 'Hide Titlebar' to display the titlebar\r\n"
	              "to close the calculator, turn it OFF (pressing the second function key and the ON/CE key)", MB_OK | MB_ICONINFORMATION);
}

/***************************************************************
** 
**  The framework calls this member function after the CWnd object 
**	has been moved.
**  
***************************************************************/
//
void CHP12C_cDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	if(m_bHideTitlebar)
	{
		static int oldYPos = 1;
		if(oldYPos < 0){
			SetWindowPos(NULL, x-3,oldYPos-(::GetSystemMetrics(SM_CYCAPTION)+3), 0,0, SWP_NOSIZE|SWP_NOZORDER);
		}
		oldYPos = y;
	}
}

/***************************************************************
** 
**  The framework calls this member function when the display 
**  resolution has changed
**  
***************************************************************/
LPARAM CHP12C_cDlg::OnDisplayChange( WPARAM wp, LPARAM lp )
{ return 0;
	if( !(LOWORD(lp)<=800) )
	{
		CBitmap bmp; CImage img;
		bmp.LoadBitmapA(MAKEINTRESOURCE(IDB_BITMAP1));
		img.Attach((HBITMAP)bmp);
		// This case may occur when the user has started an application in the 800x600 resolution. 
		// In this resolution, m_rcWindowRect might contain junk values.
		if(m_rcWindowRect.left < 0){
			m_rcWindowRect.left = 0; m_rcWindowRect.top = 0;
			m_rcWindowRect.right = img.GetWidth(); m_rcWindowRect.bottom = img.GetHeight()+::GetSystemMetrics(SM_CYCAPTION)+::GetSystemMetrics(SM_CYMENU);
		}

		// change window postion and size
		if(m_rcWindowRect.left < 0 || m_rcWindowRect.top < 0 || m_rcWindowRect.left > ::GetSystemMetrics(SM_CXSCREEN) 
			|| m_rcWindowRect.top > ::GetSystemMetrics(SM_CYSCREEN) )
			this->SetWindowPos(NULL, 0,0, m_rcWindowRect.right-m_rcWindowRect.left, m_rcWindowRect.bottom-m_rcWindowRect.top,  0);
		else
			this->SetWindowPos(NULL, m_rcWindowRect.left,m_rcWindowRect.top, 
				m_rcWindowRect.right-m_rcWindowRect.left, m_rcWindowRect.bottom-m_rcWindowRect.top,  0);
	
		if(m_bHideTitlebar){
			// If title bar is hidden, set the window region 
			HRGN rgn;
			static POINT tableHighRes[116]= { {26, 8+21 }, {400, 8+21 }, {403, 8+21}, {406, 8+21 }, {409, 9+21 }, {411, 10 +21}, {412, 10 +21}, {414, 11 +21}, {415, 12 +21}, {416, 13 +21}, {417, 14 +21}, {418, 15 +21}, {419, 17 +21}, {420, 19 +21}, {421, 20 +21}, {422, 22 +21}, {423, 24 +21}, {423, 26 +21}, {424, 29 +21}, {424, 768 +21}, {424, 771 +21}, {423, 774 +21}, {422, 778 +21}, {421, 781 +21}, {420, 783 +21}, {419, 785 +21}, {418, 787 +21}, {416, 789 +21}, {415, 792 +21}, {413, 794 +21}, {412, 795 +21}, {410, 797 +21}, {408, 799 +21}, {405, 801 +21}, {403, 803 +21}, {401, 804 +21}, {397, 806 +21}, {394, 808 +21}, {392, 809 +21}, {390, 810 +21}, {387, 812 +21}, {385, 813 +21}, {382, 815 +21}, {380, 816 +21}, {378, 817 +21}, {376, 817 +21}, {374, 818 +21}, {372, 819 +21}, {370, 819 +21}, {53, 819+21}, {55, 819 +21}, {53, 819 +21}, {51, 818 +21}, {49, 817 +21}, {47, 816 +21}, {46, 816 +21}, {44, 815 +21}, {42, 815 +21}, {40, 814 +21}, {39, 813 +21}, {37, 812 +21}, {36, 811 +21}, {34, 811 +21}, {32, 810 +21}, {31, 809 +21}, {31, 809 +21}, {30, 808 +21}, {28, 807 +21}, {27, 806 +21}, {27, 805 +21}, {25, 805 +21}, {24, 804 +21}, {23, 803 +21}, {22, 803 +21}, {21, 802 +21}, {20, 801 +21}, {19, 800 +21}, {18, 799 +21}, {17, 798 +21}, {16, 797 +21}, {14, 795 +21}, {13, 794 +21}, {12, 793 +21}, {11, 791 +21}, {9, 788 +21}, {9, 787 +21}, {8, 784 +21}, {7, 782 +21}, {7, 781 +21}, {8, 779 +21}, {8, 777 +21}, {8, 775 +21}, {8, 774 +21}, {8, 772 +21}, {8, 29 +21}, {8, 27 +21}, {8, 25 +21}, {10, 24 +21}, {10, 23 +21}, {11, 22 +21}, {11, 21 +21}, {12, 20 +21}, {13, 19 +21}, {13, 18 +21}, {14, 17 +21}, {14, 16 +21}, {14, 15 +21}, {17, 14 +21}, {18, 13 +21}, {19, 12 +21}, {20, 12 +21}, {21, 11 +21}, {21, 10 +21}, {22, 10 +21}, {23, 9 +21}, {24, 8 +21}  };
			rgn = CreatePolygonRgn (tableHighRes, 114, ALTERNATE);	
			SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)&~WS_CAPTION); 
			SetWindowRgn(rgn, TRUE);
		}		
		// Display original bitmap
		m_Background.SetBitmap((HBITMAP)img);
		img.Detach(); bmp.Detach();
		bmp.DeleteObject(); img.Destroy();
		// change LCD position
		m_VirtualLCD.SetWindowPos(NULL, m_VirtualLCD.m_rcOrgLCDPos.left, 
			m_VirtualLCD.m_rcOrgLCDPos.top, 
			m_VirtualLCD.m_rcOrgLCDPos.right - m_VirtualLCD.m_rcOrgLCDPos.left, 
			m_VirtualLCD.m_rcOrgLCDPos.bottom - m_VirtualLCD.m_rcOrgLCDPos.top, 0);
		m_bImgShrinked = false;
    	return 0; 
	}
	else
	{
		GetWindowRect(&m_rcWindowRect); 
		ShrinkCalculator();
	}
	UpdateDlgScreen();
	UpdateScreenContent();
	updatescreen( calc );
	return 0;
}
/***************************************************************
** 
**  To cut the calculator bitmap (IDC_BITMAP1) when resolution has changed to 800x600
**  
***************************************************************/
//
void CHP12C_cDlg::ShrinkCalculator()
{
	const int yPosVirtualLCD = 62;
	if(m_bHideTitlebar){
		HRGN rgn;
		static POINT tableLowRes[4]= { {7, 0+22 }, {7, 557+29 }, {418, 557+29 }, {418, 0+22 } }; 
		rgn = CreatePolygonRgn (tableLowRes, 4, ALTERNATE);	
		SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)&~WS_CAPTION); 
		SetWindowRgn(rgn, TRUE);
	}
	BITMAP bmp1;
	CBitmap bmp, destBmp;
	CImage img, img2;
	// Load bitmap from resorce file 
	bmp.LoadBitmapA(MAKEINTRESOURCE(IDB_BITMAP1));
	img.Attach((HBITMAP)bmp);
	bmp.GetBitmap(&bmp1);
	destBmp.CreateBitmapIndirect(&bmp1);
	img2.Attach((HBITMAP)destBmp);

	// Resize the window as per resolution 
	SetWindowPos(NULL, 0, 0,img2.GetWidth(), (95)+(230-103)+((311+7)-(271-7))+((379+7)-(339-7))+ ((471+7)-(431-7))+((541+7)-(499-7))+((610+7)-(567-7))+((679+7)-(636-7))+	((749+7)-(706-7)), 0);
	int ydest = 0; int y = 0; int x = 0;
	// top HP
	for(y = 45; y < 95; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	//  LCD
	for(y = 103; y < 230; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	// row 1
	for(y = 271-7; y < 311+7; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	// row 2
	for(y = (339-7); y < (379+7); y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	//row 3
	for(y = 431-7; y < 471+7; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	//row 4
	for(y = 499-7; y < 541+7; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	//row 5
	for(y = 567-7; y < 610+7; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	//row 6
	for(y = 636-7; y < 679+7; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	//row 6
	for(y = 706-7; y < 749+7; y++, ydest++){
		for(x  = 0; x < img.GetWidth(); x++){
			COLORREF org = img.GetPixel(x,y);	img2.SetPixel(x,ydest, org);
		}
	}
	
	m_Background.SetBitmap((HBITMAP)img2);
	img2.Detach();img.Detach();
	bmp.Detach();destBmp.Detach();bmp.DeleteObject();destBmp.DeleteObject();
	img2.Destroy();		img.Destroy();
	m_bImgShrinked = true;
	m_VirtualLCD.SetWindowPos(NULL, m_VirtualLCD.hpGetXPos()-3, yPosVirtualLCD, m_VirtualLCD.hpGetWidth(),  m_VirtualLCD.hpGetHeight(), 0);
}


void CHP12C_cDlg::OnStnClickedStaticBg()
{
  // TODO: Add your control notification handler code here
}

void CHP12C_cDlg::OnCalculatorAssignasdefaulthpcalculator()
{
}

void CHP12C_cDlg::OnCalculatorManagehpcalculatoremulators()
{
}

void CHP12C_cDlg::OnHelpAdvancedhelp()
{
}
