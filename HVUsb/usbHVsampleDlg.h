// usbHVsampleDlg.h : header file
//

#pragma once

#define EnabledTextColor RGB(255,255,255)// output enabled inidcator
#define EnabledBrushColor RGB(255,0,0)
#define DisabledTextColor RGB(0,0,0)
#define DisabledBrushColor RGB(150,150,150)

#define TBSIZE 200	// size of temp character buffer

// CusbHVsampleDlg dialog
class CusbHVsampleDlg : public CDialog
{
// Construction
public:
	CusbHVsampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_USBHVSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;					// icon for status bar
	HINSTANCE usbHVlib;				// handle to usbHVcom.dll
	int err;						// error return from usbHVcom functions
	CString MBTitle;				// title string for message boxes
	CString PartName;				// EMCO part name USBHV-xxx
	unsigned char Status;			// latest status return from usbHV
	bool Enabled;					// output enable status
	bool Connected;					// usb connection status
	bool ConnAtStart;				// set true if device is attached at app start
	float Vmon;						// output voltage monitor
	float MaxI;						// max current output
	float MaxV;						// max voltage output
	float VpgmReq;					// requested programming voltage
	CString Prec;					// recommended floating point precision format string
	const char *FWVer;				// pointer to firmaware revision string
	char TmpBuf[TBSIZE];			// global temporary text buffer

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR TimerID);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
public:
	afx_msg void OnBnClickedDisable();
	afx_msg void OnBnClickedEnable();
	void StartDatacqTimer(void);// start the data acquisition timer
	void StopDatacqTimer(void);	// stop the data acquisition timer
	void Cleanup(void);			// free handles, libraries, etc before exit
	void GetPartInfo(void);		// get info about attached emco device
	CBrush DisabledBrush;		// background brush for output disabled state
	CBrush EnabledBrush;		// background brush for output enabled state
public:
	afx_msg void OnBnClickedSetvpgm();
};
