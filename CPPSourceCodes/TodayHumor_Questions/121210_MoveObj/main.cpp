#include <iostream>
#include <algorithm>

class OBJECT
{
	int x;
public:
	void moveX(int _x){ x = _x; }
	int getX(){return x;}
	OBJECT():x(0){}
};

OBJECT* object[100000];

int main(void)
{
	int nCurIdx = 0;
	std::for_each(object, object + _countof(object), 
		[&nCurIdx](OBJECT*& object){
			if(nullptr == object)
				object = new OBJECT();
			if(object) 
			{
				object->moveX(nCurIdx++);
				//std::cout << object->getX() << std::endl;
			}
	}
	);

	std::for_each( object, object + _countof(object), [](OBJECT*& object)
	{ 
		if(object)
		{
			delete object;
			object = nullptr;
		}
	} );
}