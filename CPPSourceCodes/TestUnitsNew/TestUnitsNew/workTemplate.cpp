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
#include <comdef.h>
#include <string>


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

	/*
	template<class T> const char* GetMsgName(T& tMsg){ return "INVALID"; }
	template<> const char* GetMsgName(MsgBase& tMsg){ return MsgUtil::GetMsgName(tMsg.m_wMH); }
	*/
	//file: __FILE__, line: __LINE__, function: __FUNCTION__ cond: %s, MSGNAME: %s, ????, GetMsgName() ,

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
	CreateDynamicArgumentStmt( strTemp, 64*3); //64(Array Size), 3(member count)

	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION info;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	ZeroMemory(&info, sizeof(info));	
	
	DWORD dwErr;
	char szWindowsDir[MAX_PATH];	
	GetWindowsDirectoryA((LPSTR)&szWindowsDir, _countof(szWindowsDir));
	strcat_s( szWindowsDir, "\\system32");
	char* parameter = "cmd.exe /c @echo Duplicate Session & pause";


	if( CreateProcessA( NULL, parameter, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, szWindowsDir, &startupInfo, &info ) )
	{
		// Wait till cmd do its job
		//WaitForSingleObject( info.hProcess, INFINITE );
		// Check whether our command succeeded?
		//GetExitCodeProcess( info.hProcess, &dwErr );
		// Avoid memory leak by closing process handle
		CloseHandle( info.hProcess );
	}
	
}

template< class T >
struct DebugAssignString
{

public:
	void Byte3Operation(
		std::string& pointStr,
		size_t& idx,
		std::string& toAssign,
		size_t& assignIdx,
		std::stringstream& resultStr
		)
	{
		//2���� ���� ������ 1���� \0���� �� �ش�.
		char buffer[2];

		size_t sizeToWrite = 0;
		for each(char& pCur in buffer)
		{
			pCur = toAssign[idx++];
			++sizeToWrite;
			if(idx >= toAssign.length())
				break;				
		}

		std::string strValue;
		for( size_t bufferLen = _countof(buffer);
			bufferLen > 0; --bufferLen)
		{
			char cCur = buffer[bufferLen-1];
			if(cCur == 0) strValue += "\\0";
			strValue += cCur;
		}

		resultStr << "(*((short*)&" << pointStr << "[" << assignIdx << "])='" << strValue << "')&";
		assignIdx+=2;
		resultStr << "(*((char*)&" << pointStr << "[" << assignIdx << "])='\\0')";
		assignIdx+=1;
	}

	void operator()(
		std::string& pointStr,
		size_t lenOfPtr,
		std::string& toAssign ,
		std::stringstream& resultStr)
	{
		char buffer[4];
		size_t idx = 0;
		resultStr.clear();

		size_t assignIdx = 0;
		
		while( idx < toAssign.length())
		{
			memset(buffer, 0, sizeof(buffer));
			if(0 >= lenOfPtr)
			{
				break ;
			}
			
			if(lenOfPtr == 3)
			{
				Byte3Operation(pointStr, idx, toAssign, assignIdx, resultStr);
				break;
			}
			size_t sizeToWrite = 0;
			for each(char& pCur in buffer)
			{
				pCur = toAssign[idx++];
				++sizeToWrite;
				if(idx >= toAssign.length())
					break;				
			}
			

			std::string strValue;
			for( size_t bufferLen = _countof(buffer);
				bufferLen > 0; --bufferLen)
			{
				char cCur = buffer[bufferLen-1];
				if(cCur == 0) strValue += "\\0";
				strValue += cCur;
			}


			std::string strTypeName;

			switch(sizeToWrite)
			{
			case 1: strTypeName = "char";	break;
			case 2: strTypeName = "short";	break;
			default: strTypeName = "int";	break;
			}
			resultStr << "(*((" << strTypeName << "*)&" << pointStr << "[" << assignIdx << "])='" << strValue << "')";
			switch(sizeToWrite)
			{
			case 1: assignIdx+=1;	break;
			case 2: assignIdx+=2;	break;
			default: assignIdx+=4;	break;
			}
			if(idx < toAssign.length())
				resultStr << "&";
			
			lenOfPtr -=  sizeToWrite;

		}
	}
};

