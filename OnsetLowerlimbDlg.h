#ifndef _ONSET_LOWERLIMB_H_
#define _ONSET_LOWERLIMB_H_
// OnsetLowerlimbDlg.h: 헤더 파일
//

#pragma once

#include <ctime>
#include <thread>
#include "taskDAQ.h"
#include "MatchDevice.h"

#include <GL/freeglut.h>


// COnsetLowerlimbDlg 대화 상자
class COnsetLowerlimbDlg : public CDialogEx
{
// 생성입니다.
public:
	COnsetLowerlimbDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ONSETLOWERLIMB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void LogPrintf(CString input);
	void LogPrintf(const char* fmt, ...);
	void TimePrintf();
	void TrialPrintf();
	void DisplayGraph();

	void DisplayGraph_monitor();


private:
	taskDAQ Task;
	UINT_PTR htimer;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedTrigger();
	afx_msg void OnBnClickedReady();
	CStatic m_graph;
	CStatic m_monitor;

private:
	HGLRC m_hRC;
	CDC* m_pDC;

private:
	HGLRC m_hRC_monitor;
	CDC* m_pDC_monitor;

public:
	BOOL GetRenderingContext();
	//BOOL GetRenderingContext_M();
	BOOL GetOldStyleRenderingContext();
	//BOOL GetOldStyleRenderingContext_M();
	afx_msg void OnStnClickedTrial();
};

#endif