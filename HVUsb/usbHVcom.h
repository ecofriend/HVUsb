/*
EMCO High Voltage Corp.

usbHVcom.h

Header file for use with usbHVcom.dll
*/

// compile once only
#ifndef _USBHV_H_
#define _USBHV_H_
//
// error return definitions
#define USBHV_ERR_NOHIDDLL	1	// can't find atusbhid.dll
#define USBHV_ERR_NOTFOUND	2	// usbHV not found on bus
#define USBHV_ERR_NOCAPS	3	// can't get device capabilities
#define USBHV_ERR_WFAULT	4	// write fault
#define USBHV_ERR_RFAULT	5	// read fault
#define USBHV_ERR_ISCONN	6	// call to _Find when already connected
#define USBHV_ERR_ISINIT	7	// call to _Init when already initialized
#define USBHV_ERR_NOTCONN	8	// call to comm functions without call to _Find
#define USBHV_ERR_TIMEOUT	9	// datacq data return timeout
#define USBHV_ERR_NOHANDLE	10	// handle to library is null
//
// flags (high nybble of byte returned by _Status function)
#define USBHV_ENABLED 0x80
// part type (low nybble of byte returned by _Status function)
// [see also the _PartName(...) function]
#define USBHV_02P 0
#define USBHV_02N 1
#define USBHV_05P 2
#define USBHV_05N 3
#define USBHV_10P 4
#define USBHV_10N 5
#define USBHV_12P 6
#define USBHV_12N 7
#define USBHV_20P 8
#define USBHV_20N 9

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the USBHVCOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// USBHVCOM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef USBHVCOM_EXPORTS
#define USBHVCOM_API extern "C" __declspec(dllexport)
#else
#define USBHVCOM_API extern "C" __declspec(dllimport)
#endif

#define SCALL __cdecl

// These macros make calling DLL functions through pointers easier.
#define DECLARE_FUNC_POINTER(FUNC)  PF_##FUNC lp##FUNC=NULL;  
#define LOAD_FUNC_POINTER(DLL,FUNC) lp##FUNC = (PF_##FUNC)GetProcAddress(DLL, #FUNC);
#define ADDR_CHK(FUNC) if (lp##FUNC == NULL) {return FALSE;}
#define DLLCALL(FUNC) lp##FUNC

typedef int (SCALL *PF_usbHVcom_Init)(void);
typedef int (SCALL *PF_usbHVcom_Free)(void);
typedef int (SCALL *PF_usbHVcom_Find)(void);
typedef int (SCALL *PF_usbHVcom_Close)(void);
typedef int (SCALL *PF_usbHVcom_Enable)(void);
typedef int (SCALL *PF_usbHVcom_Disable)(void);
typedef int (SCALL *PF_usbHVcom_Vpgm)(float);
typedef int (SCALL *PF_usbHVcom_Vmon)(float *);
typedef int (SCALL *PF_usbHVcom_Status)(unsigned char *);
typedef int (SCALL *PF_usbHVcom_RegisterNotification)(HWND);
typedef int (SCALL *PF_usbHVcom_UnRegisterNotification)(void);
typedef int (SCALL *PF_usbHVcom_IsUsbHVNotification)(DWORD);
typedef float (SCALL *PF_usbHVcom_MaxV)(void);
typedef float (SCALL *PF_usbHVcom_MaxI)(void);
typedef const char * (SCALL *PF_usbHVcom_PName)(void);
typedef const char * (SCALL *PF_usbHVcom_Prec)(void);
typedef const char * (SCALL *PF_usbHVcom_FWVer)();

//
// Function definitions

/*
Function:	int usbHVcom_Init(void)
Purpose:	Loads the atmel hid library "atusbhid.dll" and initializes memory
			associated with the usbHVcom_... functions
Returns:	0 if successful, or USBHV_ERR_ISINIT if _Init has already been called
			Will also return an error if atusbhid.dll cannot be found.
*/
USBHVCOM_API int SCALL usbHVcom_Init(void);


