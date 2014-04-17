#pragma once

typedef ULONGLONG SESSION_KEY;
class CSessionValidator : public ServiceFrame::CServiceBase
{

	SESSION_KEY			GenerateSessionKey();
public:
	struct SESSION{
		SESSION_KEY	m_sessionKey;
		time_t		m_timeExpiration;

		SESSION(SESSION_KEY);
		bool operator==(SESSION&);
		bool operator==(SESSION_KEY&);
	};
	typedef std::list<SESSION> WAITING_SESSION_LIST;

	virtual void		Init(void) { 	SetState(eSS_Ready); }
	virtual void		TickProcess(void);
	SESSION_KEY			PushSession();
	void				EraseSession(SESSION_KEY);

private:
	WAITING_SESSION_LIST listWaitSessions;
};

typedef GLib::CGSingletonLongevity<CSessionValidator, eLongevity_Singlton_Contents, CPolicySingleThreaded> SessionValidator;
