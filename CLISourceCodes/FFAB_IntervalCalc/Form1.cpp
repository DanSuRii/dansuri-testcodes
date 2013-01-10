#include "stdafx.h"
#include "Form1.h"

System::Void FFAB_IntervalCalc::Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{	
	textBox1->Clear();	
	
	if(dateTimePicker1->Value.CompareTo(System::DateTime::Now) > 0)
		textBox1->AppendText( dateTimePicker1->Value.Subtract(System::DateTime::Now).ToString("hh\\:mm\\:ss") );
}	

System::Void FFAB_IntervalCalc::Form1::button1_Click( System::Object^ sender, System::EventArgs^ e )
{
	dateTimePicker1->Value = System::DateTime::Now;
}
