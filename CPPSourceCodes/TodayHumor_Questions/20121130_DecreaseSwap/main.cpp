#include <iostream>

template<int N, class T, size_t SZ>
struct Functor
{
private:
	Functor();

public:
	Functor(T arr)
	{
		auto val(0);
		if( ((SZ-N)+1) < SZ)
			val = arr[(SZ-N)+1];
		
		arr[SZ-N] = val;
		Functor<N-1, T, SZ> inst(arr);
	}
};

template<class T, size_t SZ>
struct Functor<0, T, SZ>
{
private:
	Functor();

public:
	Functor(T arr)
	{
		return ;
	}
};

template<int N, class T>
struct FunctorBegin;

template<int N, class T, size_t SZ>
struct FunctorBegin<N, T[SZ]>
{
private :
	FunctorBegin();

public :
	FunctorBegin(T (&arr)[SZ])
	{
		Functor<N, decltype(arr), SZ> inst(arr);
	}
};

void main(void)
{
	int arr[] = {1, 10, 20, 30, 40, 50};

/*
	DecreaseSwap<6>(arr);
	DecreaseSwap<5>(arr);
*/
	FunctorBegin<_countof(arr), decltype(arr)> inst(arr);
	FunctorBegin<_countof(arr), decltype(arr)> inst2(arr);
	FunctorBegin<_countof(arr), decltype(arr)> inst3(arr);

	return ;
}

#if 0
template<int N, size_t SZ>
void DecreaseSwapHelper(int (&arr)[SZ]);


template<int N,typename T,  size_t SZ>
void DecreaseSwap(T (&arr)[SZ] )
{
	arr[N-1] = arr[N];
	return DecreaseSwapHelper<N-1>( arr );
}

/*
template<typename T, size_t SZ>
void DecreaseSwap<0, T, SZ>(T (&arr)[SZ])
{
	return ;
}
*/
template<int N, size_t SZ>
void DecreaseSwapHelper(int (&arr)[SZ])
{
	DecreaseSwap<N, int, SZ>(arr);
};

template<size_t SZ>
void DecreaseSwapHelper<0, SZ>(int (&arr)[SZ])
{
	return ;
}

#endif
