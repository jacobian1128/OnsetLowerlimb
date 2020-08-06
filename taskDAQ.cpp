#pragma once
#include "pch.h"
#include "taskDAQ.h"
#include "OnsetLowerlimbDlg.h"
#include "LuMatchFdn.h"
#include "MatchDevice.h"
#include <random>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>



//taskDAQ::taskDAQ(COnsetLowerlimbDlg& dlg) : _dlg(dlg)
//{
//
//}

taskDAQ::taskDAQ() : position(0)
{
	//sprintf_s(daqfilename, "daq-%d.csv", countReady);
	//fileDAQ.open(daqfilename, ios::out);
	//fileSensor.open("sensor.csv", ios::out);

	// fileopen은 모두 setReady()로 옮김
	makeRandom();

}

taskDAQ::~taskDAQ()
{
	//fileDAQ.close();
	//fileSensor.close();
	// fileclose는 각각의 thread로 옮김
}

void taskDAQ::makeRandom()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < TRIAL_NUM; i++)
	{
		random[i] = rand() % TRIAL_NUM + 1;
		for (int j = 0; j < i; j++)
		{
			if (random[i] == random[j])
			{
				i--;
				break;
			}
			
		}
	}
	
}


UINT taskDAQ::threadClock(LPVOID pParam)
{
	taskDAQ* _this = (taskDAQ*)pParam;

	// wait before trigger
	while (!_this->isTrigger()) {
		Sleep(10);
	}

	// when triggered
	_this->resetTime();
	_this->setExecute();

	// executing
	while (_this->isExecute()) {
		_this->timeCurrent = clock();

		Sleep(1);

		if (_this->getTime() >= 10) {
			_this->dismissExecute();
		}
	}

	_this->dismissDAQ();

	return 0;
}

UINT taskDAQ::threadSensor(LPVOID pParam)
{
	taskDAQ* _this = (taskDAQ*)pParam;

	_this->Sensor.InitMATCH();
	_this->Sensor.GetDataAddress();
	_this->Sensor.OpenMATCH();

	// wait before trigger
	while (!_this->isExecute()) {
		Sleep(10);
	}

	// executing
	while (_this->isExecute()) {
		//_this->Sensor.SensorData();
		_this->readSensorData();
		_this->writeSensorData();
	}
	_this->Sensor.CloseMATCH();
	_this->fileSensor.close();
	
	return 0;
}

UINT taskDAQ::threadDAQ(LPVOID pParam)
{
	taskDAQ* _this = (taskDAQ*)pParam;

	_this->initializeDAQ();

	// wait before trigger
	while (!_this->isExecute()) {
		Sleep(10);
	}

	// executing
	DAQmxStartTask(_this->taskHandle);
	while (_this->isExecute()) {
		_this->readDAQ();
		_this->writeDAQ();
	}

	_this->dismissDAQ();
	_this->fileDAQ.close();

	return 0;
}

void taskDAQ::resetTime()
{
	timeStart = clock();
	timeCurrent = timeStart;
}

double taskDAQ::getTime()
{
	// seconds
	return difftime(timeCurrent, timeStart) * 1e-3;
}

void taskDAQ::initializeDAQ()
{
	int error = 0;
	char buff[2048] = { '\0' };

	DAQmxCreateTask("", &taskHandle);
	DAQmxCreateCIAngEncoderChan(taskHandle, "Dev1/ctr1", "", DAQmx_Val_X1, 0, 0.0, DAQmx_Val_ALowBLow, DAQmx_Val_Degrees, 4096, 0.0, "");
}


void taskDAQ::dismissDAQ()
{
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
}

void taskDAQ::checkDAQ(int i, float checkvalue)
{
	if(checkvalue>30.0)
	{
		if (checkStack[i].size() >= stackSize) checkStack[i].erase(checkStack[i].begin());
		checkStack[i].push_back(1);
	}
	else
	{
		if (checkStack[i].size() >= stackSize) checkStack[i].erase(checkStack[i].begin());
		checkStack[i].push_back(0);
	}
}

