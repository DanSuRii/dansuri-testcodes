#include "stdafx.h"
#include "Work.h"

struct{
	
	union
	{
		unsigned __int64 myValue;
		struct{
			unsigned __int64 LSB4Bits		: 4;
			unsigned __int64 Middle8Bits	: 8;
			unsigned __int64 MIddle16Bits	: 16;
			unsigned __int64 BitOthers		: 28;
			unsigned __int64 MSB8Bits		: 8;
		};
	} ;

} g_bitPartition;

void WorkImpl<bitPartition>::DoWork()
{
	g_bitPartition.LSB4Bits = 3;
	g_bitPartition.Middle8Bits = 7;
	g_bitPartition.MIddle16Bits = 30000;
	g_bitPartition.BitOthers = 268435455;
	g_bitPartition.MSB8Bits = 128;
}
