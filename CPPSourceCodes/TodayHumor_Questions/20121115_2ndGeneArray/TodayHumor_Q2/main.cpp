#include <iostream>
#include <algorithm>

void GetNStrings(char (*str)[25], int N)
{
	int i;
	if(N > 100)
	{
		std::cout << 
			"입력의 개수가 100개를 초과하였습니다. 100개로 조정합니다." << std::endl;

		N = 100;
	}
	for(i=0;i<N;i++)
	{
		scanf("%s",str[i]);
	}
}


void GetNumbers(int(*integers)[3],int N)
{
	int i;
	for(i=0;i<N;i++)
	{		
		scanf("%d %d %d",
			&integers[i][0],
			&integers[i][1],
			&integers[i][2]);
	
	}
}


class Worker
{
	char* myStr;

	virtual void DoWork(int) = 0;
public:
	Worker(char* szMyMenu)
		:myStr(szMyMenu)
	{		
	}	

	void DoWork()
	{
		std::cout << myStr << std::endl;

		DoWork(0);
	}	
};

template<int>
class DerivedWorker;

template<>
class DerivedWorker<1> : public Worker
{
public:
	DerivedWorker() : Worker( "입력받을 문자열의 개수를 적어주세요: " ){}
	void DoWork(int)
	{
		char strings[100][25] = {{'\0',},};
		int nCnt = -1;

		std::cin >> nCnt;

		if(nCnt != -1)
			GetNStrings( strings, nCnt );

		std::cout << std::endl;
	};

};

template<>
class DerivedWorker<2> : public Worker
{
public:
	DerivedWorker() : Worker( "입력받을 숫자의 개수를 적어주세요: " ){}
	void DoWork(int)
	{
		int nCnt = -1;
		std::cin >> nCnt;

		if(nCnt == -1)
			return ;

		int(*nBuf)[3] = new int[nCnt][3];

		GetNumbers( nBuf, nCnt );

		if(nBuf)
			delete nBuf;
	}
};


int main(void)
{

	char* menus[] =
	{
		"1. 문자열 입력",
		"2. 숫자 입력",
		"",
		"작업 입력(숫자 외의 값은 종료): "
	};

	int nSelect = 0;


	Worker* workers[] =
	{
		{nullptr},
		{new DerivedWorker<1>},
		{new DerivedWorker<2>}
	};

	while(nSelect != -1)
	{		
		nSelect = -1;
		std::for_each( menus, menus + _countof(menus),
			[](char* pPtr)
		{
			std::cout << pPtr << std::endl;
		}
		);

		std::cin >> nSelect;

		Worker* pWorker = nullptr;
/*
		switch(nSelect)
		{
		case 1:			
			pWorker = new DerivedWorker<1>;

			break;

		case 2:			
			pWorker = new DerivedWorker<2>;
		}
*/
		if(nSelect > 0
			&& nSelect < _countof(workers))
			workers[nSelect]->DoWork();

	}

	std::for_each( workers, workers+ _countof(workers),
		[](Worker* worker)
	{
		if(worker) delete worker;
	});
	std::cout << "아무 키나 누르면 종료합니다";
	getchar();
	return 0;
}