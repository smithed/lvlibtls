#pragma once

//libressl
#include "libressl\include\tls.h"
//lv header
#include "cintools\extcode.h"
//included from C++ standard lib, could easily be replaced by C implementation
#include <queue>
#include <mutex>

// From VS:
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LVLIBTLS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LVLIBTLS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LVLIBTLS_EXPORTS
#define LVLIBTLS_API __declspec(dllexport)
#else
#define LVLIBTLS_API __declspec(dllimport)
#endif

//rather than reallocating memory, keep original block and a pointer to the 'true' start
typedef struct lvOffsetString {
	LStrHandle s;
	size_t offset;
} lvOffsetString;

//struct passed around into labview, essentially the reference to the session
typedef struct lvSocketContext {
public:
	std::queue<LStrHandle> fromTlstoNetQ;
	std::queue<lvOffsetString> toTlsfromNetQ;
} lvSocketContext;

#ifdef __cplusplus
extern "C" {
#endif

//To make this type play nice with labview events, we need to make sure things are layed out properly
//This type may no longer be needed #todo:remove?
#include "cintools\lv_prolog.h"
	typedef struct lvEventData {
		int32_t flag;
	} lvEventData;
#include "cintools\lv_epilog.h"
	
	//once a TCP connection is established, this creates the TLS structure, lv context, etc needed for a client TLS connection
	LVLIBTLS_API int tlsConnectLvSocket(struct tls *clientTls, const char *servername, lvSocketContext** lvstate);
	//server equivalent to connect -- creates TLS structures needed to manage the server, called on each new TCP connection
	LVLIBTLS_API int tlsAcceptLvSocket(struct tls *serverTls, struct tls **newConnection, lvSocketContext** lvstate);
	
	//Labview calls this function to see if there is any data it should write to the tcp connection
	LVLIBTLS_API int tlsGetOutputData(lvSocketContext* lvstate, LStrHandle* data);
	//If labview reads from a tcp connection, call this function to provide that string to libressl for decryption
	LVLIBTLS_API int tlsProvideInputData(lvSocketContext* lvstate, LStrHandle* data);

	//Once provided with input data, this function is called to process the data and decrypt. 
	//Returns libtls error codes if more of the tcp stream is needed.
	LVLIBTLS_API int tlsRead(struct tls *clientTls, LStrHandle data, size_t maxSize);
	//Writes plaintext string to libtls, which encrypts it
	//Once completed, data is avail using GetOutputData
	//Returns libtls error if the tls state machine requires more data (eg re-handshake)
	LVLIBTLS_API int tlsWrite(struct tls *clientTls, LStrHandle data);
	
	//helper to read a password-protected file using libressl lib
	LVLIBTLS_API int tlsReadFile(LStrHandle filename, LStrHandle data, LStrHandle password);
	
	//release memory associated with lvSocketContext created by connect/accept functions
	LVLIBTLS_API void tlsFreeLvState(lvSocketContext* lvstate);

	//Initializes tls lib, can be called any number of times NOT CONCURRENT. 
	//Used as initializer callback in LV
	LVLIBTLS_API MgErr lvLibReserve(InstanceDataPtr * i);

#ifdef __cplusplus
}
#endif
