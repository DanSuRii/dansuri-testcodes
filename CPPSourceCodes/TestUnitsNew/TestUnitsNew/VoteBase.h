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
		eVM_OX,					//����
		eVM_DoubleChoice,		//��������
		eVM_TripleChoice,		//��������
		eVM_QuadrupleChoice,	//��������
		eVM_MultipleChoice,		//������������ .....��� �����ؾ����� �Ф�
		
		eVM_CNT,
	};

	//��ǥ ����
	class CVoteProclaimBase
	{
	public:
	};

	//�� ����� ��ǥ�� ���� ����
	class CVotingReactionBase
	{
	public:
	};

	//��ǥ�� �Ϸ�(��� �����ڰ� ��ǥ �Ϸ�) �Ǿ��� �� ��ǥ ����
	class CVoteCloseBase
	{
	public:
	};

	//��ǥ�� �̿Ϸ� �Ǿ��� ��, �ð��� ����Ǿ��� ��
	class CVoteExpireBase
	{
	public:
	};	

	//��ǥ
	class CVoteBase
	{
	public:
		CVoteBase(void);
		virtual ~CVoteBase(void);

		virtual CVoteBase* copy() = 0; //�޼ҵ� ��ü�� �ϳ��� �ϼ��� ���·� ���;��Ѵ�.

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

		//ADD�� vote subject�� vote method�� ���ؼ� ���ο� ��ǥ�� �����ϵ��� ��û�Ѵ�.
		//�� ������ ���� string �� ������ �� ���� �ϴ�...
		//1. Vote Subject-Method�� ���ؼ� Vote��ü�� ��������
		//2. Vote Extension Information�� Vote�� �����ϱ⿡ �������� üũ�� �Ѵ�.
		//2. THE Vote ���� ID�� �ο��Ѵ�.
		//void NewVoteReq();
		//void AddVote(CVoteBase*);		
		
		void ProcessTick();

		//��ǥ���� ������ �ͼ�, ������ ��ǥ�� ���� �����ش�.
		//void DoVote(VOTEID, VOTE_METHOD*);
	private:
	};
}