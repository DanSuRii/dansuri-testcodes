#ifndef __WORK_H__
#define __WORK_H__

#define REGISTER_WORKS(x) x,
enum eWorks
{
#include "WorksBase.h"
	eWorks_CNT
};

#undef REGISTER_WORKS


class WorkBase
{
public:
	virtual void DoWork() = 0;
	virtual char* GetMyName() = 0;
};

template<eWorks eMyWork>
class WorkImpl : public WorkBase
{
	char* myName;
public:
	void DoWork();
	char* GetMyName(){ return myName; }
	WorkImpl(char* pName):myName(pName){}
};

#define REGISTER_WORKS(x) static WorkImpl<x> x##Instance(#x); pWorks[x] = &x##Instance;


class WorkRepository
{
	WorkBase* pWorks[eWorks_CNT];

	WorkRepository()
	{
#include "WorksBase.h"
	}

	void PrintMenu();
	bool MenuSelect();
	WorkBase* GetWork(int idx);

public:
	
	static WorkRepository& GetInstance(){ static WorkRepository _Instance; return _Instance; }

	void DoLoop();	

};


#endif // __WORK_H__
