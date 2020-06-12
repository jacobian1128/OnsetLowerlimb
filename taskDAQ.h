#ifndef _TASK_DAQ_H_
#define _TASK_DAQ_H_

#pragma once
#include <NIDAQmx.h>

//class COnsetLowerlimbDlg;

class taskDAQ {
public:
	//taskDAQ(COnsetLowerlimbDlg& dlg);
	taskDAQ();
	~taskDAQ();

private:
	//const COnsetLowerlimbDlg& _dlg;

public:
	void resetTime();
	double getTime();

private:
	static UINT threadClock(LPVOID pParam);
	CWinThread* pThreadClock;
	HANDLE hThreadClock;

	clock_t timeStart;
	clock_t timeCurrent;

private:
	bool bTrigger;
	bool bExecute;

public:
	bool isTrigger() { return bTrigger; }
	bool isExecute() { return bExecute; }

public:
	void setTrigger() { bTrigger = true; }
	void setExecute() { bExecute = true; }
	void dismissExecute() { bExecute = false; }

private:
	TaskHandle taskHandle;
	void initDAQ();
};

#endif
