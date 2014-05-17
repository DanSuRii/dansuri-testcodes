#include "StdAfx.h"
#include "Connection.h"
#include "HostResourceMgr.h"
#include "ConnectionManager.h"


#include "mic_visualizer.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/factory.hpp>

using namespace boost::multi_index;


using namespace SvrGame;

namespace SvrGame
{
	typedef boost::container::set<LKEY> LKEY_INDEX;
	class IndexAccessor
	{
	protected:
		IndexAccessor(CConnection::eConnectionType eType):m_eType(eType){}
	public:		
		virtual size_t	GetSize() = 0;
		
		
		virtual std::pair<LKEY_INDEX::iterator,bool> AddToIndex(HostResource* pResource) = 0;
		virtual size_t	RemoveFromIndex(HostResource* pResource) = 0;

		
		template<class T>
		std::pair<LKEY_INDEX::iterator,bool>	AddToIndex(T& rhs)
		{
			return HRIndex<T>::GetInstance()->AddToIndex(rhs);
		}
		template<class T>
		size_t	RemoveFromIndex(T& rhs)
		{
			return HRIndex<T>::GetInstance()->RemoveFromIndex(rhs);
		}

		CConnection::eConnectionType m_eType;
	};

	//HR: HostResource
	template<class T>
	class HRIndex : public IndexAccessor
	{
		static_assert( boost::is_base_of<HostResource, T>::value , "HostResourceOnly" );
		HRIndex():IndexAccessor( CConnection::eConnectionType( T::TypeValue ) ){}
	public:
		
		static HRIndex* GetInstance(){ static HRIndex _Instance; return &_Instance; }

		
		virtual std::pair<LKEY_INDEX::iterator,bool> AddToIndex(HostResource* pResource)
		{
			if(nullptr == pResource)
				return std::pair<LKEY_INDEX::iterator,bool>( m_Index.end(), false );
			return m_Index.insert(pResource->GetLKEY());
		}
		virtual size_t	RemoveFromIndex(HostResource* pResource)
		{
			if(nullptr == pResource)
				return 0;
			return m_Index.erase( pResource->GetLKEY() );
		}


		virtual std::pair<LKEY_INDEX::iterator,bool>	AddToIndex(T& rhs){	return m_Index.insert(rhs.GetLKEY());  }
		virtual size_t	GetSize(){ return m_Index.size(); }
		virtual size_t	RemoveFromIndex(T& rhs){ return m_Index.erase(rhs.GetLKEY()); }

		
	private:
		LKEY_INDEX	m_Index;
	};

	class HRIndexMgr
	{
		HRIndexMgr();
	public:

		template<class T>
		void operator()(T a);

		static HRIndexMgr& GetInstance(){ return _Instance; }

		std::pair<LKEY_INDEX::iterator,bool>	AddToIndex( HostResource* pResource );

	private:
		std::map< CConnection::eConnectionType , IndexAccessor* > m_Acceessors;

		static HRIndexMgr	_Instance;
	};

}

HRIndexMgr HRIndexMgr::_Instance;

template<CConnection::eConnectionType>
class HRTypeIndicator;

template<>
class HRTypeIndicator<CConnection::eConnectionType_Invalid>
{
public:
	typedef
	struct NullType: public HostResource
	{
		enum{TypeValue = CConnection::eConnectionType_Invalid};

	} type;
};

template<>
class HRTypeIndicator<CConnection::eConnectionType_User> { public: typedef CltCon type; };

template<>
class HRTypeIndicator<CConnection::eConnectionType_Host> { public: typedef HostCon type; };

template<>
class HRTypeIndicator<CConnection::eConnectionType_DedicatedServer> { public: typedef DedicatedServer type; };

template<class T>
void HRIndexMgr::operator()( T a ) 
{
	//여기로 전달되는 this pointer가, 실제 _Instance포인터와 주소값이 다른관계로 _Instance. 를 통해 access 한다.
	_Instance.m_Acceessors[T::value] = HRIndex< HRTypeIndicator< T::value >::type >::GetInstance();
}

