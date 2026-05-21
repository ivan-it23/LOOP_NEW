#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex> 
#include <string> 
#include "variable.h"
#include "function.h"
#include <typeinfo.h>
namespace PARAMETRS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
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
			extern bool stop_working;
			extern int norm[10];
			extern int medium[10];
			extern int out_of_norm[10];
			extern double R_loop_physical[9];
			extern double loop_radius;
			extern string Com_Name;
			extern string Com1_Name;
			extern string Com_LWD_Name;
			extern bool express;
			extern bool Ro_to_raport;
			extern bool PH_to_raport;
			extern int N_aver;
			extern bool AM_TX;
			extern bool mode;
			extern bool sonde_ro_calk;
			extern bool comp_smt;
			extern bool param_form_closed;
			ifstream conf_in;
			conf_in.open(ConfName, ios::in);
			for (int i = 0; i < 10; i++) {
				conf_in >> norm[i]; conf_in >> medium[i]; conf_in >> out_of_norm[i];
			}
			for (int i = 0; i < 8; i++) {
				conf_in >> R_loop_physical[i];
			}
			conf_in >> loop_radius;
			conf_in >> Com_Name;
			conf_in >> Com1_Name;
			conf_in >> express;
			conf_in >> Ro_to_raport;
			conf_in >> PH_to_raport;
			conf_in >> N_aver;
			conf_in >> AM_TX;
			conf_in >> sonde_ro_calk;
			conf_in >> comp_smt;			
			conf_in >> Com_LWD_Name;
			conf_in >> mode;
			conf_in.close();
			param_form_closed = 1;

#pragma region
			if(Com_Name=="")comboBox1->Text = "COM_PORT";
			else comboBox1->Text = gcnew String(Com_Name.c_str());
			if (Com1_Name == "")comboBox3->Text = "COM_PORT";
			else comboBox3->Text = gcnew String(Com1_Name.c_str());
			if (Com_LWD_Name == "")comboBox_LWD_COM->Text = "COM_PORT";
			else comboBox_LWD_COM->Text = gcnew String(Com_LWD_Name.c_str());
			
			textBox1->Text = norm[0].ToString();
			textBox2->Text = medium[0].ToString();
			textBox3->Text = out_of_norm[0].ToString();
			textBox4->Text = norm[1].ToString();
			textBox5->Text = medium[1].ToString();
			textBox6->Text = out_of_norm[1].ToString();
			textBox7->Text = norm[2].ToString();
			textBox8->Text = medium[2].ToString();
			textBox9->Text = out_of_norm[2].ToString();
			textBox10->Text = norm[3].ToString();
			textBox11->Text = medium[3].ToString();
			textBox12->Text = out_of_norm[3].ToString();
			textBox13->Text = norm[4].ToString();
			textBox14->Text = medium[4].ToString();
			textBox15->Text = out_of_norm[4].ToString();
			textBox16->Text = norm[5].ToString();
			textBox17->Text = medium[5].ToString();
			textBox18->Text = out_of_norm[5].ToString();
			textBox19->Text = norm[6].ToString();
			textBox20->Text = medium[6].ToString();
			textBox21->Text = out_of_norm[6].ToString();
			textBox22->Text = norm[7].ToString();
			textBox23->Text = medium[7].ToString();
			textBox24->Text = out_of_norm[7].ToString();
			textBox25->Text = norm[8].ToString();
			textBox26->Text = medium[8].ToString();
			textBox27->Text = out_of_norm[8].ToString();
			textBox28->Text = norm[9].ToString();
			textBox29->Text = medium[9].ToString();
			textBox30->Text = out_of_norm[9].ToString();
			
			textBox31->Text = R_loop_physical[0].ToString();
			textBox34->Text = R_loop_physical[1].ToString();
			textBox35->Text = R_loop_physical[2].ToString();
			textBox36->Text = R_loop_physical[3].ToString();
			textBox37->Text = R_loop_physical[4].ToString();
			textBox38->Text = R_loop_physical[5].ToString();
			textBox39->Text = R_loop_physical[6].ToString();
			textBox40->Text = R_loop_physical[7].ToString();
			textBox41->Text = loop_radius.ToString();
			textBox42->Text = N_aver.ToString();
			checkBox_express->Checked = express;
			checkBox_Ro->Checked = Ro_to_raport;
			checkBox_PH->Checked = PH_to_raport;
			checkBox_re_from_loop->Checked = mode;
			checkBox_Am_Tx->Checked = AM_TX;
			checkBox_smt->Checked = comp_smt;
			comboBox_Ro_calk->SelectedIndex = sonde_ro_calk;

#pragma endregion
			
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
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	protected:
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::TextBox^  textBox12;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::TextBox^  textBox14;
	private: System::Windows::Forms::TextBox^  textBox15;
	private: System::Windows::Forms::TextBox^  textBox16;
	private: System::Windows::Forms::TextBox^  textBox17;
	private: System::Windows::Forms::TextBox^  textBox18;
	private: System::Windows::Forms::TextBox^  textBox19;
	private: System::Windows::Forms::TextBox^  textBox20;
	private: System::Windows::Forms::TextBox^  textBox21;
	private: System::Windows::Forms::TextBox^  textBox22;
	private: System::Windows::Forms::TextBox^  textBox23;
	private: System::Windows::Forms::TextBox^  textBox24;
	private: System::Windows::Forms::TextBox^  textBox25;
	private: System::Windows::Forms::TextBox^  textBox26;
	private: System::Windows::Forms::TextBox^  textBox27;
	private: System::Windows::Forms::TextBox^  textBox28;
	private: System::Windows::Forms::TextBox^  textBox29;
	private: System::Windows::Forms::TextBox^  textBox30;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;

