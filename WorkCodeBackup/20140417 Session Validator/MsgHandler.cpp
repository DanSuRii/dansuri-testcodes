#include "StdAfx.h"
#include "Msg/UsrMsg.h"
#include "Msg/MsgErrorCode.h"
#include "DB/Querys.h"
#include "MsgHandler.h"
#include "PlayerManager.h"
#include "SvcNetwork.h"
#include "../../SrcCommon/AllCommon/Msg/MsgVersion.h"

using namespace SvrMaster;

BOOL		CMsgSvr::Connected(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	//플레이어 관리자를 초기화 시킨다.
	return TRUE;
}

BOOL		CMsgSvr::Disconnected(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	SvcConfig::Instance().Release(pFrom);
	return TRUE;
}

/// 각 서버에서 마스터 서버로 보내지는 메시지로 자신의 서버 정보를 담고 있다
BOOL	CMsg<eSMH_RegisterServer>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	//LOG_SVRSTATE("CMsg<eSMH_RegisterServer> recv from %s", m_szHostName);
	
	std::string strName = m_szHostName;
	CServerInfo* pServerAllocated = SvcConfig::Instance().Allocate(m_nServerType, strName, pFrom);

	if(!pServerAllocated)
	{
		LOG_ERROR("Allocate fail type: " + m_nServerType + " hostname: " + m_szHostName);
		return FALSE;
	}

	/// 서버 
	{
		CMsg<eSMH_SetConfig>	msg(TickSync::Instance().GetTick(), TickSync::Instance().GetCycleOfDay());
		CSyncStream				ss;

		strncpy_s(msg.m_szSvrName, pServerAllocated->m_strSvrName.c_str(), _TRUNCATE);
		ss << msg;

		SvcConfig::Instance().Serialize(ss);

		// config 메시지 전송
		pFrom->Send(ss);

		// broadcast changes
		SvcConfig::Instance().BroadcastSvrStatus(pServerAllocated);
		LOG_NORMAL("SvrName: " + pServerAllocated->m_strSvrName + " is registered");

		SvcConfig::Instance().SetStatus(pServerAllocated, pFrom);
	}


	{
		/// 쿼리 서버에 마스터 서버를 등록하기 위해 사용
		CMsg<eSMH_AddServer>	msg;

		strncpy_s(msg.m_szSvrName, SvcConfig::Instance().GetMyInfo()->m_strSvrName.c_str(), _TRUNCATE);

		pFrom->Send(&msg);
	}

	
	return TRUE;
}

BOOL	CMsg<eSMH_CreateCharacterWithName_Res>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	char szMsg[500];
	sprintf_s( szMsg, "eSMH_CreateCharacter_Res res = %d, uid = %d, accname = \"%s\", gid = %I64d, chrName=\"%s\"", m_wResult, m_UID, m_szAccountName, m_GID, m_szCharacterName);
	LOG_SYSTEM(N)(szMsg);
	return TRUE;
}