HRIndexMgr::HRIndexMgr()
{
	typedef boost::mpl::range_c< 
		CConnection::eConnectionType,
		CConnection::eConnectionType_Invalid,
		CConnection::eConnectionType_MAX > ConnectionTypeRange;

	boost::mpl::for_each<ConnectionTypeRange>( GetInstance() );

}


/*
template<class T>
IndexAccessor* GetAccessor( T e )
{
	return HRIndex< HRTypeIndicator<e>::type >::GetInstance();
}
*/

std::pair<LKEY_INDEX::iterator,bool> HRIndexMgr::AddToIndex( HostResource* pResource )
{
	std::pair<LKEY_INDEX::iterator,bool> nullType( LKEY_INDEX::iterator(),false );
	
	if(nullptr == pResource)
		return nullType;
	
	auto iter = m_Acceessors.find(pResource->GetType());
	if(iter == m_Acceessors.end())
		return nullType;

	IndexAccessor* pAccessor = iter->second;
	if(nullptr == pAccessor)
		return nullType;

	return pAccessor->AddToIndex( pResource );
}


HostResource::HostResource( CConnection& connection )
	:	m_lkeyConnection(connection.GetLKEY())
	,	eType( connection.GetType() )
{
}

/************************************************************************/
/*                 Host Resources                                       */
/************************************************************************/

HostResource::~HostResource()
{
}


CltCon::CltCon( CConnection& connection )
	: HostResource(connection)
{

}

#define GET_HRINDEX() HRIndex< boost::remove_reference< decltype(*this) >::type >::GetInstance()

size_t CltCon::GetCurtypeConnCnt()
{
	//return HostResourceMgr::Instance().GetIndexCount<decltype(*this)>();
	return GET_HRINDEX()->GetSize();
}

void CltCon::AddToIndex()
{
	//HostResourceMgr::Instance().AddToIndex(*this);
	GET_HRINDEX()->AddToIndex(*this);
}

size_t CltCon::RemoveFromIndex()
{
	//return HostResourceMgr::Instance().RemoveFromIndex(*this);
	return GET_HRINDEX()->RemoveFromIndex(*this);
}

void CltCon::AddToCltResource()
{
	HostResourceMgr::Instance().PushHostableClt(this);
}
void CltCon::EnterAction()
{
	__super::EnterAction();
	AddToCltResource();
}

void CltCon::LeaveAction()
{
	__super::LeaveAction();

	HostResourceMgr::Instance().PostHostInfoMsg( std::string("LEAVE_CLIENT"), GID_INVALID, GetLKEY() );
}


HostCon::HostCon( CConnection& connection )
	: HostResource(connection)
{

}

size_t HostCon::GetCurtypeConnCnt()
{
	return GET_HRINDEX()->GetSize();
}

void HostCon::AddToIndex()
{
	GET_HRINDEX()->AddToIndex(*this);
}

size_t HostCon::RemoveFromIndex()
{
	return GET_HRINDEX()->RemoveFromIndex(*this);
}

void HostCon::EnterAction()
{
	__super::EnterAction();

}

#undef GET_HRINDEX

/************************************************************************/
/*                       Hosting Info                                   */
/************************************************************************/

class HostingInfoContainer;

namespace NS_HOSTING_INFO
{
	class HostingInfo
	{
	public:
		GID		GetStageGID() const	{return m_gidStage;}
		LKEY	GetCltLKEY() const	{return m_lkeyCltConn;}
		LKEY	GetHostLKEY() const	{return m_lkeyHostConn;}

		void	SetCltLKEY(LKEY lkeyClt)	{ m_lkeyCltConn = lkeyClt; }
		void	SetHostLKEY(LKEY lkeyHost )	{ m_lkeyHostConn = lkeyHost; }
		void	ReleaseHostingInfo()
		{
			m_lkeyCltConn	=LKEY_INVALID;
			m_lkeyHostConn	=LKEY_INVALID;
		}

