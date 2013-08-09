//#include <algorithm>
#include <vector>
#include <queue>
#include <functional>
#include <boost/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <iostream>
#include <list>

#include "Work.h"
#include <windows.h>

struct myEvt
{
	DWORD m_dwNextEventTime;	
};

class myEvtComparsion
{
public:
	enum Order{ ASCENDING, DESCENDING  };

	myEvtComparsion(){myOrder=DESCENDING;}
	myEvtComparsion(const Order& orderRule):myOrder(orderRule){;}
	bool operator()(const myEvt* lhs, const myEvt* rhs)
	{
		if(nullptr == lhs || nullptr == rhs)
			return false;
		if(DESCENDING == myOrder)
			return (lhs->m_dwNextEventTime > rhs->m_dwNextEventTime);

		return (lhs->m_dwNextEventTime < rhs->m_dwNextEventTime );
	}
private:
	Order myOrder;
};

void WorkImpl<vectorPqueuePerformance>::DoWork()
{
/*
	boost::random::mt19937_64 rng;
	boost::random::uniform_int_distribution<> dist(1, INT_MAX);
	
	int nLoopCnt = 256;
	while(--nLoopCnt)
		std::cout << dist(rng) << std::endl;
*/
	myEvtComparsion comparsion;
	std::priority_queue<myEvt*, std::vector<myEvt*> , myEvtComparsion> pQueue;
	std::list<myEvt*>	listEntities;
	std::vector<myEvt*> vector;
	std::vector<myEvt*> vectorUserBinaryInsert;
	
	
	srand(time(NULL));
/*
	int nLoopCnt = 256;
	while(--nLoopCnt)
		std::cout << rand() << std::endl;
*/

	myEvt values[100000];

	int nLoopCnt = _countof(values);
	while(nLoopCnt--)	
		values[nLoopCnt].m_dwNextEventTime = rand();

	LARGE_INTEGER startL, finishL;

	{ //block pQueue begin
		QueryPerformanceCounter(&startL);
		std::cout << "pQueue input performance" << std::endl << "processing...";

		nLoopCnt = _countof(values);
		while(nLoopCnt--)
		{
			pQueue.push(&values[nLoopCnt]);
		}
		QueryPerformanceCounter(&finishL);

		std::cout << std::endl << "Finish, input time period:" << (finishL.QuadPart - startL.QuadPart) << std::endl;

	} //block pQueue end	

	std::cout << std::endl;


#if 1
	{ //block list begin
		std::cout << "list input performance" << std::endl << "processing...";

		QueryPerformanceCounter(&startL);

		nLoopCnt = _countof(values);
		while(nLoopCnt--)
		{
			myEvt* nCurInput = &values[nLoopCnt];
			if(nullptr == nCurInput)
				continue;

			auto it = listEntities.begin();

			for(; it != listEntities.end(); ++it)
			{
				myEvt* nCurValue = *it;
				if( nCurValue->m_dwNextEventTime > nCurInput->m_dwNextEventTime )
				{
					break;
				}
			}
			listEntities.insert(it, nCurInput);
		}

		QueryPerformanceCounter(&finishL);

		std::cout << std::endl << "Finish, input time period:" << (finishL.QuadPart - startL.QuadPart) << std::endl;
	} //block vector end
#endif

	std::cout << std::endl;

#if 1
	{ //block vector begin
		std::cout << "vector input performance" << std::endl << "processing...";

		QueryPerformanceCounter(&startL);

		nLoopCnt = _countof(values);
		while(nLoopCnt--)
		{
			myEvt* nCurInput = &values[nLoopCnt];
			if(nullptr == nCurInput)
				continue;

			auto it = vector.begin();

			for(; it != vector.end(); ++it)
			{
				myEvt* nCurValue = *it;
				if( nCurValue->m_dwNextEventTime > nCurInput->m_dwNextEventTime )
				{
					break;
				}
			}
			vector.insert(it, nCurInput);
		}

		QueryPerformanceCounter(&finishL);

		std::cout << std::endl << "Finish, input time period:" << (finishL.QuadPart - startL.QuadPart) << std::endl;
	} //block vector end
#endif
	
	std::cout << std::endl;

	{ //block vector begin
		std::cout << "vectorBinaryInsert input performance" << std::endl << "processing...";

		QueryPerformanceCounter(&startL);

		nLoopCnt = _countof(values);
		while(nLoopCnt--)
		{
			auto nCurInput = &values[nLoopCnt];
			auto it = std::lower_bound( vectorUserBinaryInsert.begin(), vectorUserBinaryInsert.end(), nCurInput, comparsion );
			vectorUserBinaryInsert.insert(it, nCurInput);
		}


		QueryPerformanceCounter(&finishL);

		std::cout << std::endl << "Finish, input time period:" << (finishL.QuadPart - startL.QuadPart) << std::endl;

	}

	std::cout << std::endl;



	myEvt* nCurrent = 0;
	{ //block Vector Pop Begin
		std::cout << "vector pop performance" << std::endl << "processing...";

		QueryPerformanceCounter(&startL);		

		while(vector.size() > 0)
		{
			auto it = vector.begin();
			nCurrent = *it;
			vector.erase(it);
		}

		QueryPerformanceCounter(&finishL);

		std::cout << std::endl << "Finish, pop time period:" << (finishL.QuadPart - startL.QuadPart) << std::endl;

	} //block Vector Pop End
	
	std::cout << std::endl;

	{ //block pQueue Pop Begin
		std::cout << "pQueue pop performance" << std::endl << "processing...";

		int nPrintCnt = 10;

		QueryPerformanceCounter(&startL);

		while(false == pQueue.empty())
		{
			nCurrent = pQueue.top();
			pQueue.pop();
			if(nPrintCnt > 0)
			{
				--nPrintCnt;
				std::cout << nCurrent->m_dwNextEventTime << ", ";
			}
		}

		QueryPerformanceCounter(&finishL);

		std::cout << std::endl << "Finish, pop time period:" << (finishL.QuadPart - startL.QuadPart) << std::endl;

	} //block pQueue Pop End



}


void WorkImpl<vectorBinaricinsert>::DoWork()
{
	int myints[] = {10,20,30,30,20,10,10,20};
	std::vector<int> v(myints,myints+8);           // 10 20 30 30 20 10 10 20

	std::sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30

	std::vector<int>::iterator low,up;
	low=std::lower_bound (v.begin(), v.end(), 17); //          ^
	up= std::upper_bound (v.begin(), v.end(), 60); //                   ^

	std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
	std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

	return ;
}
