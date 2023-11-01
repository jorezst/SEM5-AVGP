
// DirectSoundDlg.h : header file
//

#pragma once
#include "CDirectSound.h"


// CDirectSoundDlg dialog
class CDirectSoundDlg : public CDialogEx
{
// Construction
public:
	CDirectSoundDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTSOUND_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CDirectSound m_ds;
	LPDIRECTSOUNDBUFFER lpDSB264, lpDSBTonleiter, lpDSBDrei[3], lpDSBKlavier[9];
	double ton[9];
	int c = 264;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClicked264hz();
	afx_msg void OnBnClickedEnde();
	afx_msg void OnBnClickedCdurtonleiter();
	afx_msg void OnBnClickedStop();
};
