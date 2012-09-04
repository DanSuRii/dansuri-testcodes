#include "stdafx.h"
#include "TestUnitRepository.h"
#include <vector>

DECL_TESTUNIT(STDCopy);

typedef std::vector<int> BUFFLIST;

#define MAX_TARGET_SIZE 20
struct MSGTARGET
{
	int nBuff[MAX_TARGET_SIZE];
	MSGTARGET();
};

MSGTARGET::MSGTARGET()
{
	memset(nBuff, 0, sizeof(nBuff));
}

template<typename T>
T MIN(T lhs, T rhs)
{
	if(lhs < rhs)
		return lhs;

	return rhs;
}

template<class T>
void PrintOut(T t)
{
	std::cout << t << ',';
}

void STDCopy::DoExecute()
{
	BUFFLIST buffList;
	for(int i = 0; i < 200; ++i) buffList.push_back(i);

	int dataRemain = buffList.size();
	int dataMaxSize = _countof( ((MSGTARGET*)0)->nBuff);

	int nCurrentProceedCnt = MIN(dataRemain, dataMaxSize);

	BUFFLIST::iterator iterLastEnd = buffList.begin();
	BUFFLIST::iterator iterLastBegin = buffList.begin();
	while(dataRemain
		&& iterLastEnd != buffList.end())
	{
		MSGTARGET msg; //매번 초기화된 구조를 가져다쓴다.
				
		std::advance(iterLastEnd,  nCurrentProceedCnt);

		std::copy( iterLastBegin, iterLastEnd, msg.nBuff );

		std::for_each( msg.nBuff, msg.nBuff+_countof(msg.nBuff), PrintOut<int> );

		std::cout << std::endl;


		dataRemain -= nCurrentProceedCnt;
		nCurrentProceedCnt = MIN(dataRemain, dataMaxSize);

		iterLastBegin = iterLastEnd;
	}


}
