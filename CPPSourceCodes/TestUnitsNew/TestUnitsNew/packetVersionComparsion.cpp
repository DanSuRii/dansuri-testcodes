#include "Work.h"

#include <boost/foreach.hpp>

enum
{
	eUMH_Msg_CToS_End	= 0x100001,
	eUMH_Msg_SToC_End	= 0X200002,
	eUMH_GMTool_End		= 0X300003,
	eUMH_Game_End		= 0X400004,
	eUMH_Host_End		= 0X500005,
	eUMH_Sync_End		= 0X600006,
};


class CVt_PacketVersionComparsion
{
public:


	unsigned int GetComparsionValue() { return COMPARSION_VALUE; }

	static CVt_PacketVersionComparsion& GetInstance(){ static CVt_PacketVersionComparsion _Instance; return _Instance; }

private:
	CVt_PacketVersionComparsion()
		: COMPARSION_VALUE(0)
	{
		unsigned int PacketEnds[] = {
			eUMH_Msg_CToS_End	,
			eUMH_Msg_SToC_End	,
			eUMH_GMTool_End		,
			eUMH_Game_End		,
			eUMH_Host_End		,
			eUMH_Sync_End		,
		};

		BOOST_FOREACH(auto 값, PacketEnds)
		{
			COMPARSION_VALUE <<= 4;
			unsigned int 결과 = 값 & 0x0F;
			//(최종값 | 결과) << 4
			COMPARSION_VALUE |= 결과;
		}
	}

	unsigned int COMPARSION_VALUE;

	bool DoComparsion( unsigned int toComparsion )
	{
		return toComparsion == COMPARSION_VALUE;
	}
};

void WorkImpl<VersionComparsion>::DoWork()
{
	unsigned int myValue = CVt_PacketVersionComparsion::GetInstance().GetComparsionValue();
}
