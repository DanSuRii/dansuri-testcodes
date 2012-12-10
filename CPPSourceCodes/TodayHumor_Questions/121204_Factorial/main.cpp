#include <stdio.h>
int Factorial(int n);
int main(void)
{
	int i, j;
	int sum=0;
	printf("����. ���� �ٸ� �ΰ��� �ڿ��� a, b�� �Է��Ͽ��� ��,\n");
	printf("      (a) + (a*(a+1)) + (a*(a+1)*(a+2)) + ... + (a+(a+1)*...*b)�� ����? \n\n\n");
	printf("      ���� �ٸ� �� ���� �ڿ����� �Է��Ͻÿ� : ");
	scanf("%d", &i);
	scanf("%d", &j);

	for( i,j ; i<=0 || j<=0 || i==j ; )
	{
		printf("      ������ ������ �� ���� ���� �ٽ� �Է��Ͻÿ� : ");
		scanf("%d", &i);
		scanf("%d", &j);
		if (i>0 && j>0 && !i==j)
			break;
	}

	if(i<j)
	{
		for( ; i<=j ; i++)
		{
			sum += Factorial(i);
		}
		printf("      ����� %d �Դϴ�. \n\n\n", sum);
	}
	else
	{
		for( ; j<=i ; j++)
		{
			sum += Factorial(j);
		}
		printf("      ����� %d�Դϴ�. \n\n\n", sum);
	}

	fflush(stdin);
	getchar();
	return 0;
}

void PrintToK(int k)
{
	printf("(");
	for( int i = 1; i <= k; ++i )
	{
		printf("%d", i);
		if(i<k)
			printf("*");
	}
	printf(")");
}

int Factorial(int n)
{
	int result=1;
	int k;

	for(k=1 ; k<=n ; k++)
	{
		PrintToK(k);
		result *=k;
		if(k<n)
			printf("+");
	}

	return result;
}