private: System::Windows::Forms::TextBox^  textBox33;
private: System::Windows::Forms::TextBox^  textBox32;
private: System::Windows::Forms::TextBox^  textBox34;
private: System::Windows::Forms::TextBox^  textBox35;
private: System::Windows::Forms::TextBox^  textBox36;
private: System::Windows::Forms::TextBox^  textBox31;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::Button^  button1;






private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
private: System::Windows::Forms::ComboBox^  comboBox1;


private: System::IO::Ports::SerialPort^  serialPort1;


private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::TextBox^  textBox41;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::TextBox^  textBox37;
private: System::Windows::Forms::TextBox^  textBox39;
private: System::Windows::Forms::TextBox^  textBox40;
private: System::Windows::Forms::Label^  label19;

private: System::Windows::Forms::TextBox^  textBox38;
private: System::Windows::Forms::CheckBox^  checkBox_express;
private: System::Windows::Forms::CheckBox^  checkBox_Ro;
private: System::Windows::Forms::CheckBox^  checkBox_PH;
private: System::Windows::Forms::ComboBox^  comboBox3;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::TextBox^  textBox42;
private: System::Windows::Forms::CheckBox^  checkBox_Am_Tx;
private: System::Windows::Forms::ComboBox^  comboBox_Ro_calk;
private: System::Windows::Forms::CheckBox^  checkBox_smt;
private: System::Windows::Forms::ComboBox^  comboBox_LWD_COM;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
private: System::Windows::Forms::CheckBox^  checkBox_re_from_loop;



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
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->textBox33 = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->textBox15 = (gcnew System::Windows::Forms::TextBox());
			this->textBox16 = (gcnew System::Windows::Forms::TextBox());
			this->textBox17 = (gcnew System::Windows::Forms::TextBox());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			this->textBox19 = (gcnew System::Windows::Forms::TextBox());
			this->textBox20 = (gcnew System::Windows::Forms::TextBox());
			this->textBox21 = (gcnew System::Windows::Forms::TextBox());
			this->textBox22 = (gcnew System::Windows::Forms::TextBox());
			this->textBox23 = (gcnew System::Windows::Forms::TextBox());
			this->textBox24 = (gcnew System::Windows::Forms::TextBox());
			this->textBox25 = (gcnew System::Windows::Forms::TextBox());
			this->textBox26 = (gcnew System::Windows::Forms::TextBox());
			this->textBox27 = (gcnew System::Windows::Forms::TextBox());
			this->textBox28 = (gcnew System::Windows::Forms::TextBox());
			this->textBox29 = (gcnew System::Windows::Forms::TextBox());
			this->textBox30 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->textBox32 = (gcnew System::Windows::Forms::TextBox());
			this->textBox34 = (gcnew System::Windows::Forms::TextBox());
			this->textBox35 = (gcnew System::Windows::Forms::TextBox());
			this->textBox36 = (gcnew System::Windows::Forms::TextBox());
			this->textBox31 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->textBox37 = (gcnew System::Windows::Forms::TextBox());
			this->textBox39 = (gcnew System::Windows::Forms::TextBox());
			this->textBox40 = (gcnew System::Windows::Forms::TextBox());
			this->textBox38 = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->textBox41 = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox42 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->checkBox_express = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_Ro = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_PH = (gcnew System::Windows::Forms::CheckBox());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->checkBox_Am_Tx = (gcnew System::Windows::Forms::CheckBox());
			this->comboBox_Ro_calk = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox_smt = (gcnew System::Windows::Forms::CheckBox());
			this->comboBox_LWD_COM = (gcnew System::Windows::Forms::ComboBox());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->checkBox_re_from_loop = (gcnew System::Windows::Forms::CheckBox());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 4;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->Controls->Add(this->textBox33, 3, 0);
			this->tableLayoutPanel1->Controls->Add(this->label14, 0, 10);
			this->tableLayoutPanel1->Controls->Add(this->label5, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label4, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox1, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox2, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox3, 3, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox4, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox5, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox6, 3, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox8, 2, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox7, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox9, 3, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox10, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox11, 2, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox12, 3, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox13, 1, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox14, 2, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox15, 3, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox16, 1, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox17, 2, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox18, 3, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox19, 1, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox20, 2, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox21, 3, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox22, 1, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox23, 2, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox24, 3, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox25, 1, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox26, 2, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox27, 3, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox28, 1, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox29, 2, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox30, 3, 10);
			this->tableLayoutPanel1->Controls->Add(this->label6, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->label7, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->label8, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->label9, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->label10, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->label11, 0, 7);
			this->tableLayoutPanel1->Controls->Add(this->label12, 0, 8);
			this->tableLayoutPanel1->Controls->Add(this->label13, 0, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox32, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox34, 1, 12);
			this->tableLayoutPanel1->Controls->Add(this->textBox35, 2, 12);
			this->tableLayoutPanel1->Controls->Add(this->textBox36, 3, 12);
			this->tableLayoutPanel1->Controls->Add(this->textBox31, 0, 12);
			this->tableLayoutPanel1->Controls->Add(this->label1, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label15, 0, 11);
			this->tableLayoutPanel1->Controls->Add(this->label16, 1, 11);
			this->tableLayoutPanel1->Controls->Add(this->label17, 2, 11);
			this->tableLayoutPanel1->Controls->Add(this->label18, 3, 11);
			this->tableLayoutPanel1->Controls->Add(this->label20, 0, 13);
			this->tableLayoutPanel1->Controls->Add(this->label21, 1, 13);
			this->tableLayoutPanel1->Controls->Add(this->label22, 2, 13);
			this->tableLayoutPanel1->Controls->Add(this->label23, 3, 13);
			this->tableLayoutPanel1->Controls->Add(this->textBox37, 0, 14);
			this->tableLayoutPanel1->Controls->Add(this->textBox39, 2, 14);
			this->tableLayoutPanel1->Controls->Add(this->textBox40, 3, 14);
			this->tableLayoutPanel1->Controls->Add(this->textBox38, 1, 14);
			this->tableLayoutPanel1->Controls->Add(this->label19, 0, 15);
			this->tableLayoutPanel1->Controls->Add(this->textBox41, 1, 15);
			this->tableLayoutPanel1->Controls->Add(this->label25, 2, 15);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 16);
			this->tableLayoutPanel1->Controls->Add(this->textBox42, 3, 15);
			this->tableLayoutPanel1->Location = System::Drawing::Point(9, 9);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 17;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 22)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 32)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 19)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 40)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 26)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 16)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(333, 417);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// textBox33
			// 
			this->textBox33->BackColor = System::Drawing::SystemColors::Control;
			this->textBox33->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox33->ForeColor = System::Drawing::Color::Red;
			this->textBox33->Location = System::Drawing::Point(242, 2);
			this->textBox33->Margin = System::Windows::Forms::Padding(2);
			this->textBox33->Multiline = true;
			this->textBox33->Name = L"textBox33";
			this->textBox33->Size = System::Drawing::Size(76, 35);
			this->textBox33->TabIndex = 46;
			this->textBox33->Text = L"Провести калибровку";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label14->ForeColor = System::Drawing::Color::Black;
			this->label14->Location = System::Drawing::Point(2, 255);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(28, 13);
			this->label14->TabIndex = 43;
			this->label14->Text = L"AIR";
			this->label14->Visible = false;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label5->ForeColor = System::Drawing::Color::Black;
			this->label5->Location = System::Drawing::Point(2, 39);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(23, 13);
			this->label5->TabIndex = 7;
			this->label5->Text = L"R1";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->ForeColor = System::Drawing::Color::Black;
			this->label4->Location = System::Drawing::Point(2, 0);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(24, 20);
			this->label4->TabIndex = 6;
			this->label4->Text = L"%";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(82, 41);
			this->textBox1->Margin = System::Windows::Forms::Padding(2);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(76, 20);
			this->textBox1->TabIndex = 0;
			this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(162, 41);
			this->textBox2->Margin = System::Windows::Forms::Padding(2);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(76, 20);
			this->textBox2->TabIndex = 1;
			this->textBox2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(242, 41);
			this->textBox3->Margin = System::Windows::Forms::Padding(2);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(76, 20);
			this->textBox3->TabIndex = 2;
			this->textBox3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(82, 65);
			this->textBox4->Margin = System::Windows::Forms::Padding(2);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(76, 20);
			this->textBox4->TabIndex = 8;
			this->textBox4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(162, 65);
			this->textBox5->Margin = System::Windows::Forms::Padding(2);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(76, 20);
			this->textBox5->TabIndex = 9;
			this->textBox5->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(242, 65);
			this->textBox6->Margin = System::Windows::Forms::Padding(2);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(76, 20);
			this->textBox6->TabIndex = 10;
			this->textBox6->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(162, 89);
			this->textBox8->Margin = System::Windows::Forms::Padding(2);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(76, 20);
			this->textBox8->TabIndex = 12;
			this->textBox8->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(82, 89);
			this->textBox7->Margin = System::Windows::Forms::Padding(2);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(76, 20);
			this->textBox7->TabIndex = 11;
			this->textBox7->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(242, 89);
			this->textBox9->Margin = System::Windows::Forms::Padding(2);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(76, 20);
			this->textBox9->TabIndex = 13;
			this->textBox9->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox10
			// 
			this->textBox10->Location = System::Drawing::Point(82, 113);
			this->textBox10->Margin = System::Windows::Forms::Padding(2);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(76, 20);
			this->textBox10->TabIndex = 14;
			this->textBox10->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox11
			// 
			this->textBox11->Location = System::Drawing::Point(162, 113);
			this->textBox11->Margin = System::Windows::Forms::Padding(2);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(76, 20);
			this->textBox11->TabIndex = 15;
			this->textBox11->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox12
			// 
			this->textBox12->Location = System::Drawing::Point(242, 113);
			this->textBox12->Margin = System::Windows::Forms::Padding(2);
			this->textBox12->Name = L"textBox12";
			this->textBox12->Size = System::Drawing::Size(76, 20);
			this->textBox12->TabIndex = 16;
			this->textBox12->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox13
			// 
			this->textBox13->Location = System::Drawing::Point(82, 137);
			this->textBox13->Margin = System::Windows::Forms::Padding(2);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(76, 20);
			this->textBox13->TabIndex = 17;
			this->textBox13->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox14
			// 
			this->textBox14->Location = System::Drawing::Point(162, 137);
			this->textBox14->Margin = System::Windows::Forms::Padding(2);
			this->textBox14->Name = L"textBox14";
			this->textBox14->Size = System::Drawing::Size(76, 20);
			this->textBox14->TabIndex = 18;
			this->textBox14->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox15
			// 
			this->textBox15->Location = System::Drawing::Point(242, 137);
			this->textBox15->Margin = System::Windows::Forms::Padding(2);
			this->textBox15->Name = L"textBox15";
			this->textBox15->Size = System::Drawing::Size(76, 20);
			this->textBox15->TabIndex = 19;
			this->textBox15->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox16
			// 
			this->textBox16->Location = System::Drawing::Point(82, 161);
			this->textBox16->Margin = System::Windows::Forms::Padding(2);
			this->textBox16->Name = L"textBox16";
			this->textBox16->Size = System::Drawing::Size(76, 20);
			this->textBox16->TabIndex = 20;
			this->textBox16->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox17
			// 
			this->textBox17->Location = System::Drawing::Point(162, 161);
			this->textBox17->Margin = System::Windows::Forms::Padding(2);
			this->textBox17->Name = L"textBox17";
			this->textBox17->Size = System::Drawing::Size(76, 20);
			this->textBox17->TabIndex = 21;
			this->textBox17->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox18
			// 
			this->textBox18->Location = System::Drawing::Point(242, 161);
			this->textBox18->Margin = System::Windows::Forms::Padding(2);
			this->textBox18->Name = L"textBox18";
			this->textBox18->Size = System::Drawing::Size(76, 20);
			this->textBox18->TabIndex = 22;
			this->textBox18->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox19
			// 
			this->textBox19->Location = System::Drawing::Point(82, 185);
			this->textBox19->Margin = System::Windows::Forms::Padding(2);
			this->textBox19->Name = L"textBox19";
			this->textBox19->Size = System::Drawing::Size(76, 20);
			this->textBox19->TabIndex = 23;
			this->textBox19->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox20
			// 
			this->textBox20->Location = System::Drawing::Point(162, 185);
			this->textBox20->Margin = System::Windows::Forms::Padding(2);
			this->textBox20->Name = L"textBox20";
			this->textBox20->Size = System::Drawing::Size(76, 20);
			this->textBox20->TabIndex = 24;
			this->textBox20->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox21
			// 
			this->textBox21->Location = System::Drawing::Point(242, 185);
			this->textBox21->Margin = System::Windows::Forms::Padding(2);
			this->textBox21->Name = L"textBox21";
			this->textBox21->Size = System::Drawing::Size(76, 20);
			this->textBox21->TabIndex = 25;
			this->textBox21->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox22
			// 
			this->textBox22->Location = System::Drawing::Point(82, 209);
			this->textBox22->Margin = System::Windows::Forms::Padding(2);
			this->textBox22->Name = L"textBox22";
			this->textBox22->Size = System::Drawing::Size(76, 20);
			this->textBox22->TabIndex = 26;
			this->textBox22->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox23
			// 
			this->textBox23->Location = System::Drawing::Point(162, 209);
			this->textBox23->Margin = System::Windows::Forms::Padding(2);
			this->textBox23->Name = L"textBox23";
			this->textBox23->Size = System::Drawing::Size(76, 20);
			this->textBox23->TabIndex = 27;
			this->textBox23->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox24
			// 
			this->textBox24->Location = System::Drawing::Point(242, 209);
			this->textBox24->Margin = System::Windows::Forms::Padding(2);
			this->textBox24->Name = L"textBox24";
			this->textBox24->Size = System::Drawing::Size(76, 20);
			this->textBox24->TabIndex = 28;
			this->textBox24->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox25
			// 
			this->textBox25->Location = System::Drawing::Point(82, 233);
			this->textBox25->Margin = System::Windows::Forms::Padding(2);
			this->textBox25->Name = L"textBox25";
			this->textBox25->Size = System::Drawing::Size(76, 20);
			this->textBox25->TabIndex = 29;
			this->textBox25->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox26
			// 
			this->textBox26->Location = System::Drawing::Point(162, 233);
			this->textBox26->Margin = System::Windows::Forms::Padding(2);
			this->textBox26->Name = L"textBox26";
			this->textBox26->Size = System::Drawing::Size(76, 20);
			this->textBox26->TabIndex = 30;
			this->textBox26->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox27
			// 
			this->textBox27->Location = System::Drawing::Point(242, 233);
			this->textBox27->Margin = System::Windows::Forms::Padding(2);
			this->textBox27->Name = L"textBox27";
			this->textBox27->Size = System::Drawing::Size(76, 20);
			this->textBox27->TabIndex = 31;
			this->textBox27->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox28
			// 
			this->textBox28->Location = System::Drawing::Point(82, 257);
			this->textBox28->Margin = System::Windows::Forms::Padding(2);
			this->textBox28->Name = L"textBox28";
			this->textBox28->Size = System::Drawing::Size(76, 20);
			this->textBox28->TabIndex = 32;
			this->textBox28->Visible = false;
			this->textBox28->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox29
			// 
			this->textBox29->Location = System::Drawing::Point(162, 257);
			this->textBox29->Margin = System::Windows::Forms::Padding(2);
			this->textBox29->Name = L"textBox29";
			this->textBox29->Size = System::Drawing::Size(76, 20);
			this->textBox29->TabIndex = 33;
			this->textBox29->Visible = false;
			this->textBox29->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox30
			// 
			this->textBox30->Location = System::Drawing::Point(242, 257);
			this->textBox30->Margin = System::Windows::Forms::Padding(2);
			this->textBox30->Name = L"textBox30";
			this->textBox30->Size = System::Drawing::Size(76, 20);
			this->textBox30->TabIndex = 34;
			this->textBox30->Visible = false;
			this->textBox30->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label6->ForeColor = System::Drawing::Color::Black;
			this->label6->Location = System::Drawing::Point(2, 63);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(23, 13);
			this->label6->TabIndex = 35;
			this->label6->Text = L"R2";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label7->ForeColor = System::Drawing::Color::Black;
			this->label7->Location = System::Drawing::Point(2, 87);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(23, 13);
			this->label7->TabIndex = 36;
			this->label7->Text = L"R3";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label8->ForeColor = System::Drawing::Color::Black;
			this->label8->Location = System::Drawing::Point(2, 111);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(23, 13);
			this->label8->TabIndex = 37;
			this->label8->Text = L"R4";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label9->ForeColor = System::Drawing::Color::Black;
			this->label9->Location = System::Drawing::Point(2, 135);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(23, 13);
			this->label9->TabIndex = 38;
			this->label9->Text = L"R5";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label10->ForeColor = System::Drawing::Color::Black;
			this->label10->Location = System::Drawing::Point(2, 159);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(23, 13);
			this->label10->TabIndex = 39;
			this->label10->Text = L"R6";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label11->ForeColor = System::Drawing::Color::Black;
			this->label11->Location = System::Drawing::Point(2, 183);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(23, 13);
			this->label11->TabIndex = 40;
			this->label11->Text = L"R7";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label12->ForeColor = System::Drawing::Color::Black;
			this->label12->Location = System::Drawing::Point(2, 207);
			this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(23, 13);
			this->label12->TabIndex = 41;
			this->label12->Text = L"R8";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label13->ForeColor = System::Drawing::Color::Black;
			this->label13->Location = System::Drawing::Point(2, 231);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(28, 13);
			this->label13->TabIndex = 42;
			this->label13->Text = L"AIR";
			// 
			// textBox32
			// 
			this->textBox32->BackColor = System::Drawing::SystemColors::Control;
			this->textBox32->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox32->ForeColor = System::Drawing::Color::Gold;
			this->textBox32->Location = System::Drawing::Point(162, 2);
			this->textBox32->Margin = System::Windows::Forms::Padding(2);
			this->textBox32->Multiline = true;
			this->textBox32->Name = L"textBox32";
			this->textBox32->Size = System::Drawing::Size(76, 35);
			this->textBox32->TabIndex = 45;
			this->textBox32->Text = L"Возможна эксплуатация";
			// 
			// textBox34
			// 
			this->textBox34->Location = System::Drawing::Point(82, 303);
			this->textBox34->Margin = System::Windows::Forms::Padding(2);
			this->textBox34->Name = L"textBox34";
			this->textBox34->Size = System::Drawing::Size(76, 20);
			this->textBox34->TabIndex = 47;
			this->textBox34->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox35
			// 
			this->textBox35->Location = System::Drawing::Point(162, 303);
			this->textBox35->Margin = System::Windows::Forms::Padding(2);
			this->textBox35->Name = L"textBox35";
			this->textBox35->Size = System::Drawing::Size(76, 20);
			this->textBox35->TabIndex = 48;
			this->textBox35->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox36
			// 
			this->textBox36->Location = System::Drawing::Point(242, 303);
			this->textBox36->Margin = System::Windows::Forms::Padding(2);
			this->textBox36->Name = L"textBox36";
			this->textBox36->Size = System::Drawing::Size(76, 20);
			this->textBox36->TabIndex = 49;
			this->textBox36->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// textBox31
			// 
			this->textBox31->Location = System::Drawing::Point(2, 303);
			this->textBox31->Margin = System::Windows::Forms::Padding(2);
			this->textBox31->Name = L"textBox31";
			this->textBox31->Size = System::Drawing::Size(76, 20);
			this->textBox31->TabIndex = 44;
			this->textBox31->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->ForeColor = System::Drawing::Color::ForestGreen;
			this->label1->Location = System::Drawing::Point(82, 0);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Норма";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label15->ForeColor = System::Drawing::Color::Black;
			this->label15->Location = System::Drawing::Point(2, 279);
			this->label15->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(48, 13);
			this->label15->TabIndex = 50;
			this->label15->Text = L"R1_Om";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label16->ForeColor = System::Drawing::Color::Black;
			this->label16->Location = System::Drawing::Point(82, 279);
			this->label16->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(48, 13);
			this->label16->TabIndex = 51;
			this->label16->Text = L"R2_Om";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label17->ForeColor = System::Drawing::Color::Black;
			this->label17->Location = System::Drawing::Point(162, 279);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(48, 13);
			this->label17->TabIndex = 52;
			this->label17->Text = L"R3_Om";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label18->ForeColor = System::Drawing::Color::Black;
			this->label18->Location = System::Drawing::Point(242, 279);
			this->label18->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(48, 13);
			this->label18->TabIndex = 53;
			this->label18->Text = L"R4_Om";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label20->ForeColor = System::Drawing::Color::Black;
			this->label20->Location = System::Drawing::Point(2, 333);
			this->label20->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(48, 13);
			this->label20->TabIndex = 57;
			this->label20->Text = L"R5_Om";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label21->ForeColor = System::Drawing::Color::Black;
			this->label21->Location = System::Drawing::Point(82, 333);
			this->label21->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(48, 13);
			this->label21->TabIndex = 58;
			this->label21->Text = L"R6_Om";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label22->ForeColor = System::Drawing::Color::Black;
			this->label22->Location = System::Drawing::Point(162, 333);
			this->label22->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(48, 13);
			this->label22->TabIndex = 59;
			this->label22->Text = L"R7_Om";
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label23->ForeColor = System::Drawing::Color::Black;
			this->label23->Location = System::Drawing::Point(242, 333);
			this->label23->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(48, 13);
			this->label23->TabIndex = 60;
			this->label23->Text = L"R8_Om";
			// 
			// textBox37
			// 
			this->textBox37->Location = System::Drawing::Point(2, 354);
			this->textBox37->Margin = System::Windows::Forms::Padding(2);
			this->textBox37->Name = L"textBox37";
			this->textBox37->Size = System::Drawing::Size(76, 20);
			this->textBox37->TabIndex = 61;
			// 
			// textBox39
			// 
			this->textBox39->Location = System::Drawing::Point(162, 354);
			this->textBox39->Margin = System::Windows::Forms::Padding(2);
			this->textBox39->Name = L"textBox39";
			this->textBox39->Size = System::Drawing::Size(76, 20);
			this->textBox39->TabIndex = 63;
			// 
			// textBox40
			// 
			this->textBox40->Location = System::Drawing::Point(242, 354);
			this->textBox40->Margin = System::Windows::Forms::Padding(2);
			this->textBox40->Name = L"textBox40";
			this->textBox40->Size = System::Drawing::Size(76, 20);
			this->textBox40->TabIndex = 64;
			// 
			// textBox38
			// 
			this->textBox38->Location = System::Drawing::Point(82, 354);
			this->textBox38->Margin = System::Windows::Forms::Padding(2);
			this->textBox38->Name = L"textBox38";
			this->textBox38->Size = System::Drawing::Size(76, 20);
			this->textBox38->TabIndex = 62;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label19->ForeColor = System::Drawing::Color::Black;
			this->label19->Location = System::Drawing::Point(2, 392);
			this->label19->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(42, 13);
			this->label19->TabIndex = 55;
			this->label19->Text = L"loop_r";
			// 
			// textBox41
			// 
			this->textBox41->Location = System::Drawing::Point(82, 394);
			this->textBox41->Margin = System::Windows::Forms::Padding(2);
			this->textBox41->Name = L"textBox41";
			this->textBox41->Size = System::Drawing::Size(76, 20);
			this->textBox41->TabIndex = 54;
			this->textBox41->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm1::digit1);
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label25->ForeColor = System::Drawing::Color::Black;
			this->label25->Location = System::Drawing::Point(162, 392);
			this->label25->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(76, 13);
			this->label25->TabIndex = 65;
			this->label25->Text = L"усреднение";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->ForeColor = System::Drawing::Color::Red;
			this->label3->Location = System::Drawing::Point(2, 418);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(0, 13);
			this->label3->TabIndex = 5;
			// 
			// textBox42
			// 
			this->textBox42->Location = System::Drawing::Point(242, 394);
			this->textBox42->Margin = System::Windows::Forms::Padding(2);
			this->textBox42->Name = L"textBox42";
			this->textBox42->Size = System::Drawing::Size(76, 20);
			this->textBox42->TabIndex = 66;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(245, 565);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(96, 19);
			this->button1->TabIndex = 1;
			this->button1->Text = L"УСТАНОВИТЬ";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm1::set_param);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(8, 519);
			this->comboBox1->Margin = System::Windows::Forms::Padding(2);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(92, 21);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->Text = L"COM_PORT";
			this->comboBox1->DropDown += gcnew System::EventHandler(this, &MyForm1::Com_port_Loop_chose);
			// 
			// checkBox_express
			// 
			this->checkBox_express->AutoSize = true;
			this->checkBox_express->Location = System::Drawing::Point(72, 464);
			this->checkBox_express->Name = L"checkBox_express";
			this->checkBox_express->Size = System::Drawing::Size(77, 17);
			this->checkBox_express->TabIndex = 62;
			this->checkBox_express->Text = L"экспресс ";
			this->checkBox_express->UseVisualStyleBackColor = true;
			// 
			// checkBox_Ro
			// 
			this->checkBox_Ro->AutoSize = true;
			this->checkBox_Ro->Location = System::Drawing::Point(150, 464);
			this->checkBox_Ro->Name = L"checkBox_Ro";
			this->checkBox_Ro->Size = System::Drawing::Size(93, 17);
			this->checkBox_Ro->TabIndex = 63;
			this->checkBox_Ro->Text = L"УЭС->Рапорт";
			this->checkBox_Ro->UseVisualStyleBackColor = true;
			// 
			// checkBox_PH
			// 
			this->checkBox_PH->AutoSize = true;
			this->checkBox_PH->Location = System::Drawing::Point(245, 464);
			this->checkBox_PH->Name = L"checkBox_PH";
			this->checkBox_PH->Size = System::Drawing::Size(100, 17);
			this->checkBox_PH->TabIndex = 64;
			this->checkBox_PH->Text = L"Фаза->Рапорт";
			this->checkBox_PH->UseVisualStyleBackColor = true;
			// 
			// comboBox3
			// 
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Location = System::Drawing::Point(114, 519);
			this->comboBox3->Margin = System::Windows::Forms::Padding(2);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(92, 21);
			this->comboBox3->TabIndex = 65;
			this->comboBox3->Text = L"COM_PORT";
			this->comboBox3->DropDown += gcnew System::EventHandler(this, &MyForm1::Com_port_DDS_chose);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(14, 501);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(63, 13);
			this->label2->TabIndex = 66;
			this->label2->Text = L"LOOP COM";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(122, 501);
			this->label24->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(57, 13);
			this->label24->TabIndex = 67;
			this->label24->Text = L"DDS COM";
			// 
			// checkBox_Am_Tx
			// 
			this->checkBox_Am_Tx->AutoSize = true;
			this->checkBox_Am_Tx->Location = System::Drawing::Point(8, 464);
			this->checkBox_Am_Tx->Name = L"checkBox_Am_Tx";
			this->checkBox_Am_Tx->Size = System::Drawing::Size(60, 17);
			this->checkBox_Am_Tx->TabIndex = 68;
			this->checkBox_Am_Tx->Text = L"AM_Tx";
			this->checkBox_Am_Tx->UseVisualStyleBackColor = true;
			// 
			// comboBox_Ro_calk
			// 
			this->comboBox_Ro_calk->FormattingEnabled = true;
			this->comboBox_Ro_calk->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Расчет фаз и УЭС на компьютере", L"Расчет фаз и УЭС прибором" });
			this->comboBox_Ro_calk->Location = System::Drawing::Point(9, 428);
			this->comboBox_Ro_calk->Margin = System::Windows::Forms::Padding(2);
			this->comboBox_Ro_calk->Name = L"comboBox_Ro_calk";
			this->comboBox_Ro_calk->Size = System::Drawing::Size(227, 21);
			this->comboBox_Ro_calk->TabIndex = 69;
			this->comboBox_Ro_calk->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm1::comboBox_Ro_calk_SelectedIndexChanged);
			// 
			// checkBox_smt
			// 
			this->checkBox_smt->AutoSize = true;
			this->checkBox_smt->Location = System::Drawing::Point(258, 430);
			this->checkBox_smt->Name = L"checkBox_smt";
			this->checkBox_smt->Size = System::Drawing::Size(72, 17);
			this->checkBox_smt->TabIndex = 70;
			this->checkBox_smt->Text = L"Симметр";
			this->checkBox_smt->UseVisualStyleBackColor = true;
			// 
			// comboBox_LWD_COM
			// 
			this->comboBox_LWD_COM->FormattingEnabled = true;
			this->comboBox_LWD_COM->Location = System::Drawing::Point(223, 519);
			this->comboBox_LWD_COM->Margin = System::Windows::Forms::Padding(2);
			this->comboBox_LWD_COM->Name = L"comboBox_LWD_COM";
			this->comboBox_LWD_COM->Size = System::Drawing::Size(92, 21);
			this->comboBox_LWD_COM->TabIndex = 71;
			this->comboBox_LWD_COM->Text = L"COM_PORT";
			this->comboBox_LWD_COM->DropDown += gcnew System::EventHandler(this, &MyForm1::Com_port_LWD_COM_chose);
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(233, 501);
			this->label26->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(59, 13);
			this->label26->TabIndex = 72;
			this->label26->Text = L"LWD COM";
			// 
			// checkBox_re_from_loop
			// 
			this->checkBox_re_from_loop->AutoSize = true;
			this->checkBox_re_from_loop->Location = System::Drawing::Point(8, 566);
			this->checkBox_re_from_loop->Name = L"checkBox_re_from_loop";
			this->checkBox_re_from_loop->Size = System::Drawing::Size(92, 17);
			this->checkBox_re_from_loop->TabIndex = 73;
			this->checkBox_re_from_loop->Text = L"Re_from_loop";
			this->checkBox_re_from_loop->UseVisualStyleBackColor = true;
			// 
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(355, 595);
			this->Controls->Add(this->checkBox_re_from_loop);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->comboBox_LWD_COM);
			this->Controls->Add(this->checkBox_smt);
			this->Controls->Add(this->comboBox_Ro_calk);
			this->Controls->Add(this->checkBox_Am_Tx);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox3);
			this->Controls->Add(this->checkBox_PH);
			this->Controls->Add(this->checkBox_Ro);
			this->Controls->Add(this->checkBox_express);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"MyForm1";
			this->Text = L"ПАРАМЕТРЫ";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm1::MyForm1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm1::MyForm1_Load);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		const char* ConfName = "Config.txt";
		// фильтр только для цифр при вводе в текстбокс
