#include "stdafx.h"
#include "TestUnitRepository.h"
#include <ctime>

DECL_TESTUNIT(ShutdownTest)


void ShutdownTest::DoExecute()
{
		static const int SECONDS_PER_YEAR = 365*24*60*60;
		time_t presentTime = time(NULL);
		time_t tRequiredYear = presentTime - time_t(SECONDS_PER_YEAR*16);
		tm tmRequiredYear;
		localtime_s(&tmRequiredYear, &tRequiredYear);

/*		//만으로 처리
		tmRequiredYear.tm_mon = 0;
		tmRequiredYear.tm_mday = 1;
		tmRequiredYear.tm_hour = 0;
		tmRequiredYear.
*/
		std::cout <<  tmRequiredYear.tm_year + 1900 << std::endl;

}