void WorkImpl<DbgAssignString>::DoWork()
{
	DebugAssignString<char> _Instance;
	char m_szToCharName[16];
	
	std::stringstream ssResult;
	_Instance(std::string("m_szToCharName"), _countof(m_szToCharName), std::string("My Name Is Dennies"), ssResult);

	std::cout << ssResult.str() << std::endl;
}


struct StaticMember
{
	StaticMember()
	{

	}

	int GetNum(){return 5;}
};

struct Foo
{

	static StaticMember aMember;	
};

StaticMember Foo::aMember;

void WorkImpl<staticTest>::DoWork()
{
	Foo _Instance;
	_Instance.aMember;
}


/*
toStr(ptr:string, size_t lenOfPtr, str:string)

	char buffer[4];
	idx


while( idx < str.len() )
{
	buffer.clear();
	for each(char* pCur, buffer)
	{
		pCur = str[idx++]
		if( str.len() < idx )
			break;
	}

	//(*((int*)m_szToCharName) = 'nned') 
	result += "(*((int*)" + ptr + "["+ itoa(+=4) +]) = " + reverse(buffer) + ")"
	if(idx < str.len())
		result += "&"
}

*/
#include <tuple>
#include <map>

struct lexicographical_TYPE
{
	lexicographical_TYPE( int a, std::string& b, unsigned short c )
		:m_a(a),m_b(b),m_c(c)
	{

	}

	int m_a;
	std::string m_b;
	unsigned short m_c;

	bool operator <(const lexicographical_TYPE& rhs) const
	{
		return std::tie(m_a, m_b, m_c) < std::tie(rhs.m_a, rhs.m_b, rhs.m_c);
	}
};

/*
auto make_tie(const lexicographical_TYPE& rhs) -> decltype( std::tr1::tuple& )
{
	return std::tie(rhs.m_a, rhs.m_b, rhs.m_c);
}

bool lexicographical_TYPE::operator <(const lexicographical_TYPE& rhs) const
{
	//return std::tie(m_a, m_b, m_c) < std::tie(rhs.m_a, rhs.m_b, rhs.m_c);
	return make_tie(*this) < make_tie(rhs);
}
*/
std::ostream& operator <<(std::ostream& lhs, const lexicographical_TYPE& rhs)
{
	return lhs << "A(" << rhs.m_a << "),B("<< rhs.m_b << "),C("<<rhs.m_c << ")";
}

class DestructA{
public:
	virtual ~DestructA()
	{
	}
};
class DestructB : public DestructA{
public:
	virtual ~DestructB()
	{
	}

};
class DestructC : public DestructB{
public:
	~DestructC()
	{
	}

};
class DestructD : public DestructC{
public:
	~DestructD()
	{
	}
};

void WorkImpl<lexicographical_comparison>::DoWork()
{
	lexicographical_TYPE A[]= 
	{
		lexicographical_TYPE(32, std::string("MyTimes"), 64),
		lexicographical_TYPE(32, std::string("MyTimes"), 72),
		lexicographical_TYPE(31, std::string("MyTimes"), 64),
		lexicographical_TYPE(32, std::string("MyTime"), 64),
		lexicographical_TYPE(32, std::string("MyTimes"), 71),
	};
	
	std::map< lexicographical_TYPE, int > myMap;
	int Value = 0;
	for each(auto& a in A)
	{
		myMap.insert( std::make_pair(a, ++Value) );
	}

	auto iter = myMap.begin();
	do{
		std::cout << "KEY("<< iter->first << ") VALUE(" << iter->second << ")" << std::endl;
	}while(++iter != myMap.end());

	DestructA* pA = new DestructD;
	delete pA;

	return ;
}


