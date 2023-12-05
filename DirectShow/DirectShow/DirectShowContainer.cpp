#include "pch.h"
#include "DirectShowContainer.h"

CDirectShowContainer::CDirectShowContainer() {
	filename = L"test.mpg";
	Init();
}

CDirectShowContainer::~CDirectShowContainer()
{
	//CleanUp();
}

void CDirectShowContainer::Init() {
	CoInitialize(NULL); // zur Initialisierung des COM-Interfaces
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&pGraph);

	// Query Interface
	pGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
	pGraph->QueryInterface(IID_IMediaEventEx, (void**)&pEvent);
	pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);
	pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeek);

	pGraph->RenderFile(filename, NULL);

	// set timeformat to 100-nanoseconds units
	if (pSeek->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME) == S_OK)
		pSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	else
		AfxMessageBox(L"Zeitformat wird nicht unterstuetzt");
}

void CDirectShowContainer::setWindow(OAHWND parentwindow) {
	window = parentwindow;
}

void CDirectShowContainer::Resume() {
	if (pMediaControl != 0)
		pMediaControl->Run();
}

void CDirectShowContainer::Pause() {
	if (pMediaControl != 0)
		pMediaControl->Pause();
}

void CDirectShowContainer::Stop() {
	if (pMediaControl != 0) {
		pMediaControl->Stop(); CleanUp();
	}
}

void CDirectShowContainer::fullscreen(bool v) {
	if (pGraph) {
		IVideoWindow* pVidWin1 = NULL;
		pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin1);
		pVidWin1->put_FullScreenMode(v ? OATRUE : OAFALSE);
		pVidWin1->Release();
	}
}

void CDirectShowContainer::Run() {
	pMediaControl->Run();
}

void CDirectShowContainer::setZero() {
	REFERENCE_TIME pos = 0;
	pSeek->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning);
}

void CDirectShowContainer::seek(REFERENCE_TIME* rtTotal, REFERENCE_TIME* rtNow) {
	pSeek->GetDuration(rtTotal);
	pSeek->GetCurrentPosition(rtNow);
}

void CDirectShowContainer::setCurrentPosition(REFERENCE_TIME pos) {
	if (pSeek) {
		pSeek->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
			NULL, AM_SEEKING_NoPositioning);
	}
}

REFERENCE_TIME CDirectShowContainer::getCurrentPosition() {
	REFERENCE_TIME time;
	pSeek->GetCurrentPosition(&time);
	return time;
}

REFERENCE_TIME CDirectShowContainer::getLength() {
	REFERENCE_TIME time;
	pSeek->GetDuration(&time);
	return time;
}

LRESULT CDirectShowContainer::GetIt(WPARAM wparam, LPARAM lparam) {
	long evCode, param1, param2; HRESULT hr;
	while (SUCCEEDED(pEvent->GetEvent(&evCode, &param1, &param2, 0))) {
		pEvent->FreeEventParams(evCode, param1, param2);
		switch (evCode) {
		case EC_COMPLETE:
		case EC_USERABORT:
			CleanUp(); return 0;
		}
	}
	return 0;
}

void CDirectShowContainer::setNotifyWindow(UINT NEAR WM_GRAPHNOTIFY) {
	pEvent->SetNotifyWindow(window, WM_GRAPHNOTIFY, 0);
}

void CDirectShowContainer::setVideoWindow() {
	pVidWin->put_Owner(window);
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pVidWin->put_Visible(OATRUE);
	pVidWin->SetWindowPosition(10, 70, 300, 200);
	// Nachrichtenbehandlung (Maus, Keyboard)
	pVidWin->put_MessageDrain(window);
}

void CDirectShowContainer::setFilename(CString newfilename) {
	CleanUp();
	filename = newfilename;
	Init();
}

CString CDirectShowContainer::getFilename() {
	return filename;
}

void CDirectShowContainer::CleanUp() {
	if (pGraph) {
		fullscreen(FALSE);
		pVidWin->put_Visible(OAFALSE);
		pVidWin->put_Owner(NULL);
		pSeek->Release();
		pMediaControl->Release();
		pVidWin->Release();
		pEvent->Release();
		pGraph->Release();
		pMediaControl = 0; pVidWin = 0;
		pEvent = 0; pGraph = 0;
	}
	//CoUninitialize();
}