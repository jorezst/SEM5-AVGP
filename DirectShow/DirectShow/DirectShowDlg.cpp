
// DirectShowDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DirectShow.h"
#include "DirectShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDirectShowDlg dialog

static UINT NEAR WM_GRAPHNOTIFY = RegisterWindowMessage(L"GRAPHNOTIFY");



CDirectShowDlg::CDirectShowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTSHOW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirectShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDirectShowDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PLAY, &CDirectShowDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, &CDirectShowDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_RESUME, &CDirectShowDlg::OnBnClickedResume)
	ON_REGISTERED_MESSAGE(WM_GRAPHNOTIFY, GetIt)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CDirectShowDlg::OnBnClickedStop)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_FULLSCREEN, &CDirectShowDlg::OnBnClickedFullscreen)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_FILE, &CDirectShowDlg::OnBnClickedFile)
END_MESSAGE_MAP()


// CDirectShowDlg message handlers

BOOL CDirectShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	directshow.setWindow((OAHWND)GetSafeHwnd());
	directshow.setNotifyWindow(WM_GRAPHNOTIFY);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDirectShowDlg::OnPaint()
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
HCURSOR CDirectShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDirectShowDlg::GetIt(WPARAM wparam, LPARAM lparam) {
	return directshow.GetIt(wparam, lparam);
}

// Timer fragt regelmäßig ab, an welcher Stelle der Film gerade ist
void CDirectShowDlg::OnTimer(UINT_PTR nIDEvent) {
	REFERENCE_TIME rtTotal, rtNow = 0;
	CString s;
	CSliderCtrl* sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	directshow.seek(&rtTotal, &rtNow);

	if ((rtNow * 100) / rtTotal == 100) {
		directshow.setZero();
		directshow.Pause();
		directshow.fullscreen(FALSE);
		sl->SetPos(0);
	}
	s.Format(L"%02d:%02d (%d%%)",
		(int)((rtNow / 10000000L) / 60), // min
		(int)((rtNow / 10000000L) % 60), // sek
		(int)((rtNow * 100) / rtTotal)); // Prozent
	GetDlgItem(IDC_STATUS)->SetWindowText(s);

	REFERENCE_TIME d;
	d = directshow.getLength();

	sl->SetRange(0, (int)(d / 1000000));
	sl->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos((int)(rtNow / 1000000));

	CDialogEx::OnTimer(nIDEvent);
}

void CDirectShowDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSliderCtrl* sl1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	if ((void*)sl1 == (void*)pScrollBar) {
		REFERENCE_TIME pos = (REFERENCE_TIME)sl1->GetPos() * 1000000;
		directshow.setCurrentPosition(pos);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDirectShowDlg::OnBnClickedPlay() {
	directshow.setVideoWindow();
	directshow.Run();
	SetTimer(1, 200, NULL);
}

void CDirectShowDlg::OnBnClickedPause() {
	directshow.Pause();
}

void CDirectShowDlg::OnBnClickedResume() {
	directshow.Resume();
}

void CDirectShowDlg::OnBnClickedFullscreen() {
	directshow.fullscreen(TRUE);

}

void CDirectShowDlg::OnBnClickedStop() {
	// Timer vorher beenden, damit keine Werte mehr im Timer aufgerufen werden können
	KillTimer(1);
	directshow.Stop();
}

void CDirectShowDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	directshow.fullscreen(FALSE);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDirectShowDlg::OnBnClickedFile() {

	CString szFilters = L"Media Files|*.mpg;*.avi;*.wma;*.mov;*.wav;*.mp2;*.mp3;*.mp4|All Files (*.*)|*.*||";
	// Erstelle einen OpenDialog
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

	// Zeige den FileDialog ; Wenn der Nutzer OK klickt -> fileDlg.DoModal()
	if (fileDlg.DoModal() == IDOK)
	{
		directshow.setFilename(fileDlg.GetPathName());
		CString gfn = fileDlg.GetFileName();
		GetDlgItem(IDC_FILENAME)->SetWindowText(gfn);
	}
}