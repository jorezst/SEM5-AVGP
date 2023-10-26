
// MCIDlg.h : header file
//

#pragma once

#include "CMCIObject.h"


// CMCIDlg dialog
class CMCIDlg : public CDialogEx
{
// Construction
public:
	CMCIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MCI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CMCIObject mci;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenVideo();
	afx_msg void OnBnClickedButtonOpenMP3();
	afx_msg void OnBnClickedButtonOpenMIDI();
	afx_msg void OnBnClickedButtonOpenCD();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnLbnSelchangeTracklist();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonOpenWAV();
};
