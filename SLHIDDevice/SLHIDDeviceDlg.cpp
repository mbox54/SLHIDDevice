
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
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CSLHIDDeviceDlg::OnBnClickedButtonChoose)

	ON_CBN_CLOSEUP(IDC_COMBO_HIDLIST, &CSLHIDDeviceDlg::OnCbnCloseupComboHidlist)
	ON_CBN_DROPDOWN(IDC_COMBO_HIDLIST, &CSLHIDDeviceDlg::OnCbnDropdownComboHidlist)
END_MESSAGE_MAP()

// -------------------------------------------------------------------
// CSLHIDDeviceDlg message handlers
// -------------------------------------------------------------------
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
	// > Get string
	CString str_sInput;
	
	UpdateData(TRUE);
	str_sInput.Append(this->m_sEdit_Input);

	// > Check control content
	if (str_sInput == "")
	{
		// [EMPTY]

		// NOP
		return;
	}

	// > Output raw command
	Trace(str_sInput);
	Trace(_T("\n"));

	// > Parse string
	BYTE act = 0;

	if (str_sInput == "exit")
	{
		Trace(_T("exit program\n"));

		OnCancel();

		act = 1;
	}

	if (str_sInput == "show devices")
	{
		ShowAllDevices();

		act = 1;
	}

	if (act == 0)
	{
		// [UNKNOWN COMMAND]

		Trace(_T("unknown command\n"));
	}
	else
	{
		// [VALID COMMAND]

	}

	// clear Input control
	m_sEdit_Input.Truncate(0);

	UpdateData(FALSE);
	
}

// output full info about all found HID Devices
void CSLHIDDeviceDlg::ShowAllDevices()
{
	DWORD numDevices;

	// > Get number of HID devices with matching VID/PID (0/0 means not filtered)
	numDevices = HidDevice_GetNumHidDevices(VID, PID);

	Trace(_T("HID Device count: %d\n"), numDevices);

	if (numDevices > 0)
	{
		// [Devices found]

		Trace(_T("--------------------------\n"));

		// > Iterate through each HID device with matching VID/PID
		

		for (DWORD k = 0; k < numDevices; k++)
		{
			Trace(_T("HID Device %02d:\n"), k);

			// > Output serial strings to control

			char deviceInfo0[MAX_VID_LENGTH]; 
			Trace(_T("Vendor ID: "));
			if (HidDevice_GetHidString(k, VID, PID, HID_VID_STRING, deviceInfo0, MAX_VID_LENGTH) == HID_DEVICE_SUCCESS)
			{
				Trace(_T("%S \n"), deviceInfo0);
			}
			else
			{
				Trace(_T("failure to get.. \n"));
			}

			char deviceInfo1[MAX_PID_LENGTH];
			Trace(_T("Product ID: "));
			if (HidDevice_GetHidString(k, VID, PID, HID_PID_STRING, deviceInfo1, MAX_PID_LENGTH) == HID_DEVICE_SUCCESS)
			{
				Trace(_T("%S \n"), deviceInfo1);
			}
			else
			{
				Trace(_T("failure to get.. \n"));
			}

			char deviceInfo2[MAX_PATH_LENGTH];
			Trace(_T("Path String: "));
			if (HidDevice_GetHidString(k, VID, PID, HID_PATH_STRING, deviceInfo2, MAX_PATH_LENGTH) == HID_DEVICE_SUCCESS)
			{
				CString str_tmp;
				WORD uiTmpStrLength = strlen(deviceInfo2);

				for (WORD kk = 0; kk < uiTmpStrLength; kk++)
				{
					str_tmp.AppendChar(deviceInfo2[kk]);
				}

				str_tmp.AppendChar('\n');
				Trace(str_tmp);
			}
			else
			{
				Trace(_T("failure to get.. \n"));
			}

			char deviceInfo3[MAX_SERIAL_STRING_LENGTH];
			Trace(_T("Serial String: "));
			if (HidDevice_GetHidString(k, VID, PID, HID_SERIAL_STRING, deviceInfo3, MAX_SERIAL_STRING_LENGTH) == HID_DEVICE_SUCCESS)
			{
				CString str_tmp;
				WORD uiTmpStrLength = strlen(deviceInfo3);

				for (WORD kk = 0; kk < uiTmpStrLength; kk++)
				{
					str_tmp.AppendChar(deviceInfo3[kk]);
				}

				str_tmp.AppendChar('\n');
				Trace(str_tmp);
			}
			else
			{
				Trace(_T("failure to get.. \n"));
			}

			char deviceInfo4[MAX_MANUFACTURER_STRING_LENGTH];
			Trace(_T("Manufacturer String: "));
			if (HidDevice_GetHidString(k, VID, PID, HID_MANUFACTURER_STRING, deviceInfo4, MAX_MANUFACTURER_STRING_LENGTH) == HID_DEVICE_SUCCESS)
			{
				CString str_tmp;
				WORD uiTmpStrLength = strlen(deviceInfo4);

				for (WORD kk = 0; kk < uiTmpStrLength; kk++)
				{
					str_tmp.AppendChar(deviceInfo4[kk]);
				}

				str_tmp.AppendChar('\n');
				Trace(str_tmp);
			}
			else
			{
				Trace(_T("failure to get.. \n"));
			}

			char deviceInfo5[HID_PRODUCT_STRING];
			Trace(_T("Product String: "));
			if (HidDevice_GetHidString(k, VID, PID, HID_PRODUCT_STRING, deviceInfo5, HID_PRODUCT_STRING) == HID_DEVICE_SUCCESS)
			{
				CString str_tmp;
				WORD uiTmpStrLength = strlen(deviceInfo5);

				for (WORD kk = 0; kk < uiTmpStrLength; kk++)
				{
					str_tmp.AppendChar(deviceInfo5[kk]);
				}

				str_tmp.AppendChar('\n');
				Trace(str_tmp);
			}
			else
			{
				Trace(_T("failure to get.. \n"));
			}

			Trace(_T(" \n"));
		}

	}//if (numDevices > 0)


}


