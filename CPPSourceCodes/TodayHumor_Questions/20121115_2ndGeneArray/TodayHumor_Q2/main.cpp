#include <iostream>
#include <algorithm>

void GetNStrings(char (*str)[25], int N)
{
	int i;
	if(N > 100)
	{
		std::cout << 
			"�Է��� ������ 100���� �ʰ��Ͽ����ϴ�. 100���� �����մϴ�." << std::endl;

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
	DerivedWorker() : Worker( "�Է¹��� ���ڿ��� ������ �����ּ���: " ){}
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
	DerivedWorker() : Worker( "�Է¹��� ������ ������ �����ּ���: " ){}
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
		"1. ���ڿ� �Է�",
		"2. ���� �Է�",
		"",
		"�۾� �Է�(���� ���� ���� ����): "
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
	std::cout << "�ƹ� Ű�� ������ �����մϴ�";
	getchar();
	return 0;
}