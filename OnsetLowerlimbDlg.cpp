﻿
// OnsetLowerlimbDlg.cpp: 구현 파일
//
#pragma once
#include "pch.h"
#include "framework.h"
#include "OnsetLowerlimb.h"
#include "OnsetLowerlimbDlg.h"
#include "afxdialogex.h"
#include "MatchDevice.h"
#include "LuMatchFdn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COnsetLowerlimbDlg 대화 상자



COnsetLowerlimbDlg::COnsetLowerlimbDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ONSETLOWERLIMB_DIALOG, pParent) 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COnsetLowerlimbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, m_graph);
	DDX_Control(pDX, IDC_MONITOR, m_monitor);
}

BEGIN_MESSAGE_MAP(COnsetLowerlimbDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TRIGGER, &COnsetLowerlimbDlg::OnBnClickedTrigger)
	ON_BN_CLICKED(IDC_READY, &COnsetLowerlimbDlg::OnBnClickedReady)
	ON_STN_CLICKED(IDC_TRIAL, &COnsetLowerlimbDlg::OnStnClickedTrial)
END_MESSAGE_MAP()


// COnsetLowerlimbDlg 메시지 처리기

BOOL COnsetLowerlimbDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Task.resetTime();
	SetTimer(htimer, 10, NULL);

	if (!GetRenderingContext()) {
		return -1;
	}
	/*
	if (!GetRenderingContext_M()) {
		return -1;
	}
	*/
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COnsetLowerlimbDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COnsetLowerlimbDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CClientDC dc(this);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COnsetLowerlimbDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COnsetLowerlimbDlg::LogPrintf(CString input)
{
	// read current string and add a line
	CString str;
	GetDlgItemText(IDC_LOG, str);
	if (str.GetLength() != 0) str += "\r\n";
	str += input;
	SetDlgItemText(IDC_LOG, str);

	// move cursor to the last line
	CEdit* e = (CEdit*)GetDlgItem(IDC_LOG);
	e->SetFocus();
	e->SetSel(-1);
}

void COnsetLowerlimbDlg::LogPrintf(const char* fmt, ...)
{
	// printf style (variable args) overload function
	va_list args;
	char str[1024];

	va_start(args, fmt);
	vsprintf_s(str, fmt, args);
	va_end(args);

	CString input(str);
	LogPrintf(input);
}

void COnsetLowerlimbDlg::TimePrintf()
{
	char time[100];
	sprintf_s(time, "time: %8.3f [s]\r\nDegree: %+8.3f [deg]\r\n", 
		Task.getTime(), Task.getPosition());
	CString str(time);
	SetDlgItemText(IDC_TIME, str);
}

void COnsetLowerlimbDlg::TrialPrintf()
{
	char trial[100];
	sprintf_s(trial, "trial: %d", Task.countReady);
	LogPrintf("trial: %d", Task.countReady);
	//CString str(trial);
	//SetDlgItemText(IDC_TRIAL, str);
}



void COnsetLowerlimbDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TimePrintf();
	//TrialPrintf();
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	DisplayGraph();
	wglMakeCurrent(m_pDC_monitor->GetSafeHdc(), m_hRC_monitor);
	DisplayGraph_monitor();

	
}


void COnsetLowerlimbDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(htimer);
}


void COnsetLowerlimbDlg::OnBnClickedTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Task.setTrigger();
}


void COnsetLowerlimbDlg::OnBnClickedReady()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Task.setReady();
	TrialPrintf();
}


BOOL COnsetLowerlimbDlg::GetRenderingContext()
{
	//픽처 컨트롤에만 그리도록 DC 생성
	//참고 https://goo.gl/CK36zE


	CWnd* pImage = GetDlgItem(IDC_GRAPH);
	CRect rc;
	pImage->GetWindowRect(rc);
	m_pDC = pImage->GetDC();

	


	if (NULL == m_pDC)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}
	
	
	CWnd* pImage_monitor = GetDlgItem(IDC_MONITOR);
	CRect rc_monitor;
	pImage_monitor->GetWindowRect(rc_monitor);
	m_pDC_monitor = pImage_monitor->GetDC();
	

	if (NULL == m_pDC_monitor)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}
	
	



	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}
	/*
	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}
	*/

	return TRUE;
}
/*
BOOL COnsetLowerlimbDlg::GetRenderingContext_M()
{
	//픽처 컨트롤에만 그리도록 DC 생성
	//참고 https://goo.gl/CK36zE

	
	CWnd* pImage_monitor = GetDlgItem(IDC_MONITOR);
	CRect rc_monitor;
	pImage_monitor->GetWindowRect(rc_monitor);
	m_pDC_monitor = pImage_monitor->GetDC();


	if (NULL == m_pDC_monitor)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}
	


	if (!GetOldStyleRenderingContext_M())
	{
		return TRUE;
	}


	return TRUE;
}
*/