// -------------------------------------------------------------------
// HID Functionality
// -------------------------------------------------------------------

// read System HID Devices, list their Serial string
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
		// GetSelectedDevice(serial);

		// > Remove all strings from the combobox control
		m_comboDeviceList.ResetContent();

		// > Get number of HID devices with matching VID/PID (0/0 means not filtered)
		numDevices = HidDevice_GetNumHidDevices(VID, PID);

		if (numDevices > 0)
		{
			// [Devices found]
	
			// > Iterate through each HID device with matching VID/PID
			for (DWORD k = 0; k < numDevices; k++)
			{
				// Add serial strings to the combobox
				if (HidDevice_GetHidString(k, VID, PID, HID_PID_STRING, deviceString, MAX_SERIAL_STRING_LENGTH) == HID_DEVICE_SUCCESS)
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

// try to find HID Device from selected Serial string
BOOL CSLHIDDeviceDlg::FindDevice(CString serial, DWORD & deviceNum)
{
	
	char str_deviceString[MAX_SERIAL_STRING_LENGTH];

	// > Get number of HID devices with matching VID/PID (0/0 means not filtered)
	DWORD numDevices = HidDevice_GetNumHidDevices(VID, PID);

	BOOL bFound = FALSE;

	if (numDevices > 0)
	{
		// [Devices found]

		// > Iterate through each HID device with matching VID/PID
		for (DWORD k = 0; k < numDevices; k++)
		{
			if (HidDevice_GetHidString(k, VID, PID, HID_PID_STRING, str_deviceString, MAX_SERIAL_STRING_LENGTH) == HID_DEVICE_SUCCESS)
			{
				// [DEVICE OBTAINED]

				if (serial.CompareNoCase(CString(str_deviceString)) == 0)
				{
					// [MATCH FOUND]					
					deviceNum = k;

					bFound = TRUE;

					break;
				}
			}//if (HidDevice_GetHidString
		}//for (DWORD k = 0; k < numDevices; k++)
	}//if (numDevices > 0)

	return bFound;
}

// Get the combobox device selection
// If a device is selected, return TRUE and return the serial string
// Otherwise, return FALSE
BOOL CSLHIDDeviceDlg::GetSelectedDevice(CString & serial)
{
	CString		str_selText;

	// Get current selection index or 
	// CB_ERR(-1) if no device is selected
	int	iSel = m_comboDeviceList.GetCurSel();

	BOOL bSelected = FALSE;

	if (iSel != CB_ERR)
	{
		// Get the selected device string
		m_comboDeviceList.GetLBText(iSel, str_selText);

		serial = str_selText;

		bSelected = TRUE;
	}

	return bSelected;
}


void CSLHIDDeviceDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	// CDialogEx::OnOK();
}


// -------------------------------------------------------------------
// Control Events
// -------------------------------------------------------------------
void CSLHIDDeviceDlg::OnBnClickedButtonChoose()
{
	MessageBox(_T("Choose"), _T("Msg1"));

}

void CSLHIDDeviceDlg::OnCbnCloseupComboHidlist()
{
	
	CString		serial;
	DWORD		deviceNum;

	// > Check for valid selected control Item
	if (GetSelectedDevice(serial))
	{
		// > Check if the selected device has been removed
		if (!FindDevice(serial, deviceNum))
		{
			// [DEVICE LOST]

			// Then update the device list
			UpdateDeviceList();
			//UpdateDeviceInformation(FALSE);
		}
		else
		{
			// [DEVICE EXISTS]

			// output to controls
			serial.AppendChar('\n');

			Trace(serial);
		}
	}

}


void CSLHIDDeviceDlg::OnCbnDropdownComboHidlist()
{
	// Load control with List of HID Devices Serial string
	UpdateDeviceList();
}
