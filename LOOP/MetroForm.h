#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex> 
#include <string> 
#include "variable.h"
#include "function.h"
#include <typeinfo>
#include <sstream>
#include <msclr\marshal_cppstd.h>
#include <bitset>

extern HINSTANCE AMK_DLL;// Объявляем идентификатор библиотеки
extern StartComPort  AMK_StartComPort;  // Объявляем указатель на  функцию AMK_StartComPort
extern GetCurrState AMK_GetCurrState;
extern Init AMK_Init;
extern BeginCyclogram AMK_BeginCyclogram;
extern GetData AMK_GetData;
extern SetIdle AMK_SetIdle;
extern PwrOn AMK_PwrOn;
extern PwrOff AMK_PwrOff;
extern DeleteComPort AMK_DeleteComPort;
extern StopDDS3 AMK_StopDDS3;
extern ResumeDDS3 AMK_ResumeDDS3;
extern SetIQPhaseOffset AMK_SetIQPhaseOffset;
extern DDS3FreqOffset AMK_DDS3FreqOffset;
extern GetMetroData AMK_GetMetroData;
extern SetMetroData AMK_SetMetroData;
extern GetSerial AMK_GetSerial;
extern void *ComPort;
extern uint32_t DDS_3FreqOffset;
extern struct PhaseOffset phaseOffset;


