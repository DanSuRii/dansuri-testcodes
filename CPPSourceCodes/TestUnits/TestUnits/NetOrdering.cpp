#include "stdafx.h"
#include "TestUnitRepository.h"

#define INOUT 

enum ORDERING
{
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

static const ORDERING MY_MACHINE_ORDERING = LITTLE_ENDIAN;

namespace packetReorder
{
	template<typename T> void toNet(T& INOUT fromToVal);
	template<typename T> void toHost(T& INOUT fromToVal);
};

template<>
void packetReorder::toNet<int>(int& fromToVal)
{
	fromToVal = htonl(fromToVal);
}

template<>
void packetReorder::toHost<int>(int& fromToVal)
{
	fromToVal = ntohl(fromToVal);
}

template<>
void packetReorder::toNet<short>(short& fromToVal)
{
	fromToVal = htons(fromToVal);
}

template<>
void packetReorder::toHost<short>(short& fromToVal)
{
	fromToVal = ntohs(fromToVal);
}




DECL_TESTUNIT(netOrdering);

void netOrdering::DoExecute()
{
	int myVal = 0x01000307;

	packetReorder::toNet(myVal);

	short myValShort = 0x0813;

	packetReorder::toNet(myValShort);

	__int64 myVal64bit = 0x010304902;
	
	packetReorder::toNet(myVal64bit);

	return ;
}