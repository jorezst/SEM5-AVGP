
// MCIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MCI.h"
#include "MCIDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMCIDlg dialog



CMCIDlg::CMCIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MCI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMCIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMCIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMCIDlg::OnBnClickedButtonOpenVideo)
	ON_BN_CLICKED(IDC_BUTTON2, &CMCIDlg::OnBnClickedButtonOpenMP3)
	ON_BN_CLICKED(IDC_BUTTON3, &CMCIDlg::OnBnClickedButtonOpenMIDI)
	ON_BN_CLICKED(IDC_BUTTON4, &CMCIDlg::OnBnClickedButtonOpenCD)
	ON_BN_CLICKED(IDC_BUTTON5, &CMCIDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON6, &CMCIDlg::OnBnClickedButtonPause)
	ON_LBN_SELCHANGE(IDC_TRACKLIST, &CMCIDlg::OnLbnSelchangeTracklist)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON9, &CMCIDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON8, &CMCIDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON7, &CMCIDlg::OnBnClickedButtonOpenWAV)
END_MESSAGE_MAP()


// CMCIDlg message handlers

BOOL CMCIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	SetTimer(1, 200, 0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMCIDlg::OnPaint()
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
HCURSOR CMCIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMCIDlg::OnBnClickedButtonOpenVideo()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.

	CRect r;
	GetDlgItem(IDC_DESTIN)->GetWindowRect(r);
	ScreenToClient(r);
	mci.OpenFile(L"test.mpg");
	mci.SetVideoPosition(GetSafeHwnd(),
		CRect(r.left, r.top, r.Width(), r.Height()));
	
	mci.Play();
}


void CMCIDlg::OnBnClickedButtonOpenMP3()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.

	mci.OpenFile(L"maus_castor_stereo.mp3");
	mci.Play();
}


void CMCIDlg::OnBnClickedButtonOpenMIDI()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.


	mci.OpenFile(L"canyon.mid");
	mci.Play();
}


void CMCIDlg::OnBnClickedButtonOpenCD()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	BYTE tracks;
	mci.OpenAudioCD(0, tracks);

	BYTE min, sek, frame;
	((CListBox*)GetDlgItem(IDC_TRACKLIST))->ResetContent();
	RedrawWindow();
	for (int i = 1; i <= tracks; i++) {
		mci.GetTrackOrMediaLength(i, min, sek, frame);
		CString temp; // z.B.Eintrag in eine ListBox : -)
		temp.Format(L"[%02d] %02d:%02d", i, min, sek);
		((CListBox*)GetDlgItem(IDC_TRACKLIST))->AddString(temp);
	}

	
	mci.Play();
}


void CMCIDlg::OnBnClickedButtonPlay()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	mci.Play();
}


void CMCIDlg::OnBnClickedButtonPause()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	mci.Pause();
}

void CMCIDlg::OnBnClickedButtonOpenWAV()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	mci.OpenFile(L"CLASSICA.WAV");
	mci.Play();
}

void CMCIDlg::OnBnClickedButtonClose()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	mci.Close();
}

void CMCIDlg::OnBnClickedButtonExit()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	OnOK();
}


void CMCIDlg::OnLbnSelchangeTracklist()
{
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	mci.TMSFSeek(((CListBox*)GetDlgItem(IDC_TRACKLIST))->GetCurSel() + 1, 0, 0, 0);
	mci.Play();
}


void CMCIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Fügen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.
	CString str;
	unsigned char t, m, s, f;
	mci.GetTMSFPosition(t, m, s, f);

	int akt = m * 60 + s;

	unsigned char min, sek, frame;
	mci.GetTrackOrMediaLength(t, min, sek, frame);
	int ges = min * 60 + sek;

	int progress;
	if (ges > 0) {
		progress = 100 * akt / ges;
	}
	else {
		progress = 0;
	}


	str.Format(L"[%02d] %02d:%02d - %d%%", t, m, s, progress);
	SetDlgItemText(IDC_TIME, str);

	CDialogEx::OnTimer(nIDEvent);
}





