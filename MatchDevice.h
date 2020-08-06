#ifndef _MATCH_DEVICE_H_
#define _MATCH_DEVICE_H_

#pragma once

#define DEVICE_ID			(0)		/*!< Set device(MATCH Cradle) ID. Default value is 0.*/
#define SEN_NUM				(4)		/*!< Total sensor number. Default value is 8.*/
#define SAMPLE_PERIOD_MS	(1)		/*!< Data sampling time from MATCH.\n minimum value is 1.\n Unit: milisencond.*/
#define SAMPLE_FREQ			(1000.0f / SAMPLE_PERIOD_MS) /*!< Data sampling frequency.\n Unit: Hz*/
#define BUF_SIZE			(100)	/*!< Sensor data buffer for use in the LuFoundation Library.*/
#define	FLT_WIN_SIZE		(100)	/*!< Low pass filter window size for sEMG signal processing.*/
#define SCREEN_REFRESH_MS	(1)		/*!< Screen refresh rate.\n Unit: milisecond*/

extern wchar_t *pVid;				/*!< This pointer variable refer to vendor name address in the LuFoundation Library.*/
extern wchar_t *pPid;				/*!< This pointer variable refer to product name address in the LuFoundation Library.*/
extern float aEmg[SEN_NUM * 3];		/*!< sEMG data\n e.g. aEMG[0], aEMG[1] and aEMG[2] represent the raw data, offseted and absoluted data, filtered data of the first MATCH sensor, respectively.*/
extern float aEuler[SEN_NUM * 3];	/*!< euler data\n e.g. aEuler[0], aEuler[1] and aEuler[2] represent the Euler angles of the x, y and z axes of the first MATCH sensor, respectively.*/
extern float aAccel[SEN_NUM * 3];	/*!< accelerometer data\n e.g. aAccel[0], aAccel[1], aAccel[2] represent the accelerometer data of the x, y and z axes of the first MATCH sensor, respectively.*/
extern float aGyro[SEN_NUM * 3];	/*!< gyroscope data\n e.g. aGyro[0], aGyro[1], aGyro[2] represent the gyroscope data of the x, y and z axes of the first MATCH sensor, respectively.*/
extern float aMagnet[SEN_NUM * 3];	/*!< magnetometer data\n e.g. aMagnet[0], aMagnet[1], aMagnet[2] represent the magnetometer data of the x, y and z axes of the first MATCH sensor, respectively.*/
extern int   aEmgAmpGain[SEN_NUM];	/*!< sEMG Amp. gain data\n e.g. aEmgAmpGain[0] represents the amplifier gain of the first MATCH sensor.*/
extern int   aOpt1Type[SEN_NUM];	/*!< option-space1 type data\n e.g. aOpt1Type[0] represents the option-space1 type*/
extern int   aOpt2Type[SEN_NUM];	/*!< option-space2 type data\n e.g. aOpt2Type[0] represents the option-space2 type*/
extern int   aBattery[SEN_NUM];		/*!< battery infomation data\n e.g. aBattery[0] represents the battery data of the first MATCH sensor.*/
extern int senIdx;

using namespace std;

class MATCH {
public:
	MATCH();
	~MATCH();

public:
	int InitMATCH();
	int GetDataAddress();
	int OpenMATCH();
	void SensorData();
	void CloseMATCH();
	// int KeyCommand();

public:
	void GetEMG(float* data);
	void GetAcc(float* data);
	void GetGyro(float* data);
	void GetEuler(float* data);
	
};
#endif