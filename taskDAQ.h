#ifndef _TASK_DAQ_H_
#define _TASK_DAQ_H_

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <NIDAQmx.h>
#include "MatchDevice.h"
#include <time.h>

#include "Eigen/Dense"

using namespace Eigen;

#define stackSize (30000)
#define TRIAL_NUM	(10)	

using namespace std;

class taskDAQ {
public:
	taskDAQ();
	~taskDAQ();

	void makeRandom();

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

public:
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
	void checkDAQ(int i, float checkvalue);
	void readDAQ();
	void writeDAQ();

	float64 position;
	float64 velocity;

	fstream fileDAQ;
	fstream fileSensor;

public:
	MATCH Sensor;
	void writeSensorData();
	void readSensorData();
	float64 data[stackSize];

	vector<float64> dataStack[1];
	vector<int> checkStack[SEN_NUM];

	int countReady = 0; // ready 버튼 누른 횟수 체크
	char daqfilename[30]; // daq.csv 저장할 파일명
	char sensorfilename[30]; //sensor.csv 저장할 파일명

	vector<float> stackEMG[SEN_NUM];

	vector<float> stackEMGR[SEN_NUM];
	vector<float> stackEMGO[SEN_NUM];
	vector<float> stackEMGF[SEN_NUM];

	//float sEMG 신호 8채널 어레이;

public:
	int random[50];
	clock_t timecheck;

};

#endif
