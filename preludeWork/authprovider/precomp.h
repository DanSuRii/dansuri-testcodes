#pragma once

#include <cstdio>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <deque>

template<typename VALUE_T>
class deque_mp
{
public:
	typedef std::deque<VALUE_T> type;
};


#pragma comment(lib, "ws2_32.lib")



//Macros
#define CRLF { OutputDebugStringA("\r\n"); }

#define ASSERT(exp)					if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; }
#define ASSERT_RETVAL(exp, x)		if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return x; }
#define ASSERTV_RETURN(x, fmt, ...) if( !(x) ){ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; return; }
#define ASSERT_RETFALSE(exp)		if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return false; }
#define ASSERTV_DO(x, fmt, ...)		if( !(x) ){ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; } if( !(x) )
#define	ASSERT_RETURN(exp)			if( !(exp) ){ char szErr[256]; sprintf_s(szErr, "%s %s %s", __FUNCTION__, __FILE__, __LINE__ ); OutputDebugStringA(szErr); CRLF; return ; }

#define TraceVerbose(x, fmt, ...)	{ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; }
#define TraceWarn(x, fmt, ...)		{ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF; }
#define TraceError(fmt, ...)		{ char szErr[256]; sprintf_s(szErr, fmt, __VA_ARGS__ ); OutputDebugStringA(szErr); CRLF;  }


#define MEMORYPOOL_NEW(pool)		new
#define MEMORYPOOL_DELETE(pool, x)	delete x