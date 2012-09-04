#ifndef __REGISTERUNIT_H__
#define __REGISTERUNIT_H__

/*
template<class T1, class RepositorySingleton>
class RegisterUnit
{
public:
	RegisterUnit();
};
*/

/*
template<class T1, class RepositorySingleton>
RegisterUnit<T1, RepositorySingleton>::RegisterUnit()
{
	static T1 _Instance;
	RepositorySingleton::GetInstance().DoRegist(&_Instance);
}
*/


/*

template<class TargetInstance> RegisterUnit<class TargetInstance, TestUnitRepository>::RegisterUnit()
{	
	//TestUnitRepository::GetInstance().DoRegist(TargetInstance::GetInstance());
}
*/



#endif // __REGISTERUNIT_H__