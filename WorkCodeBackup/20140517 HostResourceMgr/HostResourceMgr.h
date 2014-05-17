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
		CConnection::eConnectionType const	eType;				//�ѹ� �������� ������ �Ѵ�.
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

	

	//���� �����̳�, ITERATOR

	
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

		//�ν��Ͻ��� ���� access ���� �ʰ�, ���� ������ �о�־ �����Ѵ�.
		CConnection*		GetConnection( LKEY lkey );
		inline CConnection* GetConnection( HostResource& rhs ){ GetConnection(rhs.GetLKEY()); }

		HostResource*		GetResource( LKEY lkey );

		void			PushHostableClt(CltCon*);

		void			PostHostInfoMsg( std::string&&, GID, LKEY lkey = LKEY_INVALID);
		void			Update();		
	private:		

		
		// 1. �����ϰ� (size_t|unsigned) HostResource::m_HostableIdx �� �����Ѵٸ�, �߰��߰� ������ ���� ���� HostableIdx�� Ʋ������.
		// 2. Iterator�� ���ü��� ����...
		// 3. (O)��Ƽ �ε���(by boost)�� ����ؼ� hostinfo�� ���� ������ �Ѵ�. //solution

		//boost::intrusive::set<HostingInfo>			m_setHostingInfo;

		//�׳� �ܼ��ϰ�, ��ü �÷��̾ iterate�ϸ鼭 'host���������� ���� Ŭ���̾�Ʈ'�� 'host�������� Ŭ���̾�Ʈ'�� ī��Ʈ �ص� ���� ������... (���� ���̵��)
		std::vector<CltCon*>							m_HostableClt;			//ȣ���� ������ Ŭ���̾�Ʈ
		boost::container::map<LKEY, HostResource*>		m_ResourceContainer;

		CConnectionManager&		m_ConnectionManager;
		HostingInfoContainer*	m_HostingInfoCont;

#pragma region ����ڵ��
		//����ڵ�
#if 0
		//��� 1
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

		//��� 2
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
#pragma endregion ����ڵ��
	};
}

typedef GLib::CGSingletonLongevity<SvrGame::CHostResourceMgr, eLongevity_Singlton_Contents, CPolicySingleThreaded> HostResourceMgr;