BOOL	CMsg<eSMH_DBQuery_Res>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CSyncStream ss(pExtra, wSizeExtra);
	CBaseType*	pType;
	void*		pEntity;

	FTQuerys::RecvQuery(ss, pEntity, pType);

	CVSock* pSock = SvcConfig::Instance().FindServerSock(m_keySvr);

	if (pType->GetTypeID() == FTQuerys::CDBQueryLogin::GetComplexType().GetTypeID())
	{
		// Account 로그인 처리.
		if(NULL == pSock)
		{
			LOG_ERROR("[FTQuerys::CDBQueryLogin] pSock is NULL.");
			return FALSE;
		}

		FTQuerys::CDBQueryLogin* pRes = (FTQuerys::CDBQueryLogin*)pEntity;

		std::stringstream ssQueryResult;
		{
			ssQueryResult <<
				"Result("		<< pRes->m_nResult		<< "),"
				"UID("			<< pRes->m_UID			<< "),"
				"AccountID("	<< pRes->m_szUSER_ID		<< "),"
				"IP Address("	<< pRes->m_szUSER_IP		<< ")"
				;
		}

		// 임시로, 현재 비밀 번호를 디테일하게 체크하지 않는다.
		if ( pRes->m_UID == UID_INVALID || pRes->m_nResult != eER_DBQuery_Succeed)
		{
			if(pRes->m_nResult == eER_DBError_Account_Password_Fail)
			{
				// 비밀 번호 체크는 현재 디테일하게 체크하지 않는다. 일단 로그만 출력하자.
				LOG_ERROR("User Password Error.");
			}	
			else
			{
				LOG_SYSTEM(E)( __FILE__LINE__ "FTQuerys::CDBQueryLogin Fail, %s",  ssQueryResult.str().c_str());

				SendErrorMsg<CMsg<eUMH_Login_Res_Error>>(pFrom, m_keyUsr, pRes->m_nResult);
				return TRUE;
			}
		}

		// 등록과 동시에 중복 접속 체크를 한다.
		CPlayer* pPlayer = PlayerManager::Instance().LogIn(pRes->m_UID, pRes->m_szUSER_ID, m_keySvr);
		if(!pPlayer)
		{
			bool bTryKick = SvcConfig::Instance().GetServerSettings().m_bKickByDupeLogin;
			if(true == bTryKick)
			{
				SvcConfig::Instance().ShutdownUser( CMsg<eSMH_ShutdownUsr>(pRes->m_UID, eER_Kick_User_Another_Session ,pRes->m_szUSER_IP) );
				//한번 더
				pPlayer = PlayerManager::Instance().LogIn(pRes->m_UID, pRes->m_szUSER_ID, m_keySvr);
			}
			
			if(!pPlayer)
			{
				if(bTryKick)	LOG_SYSTEM(E)( __FILE__LINE__ "System Critical Fail");
				else			LOG_SYSTEM(E)( __FILE__LINE__ "duplicated User login [%s]", pRes->m_szUSER_ID); // 중복로그인.

				SendErrorMsg<CMsg<eUMH_Login_Res_Error>>(pFrom, m_keyUsr, eER_DBError_Duplication_UserID);
				return FALSE;
			}
		}

		LOG_SYSTEM(N)("[eSMH_DBQuery_Res::CDBQueryLogin] Login Account(%s), UID(%d)", pRes->m_szUSER_ID, pRes->m_UID);
		
		
		CServerInfo* pGameServer = SvcConfig::Instance().FindLowUserGameServer(pRes->m_btRecentlyWorldID);
/*
		if(nullptr == pGameServer)
		{
			pGameServer = SvcConfig::Instance().FindAvilableGameServer();
		}
*/

		pPlayer->SetPlayerAuthKey(1);		/// 접속 이동 설정

		CVSock* pGameServerSocket = (nullptr != pGameServer) ? pGameServer->m_pSock : nullptr;
		if(nullptr == pGameServerSocket)
		{
			LOG_SYSTEM(E)( __FILE__LINE__ "() GameServer socket unavailable", pRes->m_szUSER_ID);
		}
		else
		{

			CMsg<eSMH_User_Game_AuthInfo> msgGame;
			msgGame.m_UID = pRes->m_UID;
			strncpy_s(msgGame.m_szUSER_ID, pRes->m_szUSER_ID, _TRUNCATE);
			strncpy_s(msgGame.m_szUSER_IP, pRes->m_szUSER_IP, _TRUNCATE);
			msgGame.m_dw64PlayerAuthKey = pPlayer->GetPlayerAuthKey();
			msgGame.m_dw64MasterSessionKey = pPlayer->GetSessionKey();

			pGameServerSocket->Send(&msgGame);
		}
		// 로그인 처리.
		CMsg<eUMH_Login_Res_OK>	msg;
		msg.m_Key				= m_keyUsr;
		msg.m_UID				= pRes->m_UID;
		msg.m_dw64PlayerAuthKey = pPlayer->GetPlayerAuthKey();
		msg.m_dwVersion			= 0;
		strncpy_s( msg.m_szAccountName, pRes->m_szUSER_ID, _TRUNCATE);
		if(NULL != pGameServer)
		{
			strncpy_s( msg.m_szServerIP, pGameServer->m_strExternal_IP.c_str(), _TRUNCATE);
			msg.m_wPort = WORD(pGameServer->m_nExternal_Port);
		}
		
		pSock->Send(&msg);

		LOG_SYSTEM(N)( __FILE__LINE__ "FTQuerys::CDBQueryLogin Successfully Login Request, %s",  ssQueryResult.str().c_str());

		return TRUE;
	}

	return TRUE;
}

BOOL	CMsg<eSMH_SvrStatus>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CSyncStream	ss(pExtra, wSizeExtra);

	CServerInfo* pInfo = SvcConfig::Instance().UpdateServerStatus(ss);

	SvcConfig::Instance().BroadcastSvrStatus(pInfo);

	SvcConfig::Instance().UpdateViewer();

	return TRUE;
}