private: System::Void digit(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
}

private: System::Void set_param(System::Object^  sender, System::EventArgs^  e) {
	extern int norm[10];
	extern int medium[10];
	extern int out_of_norm[10];
	extern double R_loop_physical[9];
	extern double loop_radius;
	extern string Metrology_Name;
	extern string Com_Name;
	extern string Com1_Name;
	extern string Com_LWD_Name;
	extern bool express;
	extern bool Ro_to_raport;
	extern bool PH_to_raport;
	extern bool mode;
	extern int N_aver;
	extern bool AM_TX;
	extern bool sonde_ro_calk;
	extern bool comp_smt;
	extern bool param_form_closed;
	
	norm[0];
	medium[0];
	out_of_norm[0];

#pragma region
	if (textBox1->Text != "") norm[0] = System::Int64::Parse(textBox1->Text);
	if (textBox2->Text != "") medium[0] = System::Int64::Parse(textBox2->Text);
	if (textBox3->Text != "")out_of_norm[0] = System::Int64::Parse(textBox3->Text);

	if (textBox4->Text != "") norm[1] = System::Int64::Parse(textBox4->Text);
	if (textBox5->Text != "") medium[1] = System::Int64::Parse(textBox5->Text);
	if (textBox6->Text != "") out_of_norm[1] = System::Int64::Parse(textBox6->Text);

	if (textBox7->Text != "") norm[2] = System::Int64::Parse(textBox7->Text);
	if (textBox8->Text != "") medium[2] = System::Int64::Parse(textBox8->Text);
	if (textBox9->Text != "") out_of_norm[2] = System::Int64::Parse(textBox9->Text);

	if (textBox10->Text != "") norm[3] = System::Int64::Parse(textBox10->Text);
	if (textBox11->Text != "") medium[3] = System::Int64::Parse(textBox11->Text);
	if (textBox12->Text != "") out_of_norm[3] = System::Int64::Parse(textBox12->Text);

	if (textBox13->Text != "") norm[4] = System::Int64::Parse(textBox13->Text);
	if (textBox14->Text != "") medium[4] = System::Int64::Parse(textBox14->Text);
	if (textBox15->Text != "") out_of_norm[4] = System::Int64::Parse(textBox15->Text);

	if (textBox16->Text != "") norm[5] = System::Int64::Parse(textBox16->Text);
	if (textBox17->Text != "") medium[5] = System::Int64::Parse(textBox17->Text);
	if (textBox18->Text != "") out_of_norm[5] = System::Int64::Parse(textBox18->Text);

	if (textBox19->Text != "") norm[6] = System::Int64::Parse(textBox19->Text);
	if (textBox20->Text != "") medium[6] = System::Int64::Parse(textBox20->Text);
	if (textBox21->Text != "") out_of_norm[6] = System::Int64::Parse(textBox21->Text);

	if (textBox22->Text != "") norm[7] = System::Int64::Parse(textBox22->Text);
	if (textBox23->Text != "") medium[7] = System::Int64::Parse(textBox23->Text);
	if (textBox24->Text != "") out_of_norm[7] = System::Int64::Parse(textBox24->Text);

	if (textBox25->Text != "") norm[8] = System::Int64::Parse(textBox25->Text);
	if (textBox26->Text != "") medium[8] = System::Int64::Parse(textBox26->Text);
	if (textBox27->Text != "") out_of_norm[8] = System::Int64::Parse(textBox27->Text);

	if (textBox28->Text != "") norm[9] = System::Int64::Parse(textBox28->Text);
	if (textBox29->Text != "") medium[9] = System::Int64::Parse(textBox29->Text);
	if (textBox30->Text != "") out_of_norm[9] = System::Int64::Parse(textBox30->Text);


	if (textBox31->Text != "") R_loop_physical[0] = System::Double::Parse(textBox31->Text);
	if (textBox34->Text != "") R_loop_physical[1] = System::Double::Parse(textBox34->Text);
	if (textBox35->Text != "") R_loop_physical[2] = System::Double::Parse(textBox35->Text);
	if (textBox36->Text != "") R_loop_physical[3] = System::Double::Parse(textBox36->Text);
	if (textBox37->Text != "") R_loop_physical[4] = System::Double::Parse(textBox37->Text);
	if (textBox38->Text != "") R_loop_physical[5] = System::Double::Parse(textBox38->Text);
	if (textBox39->Text != "") R_loop_physical[6] = System::Double::Parse(textBox39->Text);
	if (textBox40->Text != "") R_loop_physical[7] = System::Double::Parse(textBox40->Text);
	if (textBox41->Text != "") loop_radius = System::Double::Parse(textBox41->Text);
	if (textBox42->Text != "") N_aver = System::Int64::Parse(textBox42->Text);

	express = checkBox_express->Checked;
	Ro_to_raport = checkBox_Ro->Checked;
	PH_to_raport = checkBox_PH->Checked;
    AM_TX = checkBox_Am_Tx->Checked;
	comp_smt = checkBox_smt->Checked ;
	mode = checkBox_re_from_loop->Checked;
	sonde_ro_calk = comboBox_Ro_calk->SelectedIndex;
	param_form_closed = 1;
	//cout << "sonde_ro_calk " << sonde_ro_calk << endl;
#pragma endregion

	ofstream conf_out;
	conf_out.open(ConfName, ios::out);
	
	for (int i = 0; i < 10; i++) {
		conf_out << norm[i] << " " << medium[i] << " " << out_of_norm[i] << endl;
	}
	for (int i = 0; i < 8; i++) {
		conf_out << R_loop_physical[i] << " ";
	}

	conf_out << endl;
	conf_out << loop_radius << endl;
	conf_out <<  Metrology_Name << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox1->Text).ToPointer() << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox3->Text).ToPointer() << endl;

	conf_out << checkBox_express->Checked << endl;
	conf_out << checkBox_Ro->Checked << endl;
	conf_out << checkBox_PH->Checked << endl;
	conf_out << N_aver << endl;
	conf_out << AM_TX << endl;
	conf_out << sonde_ro_calk << endl;
	conf_out << comp_smt << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox_LWD_COM->Text).ToPointer() << endl;
	conf_out << checkBox_re_from_loop->Checked << endl;
	conf_out.close();
	Com_Name = (char*)Marshal::StringToHGlobalAnsi(comboBox1->Text).ToPointer() ;
	Com1_Name = (char*)Marshal::StringToHGlobalAnsi(comboBox3->Text).ToPointer();
	Com_LWD_Name = (char*)Marshal::StringToHGlobalAnsi(comboBox_LWD_COM->Text).ToPointer();
	conf_out.open("serial_num.ini", ios::out);
	conf_out << "serial_num = " << Com_LWD_Name.substr(3, 2) ;
	conf_out.close();
	//cout << (Com_Name)<<endl;
	//cout << (Com1_Name) << endl;
	MyForm1::Close();

}