class CLocalBraceStatic
{
public:
	CLocalBraceStatic(){}
	~CLocalBraceStatic(){}
};

union HostParamAddr
{	
	DWORD64 dw64Value;
	struct
	{
		DWORD64 dummyLSB:3;
		DWORD64 wPort:16;
		DWORD64 dummy1: 6;
		DWORD64 dwIP:32;
		DWORD64 dummy2: 6;
		DWORD64 MSB: 1;
	};

	HostParamAddr():MSB(0){}
};

void WorkImpl<localBraceStatic>::DoWork()
{
	static bool bInitFlag = true;
	
	if(true == bInitFlag)
	{
		static CLocalBraceStatic _Instance;
	}

/*
	HostParamAddr hostParamAddr;
	hostParamAddr.wPort = 0x1F1F1F1F1F1F1F1F;
	hostParamAddr.dummy1 = 0x2F2F2F2F2F2F2F2F;
	hostParamAddr.dwIP = 0x3F3F3F3F3F3F3F3F;
	hostParamAddr.dummy2 = 0x4F4F4F4F4F4F4F4F;
	hostParamAddr.dummyMSB = 0x5F5F5F5F5F5F5F5F;
*/
}

enum eBitLists
{
	eBitLists_INVALID,
	eBitLists_A,
	eBitLists_B,
	eBitLists_C,

	eBitLists_CNT,
};

template<int> struct BitTypeT;

template<> struct BitTypeT<eBitLists_INVALID>
{
public:
	enum { bitSize = 0, byteSize = 0 };
};

#define DECLARE_BITTYPE( eBitType, curBitSize ) template<> class BitTypeT< eBitType > {  \
public:\
	enum {	bitSize = curBitSize + BitTypeT< eBitType-1 >::bitSize,\
	byteSize = curBitSize + BitTypeT< eBitType-1 >::byteSize, };};

DECLARE_BITTYPE(eBitLists_A, 4);
DECLARE_BITTYPE(eBitLists_B, 7);
DECLARE_BITTYPE(eBitLists_C, 5);

template<> struct BitTypeT<eBitLists_CNT>
{
	enum {	bitSize = BitTypeT< eBitLists_CNT-1 >::bitSize,
			byteSize = 1 + (BitTypeT< eBitLists_CNT-1 >::byteSize / 8), };
};


class HOSTEXECUTER
{
	HostParamAddr paramAddr;
	char lastInput[10];

public:
	void Run();

	//return false if user select quit
	bool GetInput(){
		PrintWelcome();

		std::cin >> lastInput;
		if(lastInput[0] == 'q')
			return false;

		return true;
	}

	void PrintWelcome();

	HANDLE HostsExecute( unsigned long long nCnt );



};

class HostProcessPool
{
public:
	HostProcessPool():_HostProcessQuantity(0){ for each( HANDLE& handle in _HostProcesses) handle = INVALID_HANDLE_VALUE; }

	bool CheckAlive();
	//void HostsExecute( size_t sizeToExecute, HOSTEXECUTER& executer );
	void EventSignaled( int nIdx );
	void GrowSize(size_t sizeToGrow);

	void Run( HOSTEXECUTER& );


private:
	size_t		_HostProcessQuantity;
	HANDLE		_HostProcesses[256];
};

void HostProcessPool::GrowSize( size_t sizeToGrow )
{
	size_t sizeResult = _HostProcessQuantity + sizeToGrow;
	if( sizeResult < _countof(_HostProcesses)  )
		_HostProcessQuantity = sizeResult;

/*
	InterlockedCompareExchange(
		&_HostProcessQuantity,
		_HostProcessQuantity + sizeToGrow,
		(_HostProcessQuantity + sizeToGrow) < _countof(_HostProcesses));
*/
	return ;
}