BOOL CMsg<eSMH_ShutdownSvr>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CServerInfo* pInfo = SvcConfig::Instance().FindServer(pFrom);
	if(pInfo)
	{
		pInfo->SetServerStatus(CServerStatus::eSS_Closing);

		if (SvrControlMgr::Instance().ChkShutdownSvr(static_cast<CServerConfig::ESvrType>(pInfo->m_nSvrType)) 
			&& SvrControlMgr::Instance().Get_m_svr_close_level() == CSvrControlMgr::eSCL_ForegroundSvr) 
		{
			SvrControlMgr::Instance().NextStepLevel();
			SvrControlMgr::Instance().GracefulShutdownAllSvr();
		}
		
		if (SvrControlMgr::Instance().Get_m_svr_close_level() == CSvrControlMgr::eSCL_BackgroundSvr)
		{
			SvrControlMgr::Instance().SvrClose();
		}
	}else{
		LOG_ERROR("CServerInfo NULL");
		return FALSE;
	}
	
	return TRUE;
}

#if !defined(_UIDEN) && !defined(ONCE)
#define _UIDEN	UIDEN##__LINE__
#define ONCE(_exp) for( int _UIDEN = 0 ;(_exp)&& _UIDEN == 0; ++_UIDEN)
#endif


BOOL CMsg<eSMH_Game_LogOut>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CPlayerManager::LOGOUT_INFORMATION logoutInfo;
	if(false == PlayerManager::Instance().LogOut(m_AccountUID, pFrom, &logoutInfo))
	{
		LOG_SYSTEM(E)("[eSMH_Game_LogOut] User(%d) Logout Fail.", m_AccountUID);
		return FALSE;
	}

	//

	ONCE(true == logoutInfo.bRemovedResources)
	{
		LKEY keySvr = logoutInfo.keyLastAccessedServer;
		CServerInfo* pInfo = SvcConfig::Instance().FindServer(keySvr);	
		if(nullptr == pInfo)
		{
			LOG_SYSTEM(E)(__FUNCTION__"() [%s] Invalid last accessed server", MsgUtil::CMsgNameHolder::GetMsgName(m_wMH) );
			break;
		}

		if(FALSE == pInfo->IsType(CServerConfig::eSI_ST_COMMUNITY))
		{
			LOG_SYSTEM(E)(__FUNCTION__"() [%s] server type check fail", MsgUtil::CMsgNameHolder::GetMsgName(m_wMH));
			break;
		}

		FTQuerys::CDBQueryLogout query;

		query.m_UID = m_AccountUID;
		query.m_btRecentlyWorldID = pInfo->m_nWorldID;

		if ( !SvcNetwork::Instance().SendQueryReq(query, m_Key, pFrom) )
		{
			LOG_SYSTEM(E)(__FUNCTION__"() [%s] DBServer Not ready.", MsgUtil::CMsgNameHolder::GetMsgName(m_wMH) );
		}
	}


	return TRUE;
}

BOOL CMsg<eSMH_Master_Community_All_Player_Info>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CSyncStream	ss(pExtra, wSizeExtra);

	if(0 >= m_nMaxPlayerCount)
	{
		LOG_ERROR("[eSMH_Master_Community_All_Player_Info] m_nMaxPlayerCount is Zero.");
		return FALSE;
	}

	UID			uidAccount	= UID_INVALID;
	CPlayer*	pPlayer		= NULL;
	std::string strAccountName;
	
	LKEY keySvr = SvcConfig::Instance().FindServerKey(pFrom);

	for(int i = 0; i < m_nMaxPlayerCount; ++i)
	{
		ss >> uidAccount;
		ss >> strAccountName;

		pPlayer = PlayerManager::Instance().GetPlayerByUID(uidAccount);
		if(NULL == pPlayer)
		{
			// 중복이 아닐 경우에만 넣어 준다.
			if(false == PlayerManager::Instance().LogIn(uidAccount, (char*)strAccountName.c_str(), keySvr))
			{
				LOG_ERROR("[eSMH_Master_Community_All_Player_Info] pPlayer is NULL.");
			}
		}
		else
		{
			// 로그만 출력한다. 나오면 안되는 상황.
			LOG_SYSTEM(E)("[eSMH_Master_Community_All_Player_Info] Player(%d, %s) Duplicated Error.", uidAccount, strAccountName.c_str());
		}

		uidAccount = UID_INVALID;
		strAccountName.clear();
	}

	return TRUE;
}

