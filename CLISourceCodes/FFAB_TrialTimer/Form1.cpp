#include "stdafx.h"
#include "Form1.h"

#define ASSERT_RETURN(exp, msg) if( !(exp) ){System::Windows::Forms::MessageBox::Show(msg); return;}

System::Void FFAB_TrialTimer::Form1::Calc_Click(System::Object^  sender, System::EventArgs^  e)
{
	System::Int32 maxLP(this->numericUpDown_maxLP->Value);
	auto trialLP = this->numericUpDown_trialLP->Value;

	ASSERT_RETURN( maxLP > trialLP, "Max LP less then trialLP");
	ASSERT_RETURN(dateTimePicker1->Value > System::DateTime::Now, "time must be future");

	static const System::Int32 minutesPerLP = 3;
	
	System::Int32 minutesToZeroLP = ((maxLP) * minutesPerLP);
	System::TimeSpan tsToZeroLP( 0, minutesToZeroLP, 0 );
	System::DateTime dtZeroLP(dateTimePicker1->Value.Subtract( tsToZeroLP ));

	System::Int32 minutesPerTrial(trialLP * minutesPerLP);

	System::DateTime curTrialDatetime( dtZeroLP.AddMinutes(minutesPerTrial) );

	System::String^ toWrite =
		gcnew System::String( System::String::Format(
		"완충시간: {0} {1}\r\n시도당LP: {2}\t시도당 필요시간: {3}\r\n최대LP: {4}\r\n",
		dateTimePicker1->Value.ToLongDateString(),
		dateTimePicker1->Value.ToShortTimeString(),		
		trialLP,
		minutesPerTrial,
		maxLP ) );	


	System::DateTime reverseTrialDateTime( dateTimePicker1->Value );
	System::TimeSpan tsMinutesPerTrial(0, minutesPerTrial, 0);

	
	for(int i = 1;curTrialDatetime < dateTimePicker1->Value;++i)
	{

		if(curTrialDatetime >= System::DateTime::Now)
			toWrite = System::String::Concat( toWrite, System::String::Format("{0}회: {1}", i, curTrialDatetime.ToShortTimeString()) , "\r\n" );		

/*
		reverseTrialDateTime = reverseTrialDateTime.Subtract( tsMinutesPerTrial );
		toWrite = System::String::Concat(toWrite,"\t", System::String::Format(""));
*/

		//toWrite = System::String::Concat( toWrite, "\r\n" );

		curTrialDatetime = curTrialDatetime.AddMinutes(minutesPerTrial);

	}

	textBox1->Clear();
	textBox1->AppendText(toWrite);
	textBox1->SelectionStart = 0;
	textBox1->ScrollToCaret();
	
}