#include <iostream>

using namespace std;

class tstring									//string��ü�Լ�
{
	char *strdata;								//���ڵ�����
public:
	tstring(char *str)							//Ŭ�����ʱ�ȭ
	{
		int len=0, i=0;
		while (*(str+len)!=0){len++;}
		strdata = new char[len+1];
		for(i=0; i<=len; i++)
			*(strdata+i)=*(str+i);
	}
	char getichr(int i){return *(strdata+i);}	//i��°���ִ� ���� ��ȯ

	int len()									//���ڿ� ���� ��ȯ
	{
		int i=0;
		while (*(strdata+i)!=0){i++;}
		return i;
	}

	~tstring()									//Ŭ���� �޸� ��ȯ
	{
		delete []strdata;
	}
};

char *t_strcpy(char *setadr, tstring &string)	//char�� ���� ����
{
	char *newadr;
	//delete []setadr;//<-�����ִ� �޸𸮴� ��ȯ�ϰ� ���ɷ� ����������.
	if(setadr)
		delete setadr;
	newadr = new char[string.len()+1];

	for(int i=0; i<=string.len(); i++)
		*(newadr+i)=string.getichr(i);
	return newadr;
}

int main()
{
	tstring s1("TodayHumor");
	tstring s2("HelloCppWorld");
	char *t=0;

	t=t_strcpy(t,s1);
	cout<<t<<endl;

	t=t_strcpy(t,s2);
	cout<<t<<endl;

	int i;
	cin>>i;
}