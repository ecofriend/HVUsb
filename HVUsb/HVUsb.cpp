// HVUsb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>							// for _getch() used to check if Escape key was pressed.
#include <stdio.h>
#include <string>


#include "usbHVcom.h"

//#include "usbHVsampleDlg.h"

//HMODULE aLib;
HMODULE usbHVlib;

bool Connected;	
bool ConnAtStart;
CString PartName;
float MaxI;
float MaxV;
CString Prec;
const char *FWVer;	
char setVoltage[5];
float volts;
char timer[7];
int delay;


void GetPartInfo(void);
void SetVoltage(float);
float GetVoltage(void);
void SimulateVoltage(float);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure		(GUI related)
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{	
			//aLib = LoadLibrary(_T("AtUsbHid.dll"));				// Not necessary here but is required in the same folder as the EXE.
			usbHVlib = LoadLibrary(_T("usbHVcom.dll"));				// Load USB HV Supply related DLL. Should also exist in same folder as the EXE.

			if (usbHVlib == NULL)        
			{
				printf("Unable to load usbHV library");
			}

			if (loadFuncPointers(usbHVlib) == NULL) // prepare to call the library
			{
				printf("Unable to load library functions addresses");
				FreeLibrary(usbHVlib);
				exit(-1);
			}

			int err;

			// The following block appears not to be necessary for the console app.
			/*
			if((err = DLLCALL(usbHVcom_Init())) != 0) // Initialize the library
			{
				printf("fail init");
				//sprintf_s(TmpBuf,TBSIZE,"Init Failed: %i",err);	(GUI related)
				//MessageBox(TmpBuf,MBTitle,MB_ICONSTOP);			(GUI related)
				exit(-1);
			}
			*/

			if((err = DLLCALL(usbHVcom_Find())) != 0) // attempt to connect
			{
				Connected = FALSE;
				ConnAtStart = FALSE;
				printf("HV USB supply not detected. Please make sure it \nis plugged into a working USB port and try again.\n\n");
				printf("Press \"Esc\" to quit or any other key to continue in simulation mode.\n");
				if(_getch() == 0x1B)
				{
					printf("Program will exit in 5 seconds.");
					Sleep(5000);								
					exit(-1);
				}
				printf("Program will now continue in simulation mode.\n\n");				
			}
			else
			{
				Connected = TRUE;
				ConnAtStart = TRUE;
				GetPartInfo();					// retrieve part name, max volts, max amps and display
			}

			volts = GetVoltage();				// get voltage from user
			
			printf("Enter time delay (in milliseconds) between voltage settings: \n");
			delay = atoi(gets_s(timer));
			//printf("voltage: %i", voltage);

			
			if (volts <= 0 || volts > 1000)
			{
				printf("\n\nVoltage is out of range. Please enter value greater than 0\n");
				printf("and less than or equal to 1000.\n\n");
				volts = GetVoltage();
			}

			if (Connected)
			{
				SetVoltage(volts);				// Executes when HV USB Supply is connected.
			}
			else
			{
				SimulateVoltage(volts);			// Executes when HV USB Supply isn't found.
			}
				
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


float GetVoltage(void)
{
	printf("Please enter integer value for target voltage (no greater than 1000):\n");
	gets_s(setVoltage);						// user input of voltage
	return atof(setVoltage);				// convert user supplied string to float
}

void GetPartInfo(void)
{
	PartName = DLLCALL(usbHVcom_PName());	// get part name
	MaxI = DLLCALL(usbHVcom_MaxI());		// get max current
	MaxV = DLLCALL(usbHVcom_MaxV());		// get max voltage
	Prec = DLLCALL(usbHVcom_Prec());		// get recommended floating point precision format string
	FWVer = DLLCALL(usbHVcom_FWVer());		// get the firmware revision string
	printf("Part Type = %s\nMax Output V = %i\nMax Output I = %.4f\nPrecision = %s\nFirmware Rev = %s \n\nDevice initialized successfully!\n\n",PartName,(int)MaxV,MaxI,Prec,FWVer);
}

void SetVoltage(float voltage)
{
	DLLCALL(usbHVcom_Enable());				// Enable USB HV Supply
	float ii = 0;
	while (ii < voltage)					// While target voltage hasn't been reached...
	{
		DLLCALL(usbHVcom_Vpgm(ii));			// ... set new voltage.
		Sleep(delay);						// Pause program for time (in milliseconds) defined by user. 
		ii = ii + 10;						// Increment ii by 10, which will be the new voltage.
	}
	printf("High voltage set.\n");
	printf("Press \"Esc\" to power down USB HV Supply.\n\n");
	bool powerDown = false;
	while (!powerDown)						// Causes the program to 'pause' by creating an infinite loop.
	{
		if(_getch() == 0x1B)				// Check to see if Escape key was pressed.
		{			
			powerDown = true;				// If Escape key is pressed, exit the infinite loop.
		}
	}

	while (ii > 0)							// Reduce voltage loop.
	{
		DLLCALL(usbHVcom_Vpgm(ii));
		Sleep(delay);
		ii = ii - 10;
	}
	
	DLLCALL(usbHVcom_Vpgm(0));				// Set Voltage to 0
	DLLCALL(usbHVcom_Disable());			// Disable USB HV Supply

	printf("Voltage set to 0 and device disabled.");
	if(_getch())							// Check to see if a key was pressed.
	{			
		exit(-1);							// Exit program
	}
}

void SimulateVoltage(float voltage)
{
	float ii = 0;
	while (ii < voltage)					// While target voltage hasn't been reached...
	{
		Sleep(delay);						// Pause program for 5000 milli-seconds. 
		ii = ii + 10;						// Increment ii by 10, which will be the new voltage.
		printf("***SIMULATION*** Voltage set at %i volts\n", (int)ii);
	}
	printf("***SIMULATION*** High voltage set.\n");
	printf("***SIMULATION*** Press \"Esc\" to power down USB HV Supply.\n\n");
	bool powerDown = false;
	while (!powerDown)						// Causes the program to 'pause' by creating an infinite loop.
	{
		if(_getch() == 0x1B)				// Check to see if Escape key was pressed.
		{			
			powerDown = true;				// If Escape key is pressed, exit the infinite loop.
		}
	}

	while (ii > 0)							// Reduce voltage loop.
	{
		Sleep(delay);
		ii = ii - 10;
		printf("***SIMULATION*** Voltage set at %i volts\n", (int)ii);
	}

	printf("Simulation over! Please press any key to exit.");
	if(_getch())							// Check to see if a key was pressed.
	{			
		exit(-1);							// Exit program
	}
}