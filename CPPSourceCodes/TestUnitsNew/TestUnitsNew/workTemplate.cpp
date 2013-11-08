#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Work.h"

#include "NulltermChecker.h"

#include "boost/dynamic_bitset.hpp"



void WorkImpl<workOne>::DoWork()
{

}

void WorkImpl<workTwo>::DoWork()
{

}

void WorkImpl<vectorPushBack>::DoWork()
{
	int temp[] = {13, 26, 32, 56, 77, 63, 18};
	std::vector<int> myInts(temp, temp + _countof(temp));

	size_t repeatCnt = 700;
	while(repeatCnt-- != 0)
	{
		myInts.push_back( rand() );
	}
}

LONG WINAPI aCrashHandler(
	__in struct _EXCEPTION_POINTERS *ExceptionInfo
	)
{
	int n = 0;

	DebugBreak();

	return EXCEPTION_CONTINUE_SEARCH;
}

void WorkImpl<crashHandler>::DoWork()
{
	unsigned long myValue = 0xFFFFFFFF;
	__int64 myInts64(myValue);

	::SetUnhandledExceptionFilter(aCrashHandler);	

}

void WorkImpl<doCrash>::DoWork()
{
	char* aPtr = nullptr;
	*aPtr = 5;
}

void WorkImpl<queueTest>::DoWork()
{
	std::queue<int> myQueue;
	myQueue.push(11);
	myQueue.push(33);

	size_t nTrialCnt = 3;
	while(nTrialCnt-- > 0)
	{
		std::cout << myQueue.front() << std::endl;
		myQueue.pop();
	}
}

void WorkImpl<interlockTest>::DoWork()
{
	volatile LONG lastValue = INT_MAX;
	std::cout << lastValue << std::endl;


	InterlockedIncrement(&lastValue);

	std::cout << lastValue << std::endl;

	InterlockedIncrement(&lastValue);
	
	std::cout << lastValue << std::endl;


}

#define GetLastOfArr(arr) (arr + _countof(arr))


void WorkImpl<nulltermSearch>::DoWork()
{
	char firstNullterm[256] = {'\0',};
	char secondNulltermAtEnd[256] = {0xFF,};
	char thirdUnsafty[256] = {0xFF,};

	std::fill( secondNulltermAtEnd, GetLastOfArr(secondNulltermAtEnd), 0XFF );
	std::fill( thirdUnsafty, GetLastOfArr(thirdUnsafty), 0XFF );

	secondNulltermAtEnd[255] = '\0';
	
	
	auto firstLast = GetLastOfArr(firstNullterm);
	auto secondLast = GetLastOfArr(secondNulltermAtEnd);
	auto thirdLast = GetLastOfArr(thirdUnsafty);

	auto firsFinded = std::find( firstNullterm, GetLastOfArr(firstNullterm), '\0' );
	auto secondFined = std::find( secondNulltermAtEnd, GetLastOfArr(secondNulltermAtEnd), '\0' );
	auto thirdFined = std::find( thirdUnsafty, GetLastOfArr(thirdUnsafty), '\0' );

	bool bSuccess = NULLTERM_CHECKER::NulltermCheck(firstNullterm);
	bSuccess = NULLTERM_CHECKER::NulltermCheck( secondNulltermAtEnd );
	bSuccess = NULLTERM_CHECKER::NulltermCheck( thirdUnsafty );

	char fArr[10], sArr[20];
	NULLTERM_CHECKER::assign(fArr, sArr);

	char MinusOne[] = {0xFF, 0XFF, 0XFF, 0X7F};
	int nMinusOne = *((int*)MinusOne);


}





void WorkImpl<arrayTemplateCount>::DoWork()
{
	char myInts[256] = {0,};

	bool bSuccess = NULLTERM_CHECKER::NulltermCheck(myInts);


	wchar_t myWchars[348] = {L'\0',};
	bSuccess = NULLTERM_CHECKER::NulltermCheck( myWchars );

	//TestFunction(myInts);
}


void WorkImpl<stdFindArgument>::DoWork()
{
	struct MYSTR
	{
		DWORD searchKey;

		bool operator ==(DWORD dwSearchKey){ return searchKey == dwSearchKey; }
		MYSTR(DWORD key):searchKey(key){}
	};


	MYSTR myStrs[] = { MYSTR(1), MYSTR(2), MYSTR(3) };
	std::vector<MYSTR> vecMystr(myStrs, myStrs + _countof(myStrs));

	std::copy( myStrs , myStrs + _countof(myStrs) ,vecMystr.begin() );

	DWORD dwSearchKey = 7 ;
	std::find(vecMystr.begin(), vecMystr.end(), dwSearchKey );
}