		void	ReleaseClt()
		{
			//사실 클라이언트가 leave 할 때, 호스팅 인포가 있다면 이 시점에서 후처리를 해 줘야한다.
			//예를들자면 호스트 마이그레이션을 실행 한다던지 하는 부분들 말이다.
			m_lkeyCltConn = LKEY_INVALID;
		}


		HostingInfo()
			: m_gidStage( GID_INVALID )
			, m_lkeyCltConn	( LKEY_INVALID )
			, m_lkeyHostConn( LKEY_INVALID )
		{		
		}

		HostingInfo( GID gidStage )
			: m_gidStage(gidStage)
			, m_lkeyCltConn	( LKEY_INVALID )
			, m_lkeyHostConn( LKEY_INVALID )
		{
		}		

		bool operator <(const HostingInfo& rhs)const{
			return m_gidStage < rhs.m_gidStage;
		}
		bool operator ==(const HostingInfo& rhs) const{
			return m_gidStage == rhs.m_gidStage;
		}

	private:
		GID		m_gidStage;
		LKEY	m_lkeyCltConn;			//마이그레이션때는 unavailable 할 수 있다.
		LKEY	m_lkeyHostConn;
	};

	//Index tags
	struct PRIMARY{};
	struct CLT_LKEY{};
	struct HOST_LKEY{};
	
	//HI: Hosting Info
	struct HI_MSG_BASE
	{
		virtual void operator ()(HostingInfoContainer&)=0;
		
		HI_MSG_BASE(GID , LKEY ):bCreated(false){;}
		HI_MSG_BASE(GID, LKEY, LKEY){}

		bool IsCreated() const{ return bCreated; }

		//생성 실패 플래그
	protected:
		void SetCreated(){ bCreated=true; }

		HI_MSG_BASE():bCreated(false){;}
		bool bCreated;
	};

	struct ASSIGN_CLT_HOST : public HI_MSG_BASE
	{
		GID			m_gidStage;
		LKEY		m_lkeyClt;

		ASSIGN_CLT_HOST(GID gidStage, LKEY lkeyClt)
			: m_gidStage(gidStage)
			, m_lkeyClt(lkeyClt)
		{SetCreated();}

		virtual void operator()(HostingInfoContainer&);
	};

	struct JOIN_HOST : public HI_MSG_BASE
	{
		JOIN_HOST(GID gidStage, LKEY lkeyHost)
			: m_gidStage(gidStage)
			, m_lkeyHost(lkeyHost)
		{ SetCreated(); }
		
		GID			m_gidStage;
		//클라이거나, 호스트거나
		LKEY		m_lkeyHost;
		
		virtual void operator()(HostingInfoContainer&);
	};
	struct LEAVE_HOST			: public HI_MSG_BASE
	{
		LEAVE_HOST(GID gidStage, LKEY lkeyHost)
			: m_gidStage(gidStage)
			, m_lkeyHost(lkeyHost)
		{ SetCreated(); }

		GID			m_gidStage;
		LKEY		m_lkeyHost;

		virtual void operator()(HostingInfoContainer&);
	};

	struct LEAVE_CLIENT			: public HI_MSG_BASE
	{
		LEAVE_CLIENT(GID , LKEY lkeyClt)
			: m_lkeyClient(lkeyClt)
		{ SetCreated(); }

		LKEY		m_lkeyClient;

		virtual void operator()(HostingInfoContainer&);
	};
	
	struct CREATE_INSTANCE_ZONE	: public HI_MSG_BASE
	{

		GID			m_gidStage;

		CREATE_INSTANCE_ZONE(GID gidStage, LKEY):m_gidStage(gidStage){SetCreated();}
		
		virtual void operator()(HostingInfoContainer&);
	};

	struct DESTROY_INSTANCE_ZONE : public HI_MSG_BASE
	{
		GID			m_gidStage;

