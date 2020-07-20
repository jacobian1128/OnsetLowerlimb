#include <stdio.h>
#include <conio.h>
#include "pch.h"
#include "LuMatchFdn.h"
#include "MatchDevice.h"

wchar_t *pVid;				/*!< This pointer variable refer to vendor name address in the LuFoundation Library.*/
wchar_t *pPid;				/*!< This pointer variable refer to product name address in the LuFoundation Library.*/

float aEmg[SEN_NUM * 3];	/*!< sEMG data\n e.g. aEMG[0], aEMG[1] and aEMG[2] represent the raw data, offseted and absoluted data, filtered data of the first MATCH sensor, respectively.*/
float aEuler[SEN_NUM * 3];	/*!< euler data\n e.g. aEuler[0], aEuler[1] and aEuler[2] represent the Euler angles of the x, y and z axes of the first MATCH sensor, respectively.*/
float aAccel[SEN_NUM * 3];	/*!< accelerometer data\n e.g. aAccel[0], aAccel[1], aAccel[2] represent the accelerometer data of the x, y and z axes of the first MATCH sensor, respectively.*/
float aGyro[SEN_NUM * 3];	/*!< gyroscope data\n e.g. aGyro[0], aGyro[1], aGyro[2] represent the gyroscope data of the x, y and z axes of the first MATCH sensor, respectively.*/
float aMagnet[SEN_NUM * 3];	/*!< magnetometer data\n e.g. aMagnet[0], aMagnet[1], aMagnet[2] represent the magnetometer data of the x, y and z axes of the first MATCH sensor, respectively.*/
int   aEmgAmpGain[SEN_NUM];	/*!< sEMG Amp. gain data\n e.g. aEmgAmpGain[0] represents the amplifier gain of the first MATCH sensor.*/
int   aOpt1Type[SEN_NUM];	/*!< option-space1 type data\n e.g. aOpt1Type[0] represents the option-space1 type*/
int   aOpt2Type[SEN_NUM];	/*!< option-space2 type data\n e.g. aOpt2Type[0] represents the option-space2 type*/
int   aBattery[SEN_NUM];	/*!< battery infomation data\n e.g. aBattery[0] represents the battery data of the first MATCH sensor.*/
int senIdx = 0;

void CalibEmgOffset();
void ChangeOpt1Type(int type);
void ChangeOpt2Type(int type);
void SaveData(bool start);
void SetPowerOff();

MATCH::MATCH() 
{
}

MATCH::~MATCH() 
{
}

int MATCH::InitMATCH()
{
	int rtn = 0;
	int devNum = 0;

	printf("*Scanning for connected MATCH ... ");
	devNum = LufScanSensor();
	printf("done\n");

	if (devNum)
	{
		printf("Found %d MATCH\n\n", devNum);

		printf("*Initialization of connected MATCH ... ");
		rtn = LufInitDev(DEVICE_ID, SEN_NUM, SAMPLE_PERIOD_MS, BUF_SIZE, FLT_WIN_SIZE);
		printf("done\n");

		if (!rtn)
		{
			printf("Sampling Time [ms]: %d\n", SAMPLE_PERIOD_MS);
			printf("Buffer Size: %d\n", BUF_SIZE);
			printf("Filter Window Size: %d\n\n", FLT_WIN_SIZE);
		}
		else
		{
			printf("Can not Initialize LuFoundation API\nError: %d", rtn);
			return -1;
		}
	}
	else
	{
		printf("Check the device connection\nDevice Num : %d\n", devNum);
		return -1;
	}

	return 0;
}

int MATCH::GetDataAddress()
{
	printf("*Get Sensor Data Address ... ");

	pVid = (wchar_t *)LufGetVendorString(DEVICE_ID);
	pPid = (wchar_t *)LufGetProductString(DEVICE_ID);

	printf("done\n");
	printf("Vender: %S\n", pVid);
	printf("Product: %S\n", pPid);
	printf("\n");

	return 0;
}

int MATCH::OpenMATCH()
{
	int rtn = LufOpenDev(DEVICE_ID, SEN_NUM, SAMPLE_PERIOD_MS, BUF_SIZE, FLT_WIN_SIZE);
	if (rtn)
		printf("Can not connect the device\nError code : %d", rtn);
	return rtn;
}

void MATCH::ReadSensorData()
{
	//while (1)
	//{
	LufGetAllEmgAmpGain(DEVICE_ID, aEmgAmpGain, sizeof(aEmgAmpGain));
	LufGetAllEmg_V(DEVICE_ID, aEmg, sizeof(aEmg));
	LufGetAllEuler_deg(DEVICE_ID, aEuler, sizeof(aEuler));
	LufGetAllAccel_g(DEVICE_ID, aAccel, sizeof(aAccel));
	LufGetAllGyro_dps(DEVICE_ID, aGyro, sizeof(aGyro));
	LufGetAllMagnet_uT(DEVICE_ID, aMagnet, sizeof(aMagnet));
	LufGetAllOpt1Type(DEVICE_ID, aOpt1Type, sizeof(aOpt1Type));
	LufGetAllOpt2Type(DEVICE_ID, aOpt2Type, sizeof(aOpt2Type));
	LufGetAllBattery_pc(DEVICE_ID, aBattery, sizeof(aBattery));
	//}
}

void MATCH::CloseMATCH()
{
	/* Release Memory with respect to device number */
	LufCloseDev(DEVICE_ID);

	/* Disconnect USB HID and Release Memory about all device */
	LufExitAllDev();
}

void MATCH::GetEMG(float* data) 
{
	memcpy(data, aEmg, SEN_NUM * 3 * sizeof(float));
}

void MATCH::GetAcc(float* data)
{
	memcpy(data, aAccel, SEN_NUM * 3 * sizeof(float));
}

void MATCH::GetGyro(float* data)
{
	memcpy(data, aGyro, SEN_NUM * 3 * sizeof(float));
}

void MATCH::GetEuler(float* data)
{
	memcpy(data, aEuler, SEN_NUM * 3 * sizeof(float));
}

void CalibEmgOffset()
{
	int rtn = LufSetCalibEmgOffset(DEVICE_ID);
	if (rtn) printf("Error Code: %d                     ", rtn);
}

void ChangeOpt1Type(int type)
{
	int rtn = LufSetOptionType(DEVICE_ID, senIdx, type, eTypeDoNotChange);
	if (rtn) printf("Error Code: %d                     ", rtn);
}

void ChangeOpt2Type(int type)
{
	int rtn = LufSetOptionType(DEVICE_ID, senIdx, eTypeDoNotChange, type);
	if (rtn) printf("Error Code: %d                     ", rtn);
}

void SaveData(bool start)
{
	int rtn = 0;
	if (start) rtn = LufSetStartSaveData(DEVICE_ID, TEXT("LuData.csv"));
	else rtn = LufSetStopSaveData(DEVICE_ID);

	if (rtn) printf("Error Code: %d                     ", rtn);
}

void SetPowerOff()
{
	int rtn = LufSetPowerOff(DEVICE_ID, senIdx);
	if (rtn) printf("Error Code: %d                     ", rtn);
}