void HostProcessPool::Run( HOSTEXECUTER& executer )
{
	if(false == CheckAlive())
	{
		HANDLE* iterCur = _HostProcesses;
		HANDLE* iterEnd = _HostProcesses + _HostProcessQuantity;

		while(iterCur != iterEnd)
		{
			if( (*iterCur) == INVALID_HANDLE_VALUE)
				(*iterCur) = executer.HostsExecute( 1 );

			iterCur++;
		}
	}
}



bool HostProcessPool::CheckAlive()
{
	DWORD dwRet = WaitForMultipleObjects(_HostProcessQuantity, _HostProcesses, FALSE, 0);
	 switch(dwRet )
	 {
	 case WAIT_TIMEOUT:
		 return true;

	 case WAIT_FAILED:
		 {
			 HRESULT hrRet = GetLastError();
			 _com_error err(hrRet);
			 OutputDebugStringW(err.ErrorMessage());
			 if(ERROR_INVALID_HANDLE != hrRet)
				 std::cout << __FUNCTION__ "() fatal error occurred" << std::endl;
		 }
		 return false;

	 default:
		 EventSignaled( dwRet - WAIT_OBJECT_0 );
		 return false;
	 }
}

void HostProcessPool::EventSignaled( int nIdx )
{
	if(nIdx < 0 || nIdx >= _HostProcessQuantity)
	{
		std::cout << __FUNCTION__ "() invalid index ("<< nIdx << ")" << std::endl;
		return ;
	}
	std::cout << __FUNCTION__ "() Process terminated " << std::endl
		<< "HANDLE(" << _HostProcesses[nIdx]	<< ")"
		<< "INDEX("  << nIdx					<< ")";
	_HostProcesses[nIdx] = INVALID_HANDLE_VALUE;
}



void HOSTEXECUTER::Run()
{
	paramAddr.dwIP = inet_addr("127.0.0.1");
	paramAddr.wPort = 5200;

	HostProcessPool processPool;

	while(GetInput())
	{
		if(lastInput[0] > '0' || lastInput[0] <= '9')
			processPool.GrowSize( atoi(lastInput) );
		else
			std::cout << "Unavailable input" << std::endl;

		processPool.Run( *this );
	}
}

void HOSTEXECUTER::PrintWelcome()
{
	const char* szLine = "**************************************************************";

	std::cout << std::endl
		<< szLine		<< std::endl
		<< "IP:"		<< paramAddr.dwIP		<< std::endl
		<< "Port:"		<< paramAddr.wPort		<< std::endl
		<< "HostParam:" << paramAddr.dw64Value	<< std::endl
		<< szLine		<< std::endl
		<< "Input ('q' to exit, Numbers to hosts execute count): ";	
}

HANDLE HOSTEXECUTER::HostsExecute( unsigned long long nCnt )
{
/*
	std::stringstream ssCommand;
	ssCommand << "start \"\" \"C:\\vector\\programtrunk\\bin\\BinDX11\\GameHostDX11D.exe\" -dedicated " << paramAddr.dw64Value;
	
	std::cout << ssCommand.str() << std::endl;
	system(ssCommand.str().c_str());
*/
	HANDLE handleRet = INVALID_HANDLE_VALUE;

	std::stringstream ssCommand;
	ssCommand << "\"C:\\vector\\programtrunk\\bin\\BinDX11\\GameHostDX11D.exe\" -dedicated " << paramAddr.dw64Value
		<< " -parent " << GetCurrentProcessId();
	STARTUPINFOA startInfo;
	startInfo.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION processInfo;
	
	ZeroMemory(&startInfo, sizeof(startInfo));	
	ZeroMemory(&processInfo, sizeof(processInfo));


	LPSTR szCommandLine = _strdup(ssCommand.str().c_str());
	if(FALSE == ::CreateProcessA(nullptr, szCommandLine, nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &startInfo, &processInfo))
	{
		std::cout << "Failed to create process" << std::endl;		
	}
	else
	{
		handleRet = processInfo.hProcess;
	}

	return handleRet;
}


