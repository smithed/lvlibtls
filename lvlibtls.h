#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LVLIBTLS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LVLIBTLS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "libressl\include\tls.h"
#include "cintools\extcode.h"
#include <queue>
#include <mutex>

#ifdef LVLIBTLS_EXPORTS
#define LVLIBTLS_API __declspec(dllexport)
#else
#define LVLIBTLS_API __declspec(dllimport)
#endif

typedef struct lvOffsetString {
	LStrHandle s;
	size_t offset;
} lvOffsetString;


typedef struct lvSocketContext {
public:
	std::queue<LStrHandle> fromTlstoNetQ;
	std::queue<lvOffsetString> toTlsfromNetQ;
} lvSocketContext;

#ifdef __cplusplus
extern "C" {
#endif

#include "cintools\lv_prolog.h"
	typedef struct lvEventData {
		int32_t flag;
	} lvEventData;
#include "cintools\lv_epilog.h"
	LVLIBTLS_API int tlsConnectLvSocket(struct tls *clientTls, const char *servername, lvSocketContext** lvstate);
	LVLIBTLS_API int tlsAcceptLvSocket(struct tls *serverTls, struct tls **newConnection, lvSocketContext** lvstate);

	LVLIBTLS_API int tlsGetOutputData(lvSocketContext* lvstate, LStrHandle* data);
	LVLIBTLS_API int tlsProvideInputData(lvSocketContext* lvstate, LStrHandle* data);

	LVLIBTLS_API int tlsRead(struct tls *clientTls, LStrHandle data, size_t maxSize);
	LVLIBTLS_API int tlsWrite(struct tls *clientTls, LStrHandle data);

	LVLIBTLS_API int tlsReadFile(LStrHandle filename, LStrHandle data, LStrHandle password);

	LVLIBTLS_API void tlsFreeLvState(lvSocketContext* lvstate);

	LVLIBTLS_API MgErr lvLibReserve(InstanceDataPtr * i);

#ifdef __cplusplus
}
#endif