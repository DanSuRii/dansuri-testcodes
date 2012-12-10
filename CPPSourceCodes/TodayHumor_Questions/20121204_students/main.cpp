#include<stdio.h>
#include<stdlib.h>
#define SIZE 3

struct student{
	int number;
	char name[20];
	double gpa;
};

int main(void)
{
	struct student table[SIZE]={
		{1,"KIM",3.99},
		{2,"MIN",2.68},
		{3,"LEE",4.01}
	};
	struct student s;
	FILE *fp = NULL;
	int i;

	if((fp=fopen("student.dat","wb"))==NULL)
	{
		fprintf(stderr,"출력을 위한 파일을 열 수 없습니다.\n");
		exit(1);
	}
	fwrite(table, sizeof(struct student), SIZE, fp);
	fclose(fp);

	if((fp=fopen("student.dat","rb"))==NULL)
	{
		fprintf(stderr,"출력을 위한 파일을 열 수 없습니다.\n");
		exit(1);
	}

	for(i=0 ; i<SIZE ; i++)
	{
		fread(&s, sizeof(struct student),1,fp);
		printf("학번=%d, 이름=%s, 평점=%f\n",s.number,s.name,s.gpa);
	}
	fclose(fp);

	system("pause");

	return 0;
}