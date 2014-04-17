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

//include 구문이 들어가는 것들은 .cpp로 다 내려서 다른 파일의 inteli sense에 영향을 주지 않도록 하자.

//버전 단위 Convert 함수 핸들러들을 연결 해야 한다.
//가장 최근의 데이터일 경우가 많으므로 제일 위에서는 최근데이터의 경우에는 탐색치 않도록 한다.
//Vector등에 넣어서 현재 DB저장된 데이터의 version보다 높은 컨버터 핸들러들의 서브셋 만들도록.
/*
struct {
	UINT COMPAREVERSION;
	bool (*fnConverter)(SQLCHAR* pData, size_t dataLen, DataType& toLoad)
};

단순히 메모리 전체를 날려버리는 기본 convrter의 경우에는 리턴 bool을 bRepeat으로 만들어서 repeat이 금지되도록...
애초에 그냥 수집된 것 내에서 해당 초기화가 있을 경우에는 읽는 것을 안하도록 바꾸자.
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


	//SQLCHAR는 읽는 시점에서 읽을 데이터 크기가 쓰던 시점에 비해 줄어들었을 때에도
	//쓰던 시점에 있던 데이터 양 만큼 읽어야한다.(=sqlchar[lenOfData] 형태로 정의하면 안된다)
	//계산된 데이터 크기보다 버퍼크기가 작을 때 컴파일 에러가 나도록 구현
	static const size_t lenOfData = (eEXTInfo_CNT * sizeof(DATA_HEADER)) + sizeofDatas;
	//static_assert(SQLCHAR_BUFFER_SIZE > lenOfData);
	static_assert(MaxLen_CharExtraDataBinSize > lenOfData, "Datalen fail");

	//하나의 데이터의 사이즈는 64kb 보다 작아야 한다. (header의 사이즈정보가 word로 되어있다.)
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
#undef UNREG_OLDDATA	//여기서 한번만 체크 해 줘도 중복방지가 되므로 밑에는 UNREG_OLDDATA를 한번 더 해 줄 필요가 없다.
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
		
		//나중에 데이터 크기 변화(단순 array 축소 / 확장)에 따른 치환연산을 구현 하도록 한다.
		if(nullptr == pDataInfo
			|| pDataInfo->sizeOfData != pHeader->dataSize) 
		{
			//(nullptr == pDataInfo)예전에는 있었지만 현재는 존재하지 않는 정보.
			//(pDataInfo->sizeOfData != pHeader->dataSize) 데이터 형이 바뀌었으므로 skip
			pCurr+=pHeader->dataSize;			
			continue;
		}

		void* pToWrite = GetDataPtr(pHeader->wHead, dest);
		if(nullptr == pToWrite)
		{
			//critical fail, DataInfo와 DataPtr은 한쌍의 pair를 이룬다.
			//만일 이게 없다면 문제가 있는 것. //  [11/6/2013 박 단]
			break;
		}

		CopyMemory(pToWrite, pCurr, pHeader->dataSize);
		
		pCurr+=pHeader->dataSize;

	}
}


void CharDBExtraBLOBAgent::ToBLOB( SQLCHAR (&dest)[MaxLen_CharExtraDataBinSize] , STVtCharData_DBForm& src )
{
	//길이 가변 데이터를 쓰고 읽기를 지원하는 함수를 사용자가 제공 할 수 있게 작업 해야한다.
	//기본 템플릿을 통해 Read / Write를 하도록 하고, 타입에 따른 특수화를 할 수 있도록 지원도 하는 방안을 고려.
 
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
			//critical fail, DataInfo와 DataPtr은 한쌍의 pair를 이룬다.
			//만일 이게 없다면 근본적인 문제가 있는 것. //  [11/6/2013 박 단]
			break;
		}
		if( false == ((pToWrite + sizeof(DATA_HEADER) + pCurInfoToWrite->sizeOfData) < pDestEnd) )
		{
			//이쪽으로도 올 일이 없다. 컴파일 단계에서 이미 사이즈 검증을 남겼으므로.
			//하지만 발생한다면 역시 critical fail...
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



#if 0 //원시 코드

//Objective : 각 하위 맴버들의 구조체의 순서가 바뀐다거나, 크기가 바뀌었지만 수정 이전 상태와 크기차이가 없는 경우는 고려하지 않는다.
//		해당 작업 들을 하려면 해당 structure의 indicator를 설치하고 항상 같은 위치에 assign될 수 있게 해야하는데,
//		해당 symbol table을 유지하기 위해 추가로 들어가는 용량을 지불 할 의사가 있다면 하도록 한다. //  [11/6/2013 박 단]
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

	//SQLCHAR는 내부 데이터 크기가 줄어들었을 때에도 가지고 있는 데이터만큼 읽어야한다.
	//계산된 데이터 크기보다 버퍼크기가 작을 때 컴파일 에러가 나도록 구현을 해야...
	static const size_t lenOfData = (eEXTInfo_CNT * sizeof(DATA_HEADER)) + sizeofDatas;
	//static_assert(SQLCHAR_BUFFER_SIZE > lenOfData);



	//if(DataInformations.find(DATA_HEADER::wHead).sizeOfData != DATA_HEADER::dataSize) destroy!;

	//데이터의 순서가 꼬여서 들어 가 있는 것은 상관없지만
	//데이터 자체의 타입 인덱스가 바뀌는 것은 guarentee 하지 않도록 한다.

	/*
	DWORD				m_dwSaveCoolTime[MaxCnt_SaveCoolTimeID];		/// 저장해야 될 쿨타임
	STQuickSlotBase		m_stQuickSlotInfo[MaxCnt_QuickSlotSize * MaxCnt_QuickSlotCnt];		/// 저장된 퀵슬롯 정보
	BYTE					m_btParts[MaxLen_Parts];		///< 파츠
	STVtBaseStatus		m_BaseStatus;					///< 캐릭터 상태정보(HP,MP)
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