private: System::Void Com_port_Loop_chose(System::Object^  sender, System::EventArgs^  e) {
	comboBox1->Items->Clear();
	cli::array<String^>^ ports = nullptr;
	ports = serialPort1->GetPortNames();
	comboBox1->Items->AddRange(ports);
}

private: System::Void Com_port_DDS_chose(System::Object^  sender, System::EventArgs^  e) {
	comboBox3->Items->Clear();
	cli::array<String^>^ ports = nullptr;
	ports = serialPort1->GetPortNames();
	comboBox3->Items->AddRange(ports);
}

private: System::Void Com_port_LWD_COM_chose(System::Object^  sender, System::EventArgs^  e) {
	comboBox_LWD_COM->Items->Clear();
	cli::array<String^>^ ports = nullptr;
	ports = serialPort1->GetPortNames();
	comboBox_LWD_COM->Items->AddRange(ports);
}

private: System::Void digit1(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
	if ((e->KeyChar) == ',') e->Handled = false;
}

private: System::Void comboBox_Ro_calk_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	extern METROLOGY metrology_struct;
	if (comboBox_Ro_calk->SelectedIndex == 0) {
	    if ((metrology_struct.serial & (1 << 31)) != 0) {
		    comboBox_Ro_calk->SelectedIndex = 1;
		    MessageBox::Show("верните файл метрологии");
	    }
		else checkBox_smt->Visible = true;
	}
	else checkBox_smt->Visible = false;
}

private: System::Void MyForm1_Load(System::Object^  sender, System::EventArgs^  e) {
	//extern bool stop_working;
	//stop_working = true;
}

private: System::Void MyForm1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	//extern bool stop_working;
	//stop_working = false;
}


};
}