//thread가 하나의 main
void taskDAQ::setReady()
{
	// ready 할 때마다 false 되도록 하여 trigger에 걸리도록
	bTrigger = false;
	bExecute = false;

	// 시계 리셋
	resetTime();

	// 파일 생성
	sprintf_s(daqfilename, "daq-%d.csv", countReady+1);
	fileDAQ.open(daqfilename, ios::out);
	sprintf_s(sensorfilename, "sensor-%d.csv", countReady);
	fileSensor.open(sensorfilename, ios::out);
	countReady++;

	// 벡터 클리어하고 0으로 초기화하기
	for (int j = 0; j < stackSize; j++) {
		for (int i = 0; i < SEN_NUM; i++) {
			checkStack[i].clear();
			checkStack[i].push_back(0);
		}
	}

	dataStack[0].clear();
	for (int j = 0; j < stackSize; j++) {
		dataStack[0].push_back(0);
	}

	for (int i = 0; i < SEN_NUM; i++) {
		stackEMGR[i].clear();
		stackEMGO[i].clear();
		stackEMGF[i].clear();

		for (int j = 0; j < stackSize; j++) {
			stackEMGR[i].push_back(0);
			stackEMGO[i].push_back(0);
			stackEMGF[i].push_back(0);
		}
	}

	// create thread
	pThreadClock = AfxBeginThread(threadClock, this);
	hThreadClock = pThreadClock->m_hThread;

	pThreadDAQ = AfxBeginThread(threadDAQ, this);
	hThreadDAQ = pThreadDAQ->m_hThread;

	pThreadSensor = AfxBeginThread(threadSensor, this);
	hThreadSensor = pThreadSensor->m_hThread;

	if (countReady >= TRIAL_NUM)
	{
		AfxMessageBox(CString("Trial finished"));
		
		//종료하는 코드 추가
	}

}

void taskDAQ::readDAQ()
{
	DAQmxReadCounterScalarF64(taskHandle, 1.0, &position, 0);
	
	//여기에 엔코더 값 받는 코드
		if (dataStack[0].size() >= stackSize) dataStack[0].erase(dataStack[0].begin());
		dataStack[0].push_back(position);
		checkDAQ(1, position);
}

void taskDAQ::writeDAQ()
{
	fileDAQ << getTime() << ',' << position << endl;
}

void taskDAQ::readSensorData()
{
	LufGetAllEmg_V(DEVICE_ID, aEmg, sizeof(aEmg));
		//여기에 emg값 받는 코드
	for (int i = 0; i < SEN_NUM; i++)
	{
		if (stackEMGR[i].size() >= stackSize) stackEMGR[i].erase(stackEMGR[i].begin());
		if (stackEMGO[i].size() >= stackSize) stackEMGO[i].erase(stackEMGO[i].begin());
		if (stackEMGF[i].size() >= stackSize) stackEMGF[i].erase(stackEMGF[i].begin());

		stackEMGR[i].push_back(aEmg[3 * i + 0]);
		stackEMGO[i].push_back(aEmg[3 * i + 1]);
		stackEMGF[i].push_back(aEmg[3 * i + 2]);

		//checkDAQ(i, aEmg[3 * i + 0]);
	}

	
}

void taskDAQ::writeSensorData()
{
	float data[8 * 3];

	Sensor.GetEMG(data);

	fileSensor << getTime() << ',';
	for (int i = 0; i < SEN_NUM; i++) {
		fileSensor << data[3 * i] << ',';
		stackEMG[i].push_back(data[3 * i]);
		if (stackEMG[i].size() > stackSize) {
			stackEMG[i].erase(stackEMG[i].begin());
		}
	}

	Sensor.GetAcc(data);
	for (int i = 0; i < SEN_NUM; i++) {
		fileSensor << data[3 * i] << ',';
		fileSensor << data[3 * i + 1] << ',';
		fileSensor << data[3 * i + 2] << ',';
	}

	Sensor.GetGyro(data);
	for (int i = 0; i < SEN_NUM; i++) {
		fileSensor << data[3 * i] << ',';
		fileSensor << data[3 * i + 1] << ',';
		fileSensor << data[3 * i + 2] << ',';
	}

	Sensor.GetEuler(data);
	for (int i = 0; i < SEN_NUM; i++) {
		fileSensor << data[3 * i] << ',';
		fileSensor << data[3 * i + 1] << ',';
		fileSensor << data[3 * i + 2] << ',';
	}
	fileSensor << endl;
}