void WorkImpl<hostExecuter>::DoWork()
{
	int bitSize  =  BitTypeT<eBitLists_CNT>::bitSize;
	int byteSize = BitTypeT<eBitLists_CNT>::byteSize;

	HOSTEXECUTER executer;
	executer.Run();


}
#include <boost/lockfree/queue.hpp>

#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/type_traits.hpp>

//�����̳ʿ��� ������ �� �������� ��ȿ���� ���� �� ���忡�� Ȯ���ϵ��� �Ѵ�.
//�� �����̳ʴ� Ƽ�� â���� ���Ҹ� �ϰ�, ������ �ִ����� �ٽ��ѹ� ���� ���� ������ �� �����޷��� ���� �ȴ�.
//�ٸ�... Ƽ�� ��ü�� ������ �ڽ��� �Ǵ� ��츦 �����ؼ��� �ȵȴ�...
//���ʿ� ȣ��Ʈ�� ���� �˴ٿ� �Ǵ� ����Ȳ�� ���� �Ұ��� ��Ȳ�̴�,
//�� �ý����� �������� �ʴ� ��Ȳ�� ���񽺺Ұ� ��Ȳ �ۿ� �����Ƿ� �ϰ� �� �� �ֵ��� ����.
//Hit ratio

//��� Ǯ ���� ȣ��Ʈ�� 1ȸ������ ����, ���̰� ���� ȣ��Ʈ�� �˾Ƽ� �ٽ� �ٵ��� ����.
//���ڶ�� ���� ȣ��Ʈ�� ���� ������.


//������ �ڷ��� ������ �޾Ƽ� ���� �ȵȴ�.

//�����Ǵ� entity�� Ư��. keytype = '��Ȯ����'���, entity life span = '���� �̿ܿ��� ����ִ�' ,
template< typename T >
class QueueSpinLocked
{
public:	
	static_assert( boost::is_pointer<T>::value == false, "Unable to use pointer type" );

	void push(const T& toPush)
	{
		boost::detail::spinlock::scoped_lock scopeLock(spinLock);
		_container.push(toPush);
	}
	bool pop(T& toPop)
	{
		if( true == _container.empty())
			return false;
		
		boost::detail::spinlock::scoped_lock scopeLock(spinLock);
		toPop = _container.front();
		_container.pop();

		return true;
	}

	//���� find-(remove~erase)���� ���� �����ؾ� �Ѵٸ�, ������ spin_lock ����� ���� ������ ��︮�� �ʴ´�.
	//�켱������ �������� ������(pop���� remove�� �켱�Ǿ�� invalid entity �� pop���� �ʴ´�)
	//�̷����� �켱���� ������ �ϴ� ���� ������ �Ұ��� �ϱ� �����̴�.
	//sync model�� '�켱�� ���� - ����' ���·� ���������� �Ѵ�.

private:
	boost::detail::spinlock spinLock;
	std::queue< T >	_container;
};

struct HOST_REQUEST_WAITING_CONTEXT
{
	//GID gidStage;
	time_t timeReserved;
};


struct DataForm
{
	char myChar;
	DataForm(char cChar):myChar(cChar){};
	DataForm& GetMyData() {return *this;}
};

void WorkImpl<lockfreeContainer>::DoWork()
{
	QueueSpinLocked<int> instance;
/*
	int arrINTGER[] = {3,47,85,6989,48,756,3,5,};
	boost::lockfree::queue<int> aQueue;

	for each(int& nVal in arrINTGER)
	{
		aQueue.push(nVal);
	}


	int toPop(0);
	aQueue.pop(toPop);	
*/
	DataForm dataForm('a');
	DataForm dataForm2('b');
	DataForm& rDataForm = dataForm.GetMyData();
	rDataForm = dataForm2;

	int a = 0;
	int b = 1;
	int& rA = a;

	rA = b;

	char szName[] = {"����"};
	std::string strName = szName;


	//numeric_limits<float>::epsilon();
}