/// 로비서버로부터의 접속
BOOL	CMsg<eUMH_Login_Req>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CServerInfo* pServerInfo = SvcConfig::Instance().GetMyInfo();
	if( pServerInfo->m_bCheckSrcRev == TRUE && m_dwSrcSvnRev != SVN_SRC_REVISION_NO)
	{
		LOG_SYSTEM(E)("%s Version not match.", __FUNCTION__);

		SendErrorMsg<CMsg<eUMH_Login_Res_Error>>(pFrom, m_Key, eER_Login_Invalid_SrcRev);
		return TRUE;
	}

	FTQuerys::CDBQueryLogin query;

	query.m_bSvrNum = 0;
	strncpy_s(query.m_szUSER_ID, m_szAccount, _TRUNCATE);
	strncpy_s(query.m_szUSER_PW, m_szPassword, _TRUNCATE);
	CVNetwork::GetAddressByIP(query.m_szUSER_IP, MaxLen_IP, m_dwIP);
	query.m_UID = 0;

	if(true == SvcConfig::Instance().GetServerSettings().m_bKickWhenPacketVersionDiff)
	{
		if(Version_LobbyMsg() != m_dwNormalPacketHash)
		{
			LOG_SYSTEM(W)("Client normal packet version does not certified");
			SendErrorMsg< CMsg<eUMH_Login_Res_Error> >(pFrom, m_Key, eER_VersionPacketCERTFailed);
			return TRUE;
		}

		if(Version_SyncMsg() != m_dwSyncPacketHash)
		{
			LOG_SYSTEM(W)("Client sync packet version does not certified");
			SendErrorMsg< CMsg<eUMH_Login_Res_Error> >(pFrom, m_Key, eER_VersionPacketCERTFailed);
			return TRUE;
		}

	}

	if ( !SvcNetwork::Instance().SendQueryReq(query, m_Key, pFrom) )
	{
		LOG_SYSTEM(E)("%s DBServer Not ready.", __FUNCTION__);

		SendErrorMsg<CMsg<eUMH_Login_Res_Error>>(pFrom, m_Key, eER_DBServerNotReady);
		return TRUE;
	}

	return TRUE;
}

/// 게임서버로부터의 접속
BOOL	CMsg<eUMH_Game_Login_Req>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	// Player를 찾는다.
	CPlayer* pPlayer = PlayerManager::Instance().GetPlayerByUID(m_UID);
	if(NULL == pPlayer)
	{
		LOG_SYSTEM(E)("%s pPlayer is NULL.", __FUNCTION__);

		SendErrorMsg<CMsg<eUMH_Game_Login_Res_Error>>(pFrom, m_Key, eER_UserNotFound);
		return FALSE;
	}

	if ( pPlayer->GetPlayerAuthKey() != m_dw64PlayerAuthKey )
	{
		LOG_SYSTEM(E)(__FUNCTION__"() pPlayer(%s) authenticate Fail.", pPlayer->GetAccountName());

		SendErrorMsg<CMsg<eUMH_Game_Login_Res_Error>>(pFrom, m_Key, eER_Invalid_AuthKey);
		return TRUE;
	}

	//m_dw64PlayerSessionKey는 마스터가 게임서버로 보내고, 클라이언트가 접근한 게임서버가 적어서 올리는 키다.
	//로그인 중 권한에 대한 인증은 모두 마스터서버가 맡는다. 게임서버는 수동적으로 전달만 하도록.
/*
	if(pPlayer->GetSessionKey() != m_dw64PlayerSessionKey)
	{
		LOG_SYSTEM(E)(__FUNCTION__"() pPlayer(%s) Invalid gameserver session .", pPlayer->GetAccountName());

		SendErrorMsg<CMsg<eUMH_Game_Login_Res_Error>>(pFrom, m_Key, eER_Invalid_SessionKey);
		return TRUE;

	}
*/

	pPlayer->SetPlayerAuthKey(0);
	PlayerManager::Instance().PlayerCheckIn(*pPlayer);
	
	LKEY keySvr = SvcConfig::Instance().FindServerKey(pFrom);
	if(LKEY_INVALID == keySvr)
	{
		LOG_ERROR("CMsg<eUMH_Game_Login_Req> keyCommunitySvr is INVALID");
		return FALSE;
	}

	// 해당 Player의 접속된 Community Key 값을 설정 한다.
	pPlayer->SetSvrKey(keySvr);
	CServerInfo* pInfo = SvcConfig::Instance().FindServer(keySvr);


	CMsg<eUMH_Game_Login_Res_OK>	msg;

	msg.m_Key = m_Key;
	msg.m_keySvr = m_keySvr;

	strncpy_s(msg.m_szAccount, pPlayer->GetAccountName(), _TRUNCATE);
	msg.m_UID = pPlayer->GetUID();

	pFrom->Send(&msg);
	return TRUE;
}

