#include "stdafx.h"
#include "Work.h"
#include <windows.h>
#include <string>
#include <iostream>

typedef char SQLCHAR;
const int MaxLen_CharExtraDataBinSize = 1024;

struct STVtCharData_DBForm
{
	DWORD m_dwSaveCoolTime[70];
	struct { int IDX; char cc[3]; }m_stQuickSlotInfo[37];
	BYTE m_btParts[12];
	struct { int HP; int MP; }m_BaseStatus;
}
TestData = { {1, 2, 3}, {{4, "ab"},}, {5,'6',}, { 78, 89 } }
;

//include ������ ���� �͵��� .cpp�� �� ������ �ٸ� ������ inteli sense�� ������ ���� �ʵ��� ����.

//���� ���� Convert �Լ� �ڵ鷯���� ���� �ؾ� �Ѵ�.
//���� �ֱ��� �������� ��찡 �����Ƿ� ���� �������� �ֱٵ������� ��쿡�� Ž��ġ �ʵ��� �Ѵ�.
//Vector� �־ ���� DB����� �������� version���� ���� ������ �ڵ鷯���� ����� ���鵵��.
/*
struct {
	UINT COMPAREVERSION;
	bool (*fnConverter)(SQLCHAR* pData, size_t dataLen, DataType& toLoad)
};

�ܼ��� �޸� ��ü�� ���������� �⺻ convrter�� ��쿡�� ���� bool�� bRepeat���� ���� repeat�� �����ǵ���...
���ʿ� �׳� ������ �� ������ �ش� �ʱ�ȭ�� ���� ��쿡�� �д� ���� ���ϵ��� �ٲ���.
*/


class CharDBExtraBLOBAgent
{
public:
	enum
	{
#define REG_DATA(x, y) eCDBEBLOB_##x,
#include "BLOBAgentDataForm.h"
#undef REG_DATA

		eEXTInfo_CNT,
	};

	// [type : WORD] [size : WORD] [data] ...
	struct DATA_HEADER{
		WORD wHead;
		WORD dataSize;
		DATA_HEADER(WORD rhsHead, WORD rhsSize)
			:wHead(rhsHead)
			,dataSize(rhsSize)
		{}
	};
	static const struct DataInfo{
		unsigned short twoCC;
		char*	offset;
		std::string		typeName;
		size_t	sizeOfData;
	} DataInformations[];
	
/*
	static const size_t sizeofDatas =	sizeof( ((STVtCharData_DBForm*)0x00)->m_dwSaveCoolTime )
									+ sizeof(((STVtCharData_DBForm*)0x00)->m_stQuickSlotInfo)
									+ sizeof(((STVtCharData_DBForm*)0x00)->m_btParts)
									+ sizeof(((STVtCharData_DBForm*)0x00)->m_BaseStatus);
*/
#define REG_DATA(x, y)	 + sizeof(((STVtCharData_DBForm*)0x00)->x)
	static const size_t sizeofDatas =	0
#include "BLOBAgentDataForm.h"
;
#undef REG_DATA


	//SQLCHAR�� �д� �������� ���� ������ ũ�Ⱑ ���� ������ ���� �پ����� ������
	//���� ������ �ִ� ������ �� ��ŭ �о���Ѵ�.(=sqlchar[lenOfData] ���·� �����ϸ� �ȵȴ�)
	//���� ������ ũ�⺸�� ����ũ�Ⱑ ���� �� ������ ������ ������ ����
	static const size_t lenOfData = (eEXTInfo_CNT * sizeof(DATA_HEADER)) + sizeofDatas;
	//static_assert(SQLCHAR_BUFFER_SIZE > lenOfData);
	static_assert(MaxLen_CharExtraDataBinSize > lenOfData, "Datalen fail");

	//�ϳ��� �������� ������� 64kb ���� �۾ƾ� �Ѵ�. (header�� ������������ word�� �Ǿ��ִ�.)
#define REG_DATA(x, y)	 static_assert( sizeof(((STVtCharData_DBForm*)0x00)->x) < 1024*64, #x " Size over than 64kb" );	
#include "BLOBAgentDataForm.h"		
#undef REG_DATA

	
	void ToBLOB( SQLCHAR (&dest)[MaxLen_CharExtraDataBinSize] , STVtCharData_DBForm& src);
	void FromBLOB(STVtCharData_DBForm& dest, SQLCHAR (&src)[MaxLen_CharExtraDataBinSize]);

	const DataInfo* GetDataInfo(int CC);

	template<unsigned short CC> void* GetDataPtrTpl(STVtCharData_DBForm&);

	void* GetDataPtr(unsigned short CC, STVtCharData_DBForm&);

};

#define REG_DATA(x, y) template<> void* CharDBExtraBLOBAgent::GetDataPtrTpl<y>(STVtCharData_DBForm& data) { return (void*)&(data.x); }
#include "BLOBAgentDataForm.h"
#undef REG_DATA