		DESTROY_INSTANCE_ZONE(GID gidStage, LKEY):m_gidStage(gidStage){SetCreated();}
		
		virtual void operator()(HostingInfoContainer&);
	};

	typedef boost::function< HI_MSG_BASE*(GID, LKEY) > MSG_FACTORY;
	typedef boost::unordered_map< std::string, MSG_FACTORY > MSG_FACTORIES;
}

using namespace NS_HOSTING_INFO;

typedef multi_index_container<
	HostingInfo,
	indexed_by<
	ordered_unique< tag<PRIMARY>, identity<HostingInfo> >,
	ordered_non_unique< tag<CLT_LKEY>, const_mem_fun<HostingInfo, LKEY, &HostingInfo::GetCltLKEY> >,
	ordered_non_unique< tag<HOST_LKEY>, const_mem_fun< HostingInfo, LKEY, &HostingInfo::GetHostLKEY > >
	>
> HostingInfoSet ;

VISUALIZE_MULTI_INDEX_CONTAINER(HostingInfoSet);


static boost::detail::spinlock initializer = BOOST_DETAIL_SPINLOCK_INIT; 


//멀티 스레드에서 들어오는 다양한 메세지를 queue에다 넣어서,
//메인스레드에서 처리 할 수 있도록 한다.
class HostingInfoContainer : public HostingInfoSet
{
public:

	HostingInfoContainer();
	virtual ~HostingInfoContainer(){;}
	void push_msg( std::string&& strMsg, GID arg1 = GID_INVALID, LKEY arg2 = LKEY_INVALID );
	
#pragma region 폐기코드
#if 0 
	template< class _Arg1, class _Arg2 >
	void push_msg( std::string strMsg, _Arg1 arg1, _Arg2 arg2 )
	{
		auto iter = m_msgFactories.find( strMsg );
		if(iter == m_msgFactories.end())
			return ;
		
		MSG_FACTORY msgFactory = iter->second;

		HI_MSG_BASE* pMsg = msgFactory(arg1, arg2);

		//pMsg->fn = boost::bind( , &pMsg() arg1, arg2, arg3 );
/*
		if(pMsg)
		{
			//autolock (lockMsgQueue);
			m_MsgQueue.push( boost::bind<void>( &HI_MSG_BASE::operator(), pMsg, arg1, arg2, arg3) );
		}
*/
	}
#endif
#pragma endregion 폐기코드

	//template<class T>
	void Update(CHostResourceMgr&)
	{
		//emulate private method
/*
		static_assert( boost::is_base_of<CHostResourceMgr, T>::value,
			"this method restrict to CHostResourceMgr" );
*/

		Update();
	}

/*
	std::pair<iter, iter> range = get<HOST_LKEY>.equal_range(LKEY_INVALID)
	ZoneHostUnavailableCnt = distance(range.first, range.second);
	ZoneHostAvailableCnt = size() - ZoneHostUnavailableCnt;
	//1초에 한번, over-eager evaluation 을 해서 값을 넣어두는건 어떤가...

	auto& hostIdx = cont.get<HOST_LKEY>();
	auto range = hostIdx.equal_range( LKEY_INVALID );
	range.first, range.second;
*/

private:
	void Update();


	boost::detail::spinlock		lockMsgQueue;
	std::queue< HI_MSG_BASE* >	m_MsgQueue;
	MSG_FACTORIES				m_msgFactories;
};

#define DECL_HOSTING_MSG( x ) 	m_msgFactories[ #x ] = boost::factory<x*>();

HostingInfoContainer::HostingInfoContainer()
	: lockMsgQueue(initializer)
{
	//m_msgFactories[	"MSG_BASE" ]		= boost::factory<HI_MSG_BASE*>();
	DECL_HOSTING_MSG(ASSIGN_CLT_HOST);
	DECL_HOSTING_MSG(CREATE_INSTANCE_ZONE);
	DECL_HOSTING_MSG(DESTROY_INSTANCE_ZONE);
	DECL_HOSTING_MSG(JOIN_HOST);
	DECL_HOSTING_MSG(LEAVE_HOST);
	DECL_HOSTING_MSG(LEAVE_CLIENT);
}

