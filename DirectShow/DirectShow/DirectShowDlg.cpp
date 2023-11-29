
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
END_MESSAGE_MAP()


// CDirectShowDlg message handlers

BOOL CDirectShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CoInitialize(NULL); // zur Initialisierung des COM-Interfaces

	SetTimer(1, 200, NULL);

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


void CDirectShowDlg::OnBnClickedPlay()
{
	pMediaControl = 0;
	pVidWin = 0;
	pEvent = 0;
	pSeek = 0;
	pGraph = 0;

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&pGraph);

	pGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
	pGraph->QueryInterface(IID_IMediaEventEx, (void**)&pEvent);

	pGraph->RenderFile(L"test.mpg", NULL);
	
	pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);
	pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeek);
	pGraph->QueryInterface(IID_IBasicAudio, (void**)&au);

	// set timeformat to 100-nanoseconds units
	if (pSeek->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME) == S_OK) {
		pSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	}
	else {
		AfxMessageBox(L"Zeitformat wird nicht unterstützt");
	}

	if (pSeek) {
		REFERENCE_TIME d;
		pSeek->GetDuration(&d);
		CSliderCtrl* sl;
		sl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
		sl->SetRange(0, (int)(d / 1000000)); sl->SetPos(0);
	}

	pVidWin->put_Owner((OAHWND)GetSafeHwnd());
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pVidWin->put_Visible(OATRUE);

	pVidWin->SetWindowPosition(10, 70, 300, 200);

	// Nachrichtenbehandlung (Maus, Keyboard)

	pEvent->SetNotifyWindow((OAHWND)GetSafeHwnd(), WM_GRAPHNOTIFY, 0);
	pVidWin->put_MessageDrain((OAHWND)GetSafeHwnd());

	pMediaControl->Run(); long evCode;
}


LRESULT CDirectShowDlg::GetIt(WPARAM wparam, LPARAM lparam) {
	long evCode, param1, param2; HRESULT hr;
	while (SUCCEEDED(pEvent->GetEvent(&evCode, &param1, &param2, 0))) {
		pEvent->FreeEventParams(evCode, param1, param2);
		switch (evCode) {
		case EC_COMPLETE:
		case EC_USERABORT:
			//CleanUp(); 
			return 0;
		}
	}
	return 0;
}


void CDirectShowDlg::CleanUp() {
	if (pGraph) {
		Fullscreen(false);
		pVidWin->put_Visible(OAFALSE);
		pVidWin->put_Owner(NULL);
		pMediaControl->Release();
		pVidWin->Release();
		pEvent->Release();
		pSeek->Release();
		au->Release();
		pGraph->Release();
		pMediaControl = 0;
		pVidWin = 0;
		pEvent = 0;
		pSeek = 0;
		pGraph = 0;
		
	}
	//CoUninitialize();
}


void CDirectShowDlg::OnBnClickedPause()
{
	if (pMediaControl != 0) {
		pMediaControl->Pause();
	}
		
}


void CDirectShowDlg::OnBnClickedResume()
{
	if (pMediaControl != 0) {
		pMediaControl->Run();
	}
		
}


void CDirectShowDlg::OnBnClickedStop()
{
	if (pMediaControl != 0) {
		pMediaControl->Stop(); CleanUp();
	}
}
 

void CDirectShowDlg::OnBnClickedFullscreen()
{
	Fullscreen(true);
}


void CDirectShowDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	Fullscreen(false);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDirectShowDlg::Fullscreen(bool v) {
	if (pGraph) {
		IVideoWindow* pVidWin1 = NULL;
		pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin1);
		pVidWin1->put_FullScreenMode(v ? OATRUE : OAFALSE);
		pVidWin1->Release();
	}
}


void CDirectShowDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (pSeek) {
		REFERENCE_TIME rtTotal, rtNow = 0; CString s;
		pSeek->GetDuration(&rtTotal);
		pSeek->GetCurrentPosition(&rtNow);
		s.Format(L"Abspielvorgang: %02d:%02d (%d%%)",
			(int)((rtNow / 10000000L) / 60), // min
			(int)((rtNow / 10000000L) % 60), // sek
			(int)((rtNow * 100) / rtTotal)); // Prozent
		GetDlgItem(IDC_STATUS)->SetWindowText(s);

		((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos((int)(rtNow / 1000000));
	}
	else {
		GetDlgItem(IDC_STATUS)->SetWindowText(L"Abspielvorgang: 00:00 (0%)");
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos(0);
	}
	

	CDialogEx::OnTimer(nIDEvent);
}


void CDirectShowDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pMediaControl != 0) {
		pMediaControl->Pause();
	}
	CSliderCtrl* sl1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	if ((pSeek) && ((void*)sl1 == (void*)pScrollBar)) {
		REFERENCE_TIME pos = (REFERENCE_TIME)sl1->GetPos() * 1000000;
		pSeek->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
			NULL, AM_SEEKING_NoPositioning);
	}
	if (pMediaControl != 0) {
		pMediaControl->Run();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}