/*
Function:	void usbHVcom_Free(void)
Purpose:	Frees the "atusbhid.dll" library and memory associated
			with usbHVcom_... functions. NOTE: "usbHVcom.dll" must freed by the user
			application using FreeLibrary(...)
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
*/
USBHVCOM_API int SCALL usbHVcom_Free(void);


/*
Function:	int usbHVcom_Find(void)
Purpose:	Looks for the usbHV on the bus
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			USBHV_ERR_ISCONN if this function has already been called succesfully.
			USBHV_ERR_NOTFOUND if plug-and-play does not report the usbHV is connected
*/
USBHVCOM_API int SCALL usbHVcom_Find(void);


/*
Function:	void usbHVcom_Close(void)
Purpose:	closes the connection without freeing the library
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
*/
USBHVCOM_API int SCALL usbHVcom_Close(void);


/*
Function:	int usbHVcom_Enable(void)
Purpose:	Enables the high voltage output using current
			Vpgm setting (see usbHVcom_Vpgm(...))
Parameters:	None
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			USBHV_ERR_NOTCONN if _Find has not been called succesfully
			Also possible communication errors
*/
USBHVCOM_API int SCALL usbHVcom_Enable(void);


/*
Function:	int usbHVcom_Disable(void)
Purpose:	Disables the high voltage output
Parameters:	None
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			USBHV_ERR_NOTCONN if _Find has not been called succesfully
			Also possible communication errors
*/
USBHVCOM_API int SCALL usbHVcom_Disable(void);


/*
Function:	int usbHVcom_Vpgm(float Vpgm)
Purpose:	Sets new Vpgm. Vpgm is automatically limited by this function to
			the maximum allowed by the attached device. This maximum can be
			retrieved via the _GetMaxV() function.
Parameters:	Float for new Vpgm
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			USBHV_ERR_NOTCONN if _Find has not been called succesfully;	Also
			possible communication errors. NOTE: Output Enable/Disable is
			set separately using usbHVcom_Enable(), usbHVcom_Disable()
*/
USBHVCOM_API int SCALL usbHVcom_Vpgm(float);


/*
Function:	int usbHVcom_Vmon(float *Vmon)
Purpose:	Reads the current Vmon value
Parameters:	Pointer to float to contain Vmon
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			USBHV_ERR_NOTCONN if _Find has not been called succesfully
			Also possible communication errors
*/
USBHVCOM_API int SCALL usbHVcom_Vmon(float *);


/*
Function:	usbHVcom_Status(unsigned char *Flags)
Purpose:	Reads the status flags and part type number
Parameters:	*Flags = pointer to status data, defined below
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			USBHV_ERR_NOTCONN if _Find has not been called succesfully
			Also possible communication errors
*/
USBHVCOM_API int SCALL usbHVcom_Status(unsigned char *);

/*
Function:	usbHVcom_RegisterNotification(HWND hNotify)
Purpose:	Registers a window to receive notification messages from usbHV
Parameters:	Handle to window to receive notifications
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
*/
USBHVCOM_API int SCALL usbHVcom_RegisterNotification(HWND);


/*
Function:	usbHVcom_UnRegisterNotification(HWND hNotify)
Purpose:	UnRegisters a window to stop receiving notification messages from usbHV
Parameters:	-
Returns:	0 if successful, or USBHV_ERR_NOHANDLE if _Init has not been called
			Will also return USBHV_ERR_NOHANDLE if _RegisterNotification has not been called
*/
USBHVCOM_API int SCALL usbHVcom_UnRegisterNotification(void);


/*
Function:	int usbHVcom_IsUsbHVNotification(DWORD nData)
Purpose:	Determines if notification is from usbHV
Parameters:	Notification data that was passed to the registered window
Returns:	non-zero if notification is for usbHV
*/
USBHVCOM_API int SCALL usbHVcom_IsUsbHVNotification(DWORD);


/*
Function:	int usbHVcom_MaxV(void)
Purpose:	returns max voltage of attached device
Parameters:	-
Returns:	returns max voltage of attached device
*/
USBHVCOM_API float SCALL usbHVcom_MaxV(void);


/*
Function:	float usbHVcom_MaxI(void)
Purpose:	returns max current of attached device
Parameters:	-
Returns:	max current of attached device
*/
USBHVCOM_API float SCALL usbHVcom_MaxI(void);


