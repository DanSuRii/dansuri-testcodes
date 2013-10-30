#ifndef __WORK_H__
#define __WORK_H__

#define			_MAKESTR(S)							# S
#define			MAKESTRING(S)						_MAKESTR(S)
#define			__FILE__LINE__						__FILE__ "(" MAKESTRING(__LINE__) ") : "
#define			NOTE(x)								message(__FILE__LINE__" NOTE-----------------------> : " x "\n")

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



class WorkRepository
{
	WorkBase* pWorks[eWorks_CNT];

	WorkRepository();
	void PrintMenu();
	bool MenuSelect();
	WorkBase* GetWork(int idx);

public:
	
	static WorkRepository& GetInstance(){ static WorkRepository _Instance; return _Instance; }

	void DoLoop();	

};


#endif // __WORK_H__
