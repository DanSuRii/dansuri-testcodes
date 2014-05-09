#include "StdAfx.h"
#include "Misc/Prime.h"
#include "Monitor/MonitorHolder.h"
#include "PropTable/PropMgr.h"
#include "PropTable/PropMap.h"
#include "PlayerManager.h"
#include "SvrMaster.h"

CSvcConfig::CSvcConfig(void)
	: m_vecLiveServer(GLib::GetPrimeBelow<size_t>(eMx_ServerConnection))
{
}


CSvcConfig::~CSvcConfig(void)
{
}

void CSvcConfig::TickProcess(void)
{
	TickSync::Instance().Update();
}

void CSvcConfig::Init( void )
{
	PropMgr::Instance();
	PropMgr::Instance().AfterInit();
	if (!Load("../SvrData/SvrCfg.xml", "../SvrData/SvrSetting.xml", "../SvrData/SvrGroups.xml", Environment::Instance().GetHostName().data()))
	{
		LOG_ERROR("Config Loading Failed");
		return ;
	}
	CServerInfo *pServerInfo = GetMyInfo();
	if (!pServerInfo)
	{
		LOG_ERROR("Can't find my Config");
		return ;
	}

	ViewMonitor::Instance().AddUIDlg(IDD_RESOURCE);
	ViewMonitor::Instance().AddListCtrl(IDC_LIST_RESOURCE, &CResourceInfo::GetComplexType(), &m_ResourceInfo, 1000);
	//e.g
	//ViewMonitor::Instance().AddListCtrl(IDC_LIST_TCPINFO, &CResourceInfo::GetComplexType(), &m_TCPInfo, 1000);
	ViewMonitor::Instance().Update(IDC_LIST_RESOURCE);

	ViewMonitor::Instance().AddUIDlg(IDD_MONITOR);
	ViewMonitor::Instance().AddTreeCtrl(IDC_SERVERTREE, &CServerInfo::GetComplexType(), GetMyInfo(), 1000);
	ViewMonitor::Instance().AddListCtrl(IDC_LISTSTATUS, IDC_SERVERTREE);


	ViewMonitor::Instance().CheckMenu(ID_VIEW_SERVERINFO, true);
	ViewMonitor::Instance().CheckMenu(ID_VIEW_RESOURCEINFO, false);


	std::vector<int>	vecImage;
	vecImage.push_back(IDB_SVR_CLOSED);
	vecImage.push_back(IDB_SVR_WORKING);
	vecImage.push_back(IDB_SVR_WORKING);
	vecImage.push_back(IDB_SVR_OK);
	vecImage.push_back(IDB_SVR_OK);
	vecImage.push_back(IDB_SVR_WARNING);
	vecImage.push_back(IDB_SVR_WARNING);
	vecImage.push_back(IDB_SVR_CLOSED);
	vecImage.push_back(IDB_SVR_WARNING);
	ViewMonitor::Instance().AddTreeCtrlImage(IDC_SERVERTREE, vecImage, "m_nStatus");
	ViewMonitor::Instance().Update(IDC_SERVERTREE);

	TickSync::Instance().Init(GetServerSettings().m_dwCycleOfDay);

	SetState(eSS_Ready);
}

void CSvcConfig::BroadcastSvrStatus( CServerInfo* pSvrInfo )
{
	CMsg<eSMH_SvrStatus>		msg;
	CSyncStream					ssSend;
	
	ssSend << msg;

	SvcConfig::Instance().BuildServerStatus(pSvrInfo->m_strSvrName, ssSend);

	// broadcast
	SvcConfig::Instance().Broadcast(ssSend);
}

void CSvcConfig::UpdateViewer(void)
{
	ViewMonitor::Instance().Update(IDC_SERVERTREE);
}

void	CSvcConfig::SetStatus(CServerInfo* pServerInfo, CVSock* pFrom)
{
	LKEY key = m_vecLiveServer.Set(pServerInfo);

	pServerInfo->SetStatus(pFrom, key);
}

void	CSvcConfig::Release(CVSock* pFrom)
{
	CServerInfo *pServer = FindServer(pFrom);
	if (!pServer)
		return;
	m_vecLiveServer.Del(pServer->m_key);
	pServer->Release();

	BroadcastSvrStatus(pServer);
	UpdateViewer();
}

CServerInfo*	CSvcConfig::FindServer(LKEY key)
{
	return m_vecLiveServer.Get(key);
}

CServerInfo*	CSvcConfig::FindServer(CVSock* pSock)
{
	CServerInfo *pServer = m_vecLiveServer.ForEachIf(boost::bind(&CSvcConfig::Compair, this, _1, pSock));
	return pServer;
}

CServerInfo*	CSvcConfig::FindServer(CServerConfig::ESvrType type)
{
	CServerInfo *pServer = NULL;

	m_vecLiveServer.ForEachIf(boost::bind(&CSvcConfig::CompairTypeLowUser, this, _1, &pServer, type));
	return pServer;
}

