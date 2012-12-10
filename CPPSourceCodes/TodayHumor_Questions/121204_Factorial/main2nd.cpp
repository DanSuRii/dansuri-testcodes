#if 0
#include <iostream>

template<int i>
int GetFactorial()
{

	return GetFactorial(--i)
}

template<>
int GetFactorial<0>()
{
	return 0;
}

template<int I, int J>
int Factorial()
{

}
#endif //don't use. unable