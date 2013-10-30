#include "Work.h"
#include <climits>
#include <cstring>
#include <cstdlib>

enum eDegreeOfDifficult
{
	eDegreeOfDifficult_Easy,
	eDegreeOfDifficult_Normal,
	eDegreeOfDifficult_Hard,
	eDegreeOfDifficult_Expert,
	eDegreeOfDifficult_Hero,
	eDegreeOfDifficult_Hell,

	eDegreeOfDifficult_CNT
};

template<size_t BITCOUNT, typename TySet = int>
struct BitBuff
{
	static const size_t SIZEBYTES = size_t(1) + size_t(BITCOUNT / CHAR_BIT);
	static const size_t BITCOUNT = BITCOUNT;
	typedef char BUFFER_TYPE[SIZEBYTES];
	typedef char STR_TYPE[BITCOUNT+1];

	BUFFER_TYPE buffer;

	BitBuff()
	{
		memset(buffer, 0, sizeof(buffer));
	}

	//return (success, fail)
	bool set(TySet, bool, bool* beforeBit = nullptr);
	
	//return (success, fail)
	//
	bool get(TySet, bool& toGet);

	void ImportFrom(STR_TYPE& fromStr);
	void ExportTo(STR_TYPE& toStr);
};

template<size_t BITCOUNT, typename TySet /*= int*/>
void BitBuff<BITCOUNT, TySet>::ExportTo( STR_TYPE& toStr )
{
	for(size_t sizeCur = 0; sizeCur < BITCOUNT ; ++sizeCur )
	{
		bool bit; 
		get( TySet(sizeCur), bit );
		toStr[sizeCur] = bit + '0';
	}

	toStr[BITCOUNT] = '\0';
}

template<size_t BITCOUNT, typename TySet /*= int*/>
void BitBuff<BITCOUNT, TySet>::ImportFrom( STR_TYPE& fromStr )
{
	for(size_t sizeCur = 0; sizeCur < BITCOUNT ; ++sizeCur )
	{
		char currentChar = fromStr[sizeCur];
		if( false == (currentChar == '0' || currentChar == '1') )
		{
			//assert("Bit range over");
			continue;
		}
		bool currentBit = ('1' == currentChar);
		
		set( TySet(sizeCur), currentBit );		
	}
}

template<size_t BITCOUNT, typename TySet /*= int*/>
bool BitBuff<BITCOUNT, TySet>::get( TySet whereFrom, bool& toGet )
{
	if(false == (whereFrom < BITCOUNT))
	{
		return false;
	}

	size_t bufferIdx = (whereFrom / CHAR_BIT);
	size_t bitIdx = (whereFrom % CHAR_BIT);
	
	if(false == (bufferIdx < _countof(buffer)))
	{
		return false;
	}
	toGet = 0 != (buffer[bufferIdx] & ( 1 << bitIdx ));
	return true;
}

template<size_t BITCOUNT, typename TySet /*= int*/>
bool BitBuff<BITCOUNT, TySet>::set( TySet toSet, bool aBit, bool* beforeBit)
{
	bool bRet;
	if(nullptr == beforeBit)
		beforeBit = &bRet;
	
	if(false == get(toSet, *beforeBit))
		return false;

	size_t bufferIdx = (toSet / CHAR_BIT);
	size_t bitIdx = (toSet % CHAR_BIT);

	char maskedValue	= buffer[bufferIdx] & ~( 1 << bitIdx );	
	buffer[bufferIdx]	= (maskedValue | (aBit << bitIdx));

	return true;
}

typedef BitBuff<eDegreeOfDifficult_CNT, eDegreeOfDifficult> DIFFICULT_BITBUFF;

void WorkImpl<bitBufferTest>::DoWork()
{
	DIFFICULT_BITBUFF bitBuff;
	size_t bitCounts = DIFFICULT_BITBUFF::BITCOUNT;
	//DIFFICULT_BITBUFF::STR_TYPE;

	bitBuff.set(eDegreeOfDifficult_Easy, false);
	bitBuff.set(eDegreeOfDifficult_Normal, true);
	bitBuff.set(eDegreeOfDifficult_Hard, true);
	bitBuff.set(eDegreeOfDifficult_Expert, false);
	bitBuff.set(eDegreeOfDifficult_Hero, true);
	bitBuff.set(eDegreeOfDifficult_Hell, false);

	bool bBit = false;
	bitBuff.get(eDegreeOfDifficult_Easy, bBit);
	bitBuff.get(eDegreeOfDifficult_Normal, bBit);
	bitBuff.get(eDegreeOfDifficult_Hard, bBit);
	bitBuff.get(eDegreeOfDifficult_Expert, bBit);
	bitBuff.get(eDegreeOfDifficult_Hero, bBit);
	bitBuff.get(eDegreeOfDifficult_Hell, bBit);

	DIFFICULT_BITBUFF::STR_TYPE strBitBuff = {'\0',};
	bitBuff.ExportTo(strBitBuff);

	DIFFICULT_BITBUFF::STR_TYPE strBitFrom = {"101101"};
	bitBuff.ImportFrom(strBitFrom);
}