/*
Function:	const char* usbHVcom_PartName(void)
Purpose:	returns name of attached device
Parameters:	-
Returns:	pointer to name of attached device
*/
USBHVCOM_API const char * SCALL usbHVcom_PName(void);


/*
Function:	const char* usbHVcom_Prec(void)
Purpose:	returns recommended format string for Vpgm/Vmon. This will
			be in the standard c syntax and will be "%.nf" where n is 
			recommended number of decimal places precision
Parameters:	-
Returns:	pointer to printf format string
*/
USBHVCOM_API const char * SCALL usbHVcom_Prec(void);


/*
Function:	const char* usbHVcom_FWVer(void)
Purpose:	returns the usbHV's firmware revision string.
Parameters:	-
Returns:	pointer to firmware revision string
*/
USBHVCOM_API const char * SCALL usbHVcom_FWVer();


#ifndef USBHVCOM_EXPORTS


DECLARE_FUNC_POINTER(usbHVcom_Init);
DECLARE_FUNC_POINTER(usbHVcom_Free);
DECLARE_FUNC_POINTER(usbHVcom_Find);
DECLARE_FUNC_POINTER(usbHVcom_Close);
DECLARE_FUNC_POINTER(usbHVcom_Enable);
DECLARE_FUNC_POINTER(usbHVcom_Disable);
DECLARE_FUNC_POINTER(usbHVcom_Vpgm);
DECLARE_FUNC_POINTER(usbHVcom_Vmon);
DECLARE_FUNC_POINTER(usbHVcom_Status);
DECLARE_FUNC_POINTER(usbHVcom_RegisterNotification);
DECLARE_FUNC_POINTER(usbHVcom_UnRegisterNotification);
DECLARE_FUNC_POINTER(usbHVcom_IsUsbHVNotification);
DECLARE_FUNC_POINTER(usbHVcom_MaxV);
DECLARE_FUNC_POINTER(usbHVcom_MaxI);
DECLARE_FUNC_POINTER(usbHVcom_PName);
DECLARE_FUNC_POINTER(usbHVcom_Prec);
DECLARE_FUNC_POINTER(usbHVcom_FWVer);

static bool loadFuncPointers(HINSTANCE hLib)
{
    LOAD_FUNC_POINTER(hLib, usbHVcom_Init);
    ADDR_CHK(usbHVcom_Init);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Free);
    ADDR_CHK(usbHVcom_Free);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Find);
    ADDR_CHK(usbHVcom_Find);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Close);
    ADDR_CHK(usbHVcom_Find);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Enable);
    ADDR_CHK(usbHVcom_Enable);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Disable);
    ADDR_CHK(usbHVcom_Disable);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Vpgm);
    ADDR_CHK(usbHVcom_Vpgm);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Vmon);
    ADDR_CHK(usbHVcom_Vmon);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Status);
    ADDR_CHK(usbHVcom_Status);
    LOAD_FUNC_POINTER(hLib, usbHVcom_RegisterNotification);
    ADDR_CHK(usbHVcom_RegisterNotification);
    LOAD_FUNC_POINTER(hLib, usbHVcom_UnRegisterNotification);
    ADDR_CHK(usbHVcom_UnRegisterNotification);
    LOAD_FUNC_POINTER(hLib, usbHVcom_IsUsbHVNotification);
    ADDR_CHK(usbHVcom_IsUsbHVNotification);
    LOAD_FUNC_POINTER(hLib, usbHVcom_MaxV);
    ADDR_CHK(usbHVcom_MaxV);
    LOAD_FUNC_POINTER(hLib, usbHVcom_MaxI);
    ADDR_CHK(usbHVcom_MaxI);
    LOAD_FUNC_POINTER(hLib, usbHVcom_PName);
    ADDR_CHK(usbHVcom_PName);
    LOAD_FUNC_POINTER(hLib, usbHVcom_Prec);
    ADDR_CHK(usbHVcom_Prec);
    LOAD_FUNC_POINTER(hLib, usbHVcom_FWVer);
    ADDR_CHK(usbHVcom_FWVer);

    return true;
}

#endif


#endif