#include "stdafx.h"
#include "TestUnitRepository.h"

#include "d:/PrimeSVN/Prime/source/Common/strptime.h"
#include "d:/PrimeSVN/Prime/source/Common/strptime.cpp"

DECL_TESTUNIT(pTimeTest);

void pTimeTest::DoExecute()
{
	tm tmTest;
	strptime( "2012-06-20 17:52:27", "%Y-%m-%d %H:%M:%S", &tmTest );

	static const char fmt[] = "yyyy-mm-dd hh:mm:ss";
	static const int fmtLen = _countof(fmt);

	return ;

}