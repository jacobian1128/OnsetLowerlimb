#include "pch.h"
#include "taskDAQ.h"
#include "OnsetLowerlimbDlg.h"

//taskDAQ::taskDAQ(COnsetLowerlimbDlg& dlg) : _dlg(dlg)
//{
//
//}

taskDAQ::taskDAQ() : position(0)
{
	fileDAQ.open("daq.csv", ios::out);
	fileSensor.open("sensor.csv", ios::out);
}

taskDAQ::~taskDAQ()
{
	fileDAQ.close();
	fileSensor.close();
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

	// wait before trigger
	while (!_this->isExecute()) {
		Sleep(10);
	}

	// executing
	while (_this->isExecute()) {
		_this->Sensor.ReadSensorData();
		_this->writeSensorData();
	}

	_this->Sensor.CloseMATCH();

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

void taskDAQ::setReady()
{
	pThreadClock = AfxBeginThread(threadClock, this);
	hThreadClock = pThreadClock->m_hThread;
}

void taskDAQ::readDAQ()
{
	DAQmxReadCounterScalarF64(taskHandle, 1.0, &position, 0);
}

void taskDAQ::writeDAQ()
{
	fileDAQ << getTime() << ',' << position << endl;
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