const CharDBExtraBLOBAgent::DataInfo CharDBExtraBLOBAgent::DataInformations[] =
{
#define REG_DATA(x, y)	 {y, (char*)&(((STVtCharData_DBForm*)0x00)->x), #x, sizeof(((STVtCharData_DBForm*)0x00)->x)},
#include "BLOBAgentDataForm.h"
#undef REG_DATA
} ;


const CharDBExtraBLOBAgent::DataInfo* CharDBExtraBLOBAgent::GetDataInfo( int CC )
{
	const DataInfo* toRet = nullptr;
	switch(CC)
	{
#define REG_DATA(x, y) case y: toRet = &DataInformations[eCDBEBLOB_##x]; break;
#define UNREG_OLDDATA(x, y) case y: break;
#include "BLOBAgentDataForm.h"
#undef REG_DATA
#undef UNREG_OLDDATA	//���⼭ �ѹ��� üũ �� �൵ �ߺ������� �ǹǷ� �ؿ��� UNREG_OLDDATA�� �ѹ� �� �� �� �ʿ䰡 ����.
	}

	return toRet;
}

void* CharDBExtraBLOBAgent::GetDataPtr(unsigned short CC, STVtCharData_DBForm& dataBase)
{
	void* toRet = nullptr;
	switch(CC)
	{
#define REG_DATA(x, y) case y: toRet = GetDataPtrTpl<y>(dataBase); break;
#include "BLOBAgentDataForm.h"
#undef REG_DATA
	}

	return toRet;
}


void CharDBExtraBLOBAgent::FromBLOB( STVtCharData_DBForm& dest, SQLCHAR (&src)[MaxLen_CharExtraDataBinSize] )
{
	char* pCurr = src;
	char* pEnd = &src[_countof(src)];
	while( (pCurr + sizeof(DATA_HEADER)) < pEnd )
	{
		DATA_HEADER* pHeader = (DATA_HEADER*)pCurr;
		if(pHeader->wHead == 0x0000)
			break;
		pCurr+= sizeof(DATA_HEADER);		
		if( (pCurr + pHeader->dataSize) > pEnd )
			break;

		const DataInfo* pDataInfo = GetDataInfo(pHeader->wHead);
		
		//���߿� ������ ũ�� ��ȭ(�ܼ� array ��� / Ȯ��)�� ���� ġȯ������ ���� �ϵ��� �Ѵ�.
		if(nullptr == pDataInfo
			|| pDataInfo->sizeOfData != pHeader->dataSize) 
		{
			//(nullptr == pDataInfo)�������� �־����� ����� �������� �ʴ� ����.
			//(pDataInfo->sizeOfData != pHeader->dataSize) ������ ���� �ٲ�����Ƿ� skip
			pCurr+=pHeader->dataSize;			
			continue;
		}

		void* pToWrite = GetDataPtr(pHeader->wHead, dest);
		if(nullptr == pToWrite)
		{
			//critical fail, DataInfo�� DataPtr�� �ѽ��� pair�� �̷��.
			//���� �̰� ���ٸ� ������ �ִ� ��. //  [11/6/2013 �� ��]
			break;
		}

		CopyMemory(pToWrite, pCurr, pHeader->dataSize);
		
		pCurr+=pHeader->dataSize;

	}
}


void CharDBExtraBLOBAgent::ToBLOB( SQLCHAR (&dest)[MaxLen_CharExtraDataBinSize] , STVtCharData_DBForm& src )
{
	//���� ���� �����͸� ���� �б⸦ �����ϴ� �Լ��� ����ڰ� ���� �� �� �ְ� �۾� �ؾ��Ѵ�.
	//�⺻ ���ø��� ���� Read / Write�� �ϵ��� �ϰ�, Ÿ�Կ� ���� Ư��ȭ�� �� �� �ֵ��� ������ �ϴ� ����� ���.
 
	const DataInfo* pInfoBegin = DataInformations;
	const DataInfo* pInfoEnd = &DataInformations[_countof(DataInformations)];
	
	const char* pDestEnd = &dest[_countof(dest)];
	char* pToWrite = dest;

	for(const DataInfo* pCurInfoToWrite = pInfoBegin;
		pCurInfoToWrite < pInfoEnd;
		++pCurInfoToWrite)
	{
		void* pData = GetDataPtr(pCurInfoToWrite->twoCC, src);
		if(nullptr == pData)
		{
			//critical fail, DataInfo�� DataPtr�� �ѽ��� pair�� �̷��.
			//���� �̰� ���ٸ� �ٺ����� ������ �ִ� ��. //  [11/6/2013 �� ��]
			break;
		}
		if( false == ((pToWrite + sizeof(DATA_HEADER) + pCurInfoToWrite->sizeOfData) < pDestEnd) )
		{
			//�������ε� �� ���� ����. ������ �ܰ迡�� �̹� ������ ������ �������Ƿ�.
			//������ �߻��Ѵٸ� ���� critical fail...
			break;
		}
		DATA_HEADER header( pCurInfoToWrite->twoCC, pCurInfoToWrite->sizeOfData );

		CopyMemory(pToWrite, &header, sizeof(header));
		pToWrite += sizeof(header);
		CopyMemory(pToWrite, pData, pCurInfoToWrite->sizeOfData);
		pToWrite += pCurInfoToWrite->sizeOfData;
	}	
}

