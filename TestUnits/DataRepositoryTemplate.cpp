#include "stdafx.h"
#include <set>

#include <set>
struct dataSet
{

};

template<class T_DATASET>
class BaseTemplateClass
{	
	typedef typename T_DATASET::value_type T_MANAGEDTYPE;
	T_DATASET aDataSet;
	void AddData();
	BaseTemplateClass(){};

public:
	static BOOL InitFromExcel(struct EXCEL_TABLE*);
	const T_MANAGEDTYPE GetData(){ static T_MANAGEDTYPE aNew; return aNew;}
	static BaseTemplateClass& GetInstance() {static BaseTemplateClass _Instance; return _Instance;}
};

typedef BaseTemplateClass< std::set<dataSet*> > CONCRETEType;

void Function()
{
	dataSet* pNewData = CONCRETEType::GetInstance().GetData();
	return;
}
