#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

static int X=10, Y=9;

static void display_maker(int (*arr)[20]);

static void gotoxy(int x, int y)
{
	COORD Cur ;
	Cur.X = x ;
	Cur.Y = y ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
void main__()
{
	char c;
	int score=0;
	int arr[10][20]={0,};
	while(1)
	{
		Sleep(500);
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

	gotoxy(0,0);


	for(j=10; j>=1; j--)
		for(i=0; i<20; i++)
		{
			arr[j][i] = arr[j-1][i];

		}
		enemy = rand()%20;
		
		for(i=0; i<20; i++)
		{
			if( i == enemy)
				arr[0][i] = 1;
			else
				arr[0][i] = 0;
		}




		///////출력부분
		for(j=0; j<10; j++)
		{
			for(i=0; i<20; i++)
			{
				if( j == Y && i == X)
				{
					printf("$");
				}
				else if( arr[j][i] == 1 )
					printf("@");
				else // 플레이어가 아니다 && arr[j][i] == 0 => 빈 맵
					printf(".");


			}
			printf("\n");
		}
}

