
// DirectSoundDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DirectSound.h"
#include "DirectSoundDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDirectSoundDlg dialog



CDirectSoundDlg::CDirectSoundDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTSOUND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirectSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDirectSoundDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_264Hz, &CDirectSoundDlg::OnBnClicked264hz)
	ON_BN_CLICKED(IDC_Ende, &CDirectSoundDlg::OnBnClickedEnde)
	ON_BN_CLICKED(IDC_CDurTonleiter, &CDirectSoundDlg::OnBnClickedCdurtonleiter)
	ON_BN_CLICKED(IDC_Stop, &CDirectSoundDlg::OnBnClickedStop)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Lautstaerke, &CDirectSoundDlg::OnNMCustomdrawLautstaerke)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Balance, &CDirectSoundDlg::OnNMCustomdrawBalance)
	ON_BN_CLICKED(IDC_KlavierC, &CDirectSoundDlg::OnBnClickedKlavierc)
	ON_BN_CLICKED(IDC_KlavierD, &CDirectSoundDlg::OnBnClickedKlavierd)
	ON_BN_CLICKED(IDC_KlavierE, &CDirectSoundDlg::OnBnClickedKlaviere)
	ON_BN_CLICKED(IDC_KlavierF, &CDirectSoundDlg::OnBnClickedKlavierf)
	ON_BN_CLICKED(IDC_KlavierG, &CDirectSoundDlg::OnBnClickedKlavierg)
	ON_BN_CLICKED(IDC_KlavierA, &CDirectSoundDlg::OnBnClickedKlaviera)
	ON_BN_CLICKED(IDC_KlavierH, &CDirectSoundDlg::OnBnClickedKlavierh)
	ON_BN_CLICKED(IDC_KlavierC2, &CDirectSoundDlg::OnBnClickedKlavierc2)
	ON_BN_CLICKED(IDC_PcmSound, &CDirectSoundDlg::OnBnClickedPcmsound)
	ON_BN_CLICKED(IDC_CDurDreiklang, &CDirectSoundDlg::OnBnClickedCdurdreiklang)
END_MESSAGE_MAP()


// CDirectSoundDlg message handlers

BOOL CDirectSoundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (!m_ds.Create(this))
		OnCancel();

	// init major scale C
	ton[0] = c;
	ton[1] = c * 9 / 8.;
	ton[2] = c * 5 / 4.;
	ton[3] = c * 4 / 3.;
	ton[4] = c * 3 / 2.;
	ton[5] = c * 5 / 3.;
	ton[6] = c * 15 / 8.;
	ton[7] = c * 2;
	ton[8] = 0;

	
	// create a 4 second sound buffer
	if ((lpDSB264 = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();

	if ((lpDSBTonleiter = m_ds.CreateSoundBuffer(2, 16, 22050, 4)) == 0)
		OnCancel();

	// create 3 sound buffers
	for (int i = 0; i < 3; i++) {
		if ((lpDSBDreiklang[i] = m_ds.CreateSoundBuffer(2, 16, 22050, 2)) == 0)
			OnCancel();
		m_ds.GenerateSound(lpDSBDreiklang[i], 0, 2, ton[2 * i]);
	}

	// create 9 sound buffers
	for (int i = 0; i < 9; i++) {
		if ((lpDSBKlavier[i] = m_ds.CreateSoundBuffer(2, 16, 22050, 2)) == 0)
			OnCancel();
		m_ds.GenerateSound(lpDSBKlavier[i], 0, 1, ton[i]);
	}
	


	// Default Values for Lautstaerke and Balance
	((CSliderCtrl*)GetDlgItem(IDC_Lautstaerke))->SetRange(-8000, 0);
	((CSliderCtrl*)GetDlgItem(IDC_Lautstaerke))->SetPos(-8000);
	((CSliderCtrl*)GetDlgItem(IDC_Lautstaerke))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_Balance))->SetRange(-5000, 5000);
	((CSliderCtrl*)GetDlgItem(IDC_Balance))->SetPos(5000);
	((CSliderCtrl*)GetDlgItem(IDC_Balance))->SetPos(0);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDirectSoundDlg::OnPaint()
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
HCURSOR CDirectSoundDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDirectSoundDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int j = 0, buffnr = 1, playpos;
	if ((playpos = m_ds.GetPlayPosition(lpDSBTonleiter)) == -1) {
		KillTimer(1); return;
	}
	if (((playpos > 50) && (buffnr == 0)) || ((playpos < 50) && (buffnr == 1))) {
		if ((++j) == 9) { // major scale finished
			KillTimer(1);
			j = 0;
			if (!m_ds.Stop(lpDSBTonleiter))
				return;
			return;
		}
		m_ds.GenerateSound(lpDSBTonleiter, buffnr * 2, 2, ton[j]);
		if (buffnr == 1) buffnr = 0; // change buffer
		else buffnr = 1;
	}
	CDialog::OnTimer(nIDEvent);
}


