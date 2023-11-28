
// DirectShowDlg.h : header file
//

#pragma once


// CDirectShowDlg dialog
class CDirectShowDlg : public CDialogEx
{
// Construction
public:
	CDirectShowDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTSHOW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	IGraphBuilder* pGraph; // ein Zeiger auf das COM-Interface
	IMediaControl* pMediaControl;
	IMediaEventEx* pEvent;
	IVideoWindow* pVidWin = NULL;
	IMediaSeeking* pSeek = NULL;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void CleanUp();
	void Fullscreen(bool v);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedResume();
	afx_msg LRESULT GetIt(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedFullscreen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
