#include "stdafx.h"
#include "TestUnitRepository.h"
#include <string>
#include <process.h>


#define ASSERT(x, ...) x
#define DBG_ASSERT(...)
#define DEBUG_VERSION 1

#if defined(_DEBUG)
#define ISVERSION(x) 0
#else
#define ISVERSION(x) 0
#endif

#include "critsect.h"
unsigned int SPIN_WAIT_INTERVAL	=				512;



DECL_TESTUNIT(ReadWriteLockTest);


CReaderWriterLock_NS_FAIR lockUnit(true);

class BaseLocker
{	
	std::string strMyName;
	LONG		nMyIndex;

	BaseLocker();
	BaseLocker(BaseLocker&);
protected:
	typedef void (* MYTHREAD)(void *);
	MYTHREAD pMyThread;


	BaseLocker(MYTHREAD pMyThread, int idx, const char* myName):pMyThread(pMyThread), nMyIndex(idx), strMyName(myName) {}

public:
	const std::string& GetMyName() const { return strMyName; }
	LONG GetMyIndex() const { return nMyIndex; }

	virtual void GetLock() = 0;
	virtual void ReleaseLock() = 0;

	void CreateWorkThread()
	{
		if(pMyThread)
			_beginthread( pMyThread, 0, this );
	}

};

template<class>
class ConcreteLocker : public BaseLocker
{
protected:
	static void __cdecl WorkThread(void *);
	static volatile LONG LastestIndex;	

	ConcreteLocker(const char* myName):BaseLocker(WorkThread, InterlockedIncrement(&LastestIndex) ,myName){}

	virtual void GetLock() 	{		throw std::exception("The method or operation is not implemented.");	}
	virtual void ReleaseLock() 	{	throw std::exception("The method or operation is not implemented.");	}

};

std::ostream& PrintBaseLocker( BaseLocker& baseLocker )
{
	return std::cout << baseLocker.GetMyName() << "[" << baseLocker.GetMyIndex() << "]";
}

void EntraceInfinity()
{
	bool bInfinity = true;
	while(bInfinity) {NULL; Sleep(1);}
};

template<class T>
void __cdecl ConcreteLocker<T>::WorkThread( void * pThis )
{
	if(NULL == pThis)
	{
		OutputDebugStringA("pThis is INVALID");
		return;
	}
	BaseLocker* local_this = (BaseLocker*)pThis;

	PrintBaseLocker(*local_this) << "waiting" << std::endl;

	EntraceInfinity();

	PrintBaseLocker(*local_this) << "request lock" << std::endl;	
	local_this->GetLock();

	PrintBaseLocker(*local_this) << "got lock held" << std::endl;

	EntraceInfinity();

	local_this->ReleaseLock();

	PrintBaseLocker(*local_this) << "Release lock" << std::endl;

	delete local_this;

	return ;	
}

#define CREATENAME(x) #x

class ReadLocker : public ConcreteLocker<ReadLocker>	
{
	virtual void GetLock(){ lockUnit.ReadLock(); }
	virtual void ReleaseLock() 	{	lockUnit.EndRead();	}
public:
	ReadLocker(): ConcreteLocker<ReadLocker>(CREATENAME(ReadLocker)){}

};
volatile LONG ConcreteLocker<ReadLocker>::LastestIndex;

class WriteLocker : public ConcreteLocker<WriteLocker>	
{
	virtual void GetLock(){ lockUnit.WriteLock(); }
	virtual void ReleaseLock() 	{	lockUnit.EndWrite();	}
public:
	WriteLocker(): ConcreteLocker<WriteLocker>(CREATENAME(WriteLocker)){}
};
volatile LONG ConcreteLocker<WriteLocker>::LastestIndex;





#if 0
class ReadLocker : public BaseLocker
{
public:
	static void __cdecl WorkThread(void *);
	static volatile LONG LastestIndex;

	ReadLocker():BaseLocker(WorkThread){}
};

class WriteLocker : public BaseLocker
{
	static void __cdecl WorkThread(void *);
	static volatile LONG LastestIndex;


public:
	WriteLocker():BaseLocker(WorkThread){;}
};
#endif

void ReadWriteLockTest::DoExecute()
{
	bool bDoLoop = true;
	while(true == bDoLoop)
	{
		enum{
			CREATE_METHOD_NONE,
			CREATE_METHOD_READLOCKER,
			CREATE_METHOD_WRITELOCKER
		}createMethod(CREATE_METHOD_NONE);
		BaseLocker* pBaseLocker = NULL;

		switch(createMethod)
		{
		case CREATE_METHOD_READLOCKER:
			{
				pBaseLocker = new ReadLocker;
				break;
			}

		case CREATE_METHOD_WRITELOCKER:
			{
				pBaseLocker = new WriteLocker;
				break;
			}

		default:
			break;
		}
		
		if(pBaseLocker)
			pBaseLocker->CreateWorkThread(); //원래는 생성 한 곳에서 소멸을 담당해야하지만 ㅡ ㅡ; 일단은 내부에서 소멸을 담당하도록 했다.

		Sleep(1);
	}
}