void HostingInfoContainer::push_msg( std::string&& strMsg, GID arg1 /*= GID_INVALID*/, LKEY arg2 /*= LKEY_INVALID */ )
{
/*
	DestructLogger
	{
		~DestructLogger(){ print( msg ) }

		failure...
	}
*/
	if(arg1 == GID_INVALID && arg2 == LKEY_INVALID)
		return ; //둘 다 없는데 메세지를 만들 이유가 없다.

	auto iter = m_msgFactories.find( std::forward<std::string>(strMsg) );
	if(iter == m_msgFactories.end())
	{
		//Msg %s unavailable. args(%d, %d)
		return ;
	}

	MSG_FACTORY msgFactory = iter->second;

	HI_MSG_BASE* pMsg = msgFactory(arg1, arg2);

	//(*pMsg)();
	//pMsg->fn = boost::bind( , &pMsg() arg1, arg2, arg3 );
	if(pMsg && pMsg->IsCreated())
	{
		boost::detail::spinlock::scoped_lock lock(lockMsgQueue);
		m_MsgQueue.push( pMsg );
	}
}

void HostingInfoContainer::Update()
{
	if(m_MsgQueue.empty())
		return;
	size_t repeatCnt = 100;
	while(--repeatCnt > 0)
	{
		HI_MSG_BASE* pMsg = nullptr;
		{
			boost::detail::spinlock::scoped_lock lock(lockMsgQueue);
			if(m_MsgQueue.empty())
				break;
			pMsg = m_MsgQueue.front();
			m_MsgQueue.pop();
		}
		if(pMsg)
		{
			(*pMsg)(*this);
			SAFE_DELETE(pMsg);
		}
	}
}


void ASSIGN_CLT_HOST::operator()( HostingInfoContainer& cont )
{
	auto& stageIdx = cont.get<PRIMARY>();
	auto iter = stageIdx.find( m_gidStage );
	if(iter == stageIdx.end())
		return ;

	HostingInfo hostingInfo = (*iter);
	hostingInfo.SetCltLKEY(m_lkeyClt);
	stageIdx.replace( iter, hostingInfo );
}


void CREATE_INSTANCE_ZONE::operator()( HostingInfoContainer& cont)
{
	cont.insert( HostingInfo( m_gidStage ) );
}

void DESTROY_INSTANCE_ZONE::operator()( HostingInfoContainer& cont)
{
	auto& stageIdx = cont.get<PRIMARY>();
	auto iter = stageIdx.find( m_gidStage );
	if(iter == stageIdx.end())
		return ;

	stageIdx.erase(iter);
}

void JOIN_HOST::operator()( HostingInfoContainer& cont )
{
	auto& stageIdx = cont.get<PRIMARY>();
	auto iter = stageIdx.find( m_gidStage );
	if(iter == stageIdx.end())
		return ;

	HostingInfo hostingInfo(*iter);
	hostingInfo.SetHostLKEY( m_lkeyHost );

	stageIdx.replace( iter, hostingInfo );
}


bool IsDebugMode()
{
#ifdef _DEBUG
	return true;
#else
	return false;
#endif
}

#define ASSERT_RETURN(exp, ...) if(!exp){ assert(exp); if(IsDebugMode()) DebugBreak(); return ; }