BOOL	CMsg<eSMH_Community_Game_Login>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	// Player를 찾는다.
	CPlayer* pPlayer = PlayerManager::Instance().GetPlayerByUID(m_AccountUID);
	if(NULL == pPlayer)
	{
		LOG_ERROR("[eSMH_Community_Game_Login] pPlayer is NULL.");
		return FALSE;
	}

	LKEY keyCommunitySvr = SvcConfig::Instance().FindServerKey(pFrom);
	if(LKEY_INVALID == keyCommunitySvr)
	{
		LOG_ERROR("[eSMH_Community_Game_Login] keyCommunitySvr is INVALID");
		return FALSE;
	}

	// 해당 Player의 접속된 Community Key 값을 설정 한다.
	pPlayer->SetSvrKey(keyCommunitySvr);
	return TRUE;
}


BOOL	CMsg<eSMH_ShutdownUsr>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	SvcConfig::Instance().ShutdownUser(*this);

	return TRUE;
}

BOOL	CMsg<eUMH_Game_Move_Community_Req>::Handler(CVSock* pFrom, char* pExtra, WORD wSizeExtra)
{
	CPlayer* pPlayer = PlayerManager::Instance().GetPlayerByUID(m_UID);
	if(nullptr == pPlayer)
		return TRUE; //요청이 발송 되는 사이에 유저가 끊겼을 수도 있다.

	//서비스 전용, 이 지점에서 요청한 m_CommunitySvrIdx 와 현재 접속한 커뮤니티 서버가 같으면
	//이동시키지 않는 처리를 하도록 한다. 디버깅용으로 필터 처리를 하지 않는다. [11/28/2013 박 단]
	/*pseudo code
	if(m_CommunitySvrIdx == pPlayer.GetCommunityServerIdx())
	{
		SendErr( requested_svr_currtently_same );
		return TRUE;
	}
	*/

	CServerInfo* pGameServer = SvcConfig::Instance().FindLowUserGameServer(this->m_CommunitySvrIdx);
	if(nullptr == pGameServer)
	{
		//해당 커뮤니티 서버에 접속 가능한 게임 서버가 없다.
		//SendErrorMsg<CMsg<eUMH_Login_Res_Error>>(pFrom, m_keyUsr, eER_DBError_Duplication_UserID);
		return TRUE;
	}

	//세션키를 발급해서 재접 대기시간을 준다.
	if(false == PlayerManager::Instance().ReserveMoveSession(*pPlayer))
	{
		//세션 예약에 실패했다.
		//코드 작성 시점에서는 시스템에러(이동이 아니라 애초에 로그인 진입이 안되는 레벨) [11/28/2013 박 단]
		//SendErrorMsg<CMsg<eUMH_Login_Res_Error>>(pFrom, m_keyUsr, eER_DBError_Duplication_UserID);
		return TRUE;
	}

	pPlayer->SetPlayerAuthKey(1);		/// 접속 이동 설정

	CMsg<eUMH_Community_Move_Res> msg;
	msg.m_Key				= this->m_Key; //유저의 게임에서의 LKEY
	msg.m_gidStage			= this->m_gidStage; // 유저가 속해 있는 스테이지의 GID
	msg.m_keySvr			= this->m_keySvr; //유저가 현재 접속 해 있는 게임 서버의 키
	msg.m_UID				= this->m_UID; //요청한 유저의 UID
	msg.m_dw64PlayerAuthKey = pPlayer->GetPlayerAuthKey();

	msg.m_wPort				= WORD(pGameServer->m_nExternal_Port);
	strncpy_s( msg.m_szServerIP, pGameServer->m_strExternal_IP.c_str(), _TRUNCATE);
	
	pFrom->Send(&msg);
	
	LOG_SYSTEM(N)( __FILE__LINE__ "%s Successfully request move Community Server",  MsgUtil::CMsgNameHolder::GetMsgName(m_wMH));
	return TRUE;
}
