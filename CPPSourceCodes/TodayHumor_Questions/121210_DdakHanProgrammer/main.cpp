#include <iostream>

struct OU 
{
	virtual ~OU(){
		std::cout << "ASKY!" << std::endl;
		system("pause");
	}

	void DoOU()
	{
		std::cout << "Subj: ��ǻ�� �Խ����� ���� ����.."<< std::endl;
		std::cout << "���α׷��ӵ��� �� �ִ� �� ����. \n\n ���� ����� ������..." << std::endl;
	}
};

class programmer
{
public:
	~programmer() {
		std::cout << "call ~programmer" << std::endl;
	}
};

class �����ø� : public programmer, public OU
{
public:
	�����ø�(){ DoOU(); }
};

#define if(x) for(x)

int main(void)
{
	�����ø� A;

	if(int i = 0; i < 10; ++i)
	{
		std::cout << i << std::endl;
	}

	return 0;	
}