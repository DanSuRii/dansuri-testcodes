#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <cstdlib>

//---- #define �κ� 
#define MAX  100
//---- ����ü �κ� 
typedef struct Tenon 
{
	int flag : 3 ; // bit �÷��� (1�̸� ���� 2�̸� ����)
	int year;  // ��
	int month;  // ��
	int day;  // ��
	int money;  // ��
}MONEY;
//----- �Լ������ 
void fnMenu();  // �޴���� 
void fnInput();  // �Է�
void fnDisburse(); // ����
void fnIncome(); // ���� 
void fnPrint();  // ��ü��� 
void fnExit();  // ���α׷� ���� 
void fnSave();  // ���Ͽ� ���� 
void fnOpen();  // ������ ����
void fnShow();  //�ڷ� ���
//---- �������� �����
MONEY mo[MAX]; // ����ü ����(100���� �����Ҽ� �ִ� �����)
int total;   // ��ü �ݾ�
//----- MAIN�κ�
void main()
{
	//---- ������ ���� 
	fnOpen();
	//----- �� ��
	fnMenu(); 
	//---- �� �� 
	fnExit();
	//---- ���Ͽ� ���� 
	fnSave();
}
void fnMenu()
{
	int choice; // �޴����� ���� 
	while(1)
	{
		system("cls");  // ȭ���� ����� 
		printf("�������������������������������������������������������������� \n");
		printf("�� [1]�Է� [2]���� [3]���� [4]��ü��� [5]���� [6] �ڷ���� �� \n");
		printf("�������������������������������������������������������������� \n");
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
		system("pause"); // ȭ���� ���ߴ� ��� 
	}
}
//---- ����θ� �ۼ��ϴ� �κ� 
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
			printf("[1] ���� [2]����  :  "); scanf("%d", &choice);
			if(choice == 1)
			{
				mo[i].flag = 1 ;
				printf("�����Է� : ");
				scanf("%d" , &mo[i].money); 
				total+=mo[i].money; 
				break;
			}
			else if(choice == 2)
			{
				mo[i].flag = 2;
				printf("���� �Է� : ");
				scanf("%d",&mo[i].money);
				total-=mo[i].money;
				break;
			}
			else
			{
				printf("������ �߸� �ϼ̽��ϴ� \n");
				return;
			}
		}
	}
	printf("���������� �ԷµǾ����ϴ� \n");
}
//----  ����κ� ���
void fnDisburse()
{
	int i;
	for(i=0 ; i<MAX; i++)
	{
		if(mo[i].flag == 2)
		{
			printf("----------------[%d] \n", i);
			printf("%4d�� %2d�� %2d��\n", mo[i].year , mo[i].month , mo[i].day);
			printf("���� : -%d�� \n", mo[i].money);
		}
	}
	printf("\n******** ���� �Ѿ� : %d �� *********\n", total);
}
//---- ���Ժκ� ���
void fnIncome()
{
	int i;
	for(i=0 ; i<MAX; i++)
	{
		if( mo[i].flag == 1 )
		{
			printf("----------------[%d] \n", i);
			printf("%4d�� %2d�� %2d��\n", mo[i].year , mo[i].month , mo[i].day);
			printf("���� : %d�� \n", mo[i].money);
		}
	}
	printf("\n******** ���� �Ѿ� : %d �� *********\n", total);
}
//----- ��ü����ϴ� �κ� 
void fnPrint()
{
	int i;
	for(i=0 ; i<MAX ; i++)
	{
		if(mo[i].day != 0)
		{
			printf("----------------[%d] \n", i);
			printf("%4d�� %2d�� %2d��\n", mo[i].year , mo[i].month , mo[i].day);
			if(mo[i].flag==0) { printf("\t���� : -%d�� \n", mo[i].money); }
			else    { printf("\t���� : %d�� \n", mo[i].money); }
		}
	}
	printf("\n******** ���� �Ѿ� : %d �� *********\n", total);
}
//---- ���α׷� ���� 
void fnExit()
{
	system("cls"); // ȭ���� �����
	printf("�������������������������������������������������� \n");
	printf("��    **** ����� ���α׷��� �����մϴ� ****    �� \n");
	printf("�������������������������������������������������� \n");
}


template<int _Size>
void MoneyToString(
		MONEY& fromMoney,
		char (&pszToStr)[_Size] )
{
	char* flagName[] = {
		"NULL",
		"����",
		"����"
	};
	if(fromMoney.flag >= _countof(flagName))
		return ;
	sprintf_s(pszToStr, "[%s]%d-%d-%d �ݾ�: %d ",
		flagName[fromMoney.flag],
		fromMoney.year,
		fromMoney.month,
		fromMoney.day,
		fromMoney.money);
}

//---- �� ��
void fnSave()
{
	FILE *fp;
	int i;	

	if( (fp = fopen("file.txt", "wb")) == NULL)
	{
		printf("���� ���忡 �����Ͽ����ϴ� \n");
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

	printf("���Ͽ����� : Saving");
	for(i=0 ; i<5 ; i++)
	{
		Sleep(400);
		putchar('.');
	}
	putchar('\n');
	fclose(fp);
}
//----- ������ ���� 
void fnOpen()
{
	FILE *fp;
	int i;

	if( (fp = fopen("file.txt", "rb")) == NULL)
	{
		printf("���α׷��� ó������ �����մϴ� \n");
		return;
	}
	fread(&total , sizeof(int) ,1 , fp);
	for(i=0 ; !feof(fp) ; i++)
	{
		fread( &mo[i] , sizeof(MONEY) , 1, fp);
	}
	fclose(fp);
}
void fnShow() //�ڷ� ��� 
{ 
	int i;
	for(i=0 ; i<MAX ; i++)
	{
		if(mo[i].day != 0)
		{
			printf("�ش� ������ �Է��ϼ��� :");
			scanf("%d",&mo[i].year);
			printf("%4d �⵵�� �� %d�� ����߽��ϴ�.",mo[i].year,mo[i].money);

		}
	}
}