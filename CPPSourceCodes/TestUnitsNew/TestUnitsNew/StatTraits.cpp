#include "Work.h"

enum EStatBase
{
	hp,
	mp,
	sp,

	STATBASE_CNT
};

enum EStatExtra
{
	physical_resist,
	fire_resist,
	electricty_resist,

	STATEXTRA_CNT
};

enum EStatStatus
{
	stun,
	falldown,
	breakdown,

	STATSTATUS_CNT
};

template<class T>
class TypeAssigner;

template<> class TypeAssigner<EStatBase>{public: typedef unsigned long long myValueT; };
template<> class TypeAssigner<EStatExtra>{public: typedef unsigned long long myValueT; };
template<> class TypeAssigner<EStatStatus>{public: typedef unsigned char myValueT; };

class CHARACTER
{
	TypeAssigner<EStatBase>::myValueT	baseStat[STATBASE_CNT];
	TypeAssigner<EStatExtra>::myValueT	extraStat[STATEXTRA_CNT];
	TypeAssigner<EStatStatus>::myValueT	status[STATSTATUS_CNT];

public:
	template< class T >
	void AddStat_(T eToAdd, typename TypeAssigner<T>::myValueT val);

	template<> void AddStat_<EStatBase>( EStatBase eBaseStat, TypeAssigner<EStatBase>::myValueT val){ baseStat[eBaseStat] += val; }
	template<> void AddStat_<EStatExtra>( EStatExtra eStatExtra, TypeAssigner<EStatExtra>::myValueT val){ extraStat[eStatExtra] += val; }
	template<> void AddStat_<EStatStatus>( EStatStatus eStatStatus, TypeAssigner<EStatStatus>::myValueT val){ status[eStatStatus] += val; }
};

void WorkImpl<statTraits>::DoWork()
{
	CHARACTER aCharacter;

	aCharacter.AddStat_( electricty_resist, 10);
	aCharacter.AddStat_( mp,				20);
	aCharacter.AddStat_( breakdown,			70);
}
