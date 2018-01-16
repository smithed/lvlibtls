// lvlibtls.cpp : Defines the exported functions for the DLL application.
//
#define LVLIBTLS_EXPORTS

#include "lvlibtls.h" //this header
#include "libressl\include\tls.h" //libtls
#include "cintools\extcode.h" //labview
#include "openssl\ssl.h" //required by libtls
#include <string.h>
#include <vector>
#include <stdlib.h>

//define output queue size -- ie how many written chunks may be in memory at once
#ifndef LVLIBTLSOUTQSIZE
#define LVLIBTLSOUTQSIZE 10000
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//internal callback called by libtls when it needs to read or write data
	//cb_arg is initialized in connect/accept to be the lvSocketContext referred to as lvstate
	ssize_t  lvReadCallback(struct tls *ctx, void *buf, size_t buflen, void *cb_arg);
	ssize_t lvWriteCallback(struct tls *ctx, const void *buf, size_t buflen, void *cb_arg);
	
	//once a TCP connection is established, this creates the lv context, initializes tls ref, etc needed for a client TLS connection
	//initializes lvstate
	//returns >= if OK
	LVLIBTLS_API int tlsConnectLvSocket(struct tls * clientTls, const char *servername, lvSocketContext ** lvstate)
	{
		if (clientTls && servername && lvstate) {
			(*lvstate) = new lvSocketContext{};
			return tls_connect_cbs(clientTls, lvReadCallback, lvWriteCallback, (*lvstate), servername);
		}
		else {
			return -1;
		}
	}
	
	//server equivalent to connect -- creates TLS structures needed to manage the server, called on each new TCP connection
	//initializes lvstate
	//returns >= if OK
	LVLIBTLS_API int tlsAcceptLvSocket(struct tls * serverTls, struct tls ** newConnection, lvSocketContext ** lvstate)
	{
		if (serverTls && newConnection && lvstate) {
			(*lvstate) = new lvSocketContext{};
			auto ret = tls_accept_cbs(serverTls, newConnection, lvReadCallback, lvWriteCallback, (*lvstate));
			return ret;
		}
		else {
			return -1;
		}
	}

	//release memory associated with lvSocketContext created by connect/accept functions
	//frees lvstate
	LVLIBTLS_API void tlsFreeLvState(lvSocketContext * lvstate)
	{
		if (lvstate) {
			//both queues could have initialized data, so we need to pop everything off and deallocate
			
			while (!lvstate->fromTlstoNetQ.empty()) {
				auto elem = lvstate->fromTlstoNetQ.front();
				DSDisposeHandle(elem);
				lvstate->fromTlstoNetQ.pop();
			}

			while (!lvstate->toTlsfromNetQ.empty()) {
				auto elem = lvstate->toTlsfromNetQ.front();
				DSDisposeHandle(elem.s);
				lvstate->toTlsfromNetQ.pop();
			}

			delete lvstate;
		}
	}

	//Initializes tls lib, can be called any number of times NOT CONCURRENT. 
	//Used as initializer callback in LV
	LVLIBTLS_API MgErr lvLibReserve(InstanceDataPtr * i)
	{
		tls_init();
		return mgNoErr;
	}

	//Writes plaintext string to libtls, which encrypts it
	//Once completed, data is avail using GetOutputData
	//Returns libtls error if the tls state machine requires more data (eg re-handshake)
	//returns -1 if passed invalid pointers
	LVLIBTLS_API int tlsWrite(tls * clientTls, LStrHandle data)
	{
		if (!clientTls || !data) {
			return -1;
		}
		//avoid copies because of us, pass lv buffer data directly to tls
		auto ret = tls_write(clientTls, LHStrBuf(data), LHStrLen(data));
		return ret;
	}

	//helper to read a password-protected file using libressl lib
	//Returns -1 if passed bad ptrs
	LVLIBTLS_API int tlsReadFile(LStrHandle filename, LStrHandle data, LStrHandle password)
	{
		if (filename && data && password && LHStrBuf(filename) && LHStrLen(filename) > 0) {
			
			//copy lvstrs to cstrs
			char* pw = nullptr;
			if (password && LHStrBuf(password) && LHStrLen(password) > 0) {
				pw = new char[LHStrLen(password) + 1];
				pw[LHStrLen(password)] = 0;
				memcpy(pw, LHStrBuf(password), LHStrLen(password));
			}			
			auto file = new char[LHStrLen(filename)+1];
			file[LHStrLen(filename)] = 0;
			memcpy(file, LHStrBuf(filename), LHStrLen(filename));
			
			//call the tls load to do the actual decrypting
			//can't do this directly from LV due to return value -- need to free the memory
			size_t len = 0;
			auto filestr = tls_load_file(file, &len, pw);
			delete[] file;
			delete[] pw;

			//libtls cstr->lvstr
			if (!filestr) {
				return -1;
			}
			DSSetHandleSize(data, len+sizeof(LStr::cnt));
			LStrLen(LHStrPtr(data)) = len;
			memcpy(LHStrBuf(data), filestr, len);

			for (size_t i = 0; i < len; i++)
			{
				filestr[i] = 0;
			}

			//tls_unload_file was not available at time of writing
			//should be avail in later versions

			return len;
		}
		else {
			return -1;
		}
	}

	//called when libTLS is ready to put data on the network, initialized in connect/accept
	//cb_arg is initialized in connect/accept to be the lvSocketContext referred to as lvstate
	//returns general error (-1) to libtls if passed bad ptrs
	//returns TLS_WANT_POLLOUT if output queue is filled and we need lv to do a tcp write
	//return buflen (>=0) if success
	ssize_t lvWriteCallback(struct tls *ctx, const void *buf, size_t buflen, void *cb_arg) {
		if (cb_arg && buf && ctx && buflen > 0) {
			auto lvstate = (lvSocketContext*)cb_arg;

			if ((lvstate->fromTlstoNetQ.size()) >= LVLIBTLSOUTQSIZE ) {
				//"filled" queue
				return TLS_WANT_POLLOUT;
			}

			//if we have space, allocate new lvstr and copy data (*buf) into it
			auto bufStr = (LStrHandle)DSNewHandle(buflen + sizeof(LStr::cnt));
			if (!bufStr) {
				return -1;
			}
			LStrLen(LHStrPtr(bufStr)) = buflen;
			memcpy(LHStrBuf(bufStr), buf, buflen);

			
			try {
				lvstate->fromTlstoNetQ.push(bufStr);
			}
			catch (...) {
				if(bufStr) {
					DSDisposeHandle(bufStr);
				}
				return -1;
			}
			return buflen;
		}
		else { return -1; }
	}

	//Labview calls this function to see if there is any data it should write to the tcp connection
	//return -2 if an unknown error occurred
	//return -1 if ptrs invalid
	//else return queue size BEFORE POP, >=0 (eg if == 0, data is invalid and the q is empty)
	LVLIBTLS_API int tlsGetOutputData(lvSocketContext * lvstate, LStrHandle* data)
	{
		try
		{
			if (lvstate && data) {
				LStrHandle buf;
				auto q = &(lvstate->fromTlstoNetQ);

				if (q->empty()) {
					return 0;
				}
				buf = q->front();
				q->pop();
				if (!buf) {
					return -2;
				}
				
				//swap the pointers to avoid calling DSNewHandle
				//Most likely, *data is a null handle anyway
				auto junkData = (*data);
				(*data) = buf;

				if (junkData) {
					DSDisposeHandle(junkData);
				}

				return (q->size()) + 1;
			}
			else {
				return -1;
			}
		}
		catch (...)
		{
			return -2;
		}
	}

	//Once ProvidedInputData, this function is called to process the data and decrypt. 
	//Returns libtls error codes if more of the tcp stream is needed.
	//return -1 if ptrs invalid
	//Returns data len >=0 if OK
	LVLIBTLS_API int tlsRead(tls * clientTls, LStrHandle data, size_t maxSize)
	{
		if (!clientTls || !data || 0 == maxSize) {
			return -1;
		}

		DSSetHandleSize(data, maxSize + sizeof(LStr::cnt));
		auto ret = tls_read(clientTls, LHStrBuf(data), maxSize);
		if (ret <= 0) {
			DSSetHSzClr(data, sizeof(LStr::cnt)); //empty string
		}
		else {
			DSSetHandleSize(data, ret + sizeof(LStr::cnt));
			LStrLen(LHStrPtr(data)) = ret;
		}
		return ret;
	}

	//If labview reads from a tcp connection, call this function to provide that string to libressl for decryption
	//return -1 if bad ptr or malloc failed
	//return -2 if unknown internal error (ie if enqueue fails)
	LVLIBTLS_API int tlsProvideInputData(lvSocketContext* lvstate, LStrHandle* data)
	{
		if (lvstate && data && *data && LHStrPtr(*data) && LHStrBuf(*data) && LHStrLen(*data) > 0) {
			auto emptyStr = (LStrHandle)DSNewHClr(sizeof(LStr::cnt)); //empty lstrhandle
			if (!emptyStr) {
				return -1;
			}
			
			//just swap pointers rather than copying
			//labview will get an empty string out of *data as a result
			LStrHandle localData = *data;
			*data = emptyStr;
			
			try {
				(lvstate->toTlsfromNetQ).push(lvOffsetString{ localData, 0 });
			}
			catch (...)
			{
				return -2;
			}
			return 1;
		}
		else {
			return -1;
		}
	}

	//called when libTLS is ready to read data from the 'network', initialized in connect/accept
	//cb_arg is initialized in connect/accept to be the lvSocketContext referred to as lvstate
	//returns general error (-1) to libtls if passed bad ptrs
	//returns TLS_WANT_POLLIN if input queue is empty and we need lv to do a tcp read
	//return buflen (>=0) if success
	ssize_t  lvReadCallback(struct tls *ctx, void *buf, size_t buflen, void *cb_arg) {
		if (cb_arg && buf && ctx && buflen > 0) {
			auto tlsStr = (unsigned char *)buf;
			auto lvstate = (lvSocketContext*)cb_arg;

			size_t currLen = 0;
			//currLen updated as data is read
			while (currLen < buflen) {
				if (lvstate->toTlsfromNetQ.empty()) {
					//no more data avail -- if no data was available, we need lv to do a read
					break;
				}
				else {
					//the first element of the queue will have some data in it
					//grab it and copy it into tls buf
					auto needLen = buflen - currLen;
					lvOffsetString& netBuf = lvstate->toTlsfromNetQ.front();
					
					auto netStr = netBuf.s;
					auto netOffset = netBuf.offset;
					if (needLen >= (LHStrLen(netStr) - netOffset)) {
						//want more data than avail from this buffer, so we consume this packet
						auto moveLen = (LHStrLen(netStr) - netOffset);
						memcpy((tlsStr + currLen), LHStrBuf(netStr) + netOffset, moveLen);
						currLen += moveLen;
						
						//consume:
						DSDisposeHandle(netStr);
						lvstate->toTlsfromNetQ.pop();
					}
					else {
						//does not consume full data set, so adjust offset and leave on queue
						memcpy((tlsStr + currLen), LHStrBuf(netStr) + netOffset, needLen);
						netBuf.offset += needLen; //netbuf is a *ref* to the first element in the queue
						currLen += needLen;
						//dont pop, keep data in queue
					}
				}
			}

			if (currLen) { //if we got any data -- we don't have to fill the buffer, necessarily
				return currLen;
			}
			else { //if we never got any data, tell lv to do a read
				return TLS_WANT_POLLIN;
				
			}
		}
		else { return -1; }
	}



#ifdef __cplusplus
}
#endif