class IFSTATEMENTVARIABLE
{
public:
	IFSTATEMENTVARIABLE(){;}
	IFSTATEMENTVARIABLE(const IFSTATEMENTVARIABLE&){;}
	virtual ~IFSTATEMENTVARIABLE()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	operator bool(){ return false; }
};
void WorkImpl<ifStatementVariableScope>::DoWork()
{
	IFSTATEMENTVARIABLE instanceOne;
	if( IFSTATEMENTVARIABLE _Instance = instanceOne);
	else
	{

	}


	int a = 32;
}

bool CheckStringIntegrity(const char* szStr, size_t strSize)
{
	while(strSize--)
	{
		if(szStr[strSize] == '\0')
			return true;
	}

	return false;
}

void WorkImpl<stringCheck>::DoWork()
{
	char strTest[] = {'a','b','c'};
	CheckStringIntegrity(strTest, _countof(strTest));
}


bool TestAllOrNothing( bool testOne, bool testTwo, bool testThree )
{
	bool bRet = true;

	bool arrBool[] =
	{
		testOne,
		testTwo,
		testThree
	};

	for(int i = 1; i < _countof(arrBool); ++i)
	{
		if(arrBool[i-1] != arrBool[i])
		{
			bRet = false;
			break;
		}
	}
/*
	if(testOne == true && testTwo == true && testThree == true)
		bRet = true;

	if(testOne == false && testTwo == false && testThree == false)
		bRet = true;
*/
/*
	bRet = (testOne == testTwo);
	if(true == bRet)
		bRet = (testTwo == testThree);
*/

	return bRet;
}


void WorkImpl<testAllOrNothing>::DoWork()
{
	std::cout << TestAllOrNothing(true, true, true) << std::endl;
	std::cout << TestAllOrNothing(true, false, true) << std::endl;
	std::cout << TestAllOrNothing(false, false, false) << std::endl;
	std::cout << TestAllOrNothing(false, false, true) << std::endl;
	std::cout << TestAllOrNothing(true, false, false) << std::endl;


	int SSS = 'SSS';
}




void WorkImpl<fourCCLookup>::DoWork()
{
	char aABC[5] = {"AB"};

	unsigned long nABC = 'AB';

	strrev(aABC);

	unsigned long aTonABC = (*((unsigned long*)(aABC)));

	//aTonABC = htonl(aTonABC);
	
	bool bSame = (aTonABC == nABC);

	boost::dynamic_bitset<> x(10);

	x[0] = 1;

	std::string strCurrent;
	to_string( x, strCurrent );

	std::string strNew("1011011010111");

	boost::dynamic_bitset<> y(strNew);

	const char* fileName = "resultset.txt";
	std::fstream aFile;
	aFile.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
	if(false == aFile.is_open())
	{
		assert("file open fail");
	}
	const char szFileName[] = {"resultsetF.txt"};
	
	FILE* pFile = nullptr;
	::fopen_s(&pFile, szFileName, "w+");
	
	char ret = char(-125) - char(131);
	char ret2 = char(-81) - '0';
	char myChar = '0';

	char aValue(0x00);
	int cnt = 0;
	while ( ++cnt < 257) 
	{
		std::stringstream ssCurrent;
		ssCurrent << int(aValue) << "(" << aValue << ") -'0' =\t" << unsigned short(aValue -'0');
		std::cout << ssCurrent.str() << std::endl;
		aFile << ssCurrent.str() << std::endl;

		std::string strCurrent = ssCurrent.str();
		strCurrent.append("\n");
		::fwrite(strCurrent.c_str(), sizeof(char) , strCurrent.size() , pFile);
		++aValue;
	}
	::fclose( pFile );
	pFile = nullptr;
	aFile.close();

	return ;

}

void CreateDynamicArgumentStmt(std::string& toRet, size_t sizeCnt)
{
	toRet.clear();
	while(--sizeCnt)
		toRet.append("?,");

	toRet.append("?");
}

void WorkImpl<DynamicArgument>::DoWork()
{
	std::string strTemp;

	CreateDynamicArgumentStmt( strTemp, 10);
	CreateDynamicArgumentStmt( strTemp, 20);
	CreateDynamicArgumentStmt( strTemp, 128);
}
