
// SLHIDDevice.h : main header file for the SLab HID Device application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSLHIDDeviceApp:
// See SLHIDDevice.cpp for the implementation of this class
//

class CSLHIDDeviceApp : public CWinApp
{
public:
	CSLHIDDeviceApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSLHIDDeviceApp theApp;