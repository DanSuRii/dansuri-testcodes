#pragma once

namespace VtVote
{
	enum EVoteSubject
	{
		eVS_StageSelect,

		eVS_CNT
	};

	enum EVoteMethod
	{
		eVM_OX,					//찬반
		eVM_DoubleChoice,		//이지선다
		eVM_TripleChoice,		//삼지선다
		eVM_QuadrupleChoice,	//사지선다
		eVM_MultipleChoice,		//다지선다형은 .....어떻게 구현해야할지 ㅠㅠ
		
		eVM_CNT,
	};

	//투표 선포
	class CVoteProclaimBase
	{
	public:
	};

	//한 사람의 투표에 대한 반응
	class CVotingReactionBase
	{
	public:
	};

	//투표가 완료(모든 참여자가 투표 완료) 되었을 때 투표 종료
	class CVoteCloseBase
	{
	public:
	};

	//투표가 미완료 되었을 때, 시간이 만료되었을 때
	class CVoteExpireBase
	{
	public:
	};	

	//투표
	class CVoteBase
	{
	public:
		CVoteBase(void);
		virtual ~CVoteBase(void);

		virtual CVoteBase* copy() = 0; //메소드 전체가 하나의 완성된 형태로 나와야한다.

	protected:
		CVoteProclaimBase*		pProclaim;
		CVotingReactionBase*	pVotingReaction;
		CVoteCloseBase*			pVoteClose;
		CVoteExpireBase*		pVoteExpire;


		time_t			m_tmExpire;
		EVoteSubject	m_voteSubject;
		EVoteMethod		m_voteMethod;
	};

	class CVoteStageSelect : public CVoteBase
	{		
	public:
		CVoteStageSelect(void);
		virtual ~CVoteStageSelect(void);

		CVoteBase* copy(){ return new CVoteStageSelect;}
	};


	class CVoteManager
	{
	public:
		typedef unsigned int VOTEID;

		//ADD는 vote subject와 vote method를 통해서 새로운 투표를 개설하도록 요청한다.
		//상세 주제에 대한 string 이 있으면 더 좋긴 하다...
		//1. Vote Subject-Method를 통해서 Vote개체를 가져오고
		//2. Vote Extension Information이 Vote를 생성하기에 적합한지 체크를 한다.
		//2. THE Vote 에게 ID를 부여한다.
		//void NewVoteReq();
		//void AddVote(CVoteBase*);		
		
		void ProcessTick();

		//투표함을 가지고 와서, 선택한 투표를 적용 시켜준다.
		//void DoVote(VOTEID, VOTE_METHOD*);
	private:
	};
}