//switch(cc) case '00' : DataInformations[e#DATA]

void WorkImpl<BLOBAgent>::DoWork()
{
	char chInput = '\0';
	std::cout << "Write or Read(w=Write, r=Read):";
	std::cin >> chInput;

	SQLCHAR sqlChar[MaxLen_CharExtraDataBinSize];
	STVtCharData_DBForm newData;
	switch(chInput)
	{
	case 'w': case 'W':
		CharDBExtraBLOBAgent _instance;
		_instance.ToBLOB(sqlChar, TestData);
		//I/O OPERATION
		break;
	case 'r': case 'R':
		//I/O OPERATION
		ZeroMemory(&newData, sizeof(newData));		
		_instance.FromBLOB(newData, sqlChar  );
		break;
	default:
		std::cout << "Invalid Input";
		break;
	}


	
	std::cout << std::endl;
}



#if 0 //���� �ڵ�

//Objective : �� ���� �ɹ����� ����ü�� ������ �ٲ�ٰų�, ũ�Ⱑ �ٲ������ ���� ���� ���¿� ũ�����̰� ���� ���� ������� �ʴ´�.
//		�ش� �۾� ���� �Ϸ��� �ش� structure�� indicator�� ��ġ�ϰ� �׻� ���� ��ġ�� assign�� �� �ְ� �ؾ��ϴµ�,
//		�ش� symbol table�� �����ϱ� ���� �߰��� ���� �뷮�� ���� �� �ǻ簡 �ִٸ� �ϵ��� �Ѵ�. //  [11/6/2013 �� ��]
class CharDBExtraBLOBAgent
{
public:
	enum
	{
		eSaveCoolTime,
		eQuickSlotInfo,
		eParts,
		eBaseStatus,		

		eEXTInfo_CNT,
	};

	// [type : WORD] [size : WORD] [data] ...
	struct DATA_HEADER{ WORD wHead; WORD dataSize; };
	static const struct DataInfo{
		char*	offset;
		char		typeName[30];
		size_t	sizeOfData;
	} DataInformations[];
	static const size_t sizeofDatas =	sizeof( ((STVtCharData_DBForm*)0x00)->m_dwSaveCoolTime )
									+ sizeof(((STVtCharData_DBForm*)0x00)->m_stQuickSlotInfo)
									+ sizeof(((STVtCharData_DBForm*)0x00)->m_btParts)
									+ sizeof(((STVtCharData_DBForm*)0x00)->m_BaseStatus);

	//SQLCHAR�� ���� ������ ũ�Ⱑ �پ����� ������ ������ �ִ� �����͸�ŭ �о���Ѵ�.
	//���� ������ ũ�⺸�� ����ũ�Ⱑ ���� �� ������ ������ ������ ������ �ؾ�...
	static const size_t lenOfData = (eEXTInfo_CNT * sizeof(DATA_HEADER)) + sizeofDatas;
	//static_assert(SQLCHAR_BUFFER_SIZE > lenOfData);



	//if(DataInformations.find(DATA_HEADER::wHead).sizeOfData != DATA_HEADER::dataSize) destroy!;

	//�������� ������ ������ ��� �� �ִ� ���� ���������
	//������ ��ü�� Ÿ�� �ε����� �ٲ�� ���� guarentee ���� �ʵ��� �Ѵ�.

	/*
	DWORD				m_dwSaveCoolTime[MaxCnt_SaveCoolTimeID];		/// �����ؾ� �� ��Ÿ��
	STQuickSlotBase		m_stQuickSlotInfo[MaxCnt_QuickSlotSize * MaxCnt_QuickSlotCnt];		/// ����� ������ ����
	BYTE					m_btParts[MaxLen_Parts];		///< ����
	STVtBaseStatus		m_BaseStatus;					///< ĳ���� ��������(HP,MP)
	*/
	//void ToBLOB( SQLCHAR (&dest)[] , STVtCharData_DBForm& src);
	//void FromBLOB(STVtCharData_DBForm& dest, SQLCHAR (&src)[]);
};
const CharDBExtraBLOBAgent::DataInfo CharDBExtraBLOBAgent::DataInformations[] ={
	{(char*)&(((STVtCharData_DBForm*)0x00)->m_dwSaveCoolTime), "m_dwSaveCoolTime", sizeof(((STVtCharData_DBForm*)0x00)->m_dwSaveCoolTime)},
	{(char*)&(((STVtCharData_DBForm*)0x00)->m_stQuickSlotInfo), "m_stQuickSlotInfo", sizeof(((STVtCharData_DBForm*)0x00)->m_stQuickSlotInfo)},
	{(char*)&(((STVtCharData_DBForm*)0x00)->m_btParts), "m_btParts", sizeof(((STVtCharData_DBForm*)0x00)->m_btParts)},
	{(char*)&(((STVtCharData_DBForm*)0x00)->m_BaseStatus), "m_BaseStatus",sizeof(((STVtCharData_DBForm*)0x00)->m_BaseStatus) },
} ;

//switch(cc) case '00' : DataInformations[e#DATA]

#endif