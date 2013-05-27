#pragma once

#include <cstdio>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <deque>
#include <map>

template<typename VALUE_T>
class deque_mp
{
public:
	typedef std::deque<VALUE_T> type;
};

template<typename KEY_T, typename VALUE_T>
class map_mp
{
public:
	typedef std::map<KEY_T, VALUE_T> type;
};

typedef unsigned __int64 SERVICEUSERID;


#pragma comment(lib, "ws2_32.lib")



//Macros
#define CRLF { OutputDebugStringA("\r\n"); }

#define ASSERT(exp)					if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; }
#define ASSERT_RETVAL(exp, x)		if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return x; }
#define ASSERTV_RETURN(x, fmt, ...) if( !(x) ){ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; return; }
#define ASSERT_RETFALSE(exp)		if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return false; }
#define ASSERTV_DO(x, fmt, ...)		if( !(x) ){ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; } if( !(x) )
#define	ASSERT_RETURN(exp)			if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return ; }
#define ASSERT_BREAK(exp)			if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; break; }
#define ASSERT_RETNULL(exp)			if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return NULL; }

#define TraceVerbose(x, fmt, ...)	{ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; }
#define TraceWarn(x, fmt, ...)		{ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; }
#define TraceError(fmt, ...)		{ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF;  }


#define MEMORYPOOL_NEW(pool)		new
#define MEMORYPOOL_DELETE(pool, x)	delete x

#define CONCAT_3_(x, y)					x##y
#define CONCAT_2_(x, y)					CONCAT_3_(x, y)
#define CONCAT(x, y)					CONCAT_2_(x, y)
#define UIDEN(x)						CONCAT(x, __LINE__)

#define ONCE										for (int UIDEN(_once) = 0; UIDEN(_once)++ == 0;)
#define BILLING_OK						(0)
#define PStrCmp(x, y)					strcmp(x, y)

#include <assert.h>
#define DBG_ASSERT(x)					assert(x)

#include <ctime>
#include <string>
#include "d:\PrimeSVN\Prime\source\Common\critsect.h"