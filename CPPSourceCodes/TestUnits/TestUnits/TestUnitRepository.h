#ifndef __TESTUNITREPOSITORY_H__
#define __TESTUNITREPOSITORY_H__

#include "RegisterUnit.h"

#define CONCAT_3_(x, y)					x##y
#define CONCAT_2_(x, y)					CONCAT_3_(x, y)
#define CONCAT(x, y)					CONCAT_2_(x, y)
#define UIDEN(x)						CONCAT(x, __LINE__)

#define DECL_TESTUNIT(x) \
class x : public TestUnit\
{\
public:\
	virtual void DoExecute();\
	x():TestUnit(#x){;}\
};\
static RegisterUnit<x> UIDEN(_register);
/*
void ::DoExecute()
*/


class TestUnit
{
	TestUnit();
	const char* szMyName;
public:
	virtual void DoExecute() = 0;
	const char* GetMyName(){ return szMyName; }

protected:	
	TestUnit(char* myName){ szMyName = myName;}
};


class TestUnitRepository
{
	typedef volatile LONG MENU_INDEX;
	typedef std::map<MENU_INDEX, TestUnit*> MENU_MAP;
	typedef std::pair<MENU_MAP::key_type, MENU_MAP::mapped_type> MENUMAP_PAIR;

	MENU_INDEX curMenuIndex;
	MENU_MAP menuMap;

	TestUnitRepository();
public:
	static TestUnitRepository& GetInstance(){ static TestUnitRepository _Instance; return _Instance; }
	void DoRegist(TestUnit*);
	void DoExecute();
	void PrintMenu();
	MENU_MAP::iterator GetSelection();
	void DoPrintCharArr(char** pTarget, size_t sizeArr);
};

template<class T1>
class RegisterUnit
{
public:
	RegisterUnit()
	{
		static T1 _Instance;
		static bool bInitialized = false;
		if(false == bInitialized)
		{
			//원래는 bInitialized 의 경우에는 진입지점에 Lock / free를 해야하지만 제외했다
			//그리고 DoRegist의 성공 결과를 대입 할 수도 있다.
			TestUnitRepository::GetInstance().DoRegist(&_Instance);
			bInitialized = true;
		}
	}
};



#endif // __TESTUNITREPOSITORY_H__
