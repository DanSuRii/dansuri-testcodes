#include <iostream>

struct OU 
{
	virtual ~OU(){
		std::cout << "ASKY!" << std::endl;
		system("pause");
	}

	void DoOU()
	{
		std::cout << "Subj: 컴퓨터 게시판을 보다 보면.."<< std::endl;
		std::cout << "프로그래머들이 꽤 있는 것 같다. \n\n 딱한 사람들 같으니..." << std::endl;
	}
};

class programmer
{
public:
	~programmer() {
		std::cout << "call ~programmer" << std::endl;
	}
};

class 딱한플머 : public programmer, public OU
{
public:
	딱한플머(){ DoOU(); }
};

#define if(x) for(x)

int main(void)
{
	딱한플머 A;

	if(int i = 0; i < 10; ++i)
	{
		std::cout << i << std::endl;
	}

	return 0;	
}