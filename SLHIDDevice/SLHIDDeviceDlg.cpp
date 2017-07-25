
// SLHIDDeviceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SLHIDDevice.h"
#include "SLHIDDeviceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ===================================================================
// CAboutDlg dialog used for App About
// ===================================================================
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ===================================================================
// CSLHIDDeviceDlg dialog
// ===================================================================
CSLHIDDeviceDlg::CSLHIDDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SLHIDDEVICE_DIALOG, pParent)
	, m_sEdit_Input(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSLHIDDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_EDIT_STATUS);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_sEdit_Input);
	DDV_MaxChars(pDX, m_sEdit_Input, 32);
	DDX_Control(pDX, IDC_COMBO_HIDLIST, m_comboDeviceList);
}

BEGIN_MESSAGE_MAP(CSLHIDDeviceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSLHIDDeviceDlg::OnBnClickedOk)
//	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CSLHIDDeviceDlg::OnBnClickedButtonChoose)
END_MESSAGE_MAP()

// -------------------------------------------------------------------
// CSLHIDDeviceDlg message handlers
// -------------------------------------------------------------------
BOOL CSLHIDDeviceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSLHIDDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSLHIDDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSLHIDDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// -------------------------------------------------------------------
// Support Functionality
// -------------------------------------------------------------------
// print to output control (CEdit)
void CSLHIDDeviceDlg::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	m_EDIT_STATUS.GetWindowText(strWndText);
	strWndText += str;
	m_EDIT_STATUS.SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	m_EDIT_STATUS.LineScroll(-m_EDIT_STATUS.GetLineCount());
	m_EDIT_STATUS.LineScroll(m_EDIT_STATUS.GetLineCount() - 4);
}

// print Line from InputEdit to output control
void CSLHIDDeviceDlg::Input()
{
	// get string
	CString str_sInput;
	
	UpdateData(TRUE);

	str_sInput.Append(this->m_sEdit_Input);
	str_sInput.AppendChar('\n');

	Trace(str_sInput);

}

void CSLHIDDeviceDlg::UpdateDeviceList()
{
	// Only update the combo list when the drop down list is closed
	if (!m_comboDeviceList.GetDroppedState())
	{
		int						sel;
		CString					serial;
		DWORD					numDevices;
		char					deviceString[MAX_SERIAL_STRING_LENGTH];

		// Get previous combobox string selection
		//GetSelectedDevice(serial);

		// Remove all strings from the combobox
		m_comboDeviceList.ResetContent();

		// Get number of HID devices with matching VID/PID (0/0 means not filtered)
		numDevices = HidDevice_GetNumHidDevices(VID, PID);

		if (numDevices > 0)
		{
			// [Devices found]
	
			// Iterate through each HID device with matching VID/PID
			for (DWORD k = 0; k < numDevices; k++)
			{
				// Add serial strings to the combobox
				if (HidDevice_GetHidString(k, VID, PID, HID_SERIAL_STRING, deviceString, MAX_SERIAL_STRING_LENGTH) == HID_DEVICE_SUCCESS)
				{
					m_comboDeviceList.AddString(CString(deviceString));
				}
			}
		}

		/*	// interface corrections
		sel = m_comboDeviceList.FindStringExact(-1, serial);

		// Select first combobox string
		if (sel == CB_ERR)
		{
			m_comboDeviceList.SetCurSel(0);
		}
		// Restore previous combobox selection
		else
		{
			m_comboDeviceList.SetCurSel(sel);
		}
		*/
	}

}


void CSLHIDDeviceDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	// CDialogEx::OnOK();
}


//void CSLHIDDeviceDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}


void CSLHIDDeviceDlg::OnBnClickedButtonChoose()
{
	MessageBox(_T("Choose"), _T("Msg1"));

}


BOOL CSLHIDDeviceDlg::PreTranslateMessage(MSG* pMsg)
{
	// catch KEY_DOWD = VK_RETURN
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			// Proc Input String
			Input();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
