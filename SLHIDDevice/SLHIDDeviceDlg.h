
// SLHIDDeviceDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Library Dependencies
#pragma comment (lib, "SLABHIDDevice.lib")		// DLL, HID commands implementation


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABHIDDevice.h"						// DLL, HID commands interface
#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define VID					0
#define PID					0


// ===================================================================
// CSLHIDDeviceDlg dialog
// ===================================================================
class CSLHIDDeviceDlg : public CDialogEx
{
// Construction
public:
	CSLHIDDeviceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLHIDDEVICE_DIALOG };
#endif

	CEdit m_EDIT_STATUS;
	CString m_sEdit_Input;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

// Support Functionality
protected:
	void Trace(LPCTSTR szFmt, ...);
	void Input();

// HID Functionality
public:
	// 
	void UpdateDeviceList();




	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonChoose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CComboBox m_comboDeviceList;
};