BOOL COnsetLowerlimbDlg::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                        // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};




	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	


	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}



	//Make it current. 
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		
		AfxMessageBox(CString("wglMakeCurrent failed"));
		wglMakeCurrent(NULL, NULL);
		return FALSE;
	}

			static PIXELFORMATDESCRIPTOR pfd_monitor=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,							 // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	

	int pixelFormat_monitor = ChoosePixelFormat(m_pDC_monitor->GetSafeHdc(), &pfd_monitor);
	

	if (0 == pixelFormat_monitor)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (FALSE == SetPixelFormat(m_pDC_monitor->GetSafeHdc(), pixelFormat_monitor, &pfd_monitor))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

		if (0 == (m_hRC_monitor = wglCreateContext(m_pDC_monitor->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

		if (FALSE == wglMakeCurrent(m_pDC_monitor->GetSafeHdc(), m_hRC_monitor))
	{

		AfxMessageBox(CString("wglMakeCurrent failed"));
		wglMakeCurrent(NULL, NULL);
		return FALSE;
	}



	return TRUE;
}

/*/
BOOL COnsetLowerlimbDlg::GetOldStyleRenderingContext_M()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd_monitor =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,							 // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};


	int pixelFormat_monitor = ChoosePixelFormat(m_pDC_monitor->GetSafeHdc(), &pfd_monitor);

	if (0 == pixelFormat_monitor)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (FALSE == SetPixelFormat(m_pDC_monitor->GetSafeHdc(), pixelFormat_monitor, &pfd_monitor))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	if (0 == (m_hRC_monitor = wglCreateContext(m_pDC_monitor->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

	if (FALSE == wglMakeCurrent(m_pDC_monitor->GetSafeHdc(), m_hRC_monitor))
	{

		AfxMessageBox(CString("wglMakeCurrent failed"));
		wglMakeCurrent(NULL, NULL);
		return FALSE;
	}

	return TRUE;
}
*/
void COnsetLowerlimbDlg::DisplayGraph() {


	//여기에 그래프 그리는 코드
	
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		
		// sEMG 구분선
		glColor3f(0.2f, 0.2f, 0.2f);
		for (int i = 0; i < 3; i++)
		{
			glLineWidth(1.5);
			glBegin(GL_LINE_STRIP);
			
			glVertex3f(-1.0f, -0.5+0.5*i, 0.0f);
			glVertex3f(1.0f, -0.5+0.5*i, 0.0f);
			glEnd();
		}
		
		//encoder 값 그리기
		glLineWidth(1.0);
		if (Task.dataStack[0].size() >= stackSize) {
			glBegin(GL_LINE_STRIP);
			glColor3f(0.5f, 1.0f, 1.0f);
			for (int i = 0; i < stackSize; i++) {
				glVertex3f(-1.0f + 2.0f/stackSize * i, -1.0+Task.dataStack[0][i]/45, 0);
			}
			glEnd();

		}

		//EMG 값 그리기
		glLineWidth(1.0);
		for (int j = 0; j < SEN_NUM; j++) {
			if (Task.stackEMGR[j].size() >= stackSize) {
				glBegin(GL_LINE_STRIP);
				
				glColor3f(1.0f, 0.5f, 0.5f);
				for (int i = 0; i < stackSize; i++) {
					glVertex3f(-1.0f + 2.0f / stackSize * i,-0.75+0.5*j+Task.stackEMGR[j][i] * 0.05f, 0.0f);
				}
				glEnd();
			}
		}

		// 특정 조건에서 흰색 선 그리기
		for (int j = 0; j < SEN_NUM; j++) {
			if (Task.checkStack[j].size() >= stackSize) {
				glBegin(GL_LINE_STRIP);
				glColor3f(1.0f, 1.0f, 1.0f);
				for (int i = 0; i < stackSize; i++) {
					if ((Task.checkStack[j][i] - Task.checkStack[j][i - 1]) == 1)
					{
						glVertex3f(-1.0f + 2.0f / stackSize * i, -1.0 + 0.5 * j, 0.0f);
						glVertex3f(-1.0f + 2.0f / stackSize * i, -1.0 + 0.5 * (j+1), 0.0f);
					}
					
				}
				glEnd();
			}
		}
		
		//ReleaseDC(m_pDC);
		SwapBuffers(m_pDC->GetSafeHdc());
		
}

void COnsetLowerlimbDlg::DisplayGraph_monitor() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	if (Task.getTime() >= 10.0/TRIAL_NUM*Task.random[Task.countReady-1]) {
		
		//glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		
		glLoadIdentity();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();
		glFlush();
	}


	// 전체 색깔 칠하는 거
	// 주어진 시간에 바뀌게 만들어놓고
	//ReleaseDC(m_pDC_monitor);
	SwapBuffers(m_pDC_monitor->GetSafeHdc());
}


void COnsetLowerlimbDlg::OnStnClickedTrial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
