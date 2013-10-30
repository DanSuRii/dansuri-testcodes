#ifndef NULLTERMCHECKER_H__
#define NULLTERMCHECKER_H__

class NULLTERM_CHECKER
{
	template<typename _ArrayType, size_t _SizeOfArray, _ArrayType nullTermSignuture>
	static bool NulltermCheckBase( _ArrayType (&myArray)[_SizeOfArray] )
	{
		auto last = myArray + _SizeOfArray;

		auto searchResult = FindWrapper( myArray, last, nullTermSignuture );

		return (searchResult != last);
	}

	template<class _InIt, class _Ty>
	static _InIt FindWrapper(_InIt _First, _InIt _Last, const _Ty& _Val);

	void Create();
public:
	/*
	template<typename _ArrayType, size_t _SizeOfArray>
	bool NulltermCheck(_ArrayType (&myArray)[_SizeOfArray]);
	*/

	template<size_t _SizeOfArray>
	static bool NulltermCheck(char (&myArray)[_SizeOfArray])
	{		
		return NulltermCheckBase<char, _SizeOfArray, '\0'>( myArray );
	}

	template<size_t _SizeOfArray>
	static bool NulltermCheck(wchar_t (&myArray)[_SizeOfArray])
	{		
		return NulltermCheckBase<wchar_t, _SizeOfArray, L'\0'>( myArray );
	}
};  

#endif // NULLTERMCHECKER_H__