CServerInfo*	CSvcConfig::FindLowUserGameServer(int nWorldID)
{
	CServerInfo *pServer = NULL;

	// ���� ���ӵ� Live Server�߿��� Community Server�� List�� ���� �´�.
	std::list<CServerInfo*> CommunitySvrInfoList;
	FindSvrTypeSvrList(CServerConfig::eSI_ST_COMMUNITY, CommunitySvrInfoList);

	int nCommunityCount = CommunitySvrInfoList.size();
	if(0 >= nCommunityCount)
	{
		LOG_SYSTEM(E)("[CSvcConfig::FindLowUserGameServer] WorldID(%d) don't find live server.", nWorldID);
		return NULL;
	}

	CServerInfo*						pCommunityServerInfo	= NULL;
	CServerInfo*						pNowServerInfo			= NULL;
	int									nNowServerVecSize		= 0;
	std::list<CServerInfo*>::iterator	nowIter					= CommunitySvrInfoList.begin();
	while(CommunitySvrInfoList.end() != nowIter)
	{
		pCommunityServerInfo		= (*nowIter);

		// World ID�� ���� �� ã�´�.
		if(nWorldID == pCommunityServerInfo->m_nWorldID)
		{
			nNowServerVecSize	= pCommunityServerInfo->m_vecChildren.size();

			for(int i = 0; i < nNowServerVecSize; ++i)
			{
				pNowServerInfo = NULL;
				pNowServerInfo = pCommunityServerInfo->m_vecChildren[i];
				if(NULL == pNowServerInfo)
				{
					LOG_ERROR("[CSvcConfig::FindLowUserGameServer] Children Server Info is NULL.");
					continue;
				}

				// Game Server�� ã�´�.
				if(FALSE == pNowServerInfo->IsType(CServerConfig::eSI_ST_GAME))
				{
					continue;
				}

				// Live�� ������ �Ǿ� �ִ� GameServer�� ã�´�.
				if(NULL == pNowServerInfo->m_pSock)
				{
					continue;
				}

				// ���� ���� ���� Game Server�� ���� ��� �ٷ� ����.
				if(NULL == pServer)
				{
					pServer = pNowServerInfo;
				}
				else
				{
					// ���� �˻��� ���� ���� Game Server�� ���Ͽ� ���������� ���� ���ش�.
					if(pNowServerInfo->m_nCurPlayer < pServer->m_nCurPlayer)
					{
						pServer = pNowServerInfo;
					}
				}
			}
		}	

		pCommunityServerInfo	= NULL;
		nNowServerVecSize		= 0;
		++nowIter;
	}

	return pServer;
}

void			CSvcConfig::FindSvrTypeSvrList(CServerConfig::ESvrType type, std::list<CServerInfo*>& lstSvr)
{
	m_vecLiveServer.ForEach(boost::bind(&CSvcConfig::GetSvrList, this, _1, std::ref(lstSvr), type));
}

void	CSvcConfig::SetServerStatus(int nStatus)
{
	if (!GetMyInfo())
		return;
	GetMyInfo()->m_nStatus = nStatus;
	BroadcastSvrStatus(GetMyInfo());
}

bool	CSvcConfig::Compair(CServerInfo* pServer, CVSock* pSock)
{
	if (pServer->m_pSock == pSock)
		return true;
	return false;
}

bool	CSvcConfig::CompairType(CServerInfo* pServer, int nSvrType)
{
	if (pServer->IsType(nSvrType))
		return true;
	return false;
}

bool	CSvcConfig::CompairTypeLowUser(CServerInfo* pServer, CServerInfo** ppResult, int nSvrType)
{
	if (!pServer->IsType(nSvrType))
		return false;
	if (*ppResult == NULL)
		*ppResult = pServer;
	if ((*ppResult)->m_nCurPlayer > pServer->m_nCurPlayer)
		*ppResult = pServer;
	return false;
}

void	CSvcConfig::GetSvrList(CServerInfo* pServer, std::list<CServerInfo*>& lstSvr, int nSvrType)
{
	if (pServer->IsType(nSvrType)) 
		lstSvr.push_back(pServer);
}

void CSvcConfig::ShutdownUser( CMsg<eSMH_ShutdownUsr>& msg )
{
	CSyncStream ssMsg;
	ssMsg << msg;	
	//���� ���� �α���ó���� ���� �ʾ� ���� ������ ��� Attach �Ǿ��ִ��� �� ���ɼ��� �����Ƿ�,
	//��ü ������ broadcasting �Ѵ�.
	Broadcast(ssMsg);
	PlayerManager::Instance().LogOutForce(msg.m_UID);
}

void CSvcConfig::InitDerivedServerStatus()
{
	memset(&m_ResourceInfo, 0x00, sizeof(m_ResourceInfo));
}

void CSvcConfig::ServerStatusIterCallback( const STServerInfo& pSvrInfo )
{
	m_ResourceInfo.m_wTotalHost += pSvrInfo.m_nCurHostCnt;
}

