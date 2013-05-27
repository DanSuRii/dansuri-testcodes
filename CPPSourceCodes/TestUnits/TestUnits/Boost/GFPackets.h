#ifndef __GFPackets_h__
#define __GFPackets_h__

#pragma pack(push, 1)

#define IP_ADDRESS_SIZE 10


namespace GAMEFLIER_PACKETS
{
	//패킷의 사이즈와 처리 타입은 지정 할 수 있지만,
	//처리기를 연결 하는 방법을 구상 해 보도록 한다.

	/*pseudo code
	#define PROCEDURES_BEGIN switch() {
	#define PROCEDURES_END	}
	#define REGPROCEDURES \
	case '' : worker<>->doProc( data )\

	template< typename packet<>, class callBack >
	class worker
	{		
		public:
		doProc( packet<>::type* p ){ callBack(p); }
	}
	*/
	template<class myType>
	class packetBase
	{
	public:
		typedef myType type;
		static const WORD size = sizeof(myType);
	};

	template<int code>
	class Packets;

	//middleware packets
	struct GF_Header
	{
		char head[1];
	};

	struct Request : public GF_Header
	{

		char token[100+1];
		__int64 sessionkey;
		//char ip[IP_ADDRESS_SIZE];
	};
	template<>
	class Packets<'c'>: public packetBase<Request>{};

	struct Response : public GF_Header
	{
		int result;
		char reason[50+1];
		__int64 sessionkey;
	};
	template<>
	class Packets<'s'>: public packetBase<Response>{};

	enum eMIDDLELoginResult
	{
		MIDDLELOGINRESULT_SUCCEED = 1,
		MIDDLELOGINRESULT_INVALID_IP = -19,
		MIDDLELOGINRESULT_TIMEOUT = -21,
		MIDDLELOGINRESULT_INVALID_TOKEN = -23,
		MIDDLELOGINRESULT_OTHER_ERR = -99

	};

	//PLSS packets
	struct PUREPLSS_Header
	{
		unsigned short packet_len;
		unsigned short packet_type;

	};

	struct PLSS_Header : public PUREPLSS_Header
	{
		PLSS_Header(unsigned short len, unsigned short aType)
		{
			packet_len = len;
			packet_type = aType;
		}

		bool operator ==(PLSS_Header& rhs)
		{
			return (packet_len == rhs.packet_len) && (packet_type == rhs.packet_type);
		}
	};

	struct Login : public PLSS_Header
	{
		Login():PLSS_Header(sizeof(*this), 0x0E01){}

		unsigned int	worldID;
		char			username[25];
		char			loginTime[20];
	};
	template<>
	class Packets<0x0E01> : public packetBase<Login>{};

	
	struct Ping : public PLSS_Header
	{
		Ping():PLSS_Header(sizeof(*this), 0x0E02){}		
	};

	struct LoginResult : public PLSS_Header
	{
		LoginResult():PLSS_Header(sizeof(*this), 0xD601){}

		char				userName[25];
		unsigned char		resultValue;
	};
	template<>
	class Packets<0xD601> : public packetBase<LoginResult>{};

	enum ePLSSLoginResult
	{
		PLSSLOGINRESULT_TIMEOUT = 0,
		PLSSLOGINRESULT_ALLOW_LOGIN = 1,
		PLSSLOGINRESULT_ALREADY_LOGGED_IN = 2,
		PLSSLOGINRESULT_ALREADY_LOGIN_WAIT = 3
	};
}
#pragma pack(pop)

#endif // __GFPackets_h__
