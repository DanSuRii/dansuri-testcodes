

#define IMPL_CONNECTION_ENUM(x) {x, new EnumInfo<x>(#x)},

struct EnumInfoBase
{
};

template <CConnection::eConnectionType eType>
struct EnumInfo : public EnumInfoBase
{
	const char* myName;
	EnumInfo( __notnull const char* pMyName )
	{
	} //자기자리 찾아가기 기능이 필요

	//단 한번만 생성 가능한 싱글턴을 만들어서...? 음...
};
//중복방지를 위해 template instantce 로 생성. 같은거 있으면 중복 탐지함.

struct ENUM_ARRAY
{
	CConnection::eConnectionType eType; //동일한 타입임을 탐지하기 위해 설치
	EnumInfoBase* enumInfo;

} strConnectionType[] =
	{
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_Invalid)
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_User)
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_Host)
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_Host) ///< 이 문제가 해결 되기 전 까지는 ㅠㅠ....
		//IMPL_CONNECTION_ENUM(CConnection::eConnectionType_DedicatedServer)
	}
;

static_assert(_countof(strConnectionType) == CConnection::eConnectionType_MAX, "some enumeration wasn't described");