#pragma once
#include "IDContainer/IdContainer.h"
#include "ServiceFrame/ServiceBase.h"
#include "ServerConfig/ServerConfigMgr.h"
#include "Longevity.h"
#include "ResourceInfo.h"

typedef IdContainer::CUniqueKeyGen<LKEY, (0x1111ffff)>		TServerKey;
typedef IdContainer::CIdContainer<TServerKey, CServerInfo*>	VecServerLive;

class CSvcConfig : public ServiceFrame::CServiceBase, public CServerConfigMgr
{
public:
	CSvcConfig(void);
	~CSvcConfig(void);


	void						Init(void);
	void						TickProcess(void);
	void						BroadcastSvrStatus(CServerInfo* pSvrInfo);
	void						UpdateViewer(void);

	void						SetStatus(CServerInfo* pServerInfo, CVSock* pFrom);
	void						Release(CVSock* pFrom);
	template<class TKey>
	CVSock*						FindServerSock(TKey key)
	{
		CServerInfo* pServer = FindServer(key);
		if (!pServer)
			return NULL;
		return pServer->m_pSock;
	}
	template<class TKey>
	LKEY						FindServerKey(TKey key)
	{
		CServerInfo* pServer = FindServer(key);
		if (!pServer)
			return NULL;
		return pServer->m_key;
	}
	CServerInfo*				FindServer(LKEY key);
	CServerInfo*				FindServer(CVSock* pSock);
	CServerInfo*				FindServer(CServerConfig::ESvrType type);
	CServerInfo*				FindLowUserGameServer(int nWorldID);		// World(Community) 별로 Game Server중 유저가 적은 Server를 찾아온다.


	void						SetServerStatus(int nStatus);
	void						FindSvrTypeSvrList(CServerConfig::ESvrType type, std::list<CServerInfo*>& lstSvr);

	void						ShutdownUser( CMsg<eSMH_ShutdownUsr>&  );
private:
	bool						Compair(CServerInfo* pServer, CVSock* pSock);
	bool						CompairType(CServerInfo* pServer, int nSvrType);
	bool						CompairTypeLowUser(CServerInfo* pServer, CServerInfo** ppResult, int nSvrType);
	void						GetSvrList(CServerInfo* pServer, std::list<CServerInfo*>& lstSvr, int nSvrType);

	virtual void				InitDerivedServerStatus();
	virtual void				ServerStatusIterCallback( const STServerInfo& pSvrInfo );

	VecServerLive				m_vecLiveServer;
	CResourceInfo				m_ResourceInfo;
};

typedef GLib::CGSingletonLongevity<CSvcConfig, eLongevity_Singlton_Contents, CPolicySingleThreaded> SvcConfig;
