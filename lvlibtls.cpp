// lvlibtls.cpp : Defines the exported functions for the DLL application.
//
#define LVLIBTLS_EXPORTS

#include "lvlibtls.h"
#include "libressl\include\tls.h"
#include "cintools\extcode.h"
#include "openssl\ssl.h"
#include <string.h>
#include <vector>
#include <stdlib.h>

#ifndef LVLIBTLSOUTQSIZE
#define LVLIBTLSOUTQSIZE 10000
#endif

#ifdef __cplusplus
extern "C" {
#endif
	ssize_t  lvReadCallback(struct tls *ctx, void *buf, size_t buflen, void *cb_arg);
	ssize_t lvWriteCallback(struct tls *ctx, const void *buf, size_t buflen, void *cb_arg);

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

	LVLIBTLS_API void tlsFreeLvState(lvSocketContext * lvstate)
	{
		if (lvstate) {
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

	LVLIBTLS_API MgErr lvLibReserve(InstanceDataPtr * i)
	{
		tls_init();
		return mgNoErr;
	}


	LVLIBTLS_API int tlsWrite(tls * clientTls, LStrHandle data)
	{
		if (!clientTls || !data) {
			return -1;
		}
		//avoid copies because of us
		auto ret = tls_write(clientTls, LHStrBuf(data), LHStrLen(data));
		return ret;
	}

	LVLIBTLS_API int tlsReadFile(LStrHandle filename, LStrHandle data, LStrHandle password)
	{
		if (filename && data && password && LHStrBuf(filename) && LHStrLen(filename) > 0) {
			
			char* pw = nullptr;
			if (password && LHStrBuf(password) && LHStrLen(password) > 0) {
				pw = new char[LHStrLen(password) + 1];
				pw[LHStrLen(password)] = 0;
				memcpy(pw, LHStrBuf(password), LHStrLen(password));
			}

			auto file = new char[LHStrLen(filename)+1];
			file[LHStrLen(filename)] = 0;
			memcpy(file, LHStrBuf(filename), LHStrLen(filename));

			size_t len = 0;
			auto filestr = tls_load_file(file, &len, pw);
			delete[] file;
			delete[] pw;

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

			//tls_unload_file later versions

			return len;
		}
		else {
			return -1;
		}
	}

	ssize_t lvWriteCallback(struct tls *ctx, const void *buf, size_t buflen, void *cb_arg) {
		if (cb_arg && buf && ctx && buflen > 0) {
			auto lvstate = (lvSocketContext*)cb_arg;

			if ((lvstate->fromTlstoNetQ.size()) >= LVLIBTLSOUTQSIZE ) {
				//"filled" queue
				return TLS_WANT_POLLOUT;
			}

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
				return -1;
			}
			return buflen;
		}
		else { return -1; }
	}


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


	LVLIBTLS_API int tlsProvideInputData(lvSocketContext* lvstate, LStrHandle* data)
	{
		if (lvstate && data && *data && LHStrPtr(*data) && LHStrBuf(*data) && LHStrLen(*data) > 0) {
			auto emptyStr = (LStrHandle)DSNewHClr(sizeof(LStr::cnt)); //empty lstrhandle
			if (!emptyStr) {
				return -1;
			}
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

	ssize_t  lvReadCallback(struct tls *ctx, void *buf, size_t buflen, void *cb_arg) {
		if (cb_arg && buf && ctx && buflen > 0) {
			auto tlsStr = (unsigned char *)buf;
			auto lvstate = (lvSocketContext*)cb_arg;

			size_t currLen = 0;
			while (currLen < buflen) {
				if (lvstate->toTlsfromNetQ.empty()) {
					break;
				}
				else {
					auto needLen = buflen - currLen;
					lvOffsetString& netBuf = lvstate->toTlsfromNetQ.front();
					auto netStr = netBuf.s;
					auto netOffset = netBuf.offset;
					if (needLen >= (LHStrLen(netStr) - netOffset)) {
						//want more data than avail from this buffer, so we consume this packet
						auto moveLen = (LHStrLen(netStr) - netOffset);
						memcpy((tlsStr + currLen), LHStrBuf(netStr) + netOffset, moveLen);
						currLen += moveLen;

						DSDisposeHandle(netStr);
						lvstate->toTlsfromNetQ.pop();
					}
					else {
						//does not consume full data set
						memcpy((tlsStr + currLen), LHStrBuf(netStr) + netOffset, needLen);
						netBuf.offset += needLen;
						currLen += needLen;
						//dont pop, keep data in queue
					}
				}
			}

			if (currLen) {
				return currLen;
			}
			else {
				return TLS_WANT_POLLIN;
			}
		}
		else { return -1; }
	}



#ifdef __cplusplus
}
#endif