void CDirectSoundDlg::OnBnClicked264hz()
{
	m_ds.GenerateSound(lpDSB264, 0, 2, c);
	if (!m_ds.Play(lpDSB264, true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedCdurtonleiter()
{
	
	// create timer 700ms
	SetTimer(1, 700, NULL);
	//mode = 0;

	// write 2 seconds on soundbuffer
	m_ds.GenerateSound(lpDSBTonleiter, 0, 2, 264);
	if (!m_ds.Play(lpDSBTonleiter, true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedCdurdreiklang()
{
	for (int i = 0; i < 3; i++) {
		m_ds.GenerateSound(lpDSBDreiklang[i], 0, 2, 264);
		if (!m_ds.Play(lpDSBDreiklang[i], true))
			OnCancel();
	}
}



void CDirectSoundDlg::OnBnClickedPcmsound()
{
	/*fileptr = fopen("Sound_22050_stereo_16Bit.pcm", "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	SetTimer(1, 200, NULL);
	//mode = 1; // 0 - Tonleiter, 1 - PCM-Datei
	m_ds.Play(lpDSBSecondary, true);*/
}



void CDirectSoundDlg::OnBnClickedStop()
{
	m_ds.Stop(lpDSB264);
	m_ds.Stop(lpDSBTonleiter);

	for (int i = 0; i < 3; i++) {
		m_ds.Stop(lpDSBDreiklang[i]);
	}

	for (int i = 0; i < 9; i++) {
		m_ds.Stop(lpDSBKlavier[i]);
	}
}


void CDirectSoundDlg::OnNMCustomdrawLautstaerke(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int volume = ((CSliderCtrl*)GetDlgItem(IDC_Lautstaerke))->GetPos();
	if (!m_ds.SetPlaybackVolume(lpDSB264, volume))
		OnCancel();

	if (!m_ds.SetPlaybackVolume(lpDSBTonleiter, volume))
		OnCancel();

	for (int i = 0; i < 3; i++) {
		if (!m_ds.SetPlaybackVolume(lpDSBDreiklang[i], volume))
			OnCancel();
	}

	for (int i = 0; i < 9; i++) {
		if (!m_ds.SetPlaybackVolume(lpDSBKlavier[i], volume))
			OnCancel();
	}

	*pResult = 0;
}


void CDirectSoundDlg::OnNMCustomdrawBalance(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	int volume = ((CSliderCtrl*)GetDlgItem(IDC_Balance))->GetPos();
	if (!m_ds.SetBalance(lpDSB264, volume))
		OnCancel();

	if (!m_ds.SetBalance(lpDSBTonleiter, volume))
		OnCancel();

	for (int i = 0; i < 3; i++) {
		if (!m_ds.SetBalance(lpDSBDreiklang[i], volume))
			OnCancel();
	}

	for (int i = 0; i < 9; i++) {
		if (!m_ds.SetBalance(lpDSBKlavier[i], volume))
			OnCancel();
	}

	*pResult = 0;
}


void CDirectSoundDlg::OnBnClickedKlavierc()
{	
	if (!m_ds.Play(lpDSBKlavier[0], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlavierd()
{	
	if (!m_ds.Play(lpDSBKlavier[1], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlaviere()
{	
	if (!m_ds.Play(lpDSBKlavier[2], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlavierf()
{	
	if (!m_ds.Play(lpDSBKlavier[3], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlavierg()
{	
	if (!m_ds.Play(lpDSBKlavier[4], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlaviera()
{	
	if (!m_ds.Play(lpDSBKlavier[5], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlavierh()
{
	if (!m_ds.Play(lpDSBKlavier[6], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedKlavierc2()
{
	if (!m_ds.Play(lpDSBKlavier[7], true))
		OnCancel();
}


void CDirectSoundDlg::OnBnClickedEnde()
{
	OnOK();
}