void LEAVE_HOST::operator()( HostingInfoContainer& cont )
{
	//호스트가 stand-by 상태로 전환 가능하다. 이 host의 소유 클라이언트를 호스트 가용자원으로 돌린다.
	//ASSIGN_CLT_HOST 에서 이미 여기에 호스팅 제공중인 클라이언트가 연결이 되어있다.

	//메인스레드다.
	auto& hostIdx = cont.get<HOST_LKEY>();
	auto iter = hostIdx.find( m_lkeyHost );
	if(iter == hostIdx.end())
	{
		//error, unable to find hosting info.
		return ;
	}

	HostingInfo hostingInfo(*iter);
	//클라이언트의 호스팅 정보
	if( m_gidStage != hostingInfo.GetStageGID() )
	{
		//error, leave host msg integration check fail.
		return ;
	}

	if( (LKEY_INVALID != hostingInfo.GetCltLKEY()) &&
		ConnectionManager::Instance().IsExistConnection(hostingInfo.GetCltLKEY())) // 아직 접속이 끊기지 않은 클라이언트 일 시
	{
		bool bReleased = ConnectionManager::Instance().ReleaseHostPlace( hostingInfo.GetStageGID(), hostingInfo.GetCltLKEY() );
		assert(bReleased);	

		CltCon* pCltCon = dynamic_cast<CltCon*>(HostResourceMgr::Instance().GetResource( hostingInfo.GetCltLKEY() ));
		assert(pCltCon);
		if( nullptr != pCltCon )
		{
			pCltCon->AddToCltResource();
		}
	}
	hostingInfo.ReleaseHostingInfo();

	hostIdx.replace( iter, hostingInfo );
}

void LEAVE_CLIENT::operator()( HostingInfoContainer& cont )
{
	m_lkeyClient;
	auto& cltIdx = cont.get<CLT_LKEY>();
	auto iter = cltIdx.find( m_lkeyClient );
	if(iter == cltIdx.end())
	{
		//호스팅 중인 스테이지가 없을 수 있다.
		return ;
	}
	
	HostingInfo hostingInfo(*iter);
	hostingInfo.ReleaseClt();

	cltIdx.replace(iter, hostingInfo);
}
/************************************************************************/
/*                     Host Resource Manager                            */
/************************************************************************/
CHostResourceMgr::CHostResourceMgr(void)
	: m_ConnectionManager( ConnectionManager::Instance() )
	, m_HostingInfoCont( new HostingInfoContainer )
{
	m_ConnectionManager.RegisterCallback( eCMN_TypeDescribed, boost::bind( &CHostResourceMgr::ConnectionDescribed, this, _1 ) );
	m_ConnectionManager.RegisterCallback( eCMN_Disconnected , boost::bind( &CHostResourceMgr::ConnectionDisconnected, this, _1 ) );	

	HRIndexMgr& rInstance = HRIndexMgr::GetInstance();


}

CHostResourceMgr::~CHostResourceMgr(void)
{
	auto it = m_ResourceContainer.begin();
	auto end = m_ResourceContainer.end();
	for(; it != end; ++it)
	{
		if(it->second)
			SAFE_DELETE(it->second);
	}
	SAFE_DELETE( m_HostingInfoCont);
}

void CHostResourceMgr::ConnectionDescribed( CConnection* pConnection )
{
	if(nullptr == pConnection)
		return ;
	
	//여기선 당연히 host중인 스테이지가 없어야한다.
	assert( GID_INVALID == pConnection->m_HostStage);

	CreateResource(*pConnection);
	
}

void CHostResourceMgr::ConnectionDisconnected( CConnection* pConnection )
{
	if(nullptr == pConnection)
		return ;

	RemoveResource( *pConnection );
}


HostResource* CHostResourceMgr::CreateResource( CConnection& connection )
{
	//타입에 따른 분기
	auto iter = m_ResourceContainer.find( connection.GetLKEY() );
	if(iter != m_ResourceContainer.end())
	{
		//중복된 리소스.
		return nullptr;
	}
	
	HostResource* pResource = HostResourceFactory::Create(connection);
	if(nullptr == pResource)
	{
		//invalid Connection type . type %d
		return nullptr;
	}

	m_ResourceContainer[ pResource->GetLKEY() ] = pResource;
	pResource->EnterAction();

	//호스팅 관련 리소스 세팅을 한다.
	//호스트라면, HostInfo에 집어넣고, 플레이어라면 유후 호스트 자원으로.

	return pResource;
}




