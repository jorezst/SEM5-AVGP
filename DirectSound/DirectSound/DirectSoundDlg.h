
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
	LPDIRECTSOUNDBUFFER lpDSB264, lpDSBTonleiter, lpDSBDreiklang[3], lpDSBKlavier[9];
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
	afx_msg void OnNMCustomdrawLautstaerke(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawBalance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedKlavierc();
	afx_msg void OnBnClickedKlavierd();
	afx_msg void OnBnClickedKlaviere();
	afx_msg void OnBnClickedKlavierf();
	afx_msg void OnBnClickedKlavierg();
	afx_msg void OnBnClickedKlaviera();
	afx_msg void OnBnClickedKlavierh();
	afx_msg void OnBnClickedKlavierc2();
	afx_msg void OnBnClickedPcmsound();
	afx_msg void OnBnClickedCdurdreiklang();
	afx_msg void OnBnClickedCheck1();
};
