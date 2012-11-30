#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <cstdlib>

//---- #define 부분 
#define MAX  100
//---- 구조체 부분 
typedef struct Tenon 
{
	int flag : 3 ; // bit 플래그 (1이면 지출 2이면 수입)
	int year;  // 년
	int month;  // 월
	int day;  // 일
	int money;  // 돈
}MONEY;
//----- 함수선언부 
void fnMenu();  // 메뉴출력 
void fnInput();  // 입력
void fnDisburse(); // 지출
void fnIncome(); // 수입 
void fnPrint();  // 전체출력 
void fnExit();  // 프로그램 종료 
void fnSave();  // 파일에 저장 
void fnOpen();  // 파일을 오픈
void fnShow();  //자료 통계
//---- 전역변수 선언부
MONEY mo[MAX]; // 구조체 선언(100개를 저장할수 있는 가계부)
int total;   // 전체 금액
//----- MAIN부분
void main()
{
	//---- 파일을 오픈 
	fnOpen();
	//----- 메 뉴
	fnMenu(); 
	//---- 종 료 
	fnExit();
	//---- 파일에 저장 
	fnSave();
}
void fnMenu()
{
	int choice; // 메뉴선택 변수 
	while(1)
	{
		system("cls");  // 화면을 지운다 
		printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ \n");
		printf("┃ [1]입력 [2]지출 [3]수입 [4]전체출력 [5]종료 [6] 자료통계 ┃ \n");
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ \n");
		printf("CHOICE : [   ]\b\b\b"); scanf("%d",&choice);
		switch(choice)
		{
		case 1 : fnInput();  break;
		case 2 : fnDisburse(); break;
		case 3 : fnIncome();  break;
		case 4 : fnPrint();  break;
		case 5 :  return;
		case 6 : fnShow(); break;
		}
		system("pause"); // 화면을 멈추는 기능 
	}
}
//---- 가계부를 작성하는 부분 
void fnInput()
{
	int i;
	int choice;
	for(i=0;  i<MAX ; i++)
	{
		if(mo[i].day == 0 )
		{
			printf("---------------[%d] \n",i);
			printf("Year  : " ); scanf("%d" , &mo[i].year);
			printf("Month : " ); scanf("%d" , &mo[i].month);
			printf("Day   : " ); scanf("%d" , &mo[i].day);
			printf("[1] 수입 [2]지출  :  "); scanf("%d", &choice);
			if(choice == 1)
			{
				mo[i].flag = 1 ;
				printf("수입입력 : ");
				scanf("%d" , &mo[i].money); 
				total+=mo[i].money; 
				break;
			}
			else if(choice == 2)
			{
				mo[i].flag = 2;
				printf("지출 입력 : ");
				scanf("%d",&mo[i].money);
				total-=mo[i].money;
				break;
			}
			else
			{
				printf("선택을 잘못 하셨습니다 \n");
				return;
			}
		}
	}
	printf("정상적으로 입력되었습니다 \n");
}
//----  지출부분 출력
void fnDisburse()
{
	int i;
	for(i=0 ; i<MAX; i++)
	{
		if(mo[i].flag == 2)
		{
			printf("----------------[%d] \n", i);
			printf("%4d년 %2d월 %2d일\n", mo[i].year , mo[i].month , mo[i].day);
			printf("지출 : -%d원 \n", mo[i].money);
		}
	}
	printf("\n******** 현재 총액 : %d 원 *********\n", total);
}
//---- 수입부분 출력
void fnIncome()
{
	int i;
	for(i=0 ; i<MAX; i++)
	{
		if( mo[i].flag == 1 )
		{
			printf("----------------[%d] \n", i);
			printf("%4d년 %2d월 %2d일\n", mo[i].year , mo[i].month , mo[i].day);
			printf("수입 : %d원 \n", mo[i].money);
		}
	}
	printf("\n******** 현재 총액 : %d 원 *********\n", total);
}
//----- 전체출력하는 부분 
void fnPrint()
{
	int i;
	for(i=0 ; i<MAX ; i++)
	{
		if(mo[i].day != 0)
		{
			printf("----------------[%d] \n", i);
			printf("%4d년 %2d월 %2d일\n", mo[i].year , mo[i].month , mo[i].day);
			if(mo[i].flag==0) { printf("\t지출 : -%d원 \n", mo[i].money); }
			else    { printf("\t수입 : %d원 \n", mo[i].money); }
		}
	}
	printf("\n******** 현재 총액 : %d 원 *********\n", total);
}
//---- 프로그램 종료 
void fnExit()
{
	system("cls"); // 화면을 지운다
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━┓ \n");
	printf("┃    **** 가계부 프로그램을 종료합니다 ****    ┃ \n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━┛ \n");
}


template<int _Size>
void MoneyToString(
		MONEY& fromMoney,
		char (&pszToStr)[_Size] )
{
	char* flagName[] = {
		"NULL",
		"지출",
		"수입"
	};
	if(fromMoney.flag >= _countof(flagName))
		return ;
	sprintf_s(pszToStr, "[%s]%d-%d-%d 금액: %d ",
		flagName[fromMoney.flag],
		fromMoney.year,
		fromMoney.month,
		fromMoney.day,
		fromMoney.money);
}

//---- 저 장
void fnSave()
{
	FILE *fp;
	int i;	

	if( (fp = fopen("file.txt", "wb")) == NULL)
	{
		printf("파일 저장에 실패하였습니다 \n");
		exit(1);
	}
	char totalMoney[256] = {'\0'};
	_itoa_s( total, totalMoney, sizeof(totalMoney), 10 );
	fwrite( &totalMoney , sizeof(totalMoney[0]) , strlen(totalMoney), fp);
	for(i=0 ; i<MAX ; i++)
	{
		if(mo[i].day != 0)
		{
			//fwrite( &mo[i] , sizeof(MONEY) , 1, fp);
			char moneyToStr[1024] = {'\0'};
			MoneyToString(mo[i], moneyToStr );

			fwrite( moneyToStr , sizeof(moneyToStr[0]), strlen(moneyToStr), fp);
		}
	}

	printf("파일에저장 : Saving");
	for(i=0 ; i<5 ; i++)
	{
		Sleep(400);
		putchar('.');
	}
	putchar('\n');
	fclose(fp);
}
//----- 파일을 오픈 
void fnOpen()
{
	FILE *fp;
	int i;

	if( (fp = fopen("file.txt", "rb")) == NULL)
	{
		printf("프로그램을 처음으로 시작합니다 \n");
		return;
	}
	fread(&total , sizeof(int) ,1 , fp);
	for(i=0 ; !feof(fp) ; i++)
	{
		fread( &mo[i] , sizeof(MONEY) , 1, fp);
	}
	fclose(fp);
}
void fnShow() //자료 통계 
{ 
	int i;
	for(i=0 ; i<MAX ; i++)
	{
		if(mo[i].day != 0)
		{
			printf("해당 연도를 입력하세요 :");
			scanf("%d",&mo[i].year);
			printf("%4d 년도에 총 %d를 사용했습니다.",mo[i].year,mo[i].money);

		}
	}
}