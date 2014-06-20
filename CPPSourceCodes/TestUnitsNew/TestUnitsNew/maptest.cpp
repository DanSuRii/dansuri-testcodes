#include "stdafx.h"
#include "Work.h"

#include <map>
#include <utility>
#include <string>

struct Entity
{

};

class EntityMgr
{
	typedef std::map<std::string, Entity*> MapByName;
public:
	~EntityMgr();
	Entity* GetByName(char* pstrName);	
	
	void PushEntity(char* pszName);
	
private:
	 MapByName mapEntities;
};


EntityMgr::~EntityMgr()
{
	auto it = mapEntities.begin();
	for(;it != mapEntities.end(); ++it)
	{
		delete (it->second);
	}
}
void EntityMgr::PushEntity( char* pszName )
{
	Entity* pEntity = GetByName(pszName);
	if( NULL != pEntity )
	{		
		DebugBreak();
	}
	pEntity = new Entity;

	mapEntities.insert( std::make_pair( pszName, pEntity ) );
}

Entity* EntityMgr::GetByName( char* pstrName )
{
	MapByName::iterator it = mapEntities.find(pstrName);

	if(it == mapEntities.end())
		return NULL;

	return it->second;
}

void WorkImpl<MapTest>::DoWork()
{
	char* pszName1 = "destinycs";
	char* pszName2 = "destinycs1";

	EntityMgr entityMgr;
	entityMgr.PushEntity(pszName1);
	entityMgr.PushEntity(pszName2);

}
