#include "stdafx.h"
#include "TestUnitRepository.h"
#include <algorithm>
#include <map>

template < class base_T >
class ExportableMap
{
	typedef typename base_T::key_type key_type;
	typedef typename base_T::mapped_type mapped_type;

public:

	struct INTER_STRUC
	{
		key_type aKeyType;
		mapped_type aMappedType; //�����Ͷ�� deep copy�� �ϴ� �������� �ʿ��ϴ�... �ƴϸ� ������ ���ϴ���
	};

	ExportableMap():exportMap(nullptr){}
	ExportableMap(base_T&){;}
	~ExportableMap(){ if(exportMap) delete [] exportMap; }


	void Import(base_T& param);
/*
	template<size_t sz>
	INTER_STRUC toArray()[sz]
	{

	}
*/ //�̷��� ���� �� ��쿡�� �ᱹ memcpy�� ���ؼ� �ѹ� �� ���� �ؾ� �ϹǷ�, �ǹ̰� ����.

	template<size_t sz>
	void toArray(INTER_STRUC (&target)[sz]);
	

	size_t toArray(INTER_STRUC* target, size_t sz);
	

private:

	INTER_STRUC* exportMap;



};



DECL_TESTUNIT(exportalbleMap)

void exportalbleMap::DoExecute()
{
	std::map<int, char> aMap;
	aMap[0] = 'c';
	aMap[1] = 'a';
	aMap[2] = 'b';

	//ExportMapBase& instance = ExportMapBase::Get(aMap);

	//
	//instance.Import( aMap );
	
/*
	std::min();

	std::copy_n();
*/
}

#if 0

class ExportMapBase
{
public:

	template<class base_T> static ExportMapBase& Get(base_T&);
	template<class base_T> void Import(base_T& param);
	template<class base_T> void Export(base_T& param);

	
};

//key_type - mapped_type container exporter
template < class base_T >
class ExportableMap : public ExportMapBase
{
	typedef typename base_T::key_type key_type;
	typedef typename base_T::mapped_type mapped_type;

public:

	struct INTER_STRUC
	{
		key_type aKeyType;
		mapped_type aMappedType; //�����Ͷ�� deep copy�� �ϴ� �������� �ʿ��ϴ�... �ƴϸ� ������ ���ϴ���
	};

	ExportableMap():exportMap(nullptr){}
	ExportableMap(base_T&){;}
	~ExportableMap(){ if(exportMap) delete [] exportMap; }


	void Import(base_T& param);
/*
	template<size_t sz>
	INTER_STRUC toArray()[sz]
	{

	}
*/ //�̷��� ���� �� ��쿡�� �ᱹ memcpy�� ���ؼ� �ѹ� �� ���� �ؾ� �ϹǷ�, �ǹ̰� ����.

	template<size_t sz>
	void toArray(INTER_STRUC (&target)[sz]);
	

	size_t toArray(INTER_STRUC* target, size_t sz);
	

private:

	INTER_STRUC* exportMap;



};

template < class base_T >
void ExportableMap<base_T>::Import( base_T& param )
{
	exportMap = new INTER_STRUC[ param.size() ];
}

template<class base_T>
void ExportMapBase::Import( base_T& param )
{
	ExportableMap<base_T>& _instace = dynamic_cast<ExportableMap<base_T>&>(ExportMapBase::Get(param));
	_instace.Import(param);
}


template<class base_T>
static ExportMapBase& ExportMapBase::Get(base_T&)
{
	static ExportableMap<base_T> _Instance;
	return _Instance;
}
#endif
