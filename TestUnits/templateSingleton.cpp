#include "stdafx.h"
#include "TestUnitRepository.h"

#pragma region ReferenceOthers

unsigned int SPIN_WAIT_INTERVAL	=				512;

#ifdef CS_DEBUG
#define SpinWait(ctr, cnt, ms)								sSpinWait(ctr, cnt, ms)
#else
#define SpinWait(ctr, cnt, ms)								sSpinWait(ctr, cnt)
#endif

#ifdef CS_DEBUG
static void sSpinWait(
	ULONG & counter,
	ULONG count,
	ULONG & maxspin)
#else
static void sSpinWait(
	ULONG & counter,
	ULONG count)
#endif
{
	++counter;
	if ((counter % count) == 0)
	{
#ifdef CS_DEBUG
		if (counter > maxspin)
		{
			maxspin = counter;
		}
#endif
		Sleep(0);
	}
	return;
}


class CCritSectLite
{
private:
	static const LONG LOCK_FLAG =	0x1;
	static const DWORD CS_LITE_WARN_MS = 2000; // warn if CSLite held more than 2 seconds.

	volatile LONG					lock;
	ULONG							maxspin;

#ifdef CS_DEBUG
	DWORD							owningThread;
	
	const char *					file;
	unsigned int					line;
#endif

public:
	CCritSectLite(
		void) : lock(0)
	{
	}

	void Init(
		void)
	{
		lock = 0;
#ifdef CS_DEBUG
		file = NULL;
		line = 0;
		maxspin = 0;
#endif
	}

#ifdef CS_DEBUG
	void InitDbg(
		const char * _file,
		unsigned int _line)
	{
		Init();
		file = _file;
		line = _line;
	}
#endif

	void Free(
		void)
	{
		lock = 0;
#ifdef CS_DEBUG
		if (maxspin > SPIN_WAIT_INTERVAL)
		{
			trace("CCritSectLite [FILE:%s  LINE:%d]  maxspin: %8d\n", file, line, maxspin);
		}
#endif
	}

	void Enter(
		void)
	{
		//DBG_ASSERT(!(lock & (~0x1)));
		ULONG spincounter = 0;
		while (InterlockedCompareExchange(&lock, LOCK_FLAG, 0) != 0)
		{
			SpinWait(spincounter, SPIN_WAIT_INTERVAL, maxspin);
		}
#ifdef CS_DEBUG
		owningThread = GetCurrentThreadId();
#endif
	}

	void Leave(
		void)
	{
		_WriteBarrier();
		lock = 0;
		_ReadWriteBarrier(); // The CS is released.
	}

	BOOL Try(
		void)
	{
		//DBG_ASSERT(!(lock & (~0x1)));
		return (InterlockedCompareExchange(&lock, LOCK_FLAG, 0) == 0);
	}
};


template <typename T>
class CSingleton
{
public:
	static T* GetInstance(
		void)
	{
		if (CSingleton::instance_ == NULL)
		{
			CSingleton::cs_.Enter();
			if (CSingleton::instance_ == NULL)
			{
				CSingleton::instance_ = CreateInstance();
				ScheduleForDestruction(CSingleton::Destroy);
			}
			CSingleton::cs_.Leave();
		}
		return (T*)CSingleton::instance_;
	}

protected:
	CSingleton(
		void)
	{
		ASSERT(CSingleton::instance_ == NULL);
		CSingleton::instance_ = static_cast<T*>(this);
	}

	~CSingleton(
		void)
	{
		CSingleton::instance_ = NULL;
		CSingleton::cs_.Free();
	}

	static void * StaticInit(
		void)
	{
		CSingleton::cs_.Init();
		return NULL;
	}

private:
	static volatile T*				instance_;
	static CCritSectLite			cs_;

private:
	static T* CreateInstance(
		void)
	{
		return new T();
	};

	static void ScheduleForDestruction(
		void (*fp)())
	{
		atexit(fp);
	}

	static void Destroy(
		void)
	{
		if (CSingleton::instance_ != NULL)
		{
			DestroyInstance((T*)CSingleton::instance_);
			CSingleton::instance_ = NULL;
		}
	}

	static void DestroyInstance(
		T *p)
	{
		delete p;
	}
};



template<typename T>
CCritSectLite CSingleton<T>::cs_;

template<typename T>
typename volatile T* CSingleton<T>::instance_ = (T*)CSingleton<T>::StaticInit();

#pragma endregion ReferenceOthers


namespace NS_TESTTEMPLATE
{
/*
	template<class T>
	inline T& GetInstance()
	{
		static T _Instance;
		return _Instance;
	}


	class BaseSingleton
	{
	protected:
		BaseSingleton(){;}
	public:
		template<class T>
		static inline T& GetInstance()
		{
			static T _Instance;
			return _Instance;
		}
	};

	class TestInstance// : public BaseSingleton
	{
	private:
		TestInstance(){;}
	public:		
		template<> friend TestInstance& GetInstance<TestInstance>();
	
		//template<> TestInstance& GetInstance<TestInstance>();
		
	};
	//template<> TestInstance& TestInstance::GetInstance<TestInstance>();
*/
	class TestInstance : public CSingleton<TestInstance>
	{
	private:
		friend class CSingleton<TestInstance>;
		TestInstance();

	protected:
		

	public:
	};

	template<class T>
	class Singleton
	{
	protected:
		static T& instance()
		{
			static Singleton<T> singleton;
			return singleton;
		}
	};

	class TestInstance2nd : public Singleton<TestInstance2nd>
	{
	private:
		TestInstance2nd(){;}
	};

}
DECL_TESTUNIT(templateSingleton);

void templateSingleton::DoExecute()
{
	//NS_TESTTEMPLATE::TestInstance& instance = NS_TESTTEMPLATE::TestInstance::GetInstance<NS_TESTTEMPLATE::TestInstance>();
	//NS_TESTTEMPLATE::TestInstance& instance = NS_TESTTEMPLATE::TestInstance::GetInstance();
	NS_TESTTEMPLATE::TestInstance2nd::instance();
	//NS_TESTTEMPLATE::TestInstance testInstance;
}