#include <iostream>

using namespace std;

class tstring									//string대체함수
{
	char *strdata;								//문자데이터
public:
	tstring(char *str)							//클래스초기화
	{
		int len=0, i=0;
		while (*(str+len)!=0){len++;}
		strdata = new char[len+1];
		for(i=0; i<=len; i++)
			*(strdata+i)=*(str+i);
	}
	char getichr(int i){return *(strdata+i);}	//i번째에있는 문자 반환

	int len()									//문자열 길이 반환
	{
		int i=0;
		while (*(strdata+i)!=0){i++;}
		return i;
	}

	~tstring()									//클래스 메모리 반환
	{
		delete []strdata;
	}
};

char *t_strcpy(char *setadr, tstring &string)	//char에 문자 대입
{
	char *newadr;
	//delete []setadr;//<-전에있던 메모리는 반환하고 새걸로 받으려고함.
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