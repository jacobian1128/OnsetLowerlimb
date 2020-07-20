#ifndef _TASK_DAQ_H_
#define _TASK_DAQ_H_

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include <NIDAQmx.h>

#include "MatchDevice.h"

using namespace std;

class taskDAQ {
public:
	taskDAQ();
	~taskDAQ();

public:
	void resetTime();
	double getTime();

private:
	static UINT threadClock(LPVOID pParam);
	static UINT threadSensor(LPVOID pParam);
	static UINT threadDAQ(LPVOID pParam);

	CWinThread* pThreadClock;
	CWinThread* pThreadSensor;
	CWinThread* pThreadDAQ;

	HANDLE hThreadClock;
	HANDLE hThreadSensor;
	HANDLE hThreadDAQ;

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
	void setExecute() { bExecute = true; }
	void dismissExecute() { bExecute = false; }

public:
	float64 getPosition() { return position; }
	float64 getVelocity() { return velocity; }

private:
	TaskHandle taskHandle;

	void initializeDAQ();
	void dismissDAQ();
	void readDAQ();
	void writeDAQ();

	float64 position;
	float64 velocity;

	fstream fileDAQ;
	fstream fileSensor;

private:
	MATCH Sensor;
	void writeSensorData();

	int stackSize;

	vector<float> stackEMG[SEN_NUM];
	vector<float64> stackPosition;
};

#endif
