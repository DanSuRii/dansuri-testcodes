#include "stdafx.h"
#include "Work.h"

template< unsigned long long B, unsigned char P >
struct exponentiation
{
	enum
	{
		result = B * exponentiation< B, P - 1>::result
	};
};

template< unsigned long long B>
struct exponentiation< B, 0 >
{
	enum
	{
		result = 1ULL
	};
};

template < unsigned long long B >
struct base
{
	template< unsigned char P >
	struct power
	{
		enum{ result = exponentiation< B, P >::result };
	};
};

void WorkImpl<exponentiationTpl>::DoWork()
{
	char buffer[ base<2>::power<10>::result ];
}