namespace AIRTEMP10 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace System::Threading;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Сводка для MyForm1
	/// </summary>
	public ref class MyForm1 : public System::Windows::Forms::Form
	{
	public:
		MyForm1(void)
		{
			InitializeComponent();
			//при открытии формы читаем из файла конфигурации параметры и записываем
			// в окошки техстбокс (умолчания)
			const char* Metrology = "Metrology.bin";
			unsigned int Sonde_serial;
			ifstream metro_in;
			extern GP_TOOL_METROLOGY metrology_struct;
			metro_in.open(Metrology, ios::binary);
			//metro_in.read((char*)&metrology_struct, sizeof(metrology_struct));
			metro_in.close();
			Text = L"DATA1 " + L" METRO " + gcnew String(Metrology);
			textBoxSer->Text = (metrology_struct.serial).ToString();
			Sonde_serial = metrology_struct.serial;
			//this->Width = 600;
			checkBox_read_only->Checked = true;

			for (int i = 0; i < 5; i++) {
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
				TEXTBOX_L1->Text = metrology_struct.L1[i].ToString();
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
				TEXTBOX_L2->Text = metrology_struct.L2[i].ToString();
			}
			for (int i = 0; i < 2; i++) {
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
				TEXTBOX_F->Text = metrology_struct.F[i].ToString();
			}
			for (int i = 0; i < 10; i++) {
				TextBox^ TEXTBOX_AIR = (TextBox^)this->tableLayoutPanel1->Controls["textBoxAIRT" + (i + 1).ToString()];
				TEXTBOX_AIR->Text = metrology_struct.Air_zz[i / 5][i % 5].ToString();
			}


		}




	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:


	private: System::Windows::Forms::Panel^  panel9;
	private: System::Windows::Forms::ComboBox^  comboBox_N_Tx;



	private: System::Windows::Forms::Label^  label37;

	private: System::Windows::Forms::TextBox^  textBoxSer;

	private: System::Windows::Forms::Label^  label39;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label40;
	private: System::Windows::Forms::TextBox^  textBox_sign32;





	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Label^  label44;






	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::Button^  button_smt;
	private: System::Windows::Forms::Button^  button_dfi;
	private: System::IO::Ports::SerialPort^  serialPort_LWD_1DDS;


	private: System::Windows::Forms::CheckBox^  checkBox_read_only;

	private: System::Windows::Forms::ComboBox^  comboBox_type;

	private: System::Windows::Forms::ComboBox^  comboBox_mod;
	private: System::Windows::Forms::Label^  label38;
	private: System::Windows::Forms::Label^  label66;
	private: System::Windows::Forms::TextBox^  textBox_number;
	private: System::Windows::Forms::Label^  label67;


	private: System::Windows::Forms::Label^  label41;
	private: System::Windows::Forms::TextBox^  textBox_sign_for_pallete;
	private: System::Windows::Forms::TextBox^  textBoxAIRT10;
	private: System::Windows::Forms::TextBox^  textBoxAIRT5;





	private: System::Windows::Forms::TextBox^  textBoxL2T5;


	private: System::Windows::Forms::TextBox^  textBoxL1T5;

	private: System::Windows::Forms::Label^  label65;
	private: System::Windows::Forms::Label^  label64;



	private: System::Windows::Forms::Label^  label60;

	private: System::Windows::Forms::Label^  label58;
	private: System::Windows::Forms::TextBox^  textBoxAIRT6;

	private: System::Windows::Forms::TextBox^  textBoxAIRT7;

	private: System::Windows::Forms::TextBox^  textBoxAIRT8;
private: System::Windows::Forms::TextBox^  textBoxAIRT9;




	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::Label^  label30;
	private: System::Windows::Forms::Label^  label31;

















	private: System::Windows::Forms::Label^  label51;
	private: System::Windows::Forms::Label^  label50;
	private: System::Windows::Forms::Label^  label49;
	private: System::Windows::Forms::Label^  label48;
	private: System::Windows::Forms::Label^  label36;
	private: System::Windows::Forms::Label^  label35;
	private: System::Windows::Forms::Label^  label34;
	private: System::Windows::Forms::Label^  label33;
	private: System::Windows::Forms::TextBox^  textBoxAIRT4;
	private: System::Windows::Forms::TextBox^  textBoxAIRT3;
	private: System::Windows::Forms::TextBox^  textBoxAIRT2;
	private: System::Windows::Forms::TextBox^  textBoxAIRT1;
	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::Label^  label24;


	private: System::Windows::Forms::TextBox^  textBoxFT2;
	private: System::Windows::Forms::TextBox^  textBoxFT1;


	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  textBoxL2T4;
	private: System::Windows::Forms::TextBox^  textBoxL2T3;
	private: System::Windows::Forms::TextBox^  textBoxL2T2;
	private: System::Windows::Forms::TextBox^  textBoxL2T1;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxL1T4;
	private: System::Windows::Forms::TextBox^  textBoxL1T3;
	private: System::Windows::Forms::TextBox^  textBoxL1T2;
	private: System::Windows::Forms::TextBox^  textBoxL1T1;
	private: System::Windows::Forms::Label^  labelT0;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox_min1;
	private: System::Windows::Forms::TextBox^  textBox_min2;
	private: System::Windows::Forms::TextBox^  textBox_min3;
	private: System::Windows::Forms::TextBox^  textBox_min4;
	private: System::Windows::Forms::TextBox^  textBox_min5;
	private: System::Windows::Forms::TextBox^  textBox_min6;
	private: System::Windows::Forms::TextBox^  textBox_min7;
	private: System::Windows::Forms::TextBox^  textBox_min8;
	private: System::Windows::Forms::TextBox^  textBox_min9;
	private: System::Windows::Forms::TextBox^  textBox_min10;






























	private: System::ComponentModel::IContainer^  components;



	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->panel9 = (gcnew System::Windows::Forms::Panel());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->textBox_sign_for_pallete = (gcnew System::Windows::Forms::TextBox());
			this->label67 = (gcnew System::Windows::Forms::Label());
			this->textBox_number = (gcnew System::Windows::Forms::TextBox());
			this->label66 = (gcnew System::Windows::Forms::Label());
			this->comboBox_mod = (gcnew System::Windows::Forms::ComboBox());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->comboBox_type = (gcnew System::Windows::Forms::ComboBox());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->textBoxSer = (gcnew System::Windows::Forms::TextBox());
			this->textBox_sign32 = (gcnew System::Windows::Forms::TextBox());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->comboBox_N_Tx = (gcnew System::Windows::Forms::ComboBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->button_smt = (gcnew System::Windows::Forms::Button());
			this->button_dfi = (gcnew System::Windows::Forms::Button());
			this->serialPort_LWD_1DDS = (gcnew System::IO::Ports::SerialPort(this->components));
			this->checkBox_read_only = (gcnew System::Windows::Forms::CheckBox());
			this->textBoxAIRT10 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT5 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL2T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL1T5 = (gcnew System::Windows::Forms::TextBox());
			this->label65 = (gcnew System::Windows::Forms::Label());
			this->label64 = (gcnew System::Windows::Forms::Label());
			this->label60 = (gcnew System::Windows::Forms::Label());
			this->label58 = (gcnew System::Windows::Forms::Label());
			this->textBoxAIRT6 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT7 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT8 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT9 = (gcnew System::Windows::Forms::TextBox());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->textBoxAIRT4 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT3 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT2 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAIRT1 = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->textBoxFT2 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxFT1 = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->textBoxL2T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL2T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL2T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL2T1 = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxL1T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL1T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL1T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxL1T1 = (gcnew System::Windows::Forms::TextBox());
			this->labelT0 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox_min1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_min4 = (gcnew System::Windows::Forms::TextBox());
			this->panel9->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel9
			// 
			this->panel9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel9->Controls->Add(this->label41);
			this->panel9->Controls->Add(this->textBox_sign_for_pallete);
			this->panel9->Controls->Add(this->label67);
			this->panel9->Controls->Add(this->textBox_number);
			this->panel9->Controls->Add(this->label66);
			this->panel9->Controls->Add(this->comboBox_mod);
			this->panel9->Controls->Add(this->label38);
			this->panel9->Controls->Add(this->comboBox_type);
			this->panel9->Controls->Add(this->label44);
			this->panel9->Controls->Add(this->textBoxSer);
			this->panel9->Controls->Add(this->textBox_sign32);
			this->panel9->Controls->Add(this->label39);
			this->panel9->Controls->Add(this->comboBox_N_Tx);
			this->panel9->Controls->Add(this->label37);
			this->panel9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->panel9->Location = System::Drawing::Point(13, 496);
			this->panel9->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->panel9->Name = L"panel9";
			this->panel9->Size = System::Drawing::Size(478, 98);
			this->panel9->TabIndex = 9;
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(299, 50);
			this->label41->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(136, 15);
			this->label41->TabIndex = 32;
			this->label41->Text = L"signature for pallete";
			// 
			// textBox_sign_for_pallete
			// 
			this->textBox_sign_for_pallete->Location = System::Drawing::Point(326, 66);
			this->textBox_sign_for_pallete->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_sign_for_pallete->Name = L"textBox_sign_for_pallete";
			this->textBox_sign_for_pallete->ReadOnly = true;
			this->textBox_sign_for_pallete->Size = System::Drawing::Size(76, 21);
			this->textBox_sign_for_pallete->TabIndex = 31;
			// 
			// label67
			// 
			this->label67->AutoSize = true;
			this->label67->Location = System::Drawing::Point(220, 2);
			this->label67->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label67->Name = L"label67";
			this->label67->Size = System::Drawing::Size(49, 15);
			this->label67->TabIndex = 27;
			this->label67->Text = L"номер";
			// 
			// textBox_number
			// 
			this->textBox_number->Location = System::Drawing::Point(222, 20);
			this->textBox_number->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_number->Name = L"textBox_number";
			this->textBox_number->Size = System::Drawing::Size(49, 21);
			this->textBox_number->TabIndex = 26;
			this->textBox_number->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// label66
			// 
			this->label66->AutoSize = true;
			this->label66->Location = System::Drawing::Point(172, 0);
			this->label66->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label66->Name = L"label66";
			this->label66->Size = System::Drawing::Size(33, 15);
			this->label66->TabIndex = 25;
			this->label66->Text = L"мод";
			// 
			// comboBox_mod
			// 
			this->comboBox_mod->FormattingEnabled = true;
			this->comboBox_mod->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8",
					L"9"
			});
			this->comboBox_mod->Location = System::Drawing::Point(166, 19);
			this->comboBox_mod->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->comboBox_mod->Name = L"comboBox_mod";
			this->comboBox_mod->Size = System::Drawing::Size(42, 23);
			this->comboBox_mod->TabIndex = 24;
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(2, 0);
			this->label38->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(35, 15);
			this->label38->TabIndex = 23;
			this->label38->Text = L"ТИП";
			// 
			// comboBox_type
			// 
			this->comboBox_type->FormattingEnabled = true;
			this->comboBox_type->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"АВТОНОМ", L"LWD", L"КАРТОГРАФ" });
			this->comboBox_type->Location = System::Drawing::Point(-2, 19);
			this->comboBox_type->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->comboBox_type->Name = L"comboBox_type";
			this->comboBox_type->Size = System::Drawing::Size(113, 23);
			this->comboBox_type->TabIndex = 22;
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(2, 50);
			this->label44->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(192, 15);
			this->label44->TabIndex = 15;
			this->label44->Text = L"signature32 разр. бинарный";
			// 
			// textBoxSer
			// 
			this->textBoxSer->Location = System::Drawing::Point(292, 20);
			this->textBoxSer->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxSer->Name = L"textBoxSer";
			this->textBoxSer->Size = System::Drawing::Size(126, 21);
			this->textBoxSer->TabIndex = 2;
			this->textBoxSer->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBox_sign32
			// 
			this->textBox_sign32->Location = System::Drawing::Point(2, 66);
			this->textBox_sign32->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_sign32->Name = L"textBox_sign32";
			this->textBox_sign32->Size = System::Drawing::Size(284, 21);
			this->textBox_sign32->TabIndex = 10;
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(290, 2);
			this->label39->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(144, 15);
			this->label39->TabIndex = 2;
			this->label39->Text = L"Серийный на кожухе";
			// 
			// comboBox_N_Tx
			// 
			this->comboBox_N_Tx->FormattingEnabled = true;
			this->comboBox_N_Tx->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"3 Tx", L"4 Tx", L"5 Tx" });
			this->comboBox_N_Tx->Location = System::Drawing::Point(118, 19);
			this->comboBox_N_Tx->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->comboBox_N_Tx->Name = L"comboBox_N_Tx";
			this->comboBox_N_Tx->Size = System::Drawing::Size(42, 23);
			this->comboBox_N_Tx->TabIndex = 8;
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(118, 2);
			this->label37->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(40, 15);
			this->label37->TabIndex = 6;
			this->label37->Text = L"N_Tx";
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(13, 453);
			this->button1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(154, 24);
			this->button1->TabIndex = 10;
			this->button1->Text = L"ЗАПИСАТЬ_В_ФАЙЛ";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm1::buttonWrite_tofile_Click);
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label40->Location = System::Drawing::Point(9, -2);
			this->label40->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(164, 20);
			this->label40->TabIndex = 11;
			this->label40->Text = L"Файл Метрологии";
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button3->Location = System::Drawing::Point(13, 407);
			this->button3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(154, 24);
			this->button3->TabIndex = 13;
			this->button3->Text = L"ОТКРЫТЬ_ФАЙЛ";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm1::buttonOpen_file_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button4->Location = System::Drawing::Point(179, 407);
			this->button4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(175, 24);
			this->button4->TabIndex = 14;
			this->button4->Text = L"ПРОЧИТАТЬ_ИЗ_ЗОНДА";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm1::buttonRead_from_sonde_Click);
			// 
			// button5
			// 
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button5->Location = System::Drawing::Point(179, 453);
			this->button5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(175, 24);
			this->button5->TabIndex = 15;
			this->button5->Text = L"ЗАПИСАТЬ_В_ЗОНД";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm1::buttonWrite_to_sonde_Click);
			// 
			// button_smt
			// 
			this->button_smt->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_smt->Location = System::Drawing::Point(365, 407);
			this->button_smt->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button_smt->Name = L"button_smt";
			this->button_smt->Size = System::Drawing::Size(124, 24);
			this->button_smt->TabIndex = 17;
			this->button_smt->Text = L"ВЗЯТЬ _dFI_SMT";
			this->button_smt->UseVisualStyleBackColor = true;
			this->button_smt->Click += gcnew System::EventHandler(this, &MyForm1::button_smt_Click);
			// 
			// button_dfi
			// 
			this->button_dfi->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_dfi->Location = System::Drawing::Point(365, 453);
			this->button_dfi->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button_dfi->Name = L"button_dfi";
			this->button_dfi->Size = System::Drawing::Size(124, 24);
			this->button_dfi->TabIndex = 18;
			this->button_dfi->Text = L"ВЗЯТЬ _dFI";
			this->button_dfi->UseVisualStyleBackColor = true;
			// 
			// serialPort_LWD_1DDS
			// 
			this->serialPort_LWD_1DDS->BaudRate = 115200;
			this->serialPort_LWD_1DDS->ReadTimeout = 1000;
			this->serialPort_LWD_1DDS->WriteTimeout = 1000;
			// 
			// checkBox_read_only
			// 
			this->checkBox_read_only->AutoSize = true;
			this->checkBox_read_only->Location = System::Drawing::Point(13, 377);
			this->checkBox_read_only->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkBox_read_only->Name = L"checkBox_read_only";
			this->checkBox_read_only->Size = System::Drawing::Size(96, 17);
			this->checkBox_read_only->TabIndex = 21;
			this->checkBox_read_only->Text = L"L_F_read_only";
			this->checkBox_read_only->UseVisualStyleBackColor = true;
			this->checkBox_read_only->CheckedChanged += gcnew System::EventHandler(this, &MyForm1::checkBox_read_only_CheckedChanged);
			// 
			// textBoxAIRT10
			// 
			this->textBoxAIRT10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxAIRT10->Location = System::Drawing::Point(390, 290);
			this->textBoxAIRT10->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT10->Name = L"textBoxAIRT10";
			this->textBoxAIRT10->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT10->TabIndex = 159;
			// 
			// textBoxAIRT5
			// 
			this->textBoxAIRT5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxAIRT5->Location = System::Drawing::Point(390, 130);
			this->textBoxAIRT5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT5->Name = L"textBoxAIRT5";
			this->textBoxAIRT5->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT5->TabIndex = 155;
			// 
			// textBoxL2T5
			// 
			this->textBoxL2T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL2T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL2T5->Location = System::Drawing::Point(199, 130);
			this->textBoxL2T5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL2T5->Name = L"textBoxL2T5";
			this->textBoxL2T5->Size = System::Drawing::Size(54, 21);
			this->textBoxL2T5->TabIndex = 153;
			// 
			// textBoxL1T5
			// 
			this->textBoxL1T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL1T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL1T5->Location = System::Drawing::Point(85, 130);
			this->textBoxL1T5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL1T5->Name = L"textBoxL1T5";
			this->textBoxL1T5->Size = System::Drawing::Size(57, 21);
			this->textBoxL1T5->TabIndex = 152;
			// 
			// label65
			// 
			this->label65->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label65->AutoSize = true;
			this->label65->Location = System::Drawing::Point(257, 128);
			this->label65->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label65->Name = L"label65";
			this->label65->Size = System::Drawing::Size(129, 30);
			this->label65->TabIndex = 151;
			this->label65->Text = L"AIR_4_F_LOW_mDg";
			this->label65->Click += gcnew System::EventHandler(this, &MyForm1::label65_Click);
			// 
			// label64
			// 
			this->label64->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label64->AutoSize = true;
			this->label64->Location = System::Drawing::Point(257, 288);
			this->label64->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label64->Name = L"label64";
			this->label64->Size = System::Drawing::Size(129, 15);
			this->label64->TabIndex = 150;
			this->label64->Text = L"AIR_5_F_HI_mDg";
			// 
			// label60
			// 
			this->label60->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label60->AutoSize = true;
			this->label60->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label60->Location = System::Drawing::Point(146, 128);
			this->label60->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label60->Name = L"label60";
			this->label60->Size = System::Drawing::Size(49, 30);
			this->label60->TabIndex = 146;
			this->label60->Text = L"L2_mm";
			// 
			// label58
			// 
			this->label58->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label58->AutoSize = true;
			this->label58->Location = System::Drawing::Point(2, 128);
			this->label58->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label58->Name = L"label58";
			this->label58->Size = System::Drawing::Size(79, 30);
			this->label58->TabIndex = 144;
			this->label58->Text = L"T5 _L1_mm";
			// 
			// textBoxAIRT6
			// 
			this->textBoxAIRT6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT6->Location = System::Drawing::Point(390, 162);
			this->textBoxAIRT6->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT6->Name = L"textBoxAIRT6";
			this->textBoxAIRT6->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT6->TabIndex = 41;
			this->textBoxAIRT6->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// textBoxAIRT7
			// 
			this->textBoxAIRT7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT7->Location = System::Drawing::Point(390, 194);
			this->textBoxAIRT7->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT7->Name = L"textBoxAIRT7";
			this->textBoxAIRT7->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT7->TabIndex = 60;
			this->textBoxAIRT7->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// textBoxAIRT8
			// 
			this->textBoxAIRT8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT8->Location = System::Drawing::Point(390, 226);
			this->textBoxAIRT8->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT8->Name = L"textBoxAIRT8";
			this->textBoxAIRT8->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT8->TabIndex = 61;
			this->textBoxAIRT8->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// textBoxAIRT9
			// 
			this->textBoxAIRT9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT9->Location = System::Drawing::Point(390, 258);
			this->textBoxAIRT9->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT9->Name = L"textBoxAIRT9";
			this->textBoxAIRT9->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT9->TabIndex = 62;
			this->textBoxAIRT9->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// label28
			// 
			this->label28->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(257, 160);
			this->label28->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(129, 15);
			this->label28->TabIndex = 52;
			this->label28->Text = L"AIR_1_F_HI_mDg";
			// 
			// label29
			// 
			this->label29->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(257, 192);
			this->label29->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(129, 15);
			this->label29->TabIndex = 53;
			this->label29->Text = L"AIR_2_F_HI_mDg";
			// 
			// label30
			// 
			this->label30->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(257, 224);
			this->label30->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(129, 15);
			this->label30->TabIndex = 54;
			this->label30->Text = L"AIR_3_F_HI_mDg";
			// 
			// label31
			// 
			this->label31->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(257, 256);
			this->label31->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(129, 15);
			this->label31->TabIndex = 55;
			this->label31->Text = L"AIR_4_F_HI_mDg";
			// 
			// label51
			// 
			this->label51->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(451, 224);
			this->label51->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(123, 15);
			this->label51->TabIndex = 73;
			this->label51->Text = L"MIN_R2_T3_F_HI";
			// 
			// label50
			// 
			this->label50->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(451, 192);
			this->label50->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(123, 15);
			this->label50->TabIndex = 72;
			this->label50->Text = L"MIN_R1_T2_F_HI";
			// 
			// label49
			// 
			this->label49->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(451, 160);
			this->label49->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(123, 15);
			this->label49->TabIndex = 71;
			this->label49->Text = L"MIN_R2_T1_F_HI";
			// 
			// label48
			// 
			this->label48->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label48->AutoSize = true;
			this->label48->Location = System::Drawing::Point(451, 128);
			this->label48->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(123, 30);
			this->label48->TabIndex = 70;
			this->label48->Text = L"MIN_R2_T5_F_LOW";
			// 
			// label36
			// 
			this->label36->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(451, 96);
			this->label36->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(123, 30);
			this->label36->TabIndex = 69;
			this->label36->Text = L"MIN_R1_T4_F_LOW";
			// 
			// label35
			// 
			this->label35->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(451, 64);
			this->label35->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(123, 30);
			this->label35->TabIndex = 68;
			this->label35->Text = L"MIN_R2_T3_F_LOW";
			// 
			// label34
			// 
			this->label34->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(451, 32);
			this->label34->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(123, 30);
			this->label34->TabIndex = 67;
			this->label34->Text = L"MIN_R1_T2_F_LOW";
			// 
			// label33
			// 
			this->label33->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(451, 0);
			this->label33->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(123, 30);
			this->label33->TabIndex = 66;
			this->label33->Text = L"MIN_R2_T1_F_LOW";
			this->label33->Click += gcnew System::EventHandler(this, &MyForm1::label33_Click);
			// 
			// textBoxAIRT4
			// 
			this->textBoxAIRT4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxAIRT4->Location = System::Drawing::Point(390, 98);
			this->textBoxAIRT4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT4->Name = L"textBoxAIRT4";
			this->textBoxAIRT4->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT4->TabIndex = 59;
			this->textBoxAIRT4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// textBoxAIRT3
			// 
			this->textBoxAIRT3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxAIRT3->Location = System::Drawing::Point(390, 66);
			this->textBoxAIRT3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT3->Name = L"textBoxAIRT3";
			this->textBoxAIRT3->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT3->TabIndex = 58;
			this->textBoxAIRT3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// textBoxAIRT2
			// 
			this->textBoxAIRT2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxAIRT2->Location = System::Drawing::Point(390, 34);
			this->textBoxAIRT2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT2->Name = L"textBoxAIRT2";
			this->textBoxAIRT2->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT2->TabIndex = 57;
			this->textBoxAIRT2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// textBoxAIRT1
			// 
			this->textBoxAIRT1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxAIRT1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxAIRT1->Location = System::Drawing::Point(390, 2);
			this->textBoxAIRT1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxAIRT1->Name = L"textBoxAIRT1";
			this->textBoxAIRT1->Size = System::Drawing::Size(57, 21);
			this->textBoxAIRT1->TabIndex = 56;
			this->textBoxAIRT1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit2);
			// 
			// label27
			// 
			this->label27->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(257, 96);
			this->label27->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(129, 30);
			this->label27->TabIndex = 51;
			this->label27->Text = L"AIR_3_F_LOW_mDg";
			// 
			// label26
			// 
			this->label26->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(257, 64);
			this->label26->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(129, 30);
			this->label26->TabIndex = 50;
			this->label26->Text = L"AIR_3_F_LOW_mDg";
			// 
			// label25
			// 
			this->label25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(257, 32);
			this->label25->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(129, 30);
			this->label25->TabIndex = 49;
			this->label25->Text = L"AIR_2_F_LOW_mDg";
			// 
			// label24
			// 
			this->label24->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(257, 0);
			this->label24->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(129, 30);
			this->label24->TabIndex = 48;
			this->label24->Text = L"AIR_1_F_LOW_mDg";
			// 
			// textBoxFT2
			// 
			this->textBoxFT2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxFT2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxFT2->Location = System::Drawing::Point(85, 226);
			this->textBoxFT2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxFT2->Name = L"textBoxFT2";
			this->textBoxFT2->Size = System::Drawing::Size(57, 21);
			this->textBoxFT2->TabIndex = 41;
			this->textBoxFT2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxFT1
			// 
			this->textBoxFT1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxFT1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxFT1->Location = System::Drawing::Point(85, 194);
			this->textBoxFT1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxFT1->Name = L"textBoxFT1";
			this->textBoxFT1->Size = System::Drawing::Size(57, 21);
			this->textBoxFT1->TabIndex = 40;
			this->textBoxFT1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// label17
			// 
			this->label17->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(2, 224);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(79, 15);
			this->label17->TabIndex = 33;
			this->label17->Text = L"F_HI_Hz";
			// 
			// label16
			// 
			this->label16->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label16->Location = System::Drawing::Point(2, 192);
			this->label16->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(79, 15);
			this->label16->TabIndex = 32;
			this->label16->Text = L"F_LOW_Hz";
			// 
			// textBoxL2T4
			// 
			this->textBoxL2T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL2T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL2T4->Location = System::Drawing::Point(199, 98);
			this->textBoxL2T4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL2T4->Name = L"textBoxL2T4";
			this->textBoxL2T4->Size = System::Drawing::Size(54, 21);
			this->textBoxL2T4->TabIndex = 27;
			this->textBoxL2T4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxL2T3
			// 
			this->textBoxL2T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL2T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL2T3->Location = System::Drawing::Point(199, 66);
			this->textBoxL2T3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL2T3->Name = L"textBoxL2T3";
			this->textBoxL2T3->Size = System::Drawing::Size(54, 21);
			this->textBoxL2T3->TabIndex = 26;
			this->textBoxL2T3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxL2T2
			// 
			this->textBoxL2T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL2T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL2T2->Location = System::Drawing::Point(199, 34);
			this->textBoxL2T2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL2T2->Name = L"textBoxL2T2";
			this->textBoxL2T2->Size = System::Drawing::Size(54, 21);
			this->textBoxL2T2->TabIndex = 25;
			this->textBoxL2T2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxL2T1
			// 
			this->textBoxL2T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL2T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL2T1->Location = System::Drawing::Point(199, 2);
			this->textBoxL2T1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL2T1->Name = L"textBoxL2T1";
			this->textBoxL2T1->Size = System::Drawing::Size(54, 21);
			this->textBoxL2T1->TabIndex = 24;
			this->textBoxL2T1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// label11
			// 
			this->label11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label11->Location = System::Drawing::Point(146, 96);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(49, 30);
			this->label11->TabIndex = 19;
			this->label11->Text = L"L2_mm";
			// 
			// label10
			// 
			this->label10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(146, 64);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(49, 30);
			this->label10->TabIndex = 18;
			this->label10->Text = L"L2_mm";
			// 
			// label9
			// 
			this->label9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(146, 32);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(49, 30);
			this->label9->TabIndex = 17;
			this->label9->Text = L"L2_mm";
			// 
			// label8
			// 
			this->label8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label8->Location = System::Drawing::Point(146, 0);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(49, 30);
			this->label8->TabIndex = 16;
			this->label8->Text = L"L2_mm";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(2, 96);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(79, 30);
			this->label3->TabIndex = 11;
			this->label3->Text = L"T4 _L1_mm";
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(2, 64);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(79, 15);
			this->label2->TabIndex = 10;
			this->label2->Text = L"T3_L1_mm";
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(2, 32);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(79, 30);
			this->label1->TabIndex = 9;
			this->label1->Text = L"T2 _L1_mm";
			// 
			// textBoxL1T4
			// 
			this->textBoxL1T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL1T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL1T4->Location = System::Drawing::Point(85, 98);
			this->textBoxL1T4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL1T4->Name = L"textBoxL1T4";
			this->textBoxL1T4->Size = System::Drawing::Size(57, 21);
			this->textBoxL1T4->TabIndex = 4;
			this->textBoxL1T4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxL1T3
			// 
			this->textBoxL1T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL1T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL1T3->Location = System::Drawing::Point(85, 66);
			this->textBoxL1T3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL1T3->Name = L"textBoxL1T3";
			this->textBoxL1T3->Size = System::Drawing::Size(57, 21);
			this->textBoxL1T3->TabIndex = 3;
			this->textBoxL1T3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxL1T2
			// 
			this->textBoxL1T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL1T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL1T2->Location = System::Drawing::Point(85, 34);
			this->textBoxL1T2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL1T2->Name = L"textBoxL1T2";
			this->textBoxL1T2->Size = System::Drawing::Size(57, 21);
			this->textBoxL1T2->TabIndex = 2;
			this->textBoxL1T2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// textBoxL1T1
			// 
			this->textBoxL1T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxL1T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxL1T1->Location = System::Drawing::Point(85, 2);
			this->textBoxL1T1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBoxL1T1->Name = L"textBoxL1T1";
			this->textBoxL1T1->Size = System::Drawing::Size(57, 21);
			this->textBoxL1T1->TabIndex = 1;
			this->textBoxL1T1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit);
			// 
			// labelT0
			// 
			this->labelT0->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT0->AutoSize = true;
			this->labelT0->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT0->Location = System::Drawing::Point(2, 0);
			this->labelT0->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->labelT0->Name = L"labelT0";
			this->labelT0->Size = System::Drawing::Size(79, 15);
			this->labelT0->TabIndex = 0;
			this->labelT0->Text = L"T1_L1_mm";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 8;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				83)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				61)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				53)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				58)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				133)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				61)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				127)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				96)));
			this->tableLayoutPanel1->Controls->Add(this->labelT0, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL1T1, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL1T2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL1T3, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL1T4, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->label8, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->label9, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->label10, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->label11, 2, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL2T1, 3, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL2T2, 3, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL2T3, 3, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL2T4, 3, 3);
			this->tableLayoutPanel1->Controls->Add(this->label58, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->label60, 2, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL1T5, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBoxL2T5, 3, 4);
			this->tableLayoutPanel1->Controls->Add(this->label24, 4, 0);
			this->tableLayoutPanel1->Controls->Add(this->label25, 4, 1);
			this->tableLayoutPanel1->Controls->Add(this->label26, 4, 2);
			this->tableLayoutPanel1->Controls->Add(this->label27, 4, 3);
			this->tableLayoutPanel1->Controls->Add(this->label65, 4, 4);
			this->tableLayoutPanel1->Controls->Add(this->label28, 4, 5);
			this->tableLayoutPanel1->Controls->Add(this->label29, 4, 6);
			this->tableLayoutPanel1->Controls->Add(this->label30, 4, 7);
			this->tableLayoutPanel1->Controls->Add(this->label31, 4, 8);
			this->tableLayoutPanel1->Controls->Add(this->label64, 4, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT1, 5, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT2, 5, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT3, 5, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT4, 5, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT5, 5, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT6, 5, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT7, 5, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT8, 5, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT9, 5, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBoxAIRT10, 5, 9);
			this->tableLayoutPanel1->Controls->Add(this->label33, 6, 0);
			this->tableLayoutPanel1->Controls->Add(this->label34, 6, 1);
			this->tableLayoutPanel1->Controls->Add(this->label35, 6, 2);
			this->tableLayoutPanel1->Controls->Add(this->label36, 6, 3);
			this->tableLayoutPanel1->Controls->Add(this->label48, 6, 4);
			this->tableLayoutPanel1->Controls->Add(this->label49, 6, 5);
			this->tableLayoutPanel1->Controls->Add(this->label50, 6, 6);
			this->tableLayoutPanel1->Controls->Add(this->label51, 6, 7);
			this->tableLayoutPanel1->Controls->Add(this->label4, 6, 8);
			this->tableLayoutPanel1->Controls->Add(this->label5, 6, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min1, 7, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min2, 7, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min3, 7, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min5, 7, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min6, 7, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min7, 7, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min8, 7, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min9, 7, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min10, 7, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_min4, 7, 3);
			this->tableLayoutPanel1->Controls->Add(this->label17, 0, 7);
			this->tableLayoutPanel1->Controls->Add(this->label16, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBoxFT2, 1, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBoxFT1, 1, 6);
			this->tableLayoutPanel1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tableLayoutPanel1->Location = System::Drawing::Point(13, 20);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 10;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 10)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(644, 325);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(451, 256);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(123, 15);
			this->label4->TabIndex = 160;
			this->label4->Text = L"MIN_R1_T4_F_HI";
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(451, 288);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(123, 15);
			this->label5->TabIndex = 161;
			this->label5->Text = L"MIN_R2_T5_F_HI";
			// 
			// textBox_min1
			// 
			this->textBox_min1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min1->Location = System::Drawing::Point(578, 2);
			this->textBox_min1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min1->Name = L"textBox_min1";
			this->textBox_min1->Size = System::Drawing::Size(92, 21);
			this->textBox_min1->TabIndex = 63;
			// 
			// textBox_min2
			// 
			this->textBox_min2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min2->Location = System::Drawing::Point(578, 34);
			this->textBox_min2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min2->Name = L"textBox_min2";
			this->textBox_min2->Size = System::Drawing::Size(92, 21);
			this->textBox_min2->TabIndex = 64;
			// 
			// textBox_min3
			// 
			this->textBox_min3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min3->Location = System::Drawing::Point(578, 66);
			this->textBox_min3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min3->Name = L"textBox_min3";
			this->textBox_min3->Size = System::Drawing::Size(92, 21);
			this->textBox_min3->TabIndex = 65;
			this->textBox_min3->TextChanged += gcnew System::EventHandler(this, &MyForm1::textBoxAIRT3t1_TextChanged);
			// 
			// textBox_min5
			// 
			this->textBox_min5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min5->Location = System::Drawing::Point(578, 130);
			this->textBox_min5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min5->Name = L"textBox_min5";
			this->textBox_min5->Size = System::Drawing::Size(92, 21);
			this->textBox_min5->TabIndex = 75;
			// 
			// textBox_min6
			// 
			this->textBox_min6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min6->Location = System::Drawing::Point(578, 162);
			this->textBox_min6->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min6->Name = L"textBox_min6";
			this->textBox_min6->Size = System::Drawing::Size(92, 21);
			this->textBox_min6->TabIndex = 76;
			// 
			// textBox_min7
			// 
			this->textBox_min7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min7->Location = System::Drawing::Point(578, 194);
			this->textBox_min7->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min7->Name = L"textBox_min7";
			this->textBox_min7->Size = System::Drawing::Size(92, 21);
			this->textBox_min7->TabIndex = 77;
			// 
			// textBox_min8
			// 
			this->textBox_min8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min8->Location = System::Drawing::Point(578, 226);
			this->textBox_min8->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min8->Name = L"textBox_min8";
			this->textBox_min8->Size = System::Drawing::Size(92, 21);
			this->textBox_min8->TabIndex = 78;
			// 
			// textBox_min9
			// 
			this->textBox_min9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min9->Location = System::Drawing::Point(578, 258);
			this->textBox_min9->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min9->Name = L"textBox_min9";
			this->textBox_min9->Size = System::Drawing::Size(92, 21);
			this->textBox_min9->TabIndex = 112;
			// 
			// textBox_min10
			// 
			this->textBox_min10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min10->Location = System::Drawing::Point(578, 290);
			this->textBox_min10->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min10->Name = L"textBox_min10";
			this->textBox_min10->Size = System::Drawing::Size(92, 21);
			this->textBox_min10->TabIndex = 162;
			// 
			// textBox_min4
			// 
			this->textBox_min4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_min4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_min4->Location = System::Drawing::Point(578, 98);
			this->textBox_min4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->textBox_min4->Name = L"textBox_min4";
			this->textBox_min4->Size = System::Drawing::Size(92, 21);
			this->textBox_min4->TabIndex = 74;
			// 
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(687, 605);
			this->Controls->Add(this->checkBox_read_only);
			this->Controls->Add(this->button_dfi);
			this->Controls->Add(this->button_smt);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label40);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel9);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"MyForm1";
			this->Text = L"MyForm1";
			this->panel9->ResumeLayout(false);
			this->panel9->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void buttonOpen_file_Click(System::Object^  sender, System::EventArgs^  e) {
		string  Metrology_Path;
		const char* Metrology;
		unsigned int Sonde_serial;
		ifstream metro_in;
		GP_TOOL_METROLOGY metrology_struct;

		openFileDialog1->Filter = "text files (*.bin)|*.bin";
		//openFileDialog1->InitialDirectory = gcnew String(Metrology_Path.c_str());
		//openFileDialog1->RestoreDirectory = true;
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			Metrology = (char*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer();
			Text = L"DATA1 " + L" METRO " + gcnew String(Metrology);
		}

		metro_in.open(Metrology, ios::binary);
		metro_in.read((char*)&metrology_struct, sizeof(metrology_struct));
		metro_in.close();

		/////////////////////////////////////////////////////////////////////////////
		uint32_t buff = metrology_struct.signature << 12;
		buff = buff >> 12;
		uint32_t tool_type = buff / 100000;
		uint32_t tool_N_Tx = (buff % 100000) / 10000;
		uint32_t tool_mod = (buff % 10000) / 1000;
		uint32_t tool_number = (buff % 1000);
		uint32_t signature_for_pallete = buff / 1000;
		textBox_sign_for_pallete->Text = signature_for_pallete.ToString();
		cout << metrology_struct.signature << " buff " << buff << " tool_type " << tool_type << " tool_N_Tx " << tool_N_Tx << " tool_mod " << tool_mod << " tool_number " << tool_number << " signature_for_pallete " << signature_for_pallete << endl;
		if ((tool_type == 1 || tool_type == 2 || tool_type == 3) && (tool_N_Tx == 3 || tool_N_Tx == 4 || tool_N_Tx == 5)) {
			comboBox_type->SelectedIndex = tool_type - 1;
			comboBox_N_Tx->SelectedIndex = tool_N_Tx - 3;
			comboBox_mod->SelectedIndex = tool_mod - 1;
			textBox_number->Text = tool_number.ToString();
			std::stringstream ss;
			ss << std::bitset<32>(metrology_struct.signature);
			textBox_sign32->Text = gcnew String((ss.str()).c_str());
		}
		else MessageBox::Show("сигнатура старого образца");

		textBoxSer->Text = (metrology_struct.serial).ToString();
		Sonde_serial = metrology_struct.serial;
		/////////////////////////////////////////////////////////////////////////////////

		for (int i = 0; i < 5; i++) {
			TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
			TEXTBOX_L1->Text = metrology_struct.L1[i].ToString();
			TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
			TEXTBOX_L2->Text = metrology_struct.L2[i].ToString();
		}
		for (int i = 0; i < 2; i++) {
			TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
			TEXTBOX_F->Text = metrology_struct.F[i].ToString();
		}
		for (int i = 0; i < 10; i++) {
			TextBox^ TEXTBOX_AIR = (TextBox^)this->tableLayoutPanel1->Controls["textBoxAIRT" + (i + 1).ToString()];
			TEXTBOX_AIR->Text = metrology_struct.Air_zz[i / 5][i % 5].ToString();
		}





	}

	private: System::Void buttonWrite_tofile_Click(System::Object^  sender, System::EventArgs^  e) {
		const char* Metrology_out = "Metrology.bin";
		unsigned int Sonde_serial;
		GP_TOOL_METROLOGY metrology_struct = {};
		ofstream metro_out;

		///////////////////////////////////////////////////////////////////////
		uint32_t tool_number = 0;
		if (textBox_number->Text != "")  tool_number = System::Int64::Parse(textBox_number->Text);

		uint32_t buff = metrology_struct.signature >> 20;
		buff = buff << 20;
		uint32_t buff1 =
			(comboBox_type->SelectedIndex + 1) * 100000 +
			(comboBox_N_Tx->SelectedIndex + 3) * 10000 +
			(comboBox_mod->SelectedIndex + 1) * 1000 + tool_number;

		buff = buff | buff1;
		metrology_struct.signature = buff;
		if (textBoxSer->Text != "")Sonde_serial = System::Int32::Parse(textBoxSer->Text);
		metrology_struct.serial = Sonde_serial;
		//////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < 5; i++) {
			TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
			if (TEXTBOX_L1->Text != "") metrology_struct.L1[i] = System::Int64::Parse(TEXTBOX_L1->Text);
			TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
			if (TEXTBOX_L2->Text != "") metrology_struct.L2[i] = System::Int64::Parse(TEXTBOX_L2->Text);
		}
		for (int i = 0; i < 2; i++) {
			TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
			if (TEXTBOX_F->Text != "") metrology_struct.F[i] = System::Int64::Parse(TEXTBOX_F->Text);
		}
		for (int i = 0; i < 10; i++) {
			TextBox^ TEXTBOX_AIR = (TextBox^)this->tableLayoutPanel1->Controls["textBoxAIRT" + (i + 1).ToString()];
			if (TEXTBOX_AIR->Text != "") metrology_struct.Air_zz[i / 5][i % 5] = System::Int64::Parse(TEXTBOX_AIR->Text);
		}



		saveFileDialog1->Filter = "text files (*.bin)|*.bin";
		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			Metrology_out = (char*)Marshal::StringToHGlobalAnsi(saveFileDialog1->FileName).ToPointer();
			Text = L" METRO OUT " + gcnew String(Metrology_out);
			metro_out.open(Metrology_out, ios::binary);
			metro_out.write((char*)&metrology_struct, sizeof(metrology_struct));
			metro_out.close();
		}
		else MessageBox::Show("Не удалось записать файл");


	}

	private: System::Void buttonRead_from_sonde_Click(System::Object^  sender, System::EventArgs^  e) {
		extern string Com_Name;
		extern string Com_LWD_Name;
		bool on_off_1dds = 1;
		extern bool sonde_on_off;
		unsigned int Sonde_serial;
		extern bool metro_check;
		bool plateu_test = false;

		if (on_off_1dds == true) {
			GP_TOOL_METROLOGY metrology_struct;
			metro_check = true;
			Sleep(700);
			bool stop = false;
			try {
				serialPort_LWD_1DDS->PortName = gcnew String(Com_LWD_Name.c_str());
				Console::WriteLine(serialPort_LWD_1DDS->PortName);
				Sleep(100);
				serialPort_LWD_1DDS->ReadTimeout = 1000;
				serialPort_LWD_1DDS->WriteTimeout = 1000;
				if (plateu_test == false)
					serialPort_LWD_1DDS->BaudRate = 125000;
				if (plateu_test == true)
					serialPort_LWD_1DDS->BaudRate = 115000;
				serialPort_LWD_1DDS->Open();
			}
			catch (...) { stop = 1; }

			if (!stop) {

				if (plateu_test == false) {
					uint8_t DATA[3]; DATA[0] = 0x00; DATA[1] = 0x04; DATA[2] = 240;
					send_to_tool(serialPort_LWD_1DDS, CMD_READ_EE, DATA, 3);
				}
				if (plateu_test == true) {
					uint8_t DATA[2] = { 0x05 ,0x00 };
					send_to_tool(serialPort_LWD_1DDS, 0x13, DATA, 2);
				}


				Sleep(150);
				uint8_t CMD; uint8_t DATA_IN[258];
				if (plateu_test == false) {
					receive_from_tool(serialPort_LWD_1DDS, CMD, DATA_IN, 240);
					memcpy(&metrology_struct, DATA_IN, 256);
				}
				if (plateu_test == true) {
					receive_from_tool(serialPort_LWD_1DDS, CMD, DATA_IN, 242);
					memcpy(&metrology_struct, DATA_IN + 2, 256);
				}
				serialPort_LWD_1DDS->Close();
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				uint32_t buff = metrology_struct.signature << 12;
				buff = buff >> 12;
				uint32_t tool_type = buff / 100000;
				uint32_t tool_N_Tx = (buff % 100000) / 10000;
				uint32_t tool_mod = (buff % 10000) / 1000;
				uint32_t tool_number = (buff % 1000);
				uint32_t signature_for_pallete = buff / 1000;
				textBox_sign_for_pallete->Text = signature_for_pallete.ToString();
				//cout<< metrology_struct.signature << " buff " << buff << " tool_type " << tool_type << " tool_N_Tx " << tool_N_Tx << " tool_mod " << tool_mod << " tool_number " << tool_number << " signature_for_pallete " << signature_for_pallete << endl;
				if ((tool_type == 1 || tool_type == 2 || tool_type == 3) && (tool_N_Tx == 3 || tool_N_Tx == 4 || tool_N_Tx == 5)) {
					comboBox_type->SelectedIndex = tool_type - 1;
					comboBox_N_Tx->SelectedIndex = tool_N_Tx - 3;
					comboBox_mod->SelectedIndex = tool_mod - 1;
					textBox_number->Text = tool_number.ToString();
					std::stringstream ss;
					ss << std::bitset<32>(metrology_struct.signature);
					textBox_sign32->Text = gcnew String((ss.str()).c_str());
				}
				else MessageBox::Show("сигнатура старого образца");
				textBoxSer->Text = (metrology_struct.serial).ToString();
				Sonde_serial = metrology_struct.serial;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				for (int i = 0; i < tool_N_Tx; i++) {
					TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
					TEXTBOX_L1->Text = metrology_struct.L1[i].ToString();
					TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
					TEXTBOX_L2->Text = metrology_struct.L2[i].ToString();
				}
				if (tool_N_Tx == 4) {
					for (int i = 0; i < 2; i++) {
						TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
						// F[0] - 400 кГц, F[1] - 2000 кГц (две рабочие частоты прибора)
						TEXTBOX_F->Text = metrology_struct.F[i].ToString();
					}
					for (int i = 0; i < 8; i++) {
						TextBox^ TEXTBOX_AIR = (TextBox^)this->tableLayoutPanel1->Controls["textBoxAIRT" + (i + 1).ToString()];
						TEXTBOX_AIR->Text = metrology_struct.Air_zz[i / 5][i % 5].ToString();							
					}
				}
				

				Sleep(500);
				MessageBox::Show("ФАЙЛ МЕТРОЛОГИИ  СЧИТАН ИЗ ПРИБОРА!");
				metro_check = false;
			}
			else {
				stop = 0;
				MessageBox::Show("ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ СЧИТАН !");
				metro_check = false;
				serialPort_LWD_1DDS->Close();
			}

		}
		else MessageBox::Show("Включите зонд !");


	}

	private: System::Void buttonWrite_to_sonde_Click(System::Object^  sender, System::EventArgs^  e) {
		extern bool sonde_on_off;
		unsigned int Sonde_serial;
		bool on_off_1dds = true;
		extern string Com_Name;
		extern bool metro_check;
	    bool plateu_test = false;

		if (on_off_1dds == true) {
			GP_TOOL_METROLOGY metrology_struct;
			///////////////////////////////////////////////////////////////////////
			uint32_t tool_number = 0;
			if (textBox_number->Text != "")  tool_number = System::Int64::Parse(textBox_number->Text);
			metrology_struct.signature = 0;
			uint32_t buff = metrology_struct.signature >> 20;
			buff = buff << 20;
			uint32_t buff1 =
				(comboBox_type->SelectedIndex + 1) * 100000 +
				(comboBox_N_Tx->SelectedIndex + 3) * 10000 +
				(comboBox_mod->SelectedIndex + 1) * 1000 + tool_number;

			buff = buff | buff1;
			metrology_struct.signature = buff;
			if (textBoxSer->Text != "")Sonde_serial = System::Int32::Parse(textBoxSer->Text);
			metrology_struct.serial = Sonde_serial;
			//////////////////////////////////////////////////////////////////////////////////////
			for (int i = 0; i < 5; i++) {
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
				if (TEXTBOX_L1->Text != "") metrology_struct.L1[i] = System::Int64::Parse(TEXTBOX_L1->Text);
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
				if (TEXTBOX_L2->Text != "") metrology_struct.L2[i] = System::Int64::Parse(TEXTBOX_L2->Text);
			}

			
			for (int i = 0; i < 2; i++) {
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
				// F[0] - 400 кГц, F[1] - 2000 кГц (две рабочие частоты прибора для 3/4/5 Tx)
				if (TEXTBOX_F->Text != "") metrology_struct.F[i] = System::Int64::Parse(TEXTBOX_F->Text);
			}
			for (int i = 0; i < 10; i++) {
				TextBox^ TEXTBOX_AIR = (TextBox^)this->tableLayoutPanel1->Controls["textBoxAIRT" + (i + 1).ToString()];
				if (TEXTBOX_AIR->Text != "") metrology_struct.Air_zz[i / 5][i % 5] = System::Int64::Parse(TEXTBOX_AIR->Text);
			}


			metrology_struct.serial = Sonde_serial;

			bool stop = false;
			metro_check = true;
			Sleep(500);
			try {
				serialPort_LWD_1DDS->PortName = gcnew String(Com_Name.c_str());
				Sleep(100);
				serialPort_LWD_1DDS->ReadTimeout = 1000;
				serialPort_LWD_1DDS->WriteTimeout = 1000;
				if (plateu_test == false)
					serialPort_LWD_1DDS->BaudRate = 125000;
				if (plateu_test == true)
					serialPort_LWD_1DDS->BaudRate = 115000;
				serialPort_LWD_1DDS->Open();
			}
			catch (...) { stop = 1; }

			if (!stop) {
				if (plateu_test == false) {
					uint8_t DATA_OUT[242];
					DATA_OUT[0] = 0x00; DATA_OUT[1] = 0x04;
					memcpy(DATA_OUT + 2, &metrology_struct, 240);
					send_to_tool(serialPort_LWD_1DDS, CMD_WRITE_EE, DATA_OUT, 242);
				}
				if (plateu_test == true) {
					uint8_t DATA_OUT[242];
					DATA_OUT[0] = 0xf5; DATA_OUT[1] = 0x00;
					memcpy(DATA_OUT + 2, &metrology_struct, 240);
					send_to_tool(serialPort_LWD_1DDS, 0x12, DATA_OUT, 242);
				}

				Sleep(150);
				uint8_t CMD; uint8_t DATA_IN[2];
				if (receive_from_tool(serialPort_LWD_1DDS, CMD, DATA_IN, 2) == 0) {
					MessageBox::Show("ФАЙЛ МЕТРОЛОГИИ  ЗАПИСАН В ПРИБОР !");
					metro_check = false;
				}

				else {
					MessageBox::Show("ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ ЗАПИСАН В ПРИБОР !");
					metro_check = false;
				}
				serialPort_LWD_1DDS->Close();

			}

			else {
				stop = 0;
				MessageBox::Show("ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ ЗАПИСАН В ПРИБОР !");
				bool metro_check = false;
				serialPort_LWD_1DDS->Close();
			}

		}
		else MessageBox::Show("Включите зонд !");
	}

	private: System::Void digit2(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
		if ((e->KeyChar) == '-') e->Handled = false; if ((e->KeyChar) == ',') e->Handled = false;
	}

	private: System::Void digit(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
	}

	
	private: System::Void checkBox_read_only_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

		if (checkBox_read_only->Checked == true) {
			for (int i = 0; i < 5; i++) {
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
				TEXTBOX_L1->ReadOnly = true;
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
				TEXTBOX_L2->ReadOnly = true;
			}
			for (int i = 0; i < 2; i++) {
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
				TEXTBOX_F->ReadOnly = true;
			}

		}


		if (checkBox_read_only->Checked == false) {
			for (int i = 0; i < 5; i++) {
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL1T" + (i + 1).ToString()];
				TEXTBOX_L1->ReadOnly = false;
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel1->Controls["textBoxL2T" + (i + 1).ToString()];
				TEXTBOX_L2->ReadOnly = false;
			}
			for (int i = 0; i < 2; i++) {
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel1->Controls["textBoxFT" + (i + 1).ToString()];
				TEXTBOX_F->ReadOnly = false;
			}

		}

	}

	private: System::Void button_smt_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label65_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label33_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void textBoxAIRT3t1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	};
}
