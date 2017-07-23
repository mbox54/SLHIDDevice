
// SLHIDDeviceDlg.h : header file
//

#pragma once
#pragma comment (lib, "SLABHIDDevice.lib")		// DLL, HID commands implementation

#include "SLABHIDDevice.h"			// DLL, HID commands interface


// CSLHIDDeviceDlg dialog
class CSLHIDDeviceDlg : public CDialogEx
{
// Construction
public:
	CSLHIDDeviceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLHIDDEVICE_DIALOG };
#endif

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
};
