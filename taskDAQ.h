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
	void setReady();
	void setTrigger() { bTrigger = true; }
	void setExecute() { bExecute = true; DAQmxStartTask(taskHandle); }
	void dismissExecute() { bExecute = false; }

public:
	float64 getDegree() { return deg; }

private:
	TaskHandle taskHandle;

	void initializeDAQ();
	void dismissDAQ();
	void readDAQ();
	void writeDAQ();

	float64 deg;
};

#endif
