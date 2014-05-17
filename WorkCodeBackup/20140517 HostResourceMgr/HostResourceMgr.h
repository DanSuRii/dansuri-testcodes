#pragma once
#include <boost/intrusive/set.hpp>
#include <boost/container/map.hpp>
#include <boost/container/set.hpp>

class HostingInfoContainer;

namespace SvrGame
{
	//FORWARD DECLARATION
	class CConnection;
	class CConnectionManager;		

	class HostResource
	{
	protected:
		HostResource( CConnection& connection );
	public:
		virtual ~HostResource();
		bool operator<(const HostResource& rhs) const {
			return this->m_lkeyConnection < rhs.m_lkeyConnection;
		}

		LKEY GetLKEY() const	{ return m_lkeyConnection; }
		//void SetLKEY(LKEY val)	{ m_lkeyConnection = val; }

		//void SetType(CConnection::eConnectionType val) { eType = val; }
		CConnection::eConnectionType GetType(){ return eType; }

		virtual void	EnterAction(){ AddToIndex(); }
		virtual void	LeaveAction(){ RemoveFromIndex(); }
		
		virtual size_t	GetCurtypeConnCnt() = 0;
		virtual void	AddToIndex()		= 0;
		virtual size_t	RemoveFromIndex()	= 0;
	private:
		HostResource();

		LKEY const							m_lkeyConnection; 
		CConnection::eConnectionType const	eType;				//한번 정해지면 참조만 한다.
	};

	class CltCon : public HostResource
	{
	public:
		enum{TypeValue = CConnection::eConnectionType_User};

		CltCon(CConnection&);

		virtual void	EnterAction();
		virtual void	LeaveAction();

		virtual size_t	GetCurtypeConnCnt();
		virtual void	AddToIndex();
		virtual size_t	RemoveFromIndex();

		void AddToCltResource();

	};

	class HostCon : public HostResource
	{
	public:
		enum{TypeValue = CConnection::eConnectionType_Host};

		HostCon(CConnection&);
		virtual void	EnterAction();
		
		virtual size_t	GetCurtypeConnCnt();
		virtual void	AddToIndex();
		virtual size_t	RemoveFromIndex();

	};

	class DedicatedServer : public HostResource
	{
	public:
		enum{TypeValue = CConnection::eConnectionType_DedicatedServer};
	};

	class HostResourceFactory
	{
	public:
		static HostResource* Create( CConnection& );
	};

	

	//메인 컨테이너, ITERATOR

	
	class CHostResourceMgr
	{
		HostResource*	CreateResource( CConnection&  );

		void			RemoveResource( LKEY );		
		void			RemoveResource( CConnection& connection );
	
	public:
		CHostResourceMgr(void);
		virtual ~CHostResourceMgr(void);


		//void 
		void			ConnectionDescribed(CConnection* pConnection);
		void			ConnectionDisconnected(CConnection* pConnection);

		bool			HostAllocStage(CConnection* pConnection);
		bool			HostMigrationWait(CConnection* pConnection);
		CConnection*	_AllocFromUser(GID gidStage);

		size_t			GetHostCount();
		size_t			GetHostableUsrCnt();

		//인스턴스를 직접 access 하지 않고, 관리 구조에 밀어넣어서 관리한다.
		CConnection*		GetConnection( LKEY lkey );
		inline CConnection* GetConnection( HostResource& rhs ){ GetConnection(rhs.GetLKEY()); }

		HostResource*		GetResource( LKEY lkey );

		void			PushHostableClt(CltCon*);

		void			PostHostInfoMsg( std::string&&, GID, LKEY lkey = LKEY_INVALID);
		void			Update();		
	private:		

		
		// 1. 순수하게 (size_t|unsigned) HostResource::m_HostableIdx 로 관리한다면, 중간중간 빠지고 났을 때의 HostableIdx가 틀어진다.
		// 2. Iterator의 가시성도 역시...
		// 3. (O)멀티 인덱스(by boost)를 사용해서 hostinfo에 대한 관리를 한다. //solution

		//boost::intrusive::set<HostingInfo>			m_setHostingInfo;

		//그냥 단순하게, 전체 플레이어를 iterate하면서 'host제공중이지 않은 클라이언트'와 'host제공중인 클라이언트'를 카운트 해도 되지 않을까... (나쁜 아이디어)
		std::vector<CltCon*>							m_HostableClt;			//호스팅 가능한 클라이언트
		boost::container::map<LKEY, HostResource*>		m_ResourceContainer;

		CConnectionManager&		m_ConnectionManager;
		HostingInfoContainer*	m_HostingInfoCont;

#pragma region 폐기코드들
		//폐기코드
#if 0
		//폐기 1
		class INDEX_INTERFACE
		{
		public:
			LKEY_INDEX& GetLKEYIndex(){return m_lkeyIndex;}
			virtual size_t GetResourceCount() = 0;

			LKEY_INDEX m_lkeyIndex;
		};

		template< class T >
		struct INDEX : public INDEX_INTERFACE
		{

			size_t GetResourceCount()
			{
				return GetLKEYIndex().count();
			}


		};

		template<class T>
		INDEX_INTERFACE& GetIndex(){ static INDEX<T> _instance; return _instance; }

		template<class T>
		LKEY_INDEX& GetLkeyIndex

			template<class T>
		auto AddToIndex(T& rhs)->decltype( GetIndex<T>().GetLKEYIndex().insert(rhs.GetLKEY()) )
		{
			return GetIndex<T>().GetLKEYIndex().insert(rhs.GetLKEY());
		}

#endif

		//폐기 2
#if 0
		typedef boost::container::set<LKEY> LKEY_INDEX;
		template<class T>
		LKEY_INDEX& GetLKEYIndex()
		{			
			static LKEY_INDEX index;			
			std::pair<std::string, LKEY_INDEX&> idxPair( std::string( typeid(T).name() ), index );
			return index;
		}

		template<class T>
		size_t GetIndexCount()
		{
			return GetLKEYIndex<T>().size();
		}

		template<class T>
		std::pair<LKEY_INDEX::iterator,bool> AddToIndex(T& rhs)
		{
			return GetLKEYIndex<T>().insert(rhs.GetLKEY());
		}

		template<class T>
		size_t RemoveFromIndex(T& rhs)
		{
			return GetLKEYIndex<T>().erase(rhs.GetLKEY());
		}

#endif
#pragma endregion 폐기코드들
	};
}

typedef GLib::CGSingletonLongevity<SvrGame::CHostResourceMgr, eLongevity_Singlton_Contents, CPolicySingleThreaded> HostResourceMgr;
