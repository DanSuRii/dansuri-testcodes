#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

class OnBoundFureType
{
public:
	virtual bool operator==(const int& rhs) const = 0;
};

template<int MINSIZE,int MAXSIZE>
class X_OnBound : public OnBoundFureType
{
	int X;


	bool chkBOUND(){ return (X > MIN_SIZE) && (X < MAX_SIZE); }
public:
	X_OnBound(int rValue):X(rValue){}

	int operator =(int rValue)
	{
		return X = rValue;
	}

	int& operator++() //전위
	{
		if(chkBOUND())
			++X;
		return X;
	}

	X_OnBound operator++(int) //후위
	{
		X_OnBound tmp(X);
		if(chkBOUND())
			++X;
		return tmp;
	}

	int& operator--()
	{
		if(chkBOUND())
			--X;
		return X;
	}

	int operator--(int)
	{
		X_OnBound tmp(X);
		if(chkBOUND())
			--X;
		return tmp;
	}

	bool operator==(const int& rhs) const
	{
		return X == rhs;
	}
};

bool operator==(int& lhs, OnBoundFureType& rhs)
{
	return (rhs == lhs);
}

/*
template<typename RHS_T>
int operator=(int& lValue, RHS_T& rValue)
{
	return lValue = rValue;
}
*/

/*
template<typename LHS_T, typename RHS_T>
LHS_T operator =(LHS_T& lValue, RHS_T& rValue)
{
	return lValue = rValue;	
}
*/


int X=10, Y=9;
/*
X_OnBound<0, 19> X();
int Y=9;
*/

void display_maker(int (*arr)[20]);

void gotoxy(int x, int y)
{ 
	COORD Cur ;
	Cur.X = x ;
	Cur.Y = y ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur); 
}

void main_()
{
	char c;
	int score=0;
	int arr[10][20]={ {0,}, }; 
	while(1)
	{
		//Sleep(500);
		Sleep(1);
		display_maker(arr);   
		if(_kbhit())   
		{
			c=getch();    
			if(c=='a') X--;

			else if(c=='d') X++;

		}
		score++;
	}
}

//////적 랜덤함수로 지정
void display_maker(int (*arr)[20])  
{
	int i,j;
	int enemy;
	int temp[1][20]={0,};

	gotoxy(0,0);
	//system("cls");



/*
	for(i=10; i>=1; i--)
	{  
		temp[0][rand()%20]=1;
		arr[0][i]=0;

		arr[0][rand()%20]=1;

		for(j=0; j<20; j++)
		{ 
		if(i==0)
		arr[i][j]=temp[0][j];

		else
		arr[i][j]=arr[i-1][j];
		}

	}
*/
	static ULONGLONG lastUpdateTime = GetTickCount64();
	static ULONGLONG UPDATE_INTERVAL = 500;
	//갱신시간이 지났으면
	if( (GetTickCount64() - lastUpdateTime) > UPDATE_INTERVAL )
	{
		lastUpdateTime = GetTickCount64();

		int curPoint = rand()%20;
		/*
		for(i=9; i>0; --i)
		for(j=1; j<20; ++j)
		{ 		
		memcpy_s(arr[i], sizeof(arr[i]),arr[i-1], sizeof(arr[i-1]));
		}
		*/
		for(i=9; i>0; --i)
			memcpy_s(arr[i], sizeof(arr[i]),arr[i-1], sizeof(arr[i-1]));
		
		temp[0][curPoint]=1;

		memcpy_s(arr[0], sizeof(arr[0]) ,temp[0], sizeof(temp[0]));
	}

	


	///////출력부분
	for(i=0; i<10; i++)
	{
		for(j=0; j<20; j++)
		{
			if(i==Y && X == j)
			{
				printf("$"); 
				//j++;
				continue;
			}
			if(arr[i][j]==1)
				printf("@");

			else
				printf(".");

		}
		printf("\n");
	}
}


void main()
{
	extern void main__();

	main_();
}