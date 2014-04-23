

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
	} //�ڱ��ڸ� ã�ư��� ����� �ʿ�

	//�� �ѹ��� ���� ������ �̱����� ����...? ��...
};
//�ߺ������� ���� template instantce �� ����. ������ ������ �ߺ� Ž����.

struct ENUM_ARRAY
{
	CConnection::eConnectionType eType; //������ Ÿ������ Ž���ϱ� ���� ��ġ
	EnumInfoBase* enumInfo;

} strConnectionType[] =
	{
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_Invalid)
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_User)
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_Host)
		IMPL_CONNECTION_ENUM(CConnection::eConnectionType_Host) ///< �� ������ �ذ� �Ǳ� �� ������ �Ф�....
		//IMPL_CONNECTION_ENUM(CConnection::eConnectionType_DedicatedServer)
	}
;

static_assert(_countof(strConnectionType) == CConnection::eConnectionType_MAX, "some enumeration wasn't described");