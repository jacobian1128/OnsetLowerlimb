#include "pch.h"
#include "taskDAQ.h"
#include "OnsetLowerlimbDlg.h"

//taskDAQ::taskDAQ(COnsetLowerlimbDlg& dlg) : _dlg(dlg)
//{
//
//}

taskDAQ::taskDAQ() 
{
}

taskDAQ::~taskDAQ()
{

}

UINT taskDAQ::threadClock(LPVOID pParam)
{
	taskDAQ* _this = (taskDAQ*)pParam;

	_this->initializeDAQ();

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

		_this->readDAQ();
		_this->writeDAQ();

		Sleep(1);

		if (_this->getTime() >= 10) {
			_this->dismissExecute();
		}
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
	DAQmxReadCounterScalarF64(taskHandle, 5.0, &deg, 0);
}

void taskDAQ::writeDAQ()
{

}