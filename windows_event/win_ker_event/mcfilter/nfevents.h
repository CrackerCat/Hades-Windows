#ifndef _NFEVENTS_H
#define _NFEVENTS_H

/**
*	Return status codes
**/
typedef enum _NF_STATUS
{
	NF_STATUS_SUCCESS		= 0,
	NF_STATUS_FAIL			= -1,
	NF_STATUS_INVALID_ENDPOINT_ID	= -2,
	NF_STATUS_NOT_INITIALIZED	= -3,
	NF_STATUS_IO_ERROR		= -4,
	NF_STATUS_REBOOT_REQUIRED	= -5
} NF_STATUS;

#ifndef _C_API
#define NFAPI_CC	

/////////////////////////////////////////////////////////////////////////////////////
// C++ API
/////////////////////////////////////////////////////////////////////////////////////

/*
*	�ӿ� - �ϲ����ݲ���
*/
class NF_EventHandler
{
public:
	// Kernel Callout Sys Process
	virtual void processPacket(const char* buf, int len) = 0;

	// Kernel Callout Sys Thread
	virtual void threadPacket(const char* buf, int len) = 0;

	// Kernel Callout Sys Module
	virtual void imagemodPacket(const char* buf, int len) = 0;

	// Kernel Callout Register Table
	virtual void registerPacket(const char* buf, int len) = 0;

	// Kernel Callout FileObj
	virtual void filePacket(const char* buf, int len) = 0;
};

#else // _C_API


// DLL
#define NFAPI_CC __cdecl
#define NFAPI_NS

/////////////////////////////////////////////////////////////////////////////////////
// C API
/////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" 
{
#endif

#include "nfdriver.h"

#pragma pack(push, 1)

// C analogue of the class NF_EventHandler (see the definition above)
typedef struct _NF_EventHandler
{
	void (NFAPI_CC * processPacket)(const char * buf, int len);
	void (NFAPI_CC * threadPacket)(const char * buf, int len);
	void (NFAPI_CC* imagemodPacket)(const char* buf, int len);
	void (NFAPI_CC* registerPacket)(const char* buf, int len);
	void (NFAPI_CC* filePacket)(const char* buf, int len);
} NF_EventHandler, *PNF_EventHandler;

#pragma pack(pop)

#endif // _C_API


#ifdef __cplusplus

#endif

#endif