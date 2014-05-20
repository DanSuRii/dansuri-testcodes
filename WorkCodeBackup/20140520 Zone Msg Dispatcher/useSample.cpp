#define REGISTER_PLAYERCHAR_HANDLER(T, msgID, Handler) m_pZoneMsgDispatcher->RegisterCustomHandler< CPlayerChar, T<msgID> >( &CPlayerChar::Handler, msgID );
