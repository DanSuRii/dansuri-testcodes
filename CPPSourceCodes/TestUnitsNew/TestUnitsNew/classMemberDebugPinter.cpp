#include "Work.h"
#include <windows.h>
#include <sstream>
//#include <string>
//#include <DbgHelp.h>

//concept


#define BEGINPRINT()	OutputDebugStringA(__FUNCTION__);\
					{\
					std::stringstream ss;\
					std::string strPrefix('\t', hierarchyNum);


#define PRINTMEMBER(x)	\
	{\
	ss << strPrefix;\
	ss << #x << '=';\
	ss << x;\
	ss << '\n'\
	}



#define ENDPRINT()		\
	OutputDebugStringA( ss.str().c_str() );\
}


class NilBase
{
	
};

template<class currentClassT>
void DebugPresent(currentClassT& currentClass, int& hierarchyNum)
{
	currentClassT::baseClass* pBase = (currentClassT::baseClass*)(&currentClass);
	DebugPresent( (*pBase), hierarchyNum);
	++hierarchyNum;
	currentClass.DoPresent(hierarchyNum);
}

template<>
void DebugPresent<NilBase>(NilBase&, int&)
{
	return ;
}

template<class T>
void BeginDebugPresent(T& _Instance)
{
#ifdef _DEBUG
	int hierarchyNum = 0;
	DebugPresent(_Instance,hierarchyNum);
#endif
}


class DataMember
{
	long aMember;
};

class A
{
	long memberA;
};

class B : public A
{
	float memberB;
	
public:
	typedef NilBase baseClass;
	operator NilBase*();//{ return nullptr; }

	void DoPresent(int hierarchyNum)
	{
		BEGINPRINT();
		//PRINTMEMBER(memberB);
		ENDPRINT();
	}

};

class C : public B
{
	DataMember* myPointerMember;
	DataMember myMember;

public:

	typedef B baseClass;
	void DoPresent(int hierarchyNum)
	{
	}
};

void WorkImpl<classMemberDebugPinter>::DoWork()
{
	C myC;

	BeginDebugPresent( myC );

	std::stringstream ss;

	//ss.Insert('\t', hierarchyNum);

	//std::cout << ss;
}
