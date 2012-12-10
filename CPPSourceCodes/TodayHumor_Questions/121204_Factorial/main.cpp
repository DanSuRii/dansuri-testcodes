#include <stdio.h>
int Factorial(int n);
int main(void)
{
	int i, j;
	int sum=0;
	printf("문제. 서로 다른 두개의 자연수 a, b를 입력하였을 때,\n");
	printf("      (a) + (a*(a+1)) + (a*(a+1)*(a+2)) + ... + (a+(a+1)*...*b)의 값은? \n\n\n");
	printf("      서로 다른 두 개의 자연수를 입력하시오 : ");
	scanf("%d", &i);
	scanf("%d", &j);

	for( i,j ; i<=0 || j<=0 || i==j ; )
	{
		printf("      문제에 적합한 두 개의 수를 다시 입력하시오 : ");
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
		printf("      결과는 %d 입니다. \n\n\n", sum);
	}
	else
	{
		for( ; j<=i ; j++)
		{
			sum += Factorial(j);
		}
		printf("      결과는 %d입니다. \n\n\n", sum);
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