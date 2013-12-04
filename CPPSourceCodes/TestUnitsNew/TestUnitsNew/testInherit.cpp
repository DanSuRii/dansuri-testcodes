#include "Work.h"

class InheritA 
{
public:
	int x;
protected:
	int y;
private:
	int z;

	virtual int GetX() = 0;
	virtual int GetY() = 0;
	virtual int GetZ() = 0;

	virtual int SetX(int fromX) = 0;
	virtual int SetY(int fromY) = 0;
	virtual int SetZ(int fromZ) = 0;
};

class B : public InheritA
{
	// x is public
	// y is protected
	// z is not accessible from B

	virtual int GetX(){}

	virtual int GetY(){}

	virtual int GetZ(){}

	virtual int SetX( int fromX )
	{
	}

	virtual int SetY( int fromY )
	{
	}

	virtual int SetZ( int fromZ )
	{
	}

};

class C : protected InheritA
{
	// x is protected
	// y is protected
	// z is not accessible from C
};

class D : private InheritA
{
	// x is private
	// y is private
	// z is not accessible from D
};

void WorkImpl<testInherit>::DoWork()
{
	//B InstOne;
	//C InstTwo;
	//D InstThree;
}
