#include "stdafx.h"
#include "Form1.h"
#include <cstring>
#include <cstdio>


using namespace System::Runtime::InteropServices;
typedef void* HWND;

typedef struct {
	unsigned int  cbSize;
	HWND  hwnd;
	unsigned long dwFlags;
	unsigned int  uCount;
	unsigned long dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

namespace FFAB_IntervalCalc
{
	ref struct FLASHTICK{
		System::TimeSpan^ tick;
		PFLASHWINFO info;

		FLASHTICK(System::TimeSpan^ aTick, PFLASHWINFO aInfo)
			:tick(aTick), info(aInfo)
		{}
	};
}

[DllImport("user32.dll")]
bool FlashWindowEx( PFLASHWINFO pwfi);

FLASHWINFO infoSet[] =
{
	{sizeof(FLASHWINFO), NULL, 0x00000003, 5, 0},
	{sizeof(FLASHWINFO), NULL, 0x00000003, 5, 0},
	{sizeof(FLASHWINFO), NULL, 0x00000003, 5, 0},
};



FFAB_IntervalCalc::Form1::Form1( void )
{
	InitializeComponent();
	//
	//TODO: 생성자 코드를 여기에 추가합니다.
	//

	flashTicks = gcnew array<FLASHTICK^>(3);

	flashTicks[0] = gcnew FLASHTICK(gcnew System::TimeSpan(0, 30, 0), &infoSet[0]);
	flashTicks[1] = gcnew FLASHTICK(gcnew System::TimeSpan(0, 15, 0), &infoSet[1]);
	flashTicks[2] = gcnew FLASHTICK(gcnew System::TimeSpan(0, 0, 0), &infoSet[2]);
}


System::Void FFAB_IntervalCalc::Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{	
	static HWND mainHandle = static_cast<HWND>(Handle.ToPointer());
	 
	textBox1->Clear();	
	
	if(dateTimePicker1->Value.CompareTo(System::DateTime::Now) > 0)
	{
		System::TimeSpan^ subtractResult = dateTimePicker1->Value.Subtract(System::DateTime::Now);
		textBox1->AppendText( subtractResult->ToString("hh\\:mm\\:ss") );
/*
		FLASHWINFO info;
		memset(&info, 0, sizeof(info));
		info.dwFlags = 0x00000003;//FLASHW_ALL;
		info.hwnd = handle;
		info.uCount = 5;
		info.cbSize = sizeof(info);
*/
		int minutes = subtractResult->Minutes;
		int seconds = subtractResult->Seconds;
		
/*
		int cur = minutes - seconds;
		char who[1024];
		sprintf_s(who, "%d", cur);
*/

		auto subtraceResultMinSec = gcnew System::TimeSpan(0, minutes, seconds );

		for each(FLASHTICK^ curflashTick in flashTicks)
		{
			auto compareResult = subtraceResultMinSec->CompareTo(curflashTick->tick);
			if( compareResult == 0
				//&&(curflashTick->info != nullptr)
				)
			{
				curflashTick->info->hwnd = mainHandle;
				FlashWindowEx(curflashTick->info);
			}
		}


	}
}	

System::Void FFAB_IntervalCalc::Form1::button1_Click( System::Object^ sender, System::EventArgs^ e )
{
	dateTimePicker1->Value = System::DateTime::Now;
}