void CHostResourceMgr::RemoveResource( LKEY lkey )
{
	 auto iter = m_ResourceContainer.find(lkey);
	 if( iter == m_ResourceContainer.end())
	 {
		 //invalid connection. LKEY %d
		 return ;
	 }

	HostResource* pResource = iter->second;

	m_ResourceContainer.erase( iter );
	
	
	if(nullptr == pResource)
	{
		//System Error, resource unavailable.
		return ;
	}
	
	pResource->LeaveAction();
	SAFE_DELETE(pResource);
}


void CHostResourceMgr::RemoveResource( CConnection& connection )
{
	RemoveResource(connection.GetLKEY());
}


CConnection* CHostResourceMgr::GetConnection( LKEY lkey )
{
	return m_ConnectionManager.GetConnectionByKey( lkey );
}

size_t CHostResourceMgr::GetHostCount()
{
	return HRIndex< HostCon >::GetInstance()->GetSize();
}

HostResource* CHostResourceMgr::GetResource( LKEY lkey )
{
	auto iter = m_ResourceContainer.find( lkey );
	if( m_ResourceContainer.end() == iter )
	{
		//unable to find host resource LKEY(%d)
		return nullptr;
	}

	return iter->second;
}


HostResource* HostResourceFactory::Create( CConnection& connection)
{	
	//Indexed Container...
	switch(connection.GetType())
	{
	case CConnection::eConnectionType_User:
		return new CltCon(connection);
	case CConnection::eConnectionType_Host:
		return new HostCon(connection);
	}

	return nullptr;
}


void CHostResourceMgr::PushHostableClt( CltCon* cltCon)
{
	m_HostableClt.push_back(cltCon);
}


void CHostResourceMgr::PostHostInfoMsg( std::string&& strMsg , GID gidStage , LKEY lkey/* = LKEY_INVALID*/)
{
	m_HostingInfoCont->push_msg( std::forward<std::string>(strMsg), gidStage, lkey );
}


void CHostResourceMgr::Update()
{
	m_HostingInfoCont->Update( *this );
}





/*
	메세지 기반 처리

	situation, condition,

	JOIN_HOST		: msg
	LEAVE_HOST		: msg
	CREATE_INSTANCE : msg

	
	struct MSG_BASE
	{
		bool isValid(false);

		FnMsgHandler msgHandler;
	}

	struct GeneratorInterface
	{
	public:
		MSG_BASE* Generate() = 0;
		tempalte< class T, class _arg1, class _arg2, class _arg3 >
		T* Generate( _arg1 arg1, _arg2 arg2, _arg3 arg3 ){ return new T(arg1, arg2, arg3); }
	}

	template< class T >
	struct Generator : public GeneratorInterface
	{
	public:
		MSG_BASE* Generate(){ Generate<T>( arg1, arg2, arg3 ); }
	};

	tempalte< class _arg1, class _arg2, class _arg3 >
	FnMsgHandler&& GetMsgHandler( strMsg, _arg1 arg1, _arg2 arg2, _arg3 arg3 )
	{
		Generator* pGenerator = m_MsgGenerator[strMsg];
		pGenerator->Generate( arg1, arg2, arg3 );


	}


	MSG_BASE::MSG_BASE(strMsg, arg1, arg2, arg3)
	{
		//메세지가 존재하지 않으면 통보~
		msgHandler = GetMsgHandler(strMsg, strMsg, arg1, arg2, arg3)
		isValid = !msgHandler.empty();
	}
	bind();
	
	push	

	std::string strMsg, arg1, arg2, arg3
	MSG msg(strMsg, arg1, arg2, arg3)	
	{
		autolock lock()
		push(msg)
	}

	pop
	while( ++completeCnt < 100 )
	{
		msg;
		{
			autolock lock()
			if(true == msgQueue.empty())
				break;

			msg = msgQueue.pop();
		}


		handle(msg);
	}

*/

