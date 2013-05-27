#pragma once

namespace FFAB_TrialTimer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1에 대한 요약입니다.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_maxLP;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  Calc;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown_trialLP;

	private: System::Windows::Forms::Label^  label3;

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->numericUpDown_maxLP = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->Calc = (gcnew System::Windows::Forms::Button());
			this->numericUpDown_trialLP = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_maxLP))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_trialLP))->BeginInit();
			this->SuspendLayout();
			// 
			// numericUpDown_maxLP
			// 
			this->numericUpDown_maxLP->Location = System::Drawing::Point(31, 60);
			this->numericUpDown_maxLP->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->numericUpDown_maxLP->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDown_maxLP->Name = L"numericUpDown_maxLP";
			this->numericUpDown_maxLP->Size = System::Drawing::Size(58, 21);
			this->numericUpDown_maxLP->TabIndex = 0;
			this->numericUpDown_maxLP->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(29, 36);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(32, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"총LP";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Time;
			this->dateTimePicker1->Location = System::Drawing::Point(173, 60);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(200, 21);
			this->dateTimePicker1->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(171, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"완충시간";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(31, 112);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(455, 209);
			this->textBox1->TabIndex = 4;
			// 
			// Calc
			// 
			this->Calc->Location = System::Drawing::Point(411, 61);
			this->Calc->Name = L"Calc";
			this->Calc->Size = System::Drawing::Size(75, 23);
			this->Calc->TabIndex = 3;
			this->Calc->Text = L"계산";
			this->Calc->UseVisualStyleBackColor = true;
			this->Calc->Click += gcnew System::EventHandler(this, &Form1::Calc_Click);
			// 
			// numericUpDown_trialLP
			// 
			this->numericUpDown_trialLP->Location = System::Drawing::Point(97, 60);
			this->numericUpDown_trialLP->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDown_trialLP->Name = L"numericUpDown_trialLP";
			this->numericUpDown_trialLP->Size = System::Drawing::Size(58, 21);
			this->numericUpDown_trialLP->TabIndex = 1;
			this->numericUpDown_trialLP->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(95, 36);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(60, 12);
			this->label3->TabIndex = 1;
			this->label3->Text = L"시도당 LP";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(505, 333);
			this->Controls->Add(this->Calc);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->numericUpDown_trialLP);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->numericUpDown_maxLP);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_maxLP))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_trialLP))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Calc_Click(System::Object^  sender, System::EventArgs^  e);
};
}

