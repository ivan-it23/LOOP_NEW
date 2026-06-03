#pragma once
#pragma once
#include "windows.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex> 
#include <string> 
#include "function.h"
#include "variable.h"
#include <stdio.h>
#include <time.h>
#include "MyForm1.h"
#include "Help_form.h"
#include "MetroForm.h"
#include <typeinfo>
#include <winuser.h>
#include <locale.h>
#include <urlmon.h>
#include <shellapi.h>
#include "ShellAPI.h"
#include <msclr/marshal_cppstd.h>
#include "wininet.h"
#include <algorithm>
#pragma comment (lib, "urlmon.lib")
#pragma comment (lib, "Shell32.lib")
#pragma comment (lib,"Wininet.lib")

extern void *ComPort;
namespace LOOP {

	using namespace System;
	using namespace System::Timers;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Windows::Forms::DataVisualization::Charting;
	using namespace System::Threading;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Collections::Generic;

	using namespace System::Drawing::Imaging;


	using namespace std;
	using namespace System::IO::Ports;
	using namespace System::Windows;

	
	/// <summary>
	/// Сводка для WorkForm
	/// </summary>
	public ref class WorkForm : public System::Windows::Forms::Form
	{
	public:
	 System::Timers::Timer^ Sonde_Read_Timer;
	private: System::Windows::Forms::Button^  button_R1;
	private: System::Windows::Forms::Button^  button_AIR;
	private: System::Windows::Forms::Button^  button_R8;
	private: System::Windows::Forms::Button^  button_R7;
	private: System::Windows::Forms::Button^  button_R6;
	private: System::Windows::Forms::Button^  button_R5;
	private: System::Windows::Forms::Button^  button_R4;
	private: System::Windows::Forms::Button^  button_R2;
	private: System::Windows::Forms::Button^  button_R3;
	private: System::Windows::Forms::Label^  labelT1;
	private: System::Windows::Forms::Label^  labelT7;
	private: System::Windows::Forms::Label^  labelT6;



	private: System::Windows::Forms::Label^  labelT5;

	private: System::Windows::Forms::Label^  labelT4;

	private: System::Windows::Forms::Label^  labelT3;

	private: System::Windows::Forms::Label^  labelT2;
	private: System::Windows::Forms::Label^  labelT10;
	private: System::Windows::Forms::Label^  labelT9;





	private: System::Windows::Forms::Label^  labelT8;

	private: System::Windows::Forms::TextBox^  textBox_R1_T2;



	private: System::Windows::Forms::TextBox^  textBox_R1_T1;
	private: System::Windows::Forms::TextBox^  textBox_R1_T3;
	private: System::Windows::Forms::TextBox^  textBox_R1_T4;
	private: System::Windows::Forms::TextBox^  textBox_R1_T5;
	private: System::Windows::Forms::TextBox^  textBox_R1_T6;
	private: System::Windows::Forms::TextBox^  textBox_R1_T7;
	private: System::Windows::Forms::TextBox^  textBox_R1_T8;
	private: System::Windows::Forms::TextBox^  textBox_R1_T9;
	private: System::Windows::Forms::TextBox^  textBox_R1_T10;









	private: System::Windows::Forms::Button^  button_auto;
	private: System::Windows::Forms::Button^  button_local_metro;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;



	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  textBox_L1_1;
	private: System::Windows::Forms::TextBox^  textBox_L1_2;
	private: System::Windows::Forms::TextBox^  textBox_L1_3;
	private: System::Windows::Forms::TextBox^  textBox_L1_4;
	private: System::Windows::Forms::TextBox^  textBox_L1_5;
	private: System::Windows::Forms::TextBox^  textBox_L1_6;
	private: System::Windows::Forms::TextBox^  textBox_L1_7;
	private: System::Windows::Forms::TextBox^  textBox_L1_8;
	private: System::Windows::Forms::TextBox^  textBox_L2_1;
	private: System::Windows::Forms::TextBox^  textBox_L2_2;
	private: System::Windows::Forms::TextBox^  textBox_L2_3;
	private: System::Windows::Forms::TextBox^  textBox_L2_4;
	private: System::Windows::Forms::TextBox^  textBox_L2_5;
	private: System::Windows::Forms::TextBox^  textBox_L2_6;
	private: System::Windows::Forms::TextBox^  textBox_L2_7;
	private: System::Windows::Forms::TextBox^  textBox_L2_8;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  textBox_f_1;
	private: System::Windows::Forms::TextBox^  textBox_f_2;
	private: System::Windows::Forms::TextBox^  textBox_f_3;
	private: System::Windows::Forms::TextBox^  textBox_f_4;
	private: System::Windows::Forms::TextBox^  textBox_f_5;
	private: System::Windows::Forms::TextBox^  textBox_f_6;
	private: System::Windows::Forms::TextBox^  textBox_f_7;
	private: System::Windows::Forms::TextBox^  textBox_f_8;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::TextBox^  textBox_С1;
	private: System::Windows::Forms::TextBox^  textBox_С2;
	private: System::Windows::Forms::TextBox^  textBox_С3;
	private: System::Windows::Forms::TextBox^  textBox_С4;
	private: System::Windows::Forms::TextBox^  textBox_С5;
	private: System::Windows::Forms::TextBox^  textBox_С6;
	private: System::Windows::Forms::TextBox^  textBox_С7;
	private: System::Windows::Forms::TextBox^  textBox_С8;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc1;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc2;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc3;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc4;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc5;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc6;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc7;
	private: System::Windows::Forms::TextBox^  textBox_Z_calc8;
	private: System::Windows::Forms::Label^  label12;

	private: System::Windows::Forms::Button^  button_Z1;
	private: System::Windows::Forms::Button^  button_Z2;
	private: System::Windows::Forms::Button^  button_Z3;
	private: System::Windows::Forms::Button^  button_Z4;
	private: System::Windows::Forms::Button^  button_Z5;
	private: System::Windows::Forms::Button^  button_Z6;
	private: System::Windows::Forms::Button^  button_Z7;
	private: System::Windows::Forms::Button^  button_Z8;














private: System::Windows::Forms::TextBox^  textBox_L1_9;
private: System::Windows::Forms::TextBox^  textBox_L1_10;
private: System::Windows::Forms::Button^  button_Z10;
private: System::Windows::Forms::Button^  button_Z9;




private: System::Windows::Forms::TextBox^  textBox_Z_calc10;
private: System::Windows::Forms::TextBox^  textBox_Z_calc9;
private: System::Windows::Forms::TextBox^  textBox_С10;
private: System::Windows::Forms::TextBox^  textBox_С9;

private: System::Windows::Forms::TextBox^  textBox_f_10;
private: System::Windows::Forms::TextBox^  textBox_f_9;
private: System::Windows::Forms::TextBox^  textBox_L2_10;
private: System::Windows::Forms::TextBox^  textBox_L2_9;
private: System::Windows::Forms::Button^  button_read_loop_Z;
private: System::Windows::Forms::Panel^  panel2;
private: System::Windows::Forms::Button^  button_calc_z_to_loop;
private: System::Windows::Forms::Button^  button_write_Z;
private: System::Windows::Forms::Button^  button_erase;
private: System::Windows::Forms::Button^  button_signature;
private: System::Windows::Forms::TextBox^  textBox_signature;
private: System::Windows::Forms::Button^  button_calk_C_need;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel11;
private: System::Windows::Forms::TextBox^  textBox_Z_loop10;
private: System::Windows::Forms::TextBox^  textBox_Z_loop9;
private: System::Windows::Forms::TextBox^  textBox_Z_loop8;
private: System::Windows::Forms::TextBox^  textBox_Z_loop7;
private: System::Windows::Forms::TextBox^  textBox_Z_loop1;
private: System::Windows::Forms::TextBox^  textBox_Z_loop6;
private: System::Windows::Forms::TextBox^  textBox_Z_loop2;
private: System::Windows::Forms::TextBox^  textBox_Z_loop3;
private: System::Windows::Forms::TextBox^  textBox_Z_loop4;
private: System::Windows::Forms::TextBox^  textBox_Z_loop5;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::TextBox^  textBox_R_loop2;
private: System::Windows::Forms::TextBox^  textBox_R_loop1;
private: System::Windows::Forms::TextBox^  textBox_R_loop3;
private: System::Windows::Forms::TextBox^  textBox_R_loop4;
private: System::Windows::Forms::TextBox^  textBox_R_loop5;
private: System::Windows::Forms::TextBox^  textBox_R_loop6;
private: System::Windows::Forms::TextBox^  textBox_R_loop7;
private: System::Windows::Forms::TextBox^  textBox_R_loop8;
private: System::Windows::Forms::TextBox^  textBox_R_loop9;
private: System::Windows::Forms::TextBox^  textBox_R_loop10;
private: System::Windows::Forms::CheckBox^  checkBox_Re_red;
private: System::Windows::Forms::CheckBox^  checkBox_R_red;

private: System::Windows::Forms::CheckBox^  checkBox_param_red;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::TextBox^  textBox_radius;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::TextBox^  textBox_R_wire;
private: System::Windows::Forms::Button^  button_calk;
private: System::Windows::Forms::Timer^  timer1;
private: System::Windows::Forms::TextBox^  textBox_R3_T9;
private: System::Windows::Forms::TextBox^  textBox_R3_T8;
private: System::Windows::Forms::TextBox^  textBox_R3_T7;
private: System::Windows::Forms::TextBox^  textBox_R3_T6;
private: System::Windows::Forms::TextBox^  textBox_R3_T5;
private: System::Windows::Forms::TextBox^  textBox_R3_T4;
private: System::Windows::Forms::TextBox^  textBox_R3_T3;
private: System::Windows::Forms::TextBox^  textBox_R3_T2;
private: System::Windows::Forms::TextBox^  textBox_R3_T1;
private: System::Windows::Forms::TextBox^  textBox_R2_T10;
private: System::Windows::Forms::TextBox^  textBox_R2_T9;
private: System::Windows::Forms::TextBox^  textBox_R2_T8;
private: System::Windows::Forms::TextBox^  textBox_R2_T7;
private: System::Windows::Forms::TextBox^  textBox_R2_T6;
private: System::Windows::Forms::TextBox^  textBox_R2_T5;
private: System::Windows::Forms::TextBox^  textBox_R2_T4;
private: System::Windows::Forms::TextBox^  textBox_R2_T3;
private: System::Windows::Forms::TextBox^  textBox_R2_T2;
private: System::Windows::Forms::TextBox^  textBox_R2_T1;
private: System::Windows::Forms::TextBox^  textBox_R3_T10;
private: System::Windows::Forms::TextBox^  textBox_R6_T9;
private: System::Windows::Forms::TextBox^  textBox_R6_T8;
private: System::Windows::Forms::TextBox^  textBox_R6_T7;
private: System::Windows::Forms::TextBox^  textBox_R6_T6;
private: System::Windows::Forms::TextBox^  textBox_R6_T5;
private: System::Windows::Forms::TextBox^  textBox_R6_T4;
private: System::Windows::Forms::TextBox^  textBox_R6_T3;
private: System::Windows::Forms::TextBox^  textBox_R6_T2;
private: System::Windows::Forms::TextBox^  textBox_R6_T1;
private: System::Windows::Forms::TextBox^  textBox_R5_T10;
private: System::Windows::Forms::TextBox^  textBox_R5_T9;
private: System::Windows::Forms::TextBox^  textBox_R5_T8;
private: System::Windows::Forms::TextBox^  textBox_R5_T7;
private: System::Windows::Forms::TextBox^  textBox_R5_T6;
private: System::Windows::Forms::TextBox^  textBox_R5_T5;
private: System::Windows::Forms::TextBox^  textBox_R5_T4;
private: System::Windows::Forms::TextBox^  textBox_R5_T3;
private: System::Windows::Forms::TextBox^  textBox_R5_T2;
private: System::Windows::Forms::TextBox^  textBox_R5_T1;
private: System::Windows::Forms::TextBox^  textBox_R4_T10;
private: System::Windows::Forms::TextBox^  textBox_R4_T9;
private: System::Windows::Forms::TextBox^  textBox_R4_T8;
private: System::Windows::Forms::TextBox^  textBox_R4_T7;
private: System::Windows::Forms::TextBox^  textBox_R4_T5;
private: System::Windows::Forms::TextBox^  textBox_R4_T4;
private: System::Windows::Forms::TextBox^  textBox_R4_T3;
private: System::Windows::Forms::TextBox^  textBox_R4_T2;
private: System::Windows::Forms::TextBox^  textBox_R4_T1;
private: System::Windows::Forms::TextBox^  textBox_R4_T6;
private: System::Windows::Forms::TextBox^  textBox_R6_T10;
private: System::Windows::Forms::TextBox^  textBox_R8_T10;
private: System::Windows::Forms::TextBox^  textBox_R8_T9;
private: System::Windows::Forms::TextBox^  textBox_R8_T8;
private: System::Windows::Forms::TextBox^  textBox_R8_T7;
private: System::Windows::Forms::TextBox^  textBox_R8_T6;
private: System::Windows::Forms::TextBox^  textBox_R8_T4;
private: System::Windows::Forms::TextBox^  textBox_R8_T3;
private: System::Windows::Forms::TextBox^  textBox_R8_T2;
private: System::Windows::Forms::TextBox^  textBox_R8_T1;
private: System::Windows::Forms::TextBox^  textBox_R7_T10;
private: System::Windows::Forms::TextBox^  textBox_R7_T9;

private: System::Windows::Forms::TextBox^  textBox_R7_T8;
private: System::Windows::Forms::TextBox^  textBox_R7_T7;
private: System::Windows::Forms::TextBox^  textBox_R7_T6;
private: System::Windows::Forms::TextBox^  textBox_R7_T4;
private: System::Windows::Forms::TextBox^  textBox_R7_T3;
private: System::Windows::Forms::TextBox^  textBox_R7_T1;
private: System::Windows::Forms::TextBox^  textBox_R7_T5;
private: System::Windows::Forms::TextBox^  textBox_R7_T2;
private: System::Windows::Forms::TextBox^  textBox_R8_T5;
private: System::Windows::Forms::ComboBox^  comboBox_sonde_com;
private: System::Windows::Forms::ComboBox^  comboBox_loop_com;

private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::ComboBox^  comboBox_aver;


private:

private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::CheckBox^  checkBox_express;
private: System::Windows::Forms::CheckBox^  checkBox_R8;
private: System::Windows::Forms::CheckBox^  checkBox_R7;
private: System::Windows::Forms::CheckBox^  checkBox_R6;
private: System::Windows::Forms::CheckBox^  checkBox_R5;
private: System::Windows::Forms::CheckBox^  checkBox_R4;
private: System::Windows::Forms::CheckBox^  checkBox_R3;
private: System::Windows::Forms::CheckBox^  checkBox_R2;
private: System::Windows::Forms::CheckBox^  checkBox_R1;
private: System::Windows::Forms::MenuStrip^  menuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  сохранитьРапортRTFToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  uploadToFTPToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  работаСФайломМетрологииToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  пОМОЩЬToolStripMenuItem;
private: System::Windows::Forms::RichTextBox^  richTextBox1;
private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;

private: System::Windows::Forms::Button^  button_raport;
private: System::Windows::Forms::PictureBox^  pictureBox_legend_up;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::ComboBox^  comboBox_trashhold;

private:
private: System::Windows::Forms::Label^  label9;

private: System::Windows::Forms::ComboBox^  comboBox_tool_type;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::ToolTip^  toolTip1;
internal:









	public:

	public:
		System::Timers::Timer^ Loop_Check_Timer;

	WorkForm(void)
		{
		    extern bool express;
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			
			Sonde_Read_Thread->Start();
			Loop_Check_Thread->Start();
			Loop_Work_Thread->Start();
			
			checkBox_Re_red->Checked = true;
			checkBox_R_red->Checked = true;
			checkBox_param_red->Checked = false;



			string sonde_com_Name = "";
			string loop_com_Name = "";
			string N_averaging_str = "";
			string trashhold_str = "";
			string comboBox_tool_type_str = "";
			ifstream conf_in;
			conf_in.open(ConfName, ios::in);
			conf_in >> sonde_com_Name;
			conf_in >> loop_com_Name;
			conf_in >> N_averaging_str;
			conf_in >> trashhold_str;
			conf_in >> express;
			conf_in >> comboBox_tool_type_str;
			conf_in.close();
			comboBox_sonde_com->Text = gcnew String(sonde_com_Name.c_str());
			comboBox_loop_com->Text = gcnew String(loop_com_Name.c_str());
			comboBox_aver->Text = gcnew String(N_averaging_str.c_str());
			comboBox_trashhold->Text = gcnew String(trashhold_str.c_str());
			comboBox_tool_type->Text = gcnew String(comboBox_tool_type_str.c_str());
			if (comboBox_aver->Text != "") N_averaging = Int32::Parse(comboBox_aver->Text);
			checkBox_express->Checked = express;
			
			//comboBox_tool_type->SelectedIndex = 2;//AUTONOM
			//comboBox_tool_type->SelectedIndex = 0;//LWD

			for (int N_sonde = 1; N_sonde <= 10; N_sonde++) {
				Label^ LABEL_T = (Label^)this->tableLayoutPanel1->Controls["labelT" + (N_sonde).ToString()];
				LABEL_T->Text = "Tx " ;
			}
			

			//do_legend(trashhold, 1);
			//int X[20] = { 0, };
			//do_graf(1, X, 1, trashhold);
	}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~WorkForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	protected:

	private: System::Windows::Forms::Button^  button_ON_OFF;
	private: System::IO::Ports::SerialPort^  serialPort_sonde;

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf1;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf10;

	private: System::Windows::Forms::PictureBox^  pictureBox_graf2;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf3;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf4;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf5;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf6;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf7;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf8;
	private: System::Windows::Forms::PictureBox^  pictureBox_graf9;

	private: System::Windows::Forms::TextBox^  textBox_dfi1;
	private: System::Windows::Forms::TextBox^  textBox_dfi2;
	private: System::Windows::Forms::TextBox^  textBox_dfi3;
	private: System::Windows::Forms::TextBox^  textBox_dfi4;
	private: System::Windows::Forms::TextBox^  textBox_dfi5;
	private: System::Windows::Forms::TextBox^  textBox_dfi6;
	private: System::Windows::Forms::TextBox^  textBox_dfi7;
	private: System::Windows::Forms::TextBox^  textBox_dfi8;
	private: System::Windows::Forms::TextBox^  textBox_dfi9;
	private: System::Windows::Forms::TextBox^  textBox_dfi10;
	private: System::Windows::Forms::TextBox^  textBox_ro1;
	private: System::Windows::Forms::TextBox^  textBox_ro2;
	private: System::Windows::Forms::TextBox^  textBox_ro3;
	private: System::Windows::Forms::TextBox^  textBox_ro4;
	private: System::Windows::Forms::TextBox^  textBox_ro5;
	private: System::Windows::Forms::TextBox^  textBox_ro6;
	private: System::Windows::Forms::TextBox^  textBox_ro7;
	private: System::Windows::Forms::TextBox^  textBox_ro8;
	private: System::Windows::Forms::TextBox^  textBox_ro9;
	private: System::Windows::Forms::TextBox^  textBox_ro10;




	private: System::Windows::Forms::Label^  label_ro;
	private: System::Windows::Forms::Button^  button_BT_ON;
	private: System::IO::Ports::SerialPort^  serialPort_loop;



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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button_auto = (gcnew System::Windows::Forms::Button());
			this->button_local_metro = (gcnew System::Windows::Forms::Button());
			this->button_BT_ON = (gcnew System::Windows::Forms::Button());
			this->button_ON_OFF = (gcnew System::Windows::Forms::Button());
			this->serialPort_sonde = (gcnew System::IO::Ports::SerialPort(this->components));
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->pictureBox_legend_up = (gcnew System::Windows::Forms::PictureBox());
			this->checkBox_R8 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R7 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R2 = (gcnew System::Windows::Forms::CheckBox());
			this->textBox_R8_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R5_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R2_T1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T1 = (gcnew System::Windows::Forms::TextBox());
			this->labelT10 = (gcnew System::Windows::Forms::Label());
			this->labelT9 = (gcnew System::Windows::Forms::Label());
			this->labelT8 = (gcnew System::Windows::Forms::Label());
			this->labelT7 = (gcnew System::Windows::Forms::Label());
			this->labelT6 = (gcnew System::Windows::Forms::Label());
			this->labelT5 = (gcnew System::Windows::Forms::Label());
			this->labelT4 = (gcnew System::Windows::Forms::Label());
			this->labelT3 = (gcnew System::Windows::Forms::Label());
			this->labelT2 = (gcnew System::Windows::Forms::Label());
			this->label_ro = (gcnew System::Windows::Forms::Label());
			this->textBox_ro1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ro10 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox_graf1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf4 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf6 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf8 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf9 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox_graf10 = (gcnew System::Windows::Forms::PictureBox());
			this->textBox_dfi1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T10 = (gcnew System::Windows::Forms::TextBox());
			this->button_R2 = (gcnew System::Windows::Forms::Button());
			this->button_R3 = (gcnew System::Windows::Forms::Button());
			this->button_R4 = (gcnew System::Windows::Forms::Button());
			this->button_R5 = (gcnew System::Windows::Forms::Button());
			this->button_R6 = (gcnew System::Windows::Forms::Button());
			this->button_R7 = (gcnew System::Windows::Forms::Button());
			this->button_R8 = (gcnew System::Windows::Forms::Button());
			this->button_AIR = (gcnew System::Windows::Forms::Button());
			this->button_R1 = (gcnew System::Windows::Forms::Button());
			this->textBox_dfi7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dfi8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R1_T7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R3_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R4_T6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R6_T10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R7_T2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R8_T5 = (gcnew System::Windows::Forms::TextBox());
			this->labelT1 = (gcnew System::Windows::Forms::Label());
			this->checkBox_R1 = (gcnew System::Windows::Forms::CheckBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->serialPort_loop = (gcnew System::IO::Ports::SerialPort(this->components));
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button_raport = (gcnew System::Windows::Forms::Button());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->checkBox_Re_red = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_R_red = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_param_red = (gcnew System::Windows::Forms::CheckBox());
			this->tableLayoutPanel11 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->textBox_R_loop2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_signature = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc10 = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->textBox_Z_calc9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop10 = (gcnew System::Windows::Forms::TextBox());
			this->button_Z1 = (gcnew System::Windows::Forms::Button());
			this->button_Z2 = (gcnew System::Windows::Forms::Button());
			this->button_Z3 = (gcnew System::Windows::Forms::Button());
			this->button_Z4 = (gcnew System::Windows::Forms::Button());
			this->button_Z5 = (gcnew System::Windows::Forms::Button());
			this->button_Z6 = (gcnew System::Windows::Forms::Button());
			this->button_Z7 = (gcnew System::Windows::Forms::Button());
			this->button_Z8 = (gcnew System::Windows::Forms::Button());
			this->button_Z9 = (gcnew System::Windows::Forms::Button());
			this->button_Z10 = (gcnew System::Windows::Forms::Button());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->textBox_R_loop3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_R_loop10 = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->textBox_radius = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->textBox_R_wire = (gcnew System::Windows::Forms::TextBox());
			this->button_calk_C_need = (gcnew System::Windows::Forms::Button());
			this->button_signature = (gcnew System::Windows::Forms::Button());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->button_read_loop_Z = (gcnew System::Windows::Forms::Button());
			this->button_calc_z_to_loop = (gcnew System::Windows::Forms::Button());
			this->button_write_Z = (gcnew System::Windows::Forms::Button());
			this->button_erase = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button_calk = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->comboBox_sonde_com = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox_loop_com = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_aver = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->checkBox_express = (gcnew System::Windows::Forms::CheckBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->сохранитьРапортRTFToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->uploadToFTPToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->работаСФайломМетрологииToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->пОМОЩЬToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->comboBox_trashhold = (gcnew System::Windows::Forms::ComboBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->comboBox_tool_type = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->panel1->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_legend_up))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf10))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tableLayoutPanel11->SuspendLayout();
			this->panel2->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->button_auto);
			this->panel1->Controls->Add(this->button_local_metro);
			this->panel1->Controls->Add(this->button_BT_ON);
			this->panel1->Controls->Add(this->button_ON_OFF);
			this->panel1->Location = System::Drawing::Point(1000, 42);
			this->panel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(532, 74);
			this->panel1->TabIndex = 0;
			// 
			// button_auto
			// 
			this->button_auto->Location = System::Drawing::Point(413, 2);
			this->button_auto->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_auto->Name = L"button_auto";
			this->button_auto->Size = System::Drawing::Size(107, 70);
			this->button_auto->TabIndex = 3;
			this->button_auto->Text = L"АВТО";
			this->toolTip1->SetToolTip(this->button_auto, L"Поочередно переключает сопротивления на кольце\r\n и заносит данные о поверка в рап"
				L"орт.\r\nПо окончании авто поверки можно заново произвести\r\n измерение для любого и"
				L"з сопротивлений.\r\n");
			this->button_auto->UseVisualStyleBackColor = true;
			this->button_auto->Click += gcnew System::EventHandler(this, &WorkForm::button_auto_Click);
			// 
			// button_local_metro
			// 
			this->button_local_metro->Location = System::Drawing::Point(275, 2);
			this->button_local_metro->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_local_metro->Name = L"button_local_metro";
			this->button_local_metro->Size = System::Drawing::Size(128, 70);
			this->button_local_metro->TabIndex = 2;
			this->button_local_metro->Text = L"ЛОКАЛЬНЫЙ ВОЗДУХ ";
			this->toolTip1->SetToolTip(this->button_local_metro, L"При нажатии измеряются и устанавливаются в\r\nпоправки на \"Воздух\" для данного мест"
				L"а . \r\n При повторном нажатии локальные поправки обнуляются.");
			this->button_local_metro->UseVisualStyleBackColor = true;
			this->button_local_metro->Click += gcnew System::EventHandler(this, &WorkForm::button_local_air_Click);
			// 
			// button_BT_ON
			// 
			this->button_BT_ON->Location = System::Drawing::Point(149, 2);
			this->button_BT_ON->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_BT_ON->Name = L"button_BT_ON";
			this->button_BT_ON->Size = System::Drawing::Size(116, 70);
			this->button_BT_ON->TabIndex = 1;
			this->button_BT_ON->Text = L"ВКЛЮЧИТЬ КОЛЬЦО";
			this->toolTip1->SetToolTip(this->button_BT_ON, L"Установка связи с иммитатором по Блютус или кабелю.\r\nПеред нажатием кнопки убедит"
				L"есь, что тумблер питания\r\nна иммитаторе включен и горит зеленый светодиод\r\n");
			this->button_BT_ON->UseVisualStyleBackColor = true;
			this->button_BT_ON->Click += gcnew System::EventHandler(this, &WorkForm::BT_ON);
			// 
			// button_ON_OFF
			// 
			this->button_ON_OFF->Location = System::Drawing::Point(3, 2);
			this->button_ON_OFF->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_ON_OFF->Name = L"button_ON_OFF";
			this->button_ON_OFF->Size = System::Drawing::Size(140, 70);
			this->button_ON_OFF->TabIndex = 0;
			this->button_ON_OFF->Text = L"ВКЛЮЧИТЬ ПРИБОР";
			this->toolTip1->SetToolTip(this->button_ON_OFF, L"Включение прибора. Перед нажатием кнопки\r\n подключите прибор ЭМК к компьютеру с п"
				L"омощью\r\nУСИ и включите ");
			this->button_ON_OFF->UseVisualStyleBackColor = true;
			this->button_ON_OFF->Click += gcnew System::EventHandler(this, &WorkForm::button_ON_OFF_Click);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tableLayoutPanel1->ColumnCount = 13;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				109)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				100)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				451)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				91)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				68)));
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_legend_up, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R8, 11, 11);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R7, 10, 11);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R6, 9, 11);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R5, 8, 11);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R4, 7, 11);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R3, 6, 11);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R2, 5, 11);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T10, 11, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T9, 11, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T8, 11, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T7, 11, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T6, 11, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T4, 11, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T3, 11, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T2, 11, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T1, 11, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T10, 10, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T9, 10, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T8, 10, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T7, 10, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T6, 10, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T4, 10, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T3, 10, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T1, 10, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T9, 9, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T8, 9, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T7, 9, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T6, 9, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T5, 9, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T4, 9, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T3, 9, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T2, 9, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T1, 9, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T10, 8, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T9, 8, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T8, 8, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T7, 8, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T6, 8, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T5, 8, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T4, 8, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T3, 8, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T2, 8, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R5_T1, 8, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T10, 7, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T9, 7, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T8, 7, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T7, 7, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T5, 7, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T4, 7, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T3, 7, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T2, 7, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T1, 7, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T9, 6, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T8, 6, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T7, 6, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T6, 6, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T5, 6, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T4, 6, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T3, 6, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T2, 6, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T1, 6, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T10, 5, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T9, 5, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T8, 5, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T7, 5, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T6, 5, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T5, 5, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T4, 5, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T3, 5, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T2, 5, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R2_T1, 5, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T3, 4, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T2, 4, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T1, 4, 1);
			this->tableLayoutPanel1->Controls->Add(this->labelT10, 0, 10);
			this->tableLayoutPanel1->Controls->Add(this->labelT9, 0, 9);
			this->tableLayoutPanel1->Controls->Add(this->labelT8, 0, 8);
			this->tableLayoutPanel1->Controls->Add(this->labelT7, 0, 7);
			this->tableLayoutPanel1->Controls->Add(this->labelT6, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->labelT5, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->labelT4, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->labelT3, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->labelT2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->label_ro, 3, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro1, 3, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro2, 3, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro3, 3, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro4, 3, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro5, 3, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro6, 3, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro7, 3, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro8, 3, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro9, 3, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_ro10, 3, 10);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf1, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf2, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf3, 2, 3);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf4, 2, 4);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf5, 2, 5);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf6, 2, 6);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf7, 2, 7);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf8, 2, 8);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf9, 2, 9);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox_graf10, 2, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi1, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi2, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi3, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi4, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi5, 1, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi6, 1, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi9, 1, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi10, 1, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T4, 4, 4);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T5, 4, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T6, 4, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T8, 4, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T9, 4, 9);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T10, 4, 10);
			this->tableLayoutPanel1->Controls->Add(this->button_R2, 5, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R3, 6, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R4, 7, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R5, 8, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R6, 9, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R7, 10, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R8, 11, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_AIR, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->button_R1, 4, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi7, 1, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_dfi8, 1, 8);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R1_T7, 4, 7);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R3_T10, 6, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R4_T6, 7, 6);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R6_T10, 9, 10);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T5, 10, 5);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R7_T2, 10, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_R8_T5, 11, 5);
			this->tableLayoutPanel1->Controls->Add(this->labelT1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->checkBox_R1, 4, 11);
			this->tableLayoutPanel1->Controls->Add(this->label5, 0, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(5, 63);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 12;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 8.333332F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(1496, 423);
			this->tableLayoutPanel1->TabIndex = 1;
			// 
			// pictureBox_legend_up
			// 
			this->pictureBox_legend_up->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_legend_up->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBox_legend_up->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_legend_up->Location = System::Drawing::Point(215, 3);
			this->pictureBox_legend_up->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_legend_up->Name = L"pictureBox_legend_up";
			this->pictureBox_legend_up->Size = System::Drawing::Size(445, 28);
			this->pictureBox_legend_up->TabIndex = 45;
			this->pictureBox_legend_up->TabStop = false;
			// 
			// checkBox_R8
			// 
			this->checkBox_R8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R8->AutoSize = true;
			this->checkBox_R8->Location = System::Drawing::Point(1403, 388);
			this->checkBox_R8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R8->Name = L"checkBox_R8";
			this->checkBox_R8->Size = System::Drawing::Size(18, 32);
			this->checkBox_R8->TabIndex = 44;
			this->checkBox_R8->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R8->UseVisualStyleBackColor = true;
			// 
			// checkBox_R7
			// 
			this->checkBox_R7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R7->AutoSize = true;
			this->checkBox_R7->Location = System::Drawing::Point(1311, 388);
			this->checkBox_R7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R7->Name = L"checkBox_R7";
			this->checkBox_R7->Size = System::Drawing::Size(18, 32);
			this->checkBox_R7->TabIndex = 44;
			this->checkBox_R7->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R7->UseVisualStyleBackColor = true;
			// 
			// checkBox_R6
			// 
			this->checkBox_R6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R6->AutoSize = true;
			this->checkBox_R6->Location = System::Drawing::Point(1219, 388);
			this->checkBox_R6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R6->Name = L"checkBox_R6";
			this->checkBox_R6->Size = System::Drawing::Size(18, 32);
			this->checkBox_R6->TabIndex = 44;
			this->checkBox_R6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R6->UseVisualStyleBackColor = true;
			// 
			// checkBox_R5
			// 
			this->checkBox_R5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R5->AutoSize = true;
			this->checkBox_R5->Location = System::Drawing::Point(1127, 388);
			this->checkBox_R5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R5->Name = L"checkBox_R5";
			this->checkBox_R5->Size = System::Drawing::Size(18, 32);
			this->checkBox_R5->TabIndex = 44;
			this->checkBox_R5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R5->UseVisualStyleBackColor = true;
			// 
			// checkBox_R4
			// 
			this->checkBox_R4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R4->AutoSize = true;
			this->checkBox_R4->Location = System::Drawing::Point(1035, 388);
			this->checkBox_R4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R4->Name = L"checkBox_R4";
			this->checkBox_R4->Size = System::Drawing::Size(18, 32);
			this->checkBox_R4->TabIndex = 44;
			this->checkBox_R4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R4->UseVisualStyleBackColor = true;
			// 
			// checkBox_R3
			// 
			this->checkBox_R3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R3->AutoSize = true;
			this->checkBox_R3->Location = System::Drawing::Point(943, 388);
			this->checkBox_R3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R3->Name = L"checkBox_R3";
			this->checkBox_R3->Size = System::Drawing::Size(18, 32);
			this->checkBox_R3->TabIndex = 44;
			this->checkBox_R3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R3->UseVisualStyleBackColor = true;
			// 
			// checkBox_R2
			// 
			this->checkBox_R2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R2->AutoSize = true;
			this->checkBox_R2->Location = System::Drawing::Point(851, 388);
			this->checkBox_R2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R2->Name = L"checkBox_R2";
			this->checkBox_R2->Size = System::Drawing::Size(18, 32);
			this->checkBox_R2->TabIndex = 44;
			this->checkBox_R2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R2->UseVisualStyleBackColor = true;
			// 
			// textBox_R8_T10
			// 
			this->textBox_R8_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T10->Location = System::Drawing::Point(1403, 353);
			this->textBox_R8_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T10->Name = L"textBox_R8_T10";
			this->textBox_R8_T10->ReadOnly = true;
			this->textBox_R8_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T10->TabIndex = 30;
			this->textBox_R8_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T9
			// 
			this->textBox_R8_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T9->Location = System::Drawing::Point(1403, 318);
			this->textBox_R8_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T9->Name = L"textBox_R8_T9";
			this->textBox_R8_T9->ReadOnly = true;
			this->textBox_R8_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T9->TabIndex = 30;
			this->textBox_R8_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T8
			// 
			this->textBox_R8_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T8->Location = System::Drawing::Point(1403, 283);
			this->textBox_R8_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T8->Name = L"textBox_R8_T8";
			this->textBox_R8_T8->ReadOnly = true;
			this->textBox_R8_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T8->TabIndex = 30;
			this->textBox_R8_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T7
			// 
			this->textBox_R8_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T7->Location = System::Drawing::Point(1403, 248);
			this->textBox_R8_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T7->Name = L"textBox_R8_T7";
			this->textBox_R8_T7->ReadOnly = true;
			this->textBox_R8_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T7->TabIndex = 30;
			this->textBox_R8_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T6
			// 
			this->textBox_R8_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T6->Location = System::Drawing::Point(1403, 213);
			this->textBox_R8_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T6->Name = L"textBox_R8_T6";
			this->textBox_R8_T6->ReadOnly = true;
			this->textBox_R8_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T6->TabIndex = 30;
			this->textBox_R8_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T4
			// 
			this->textBox_R8_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T4->Location = System::Drawing::Point(1403, 143);
			this->textBox_R8_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T4->Name = L"textBox_R8_T4";
			this->textBox_R8_T4->ReadOnly = true;
			this->textBox_R8_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T4->TabIndex = 30;
			this->textBox_R8_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T3
			// 
			this->textBox_R8_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T3->Location = System::Drawing::Point(1403, 108);
			this->textBox_R8_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T3->Name = L"textBox_R8_T3";
			this->textBox_R8_T3->ReadOnly = true;
			this->textBox_R8_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T3->TabIndex = 30;
			this->textBox_R8_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T2
			// 
			this->textBox_R8_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T2->Location = System::Drawing::Point(1403, 73);
			this->textBox_R8_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T2->Name = L"textBox_R8_T2";
			this->textBox_R8_T2->ReadOnly = true;
			this->textBox_R8_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T2->TabIndex = 30;
			this->textBox_R8_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T1
			// 
			this->textBox_R8_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T1->Location = System::Drawing::Point(1403, 38);
			this->textBox_R8_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T1->Name = L"textBox_R8_T1";
			this->textBox_R8_T1->ReadOnly = true;
			this->textBox_R8_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T1->TabIndex = 30;
			this->textBox_R8_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T10
			// 
			this->textBox_R7_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T10->Location = System::Drawing::Point(1311, 353);
			this->textBox_R7_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T10->Name = L"textBox_R7_T10";
			this->textBox_R7_T10->ReadOnly = true;
			this->textBox_R7_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T10->TabIndex = 29;
			this->textBox_R7_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T9
			// 
			this->textBox_R7_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T9->Location = System::Drawing::Point(1311, 318);
			this->textBox_R7_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T9->Name = L"textBox_R7_T9";
			this->textBox_R7_T9->ReadOnly = true;
			this->textBox_R7_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T9->TabIndex = 29;
			this->textBox_R7_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T8
			// 
			this->textBox_R7_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T8->Location = System::Drawing::Point(1311, 283);
			this->textBox_R7_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T8->Name = L"textBox_R7_T8";
			this->textBox_R7_T8->ReadOnly = true;
			this->textBox_R7_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T8->TabIndex = 29;
			this->textBox_R7_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T7
			// 
			this->textBox_R7_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T7->Location = System::Drawing::Point(1311, 248);
			this->textBox_R7_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T7->Name = L"textBox_R7_T7";
			this->textBox_R7_T7->ReadOnly = true;
			this->textBox_R7_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T7->TabIndex = 29;
			this->textBox_R7_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T6
			// 
			this->textBox_R7_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T6->Location = System::Drawing::Point(1311, 213);
			this->textBox_R7_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T6->Name = L"textBox_R7_T6";
			this->textBox_R7_T6->ReadOnly = true;
			this->textBox_R7_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T6->TabIndex = 29;
			this->textBox_R7_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T4
			// 
			this->textBox_R7_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T4->Location = System::Drawing::Point(1311, 143);
			this->textBox_R7_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T4->Name = L"textBox_R7_T4";
			this->textBox_R7_T4->ReadOnly = true;
			this->textBox_R7_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T4->TabIndex = 29;
			this->textBox_R7_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T3
			// 
			this->textBox_R7_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T3->Location = System::Drawing::Point(1311, 108);
			this->textBox_R7_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T3->Name = L"textBox_R7_T3";
			this->textBox_R7_T3->ReadOnly = true;
			this->textBox_R7_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T3->TabIndex = 29;
			this->textBox_R7_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T1
			// 
			this->textBox_R7_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T1->Location = System::Drawing::Point(1311, 38);
			this->textBox_R7_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T1->Name = L"textBox_R7_T1";
			this->textBox_R7_T1->ReadOnly = true;
			this->textBox_R7_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T1->TabIndex = 29;
			this->textBox_R7_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T9
			// 
			this->textBox_R6_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T9->Location = System::Drawing::Point(1219, 318);
			this->textBox_R6_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T9->Name = L"textBox_R6_T9";
			this->textBox_R6_T9->ReadOnly = true;
			this->textBox_R6_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T9->TabIndex = 28;
			this->textBox_R6_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T8
			// 
			this->textBox_R6_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T8->Location = System::Drawing::Point(1219, 283);
			this->textBox_R6_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T8->Name = L"textBox_R6_T8";
			this->textBox_R6_T8->ReadOnly = true;
			this->textBox_R6_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T8->TabIndex = 28;
			this->textBox_R6_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T7
			// 
			this->textBox_R6_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T7->Location = System::Drawing::Point(1219, 248);
			this->textBox_R6_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T7->Name = L"textBox_R6_T7";
			this->textBox_R6_T7->ReadOnly = true;
			this->textBox_R6_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T7->TabIndex = 28;
			this->textBox_R6_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T6
			// 
			this->textBox_R6_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T6->Location = System::Drawing::Point(1219, 213);
			this->textBox_R6_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T6->Name = L"textBox_R6_T6";
			this->textBox_R6_T6->ReadOnly = true;
			this->textBox_R6_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T6->TabIndex = 28;
			this->textBox_R6_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T5
			// 
			this->textBox_R6_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T5->Location = System::Drawing::Point(1219, 178);
			this->textBox_R6_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T5->Name = L"textBox_R6_T5";
			this->textBox_R6_T5->ReadOnly = true;
			this->textBox_R6_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T5->TabIndex = 28;
			this->textBox_R6_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T4
			// 
			this->textBox_R6_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T4->Location = System::Drawing::Point(1219, 143);
			this->textBox_R6_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T4->Name = L"textBox_R6_T4";
			this->textBox_R6_T4->ReadOnly = true;
			this->textBox_R6_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T4->TabIndex = 28;
			this->textBox_R6_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T3
			// 
			this->textBox_R6_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T3->Location = System::Drawing::Point(1219, 108);
			this->textBox_R6_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T3->Name = L"textBox_R6_T3";
			this->textBox_R6_T3->ReadOnly = true;
			this->textBox_R6_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T3->TabIndex = 28;
			this->textBox_R6_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T2
			// 
			this->textBox_R6_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T2->Location = System::Drawing::Point(1219, 73);
			this->textBox_R6_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T2->Name = L"textBox_R6_T2";
			this->textBox_R6_T2->ReadOnly = true;
			this->textBox_R6_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T2->TabIndex = 28;
			this->textBox_R6_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T1
			// 
			this->textBox_R6_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T1->Location = System::Drawing::Point(1219, 38);
			this->textBox_R6_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T1->Name = L"textBox_R6_T1";
			this->textBox_R6_T1->ReadOnly = true;
			this->textBox_R6_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T1->TabIndex = 28;
			this->textBox_R6_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T10
			// 
			this->textBox_R5_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T10->Location = System::Drawing::Point(1127, 353);
			this->textBox_R5_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T10->Name = L"textBox_R5_T10";
			this->textBox_R5_T10->ReadOnly = true;
			this->textBox_R5_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T10->TabIndex = 27;
			this->textBox_R5_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T9
			// 
			this->textBox_R5_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T9->Location = System::Drawing::Point(1127, 318);
			this->textBox_R5_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T9->Name = L"textBox_R5_T9";
			this->textBox_R5_T9->ReadOnly = true;
			this->textBox_R5_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T9->TabIndex = 27;
			this->textBox_R5_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T8
			// 
			this->textBox_R5_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T8->Location = System::Drawing::Point(1127, 283);
			this->textBox_R5_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T8->Name = L"textBox_R5_T8";
			this->textBox_R5_T8->ReadOnly = true;
			this->textBox_R5_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T8->TabIndex = 27;
			this->textBox_R5_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T7
			// 
			this->textBox_R5_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T7->Location = System::Drawing::Point(1127, 248);
			this->textBox_R5_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T7->Name = L"textBox_R5_T7";
			this->textBox_R5_T7->ReadOnly = true;
			this->textBox_R5_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T7->TabIndex = 27;
			this->textBox_R5_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T6
			// 
			this->textBox_R5_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T6->Location = System::Drawing::Point(1127, 213);
			this->textBox_R5_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T6->Name = L"textBox_R5_T6";
			this->textBox_R5_T6->ReadOnly = true;
			this->textBox_R5_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T6->TabIndex = 27;
			this->textBox_R5_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T5
			// 
			this->textBox_R5_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T5->Location = System::Drawing::Point(1127, 178);
			this->textBox_R5_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T5->Name = L"textBox_R5_T5";
			this->textBox_R5_T5->ReadOnly = true;
			this->textBox_R5_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T5->TabIndex = 27;
			this->textBox_R5_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T4
			// 
			this->textBox_R5_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T4->Location = System::Drawing::Point(1127, 143);
			this->textBox_R5_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T4->Name = L"textBox_R5_T4";
			this->textBox_R5_T4->ReadOnly = true;
			this->textBox_R5_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T4->TabIndex = 27;
			this->textBox_R5_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T3
			// 
			this->textBox_R5_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T3->Location = System::Drawing::Point(1127, 108);
			this->textBox_R5_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T3->Name = L"textBox_R5_T3";
			this->textBox_R5_T3->ReadOnly = true;
			this->textBox_R5_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T3->TabIndex = 27;
			this->textBox_R5_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T2
			// 
			this->textBox_R5_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T2->Location = System::Drawing::Point(1127, 73);
			this->textBox_R5_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T2->Name = L"textBox_R5_T2";
			this->textBox_R5_T2->ReadOnly = true;
			this->textBox_R5_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T2->TabIndex = 27;
			this->textBox_R5_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R5_T1
			// 
			this->textBox_R5_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R5_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R5_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R5_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R5_T1->Location = System::Drawing::Point(1127, 38);
			this->textBox_R5_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R5_T1->Name = L"textBox_R5_T1";
			this->textBox_R5_T1->ReadOnly = true;
			this->textBox_R5_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R5_T1->TabIndex = 27;
			this->textBox_R5_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T10
			// 
			this->textBox_R4_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T10->Location = System::Drawing::Point(1035, 353);
			this->textBox_R4_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T10->Name = L"textBox_R4_T10";
			this->textBox_R4_T10->ReadOnly = true;
			this->textBox_R4_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T10->TabIndex = 26;
			this->textBox_R4_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T9
			// 
			this->textBox_R4_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T9->Location = System::Drawing::Point(1035, 318);
			this->textBox_R4_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T9->Name = L"textBox_R4_T9";
			this->textBox_R4_T9->ReadOnly = true;
			this->textBox_R4_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T9->TabIndex = 26;
			this->textBox_R4_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T8
			// 
			this->textBox_R4_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T8->Location = System::Drawing::Point(1035, 283);
			this->textBox_R4_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T8->Name = L"textBox_R4_T8";
			this->textBox_R4_T8->ReadOnly = true;
			this->textBox_R4_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T8->TabIndex = 26;
			this->textBox_R4_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T7
			// 
			this->textBox_R4_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T7->Location = System::Drawing::Point(1035, 248);
			this->textBox_R4_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T7->Name = L"textBox_R4_T7";
			this->textBox_R4_T7->ReadOnly = true;
			this->textBox_R4_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T7->TabIndex = 26;
			this->textBox_R4_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T5
			// 
			this->textBox_R4_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T5->Location = System::Drawing::Point(1035, 178);
			this->textBox_R4_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T5->Name = L"textBox_R4_T5";
			this->textBox_R4_T5->ReadOnly = true;
			this->textBox_R4_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T5->TabIndex = 26;
			this->textBox_R4_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T4
			// 
			this->textBox_R4_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T4->Location = System::Drawing::Point(1035, 143);
			this->textBox_R4_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T4->Name = L"textBox_R4_T4";
			this->textBox_R4_T4->ReadOnly = true;
			this->textBox_R4_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T4->TabIndex = 26;
			this->textBox_R4_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T3
			// 
			this->textBox_R4_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T3->Location = System::Drawing::Point(1035, 108);
			this->textBox_R4_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T3->Name = L"textBox_R4_T3";
			this->textBox_R4_T3->ReadOnly = true;
			this->textBox_R4_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T3->TabIndex = 26;
			this->textBox_R4_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T2
			// 
			this->textBox_R4_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T2->Location = System::Drawing::Point(1035, 73);
			this->textBox_R4_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T2->Name = L"textBox_R4_T2";
			this->textBox_R4_T2->ReadOnly = true;
			this->textBox_R4_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T2->TabIndex = 26;
			this->textBox_R4_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T1
			// 
			this->textBox_R4_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T1->Location = System::Drawing::Point(1035, 38);
			this->textBox_R4_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T1->Name = L"textBox_R4_T1";
			this->textBox_R4_T1->ReadOnly = true;
			this->textBox_R4_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T1->TabIndex = 26;
			this->textBox_R4_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T9
			// 
			this->textBox_R3_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T9->Location = System::Drawing::Point(943, 318);
			this->textBox_R3_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T9->Name = L"textBox_R3_T9";
			this->textBox_R3_T9->ReadOnly = true;
			this->textBox_R3_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T9->TabIndex = 25;
			this->textBox_R3_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T8
			// 
			this->textBox_R3_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T8->Location = System::Drawing::Point(943, 283);
			this->textBox_R3_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T8->Name = L"textBox_R3_T8";
			this->textBox_R3_T8->ReadOnly = true;
			this->textBox_R3_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T8->TabIndex = 25;
			this->textBox_R3_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T7
			// 
			this->textBox_R3_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T7->Location = System::Drawing::Point(943, 248);
			this->textBox_R3_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T7->Name = L"textBox_R3_T7";
			this->textBox_R3_T7->ReadOnly = true;
			this->textBox_R3_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T7->TabIndex = 25;
			this->textBox_R3_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T6
			// 
			this->textBox_R3_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T6->Location = System::Drawing::Point(943, 213);
			this->textBox_R3_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T6->Name = L"textBox_R3_T6";
			this->textBox_R3_T6->ReadOnly = true;
			this->textBox_R3_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T6->TabIndex = 25;
			this->textBox_R3_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T5
			// 
			this->textBox_R3_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T5->Location = System::Drawing::Point(943, 178);
			this->textBox_R3_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T5->Name = L"textBox_R3_T5";
			this->textBox_R3_T5->ReadOnly = true;
			this->textBox_R3_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T5->TabIndex = 25;
			this->textBox_R3_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T4
			// 
			this->textBox_R3_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T4->Location = System::Drawing::Point(943, 143);
			this->textBox_R3_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T4->Name = L"textBox_R3_T4";
			this->textBox_R3_T4->ReadOnly = true;
			this->textBox_R3_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T4->TabIndex = 25;
			this->textBox_R3_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T3
			// 
			this->textBox_R3_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T3->Location = System::Drawing::Point(943, 108);
			this->textBox_R3_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T3->Name = L"textBox_R3_T3";
			this->textBox_R3_T3->ReadOnly = true;
			this->textBox_R3_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T3->TabIndex = 25;
			this->textBox_R3_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T2
			// 
			this->textBox_R3_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T2->Location = System::Drawing::Point(943, 73);
			this->textBox_R3_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T2->Name = L"textBox_R3_T2";
			this->textBox_R3_T2->ReadOnly = true;
			this->textBox_R3_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T2->TabIndex = 25;
			this->textBox_R3_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T1
			// 
			this->textBox_R3_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T1->Location = System::Drawing::Point(943, 38);
			this->textBox_R3_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T1->Name = L"textBox_R3_T1";
			this->textBox_R3_T1->ReadOnly = true;
			this->textBox_R3_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T1->TabIndex = 25;
			this->textBox_R3_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T10
			// 
			this->textBox_R2_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T10->Location = System::Drawing::Point(851, 353);
			this->textBox_R2_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T10->Name = L"textBox_R2_T10";
			this->textBox_R2_T10->ReadOnly = true;
			this->textBox_R2_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T10->TabIndex = 24;
			this->textBox_R2_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T9
			// 
			this->textBox_R2_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T9->Location = System::Drawing::Point(851, 318);
			this->textBox_R2_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T9->Name = L"textBox_R2_T9";
			this->textBox_R2_T9->ReadOnly = true;
			this->textBox_R2_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T9->TabIndex = 24;
			this->textBox_R2_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T8
			// 
			this->textBox_R2_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T8->Location = System::Drawing::Point(851, 283);
			this->textBox_R2_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T8->Name = L"textBox_R2_T8";
			this->textBox_R2_T8->ReadOnly = true;
			this->textBox_R2_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T8->TabIndex = 24;
			this->textBox_R2_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T7
			// 
			this->textBox_R2_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T7->Location = System::Drawing::Point(851, 248);
			this->textBox_R2_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T7->Name = L"textBox_R2_T7";
			this->textBox_R2_T7->ReadOnly = true;
			this->textBox_R2_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T7->TabIndex = 24;
			this->textBox_R2_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T6
			// 
			this->textBox_R2_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T6->Location = System::Drawing::Point(851, 213);
			this->textBox_R2_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T6->Name = L"textBox_R2_T6";
			this->textBox_R2_T6->ReadOnly = true;
			this->textBox_R2_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T6->TabIndex = 24;
			this->textBox_R2_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T5
			// 
			this->textBox_R2_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T5->Location = System::Drawing::Point(851, 178);
			this->textBox_R2_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T5->Name = L"textBox_R2_T5";
			this->textBox_R2_T5->ReadOnly = true;
			this->textBox_R2_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T5->TabIndex = 24;
			this->textBox_R2_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T4
			// 
			this->textBox_R2_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T4->Location = System::Drawing::Point(851, 143);
			this->textBox_R2_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T4->Name = L"textBox_R2_T4";
			this->textBox_R2_T4->ReadOnly = true;
			this->textBox_R2_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T4->TabIndex = 24;
			this->textBox_R2_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T3
			// 
			this->textBox_R2_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T3->Location = System::Drawing::Point(851, 108);
			this->textBox_R2_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T3->Name = L"textBox_R2_T3";
			this->textBox_R2_T3->ReadOnly = true;
			this->textBox_R2_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T3->TabIndex = 24;
			this->textBox_R2_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T2
			// 
			this->textBox_R2_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T2->Location = System::Drawing::Point(851, 73);
			this->textBox_R2_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T2->Name = L"textBox_R2_T2";
			this->textBox_R2_T2->ReadOnly = true;
			this->textBox_R2_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T2->TabIndex = 24;
			this->textBox_R2_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R2_T1
			// 
			this->textBox_R2_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R2_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R2_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R2_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R2_T1->Location = System::Drawing::Point(851, 38);
			this->textBox_R2_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R2_T1->Name = L"textBox_R2_T1";
			this->textBox_R2_T1->ReadOnly = true;
			this->textBox_R2_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R2_T1->TabIndex = 24;
			this->textBox_R2_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T3
			// 
			this->textBox_R1_T3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T3->Location = System::Drawing::Point(759, 108);
			this->textBox_R1_T3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T3->Name = L"textBox_R1_T3";
			this->textBox_R1_T3->ReadOnly = true;
			this->textBox_R1_T3->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T3->TabIndex = 24;
			this->textBox_R1_T3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T2
			// 
			this->textBox_R1_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T2->Location = System::Drawing::Point(759, 73);
			this->textBox_R1_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T2->Name = L"textBox_R1_T2";
			this->textBox_R1_T2->ReadOnly = true;
			this->textBox_R1_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T2->TabIndex = 23;
			this->textBox_R1_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T1
			// 
			this->textBox_R1_T1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T1->Location = System::Drawing::Point(759, 38);
			this->textBox_R1_T1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T1->Name = L"textBox_R1_T1";
			this->textBox_R1_T1->ReadOnly = true;
			this->textBox_R1_T1->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T1->TabIndex = 23;
			this->textBox_R1_T1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// labelT10
			// 
			this->labelT10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT10->AutoSize = true;
			this->labelT10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT10->Location = System::Drawing::Point(4, 351);
			this->labelT10->Name = L"labelT10";
			this->labelT10->Size = System::Drawing::Size(103, 34);
			this->labelT10->TabIndex = 39;
			this->labelT10->Text = L"T5 2000";
			this->labelT10->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelT9
			// 
			this->labelT9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT9->AutoSize = true;
			this->labelT9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT9->Location = System::Drawing::Point(4, 316);
			this->labelT9->Name = L"labelT9";
			this->labelT9->Size = System::Drawing::Size(103, 34);
			this->labelT9->TabIndex = 39;
			this->labelT9->Text = L"T4 2000";
			this->labelT9->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelT8
			// 
			this->labelT8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT8->AutoSize = true;
			this->labelT8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT8->Location = System::Drawing::Point(4, 281);
			this->labelT8->Name = L"labelT8";
			this->labelT8->Size = System::Drawing::Size(103, 34);
			this->labelT8->TabIndex = 39;
			this->labelT8->Text = L"T3 2000";
			this->labelT8->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelT7
			// 
			this->labelT7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT7->AutoSize = true;
			this->labelT7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT7->Location = System::Drawing::Point(4, 246);
			this->labelT7->Name = L"labelT7";
			this->labelT7->Size = System::Drawing::Size(103, 34);
			this->labelT7->TabIndex = 39;
			this->labelT7->Text = L"T2 2000";
			this->labelT7->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelT6
			// 
			this->labelT6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT6->AutoSize = true;
			this->labelT6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT6->Location = System::Drawing::Point(4, 211);
			this->labelT6->Name = L"labelT6";
			this->labelT6->Size = System::Drawing::Size(103, 34);
			this->labelT6->TabIndex = 38;
			this->labelT6->Text = L"T1 2000";
			this->labelT6->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelT5
			// 
			this->labelT5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT5->AutoSize = true;
			this->labelT5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT5->Location = System::Drawing::Point(4, 176);
			this->labelT5->Name = L"labelT5";
			this->labelT5->Size = System::Drawing::Size(103, 34);
			this->labelT5->TabIndex = 38;
			this->labelT5->Text = L"T5 400";
			this->labelT5->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelT4
			// 
			this->labelT4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT4->AutoSize = true;
			this->labelT4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT4->Location = System::Drawing::Point(4, 141);
			this->labelT4->Name = L"labelT4";
			this->labelT4->Size = System::Drawing::Size(103, 34);
			this->labelT4->TabIndex = 38;
			this->labelT4->Text = L"T4 400";
			this->labelT4->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelT3
			// 
			this->labelT3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT3->AutoSize = true;
			this->labelT3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT3->Location = System::Drawing::Point(4, 106);
			this->labelT3->Name = L"labelT3";
			this->labelT3->Size = System::Drawing::Size(103, 34);
			this->labelT3->TabIndex = 38;
			this->labelT3->Text = L"T3 400";
			this->labelT3->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelT2
			// 
			this->labelT2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT2->AutoSize = true;
			this->labelT2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT2->Location = System::Drawing::Point(4, 71);
			this->labelT2->Name = L"labelT2";
			this->labelT2->Size = System::Drawing::Size(103, 34);
			this->labelT2->TabIndex = 38;
			this->labelT2->Text = L"T2 400";
			this->labelT2->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label_ro
			// 
			this->label_ro->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_ro->AutoSize = true;
			this->label_ro->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label_ro->Location = System::Drawing::Point(667, 1);
			this->label_ro->Name = L"label_ro";
			this->label_ro->Size = System::Drawing::Size(85, 34);
			this->label_ro->TabIndex = 33;
			this->label_ro->Text = L"УЭС Om";
			this->label_ro->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// textBox_ro1
			// 
			this->textBox_ro1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro1->Location = System::Drawing::Point(667, 38);
			this->textBox_ro1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro1->Name = L"textBox_ro1";
			this->textBox_ro1->ReadOnly = true;
			this->textBox_ro1->Size = System::Drawing::Size(85, 27);
			this->textBox_ro1->TabIndex = 22;
			this->textBox_ro1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro2
			// 
			this->textBox_ro2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro2->Location = System::Drawing::Point(667, 73);
			this->textBox_ro2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro2->Name = L"textBox_ro2";
			this->textBox_ro2->ReadOnly = true;
			this->textBox_ro2->Size = System::Drawing::Size(85, 27);
			this->textBox_ro2->TabIndex = 23;
			this->textBox_ro2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro3
			// 
			this->textBox_ro3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro3->Location = System::Drawing::Point(667, 108);
			this->textBox_ro3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro3->Name = L"textBox_ro3";
			this->textBox_ro3->ReadOnly = true;
			this->textBox_ro3->Size = System::Drawing::Size(85, 27);
			this->textBox_ro3->TabIndex = 24;
			this->textBox_ro3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro4
			// 
			this->textBox_ro4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro4->Location = System::Drawing::Point(667, 143);
			this->textBox_ro4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro4->Name = L"textBox_ro4";
			this->textBox_ro4->ReadOnly = true;
			this->textBox_ro4->Size = System::Drawing::Size(85, 27);
			this->textBox_ro4->TabIndex = 25;
			this->textBox_ro4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro5
			// 
			this->textBox_ro5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro5->Location = System::Drawing::Point(667, 178);
			this->textBox_ro5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro5->Name = L"textBox_ro5";
			this->textBox_ro5->ReadOnly = true;
			this->textBox_ro5->Size = System::Drawing::Size(85, 27);
			this->textBox_ro5->TabIndex = 26;
			this->textBox_ro5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro6
			// 
			this->textBox_ro6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro6->Location = System::Drawing::Point(667, 213);
			this->textBox_ro6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro6->Name = L"textBox_ro6";
			this->textBox_ro6->ReadOnly = true;
			this->textBox_ro6->Size = System::Drawing::Size(85, 27);
			this->textBox_ro6->TabIndex = 27;
			this->textBox_ro6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro7
			// 
			this->textBox_ro7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro7->Location = System::Drawing::Point(667, 248);
			this->textBox_ro7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro7->Name = L"textBox_ro7";
			this->textBox_ro7->ReadOnly = true;
			this->textBox_ro7->Size = System::Drawing::Size(85, 27);
			this->textBox_ro7->TabIndex = 28;
			this->textBox_ro7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro8
			// 
			this->textBox_ro8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro8->Location = System::Drawing::Point(667, 283);
			this->textBox_ro8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro8->Name = L"textBox_ro8";
			this->textBox_ro8->ReadOnly = true;
			this->textBox_ro8->Size = System::Drawing::Size(85, 27);
			this->textBox_ro8->TabIndex = 29;
			this->textBox_ro8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro9
			// 
			this->textBox_ro9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro9->Location = System::Drawing::Point(667, 318);
			this->textBox_ro9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro9->Name = L"textBox_ro9";
			this->textBox_ro9->ReadOnly = true;
			this->textBox_ro9->Size = System::Drawing::Size(85, 27);
			this->textBox_ro9->TabIndex = 30;
			this->textBox_ro9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_ro10
			// 
			this->textBox_ro10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_ro10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_ro10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_ro10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_ro10->Location = System::Drawing::Point(667, 353);
			this->textBox_ro10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_ro10->Name = L"textBox_ro10";
			this->textBox_ro10->ReadOnly = true;
			this->textBox_ro10->Size = System::Drawing::Size(85, 27);
			this->textBox_ro10->TabIndex = 31;
			this->textBox_ro10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// pictureBox_graf1
			// 
			this->pictureBox_graf1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf1->Location = System::Drawing::Point(215, 38);
			this->pictureBox_graf1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf1->Name = L"pictureBox_graf1";
			this->pictureBox_graf1->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf1->TabIndex = 0;
			this->pictureBox_graf1->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf1, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf2
			// 
			this->pictureBox_graf2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf2->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf2->Location = System::Drawing::Point(215, 73);
			this->pictureBox_graf2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf2->Name = L"pictureBox_graf2";
			this->pictureBox_graf2->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf2->TabIndex = 1;
			this->pictureBox_graf2->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf2, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf3
			// 
			this->pictureBox_graf3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf3->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf3->Location = System::Drawing::Point(215, 108);
			this->pictureBox_graf3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf3->Name = L"pictureBox_graf3";
			this->pictureBox_graf3->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf3->TabIndex = 2;
			this->pictureBox_graf3->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf3, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf4
			// 
			this->pictureBox_graf4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf4->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf4->Location = System::Drawing::Point(215, 143);
			this->pictureBox_graf4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf4->Name = L"pictureBox_graf4";
			this->pictureBox_graf4->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf4->TabIndex = 3;
			this->pictureBox_graf4->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf4, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf5
			// 
			this->pictureBox_graf5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf5->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf5->Location = System::Drawing::Point(215, 178);
			this->pictureBox_graf5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf5->Name = L"pictureBox_graf5";
			this->pictureBox_graf5->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf5->TabIndex = 4;
			this->pictureBox_graf5->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf5, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf6
			// 
			this->pictureBox_graf6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf6->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf6->Location = System::Drawing::Point(215, 213);
			this->pictureBox_graf6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf6->Name = L"pictureBox_graf6";
			this->pictureBox_graf6->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf6->TabIndex = 5;
			this->pictureBox_graf6->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf6, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf7
			// 
			this->pictureBox_graf7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf7->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf7->Location = System::Drawing::Point(215, 248);
			this->pictureBox_graf7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf7->Name = L"pictureBox_graf7";
			this->pictureBox_graf7->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf7->TabIndex = 6;
			this->pictureBox_graf7->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf7, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf8
			// 
			this->pictureBox_graf8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf8->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf8->Location = System::Drawing::Point(215, 283);
			this->pictureBox_graf8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf8->Name = L"pictureBox_graf8";
			this->pictureBox_graf8->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf8->TabIndex = 7;
			this->pictureBox_graf8->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf8, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf9
			// 
			this->pictureBox_graf9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf9->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf9->Location = System::Drawing::Point(215, 318);
			this->pictureBox_graf9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf9->Name = L"pictureBox_graf9";
			this->pictureBox_graf9->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf9->TabIndex = 8;
			this->pictureBox_graf9->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf9, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// pictureBox_graf10
			// 
			this->pictureBox_graf10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox_graf10->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->pictureBox_graf10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox_graf10->Location = System::Drawing::Point(215, 353);
			this->pictureBox_graf10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox_graf10->Name = L"pictureBox_graf10";
			this->pictureBox_graf10->Size = System::Drawing::Size(445, 28);
			this->pictureBox_graf10->TabIndex = 9;
			this->pictureBox_graf10->TabStop = false;
			this->toolTip1->SetToolTip(this->pictureBox_graf10, L"На графике отображаются  \r\nизмерянные значений сопротивления,\r\nустановленного на "
				L"иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных коридоров.");
			// 
			// textBox_dfi1
			// 
			this->textBox_dfi1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi1->Location = System::Drawing::Point(114, 38);
			this->textBox_dfi1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi1->Name = L"textBox_dfi1";
			this->textBox_dfi1->ReadOnly = true;
			this->textBox_dfi1->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi1->TabIndex = 12;
			this->textBox_dfi1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi2
			// 
			this->textBox_dfi2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi2->Location = System::Drawing::Point(114, 73);
			this->textBox_dfi2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi2->Name = L"textBox_dfi2";
			this->textBox_dfi2->ReadOnly = true;
			this->textBox_dfi2->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi2->TabIndex = 13;
			this->textBox_dfi2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi3
			// 
			this->textBox_dfi3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi3->Location = System::Drawing::Point(114, 108);
			this->textBox_dfi3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi3->Name = L"textBox_dfi3";
			this->textBox_dfi3->ReadOnly = true;
			this->textBox_dfi3->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi3->TabIndex = 14;
			this->textBox_dfi3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi4
			// 
			this->textBox_dfi4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi4->Location = System::Drawing::Point(114, 143);
			this->textBox_dfi4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi4->Name = L"textBox_dfi4";
			this->textBox_dfi4->ReadOnly = true;
			this->textBox_dfi4->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi4->TabIndex = 15;
			this->textBox_dfi4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi5
			// 
			this->textBox_dfi5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi5->Location = System::Drawing::Point(114, 178);
			this->textBox_dfi5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi5->Name = L"textBox_dfi5";
			this->textBox_dfi5->ReadOnly = true;
			this->textBox_dfi5->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi5->TabIndex = 16;
			this->textBox_dfi5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi6
			// 
			this->textBox_dfi6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi6->Location = System::Drawing::Point(114, 213);
			this->textBox_dfi6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi6->Name = L"textBox_dfi6";
			this->textBox_dfi6->ReadOnly = true;
			this->textBox_dfi6->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi6->TabIndex = 17;
			this->textBox_dfi6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi9
			// 
			this->textBox_dfi9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi9->Location = System::Drawing::Point(114, 318);
			this->textBox_dfi9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi9->Name = L"textBox_dfi9";
			this->textBox_dfi9->ReadOnly = true;
			this->textBox_dfi9->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi9->TabIndex = 20;
			this->textBox_dfi9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi10
			// 
			this->textBox_dfi10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi10->Location = System::Drawing::Point(114, 353);
			this->textBox_dfi10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi10->Name = L"textBox_dfi10";
			this->textBox_dfi10->ReadOnly = true;
			this->textBox_dfi10->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi10->TabIndex = 21;
			this->textBox_dfi10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T4
			// 
			this->textBox_R1_T4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T4->Location = System::Drawing::Point(759, 143);
			this->textBox_R1_T4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T4->Name = L"textBox_R1_T4";
			this->textBox_R1_T4->ReadOnly = true;
			this->textBox_R1_T4->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T4->TabIndex = 40;
			this->textBox_R1_T4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T5
			// 
			this->textBox_R1_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T5->Location = System::Drawing::Point(759, 178);
			this->textBox_R1_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T5->Name = L"textBox_R1_T5";
			this->textBox_R1_T5->ReadOnly = true;
			this->textBox_R1_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T5->TabIndex = 41;
			this->textBox_R1_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T6
			// 
			this->textBox_R1_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T6->Location = System::Drawing::Point(759, 213);
			this->textBox_R1_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T6->Name = L"textBox_R1_T6";
			this->textBox_R1_T6->ReadOnly = true;
			this->textBox_R1_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T6->TabIndex = 42;
			this->textBox_R1_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T8
			// 
			this->textBox_R1_T8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T8->Location = System::Drawing::Point(759, 283);
			this->textBox_R1_T8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T8->Name = L"textBox_R1_T8";
			this->textBox_R1_T8->ReadOnly = true;
			this->textBox_R1_T8->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T8->TabIndex = 44;
			this->textBox_R1_T8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T9
			// 
			this->textBox_R1_T9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T9->Location = System::Drawing::Point(759, 318);
			this->textBox_R1_T9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T9->Name = L"textBox_R1_T9";
			this->textBox_R1_T9->ReadOnly = true;
			this->textBox_R1_T9->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T9->TabIndex = 45;
			this->textBox_R1_T9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T10
			// 
			this->textBox_R1_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T10->Location = System::Drawing::Point(759, 353);
			this->textBox_R1_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T10->Name = L"textBox_R1_T10";
			this->textBox_R1_T10->ReadOnly = true;
			this->textBox_R1_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T10->TabIndex = 46;
			this->textBox_R1_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// button_R2
			// 
			this->button_R2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R2->Location = System::Drawing::Point(851, 3);
			this->button_R2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R2->Name = L"button_R2";
			this->button_R2->Size = System::Drawing::Size(85, 28);
			this->button_R2->TabIndex = 35;
			this->button_R2->Text = L"R2";
			this->toolTip1->SetToolTip(this->button_R2, L"При включенном кольце устанавливает сопротивление R2\r\nна иммитаторе.");
			this->button_R2->UseVisualStyleBackColor = true;
			this->button_R2->Click += gcnew System::EventHandler(this, &WorkForm::button_R2_Click);
			// 
			// button_R3
			// 
			this->button_R3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R3->Location = System::Drawing::Point(943, 3);
			this->button_R3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R3->Name = L"button_R3";
			this->button_R3->Size = System::Drawing::Size(85, 28);
			this->button_R3->TabIndex = 36;
			this->button_R3->Text = L"R3";
			this->toolTip1->SetToolTip(this->button_R3, L"При включенном кольце устанавливает сопротивление R3 \r\nна иммитаторе.");
			this->button_R3->UseVisualStyleBackColor = true;
			this->button_R3->Click += gcnew System::EventHandler(this, &WorkForm::button_R3_Click);
			// 
			// button_R4
			// 
			this->button_R4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R4->Location = System::Drawing::Point(1035, 3);
			this->button_R4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R4->Name = L"button_R4";
			this->button_R4->Size = System::Drawing::Size(85, 28);
			this->button_R4->TabIndex = 36;
			this->button_R4->Text = L"R4";
			this->toolTip1->SetToolTip(this->button_R4, L"При включенном кольце устанавливает сопротивление R4 \r\nна иммитаторе.");
			this->button_R4->UseVisualStyleBackColor = true;
			this->button_R4->Click += gcnew System::EventHandler(this, &WorkForm::button_R4_Click);
			// 
			// button_R5
			// 
			this->button_R5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R5->Location = System::Drawing::Point(1127, 3);
			this->button_R5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R5->Name = L"button_R5";
			this->button_R5->Size = System::Drawing::Size(85, 28);
			this->button_R5->TabIndex = 36;
			this->button_R5->Text = L"R5";
			this->toolTip1->SetToolTip(this->button_R5, L"При включенном кольце устанавливает сопротивление R5\r\nна иммитаторе.");
			this->button_R5->UseVisualStyleBackColor = true;
			this->button_R5->Click += gcnew System::EventHandler(this, &WorkForm::button_R5_Click);
			// 
			// button_R6
			// 
			this->button_R6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R6->Location = System::Drawing::Point(1219, 3);
			this->button_R6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R6->Name = L"button_R6";
			this->button_R6->Size = System::Drawing::Size(85, 28);
			this->button_R6->TabIndex = 36;
			this->button_R6->Text = L"R6";
			this->toolTip1->SetToolTip(this->button_R6, L"При включенном кольце устанавливает сопротивление R6 \r\nна иммитаторе.");
			this->button_R6->UseVisualStyleBackColor = true;
			this->button_R6->Click += gcnew System::EventHandler(this, &WorkForm::button_R6_Click);
			// 
			// button_R7
			// 
			this->button_R7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R7->Location = System::Drawing::Point(1311, 3);
			this->button_R7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R7->Name = L"button_R7";
			this->button_R7->Size = System::Drawing::Size(85, 28);
			this->button_R7->TabIndex = 36;
			this->button_R7->Text = L"R7";
			this->toolTip1->SetToolTip(this->button_R7, L"При включенном кольце устанавливает сопротивление R7\r\nна иммитаторе.");
			this->button_R7->UseVisualStyleBackColor = true;
			this->button_R7->Click += gcnew System::EventHandler(this, &WorkForm::button_R7_Click);
			// 
			// button_R8
			// 
			this->button_R8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R8->Location = System::Drawing::Point(1403, 3);
			this->button_R8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R8->Name = L"button_R8";
			this->button_R8->Size = System::Drawing::Size(85, 28);
			this->button_R8->TabIndex = 36;
			this->button_R8->Text = L"R8";
			this->toolTip1->SetToolTip(this->button_R8, L"При включенном кольце устанавливает сопротивление R8 \r\nна иммитаторе.");
			this->button_R8->UseVisualStyleBackColor = true;
			this->button_R8->Click += gcnew System::EventHandler(this, &WorkForm::button_R8_Click);
			// 
			// button_AIR
			// 
			this->button_AIR->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_AIR->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_AIR->Location = System::Drawing::Point(114, 3);
			this->button_AIR->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_AIR->Name = L"button_AIR";
			this->button_AIR->Size = System::Drawing::Size(94, 28);
			this->button_AIR->TabIndex = 36;
			this->button_AIR->Text = L"AIR";
			this->button_AIR->UseVisualStyleBackColor = true;
			this->button_AIR->Click += gcnew System::EventHandler(this, &WorkForm::button_AIR_Click);
			// 
			// button_R1
			// 
			this->button_R1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button_R1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_R1->Location = System::Drawing::Point(759, 3);
			this->button_R1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_R1->Name = L"button_R1";
			this->button_R1->Size = System::Drawing::Size(85, 28);
			this->button_R1->TabIndex = 34;
			this->button_R1->Text = L"R1";
			this->toolTip1->SetToolTip(this->button_R1, L"При включенном кольце устанавливает сопротивление R1 \r\nна иммитаторе.");
			this->button_R1->UseVisualStyleBackColor = true;
			this->button_R1->Click += gcnew System::EventHandler(this, &WorkForm::button_R1_Click);
			// 
			// textBox_dfi7
			// 
			this->textBox_dfi7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi7->Location = System::Drawing::Point(114, 248);
			this->textBox_dfi7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi7->Name = L"textBox_dfi7";
			this->textBox_dfi7->ReadOnly = true;
			this->textBox_dfi7->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi7->TabIndex = 18;
			this->textBox_dfi7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_dfi8
			// 
			this->textBox_dfi8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_dfi8->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_dfi8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_dfi8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_dfi8->Location = System::Drawing::Point(114, 283);
			this->textBox_dfi8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_dfi8->Name = L"textBox_dfi8";
			this->textBox_dfi8->ReadOnly = true;
			this->textBox_dfi8->Size = System::Drawing::Size(94, 27);
			this->textBox_dfi8->TabIndex = 19;
			this->textBox_dfi8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R1_T7
			// 
			this->textBox_R1_T7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R1_T7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R1_T7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R1_T7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R1_T7->Location = System::Drawing::Point(759, 248);
			this->textBox_R1_T7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R1_T7->Name = L"textBox_R1_T7";
			this->textBox_R1_T7->ReadOnly = true;
			this->textBox_R1_T7->Size = System::Drawing::Size(85, 27);
			this->textBox_R1_T7->TabIndex = 43;
			this->textBox_R1_T7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R3_T10
			// 
			this->textBox_R3_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R3_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R3_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R3_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R3_T10->Location = System::Drawing::Point(943, 353);
			this->textBox_R3_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R3_T10->Name = L"textBox_R3_T10";
			this->textBox_R3_T10->ReadOnly = true;
			this->textBox_R3_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R3_T10->TabIndex = 25;
			this->textBox_R3_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R4_T6
			// 
			this->textBox_R4_T6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R4_T6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R4_T6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R4_T6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R4_T6->Location = System::Drawing::Point(1035, 213);
			this->textBox_R4_T6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R4_T6->Name = L"textBox_R4_T6";
			this->textBox_R4_T6->ReadOnly = true;
			this->textBox_R4_T6->Size = System::Drawing::Size(85, 27);
			this->textBox_R4_T6->TabIndex = 26;
			this->textBox_R4_T6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R6_T10
			// 
			this->textBox_R6_T10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R6_T10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R6_T10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R6_T10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R6_T10->Location = System::Drawing::Point(1219, 353);
			this->textBox_R6_T10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R6_T10->Name = L"textBox_R6_T10";
			this->textBox_R6_T10->ReadOnly = true;
			this->textBox_R6_T10->Size = System::Drawing::Size(85, 27);
			this->textBox_R6_T10->TabIndex = 28;
			this->textBox_R6_T10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T5
			// 
			this->textBox_R7_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T5->Location = System::Drawing::Point(1311, 178);
			this->textBox_R7_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T5->Name = L"textBox_R7_T5";
			this->textBox_R7_T5->ReadOnly = true;
			this->textBox_R7_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T5->TabIndex = 29;
			this->textBox_R7_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R7_T2
			// 
			this->textBox_R7_T2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R7_T2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R7_T2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R7_T2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R7_T2->Location = System::Drawing::Point(1311, 73);
			this->textBox_R7_T2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R7_T2->Name = L"textBox_R7_T2";
			this->textBox_R7_T2->ReadOnly = true;
			this->textBox_R7_T2->Size = System::Drawing::Size(85, 27);
			this->textBox_R7_T2->TabIndex = 29;
			this->textBox_R7_T2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R8_T5
			// 
			this->textBox_R8_T5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R8_T5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_R8_T5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_R8_T5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R8_T5->Location = System::Drawing::Point(1403, 178);
			this->textBox_R8_T5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R8_T5->Name = L"textBox_R8_T5";
			this->textBox_R8_T5->ReadOnly = true;
			this->textBox_R8_T5->Size = System::Drawing::Size(85, 27);
			this->textBox_R8_T5->TabIndex = 30;
			this->textBox_R8_T5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// labelT1
			// 
			this->labelT1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelT1->AutoSize = true;
			this->labelT1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelT1->Location = System::Drawing::Point(4, 36);
			this->labelT1->Name = L"labelT1";
			this->labelT1->Size = System::Drawing::Size(103, 34);
			this->labelT1->TabIndex = 37;
			this->labelT1->Text = L"T1 400";
			this->labelT1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// checkBox_R1
			// 
			this->checkBox_R1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkBox_R1->AutoSize = true;
			this->checkBox_R1->Location = System::Drawing::Point(759, 388);
			this->checkBox_R1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R1->Name = L"checkBox_R1";
			this->checkBox_R1->Size = System::Drawing::Size(18, 32);
			this->checkBox_R1->TabIndex = 43;
			this->checkBox_R1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox_R1->UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label5->Location = System::Drawing::Point(4, 1);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(103, 34);
			this->label5->TabIndex = 47;
			this->label5->Text = L"№ Зонда";
			this->label5->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// tabControl1
			// 
			this->tabControl1->Appearance = System::Windows::Forms::TabAppearance::Buttons;
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tabControl1->Location = System::Drawing::Point(12, 135);
			this->tabControl1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1529, 601);
			this->tabControl1->TabIndex = 33;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->label8);
			this->tabPage1->Controls->Add(this->label7);
			this->tabPage1->Controls->Add(this->label6);
			this->tabPage1->Controls->Add(this->button_raport);
			this->tabPage1->Controls->Add(this->tableLayoutPanel1);
			this->tabPage1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tabPage1->Location = System::Drawing::Point(4, 37);
			this->tabPage1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage1->Size = System::Drawing::Size(1521, 560);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Главная";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label8->Location = System::Drawing::Point(955, 517);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(326, 17);
			this->label8->TabIndex = 46;
			this->label8->Text = L"Помеченное галочкой  записываетсая в рапорт";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label7->Location = System::Drawing::Point(867, 31);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(503, 17);
			this->label7->TabIndex = 45;
			this->label7->Text = L"Таблица: процентная разница рассчетного и измерянного сопротивлений";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label6->Location = System::Drawing::Point(955, 6);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(263, 17);
			this->label6->TabIndex = 44;
			this->label6->Text = L"Кнопка: сопротивления на кольце  Ом";
			// 
			// button_raport
			// 
			this->button_raport->Location = System::Drawing::Point(1351, 500);
			this->button_raport->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_raport->Name = L"button_raport";
			this->button_raport->Size = System::Drawing::Size(153, 46);
			this->button_raport->TabIndex = 44;
			this->button_raport->Text = L"РАПОРТ";
			this->toolTip1->SetToolTip(this->button_raport, L"При нажатии отправляет данные оизмерений, помеченных галочкой\r\nв рапорт.  Данные "
				L"в таблице обнуляются.");
			this->button_raport->UseVisualStyleBackColor = true;
			this->button_raport->Click += gcnew System::EventHandler(this, &WorkForm::сохранитьРапортRtfToolStripMenuItem_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->checkBox_Re_red);
			this->tabPage2->Controls->Add(this->checkBox_R_red);
			this->tabPage2->Controls->Add(this->checkBox_param_red);
			this->tabPage2->Controls->Add(this->tableLayoutPanel11);
			this->tabPage2->Controls->Add(this->button_calk_C_need);
			this->tabPage2->Controls->Add(this->button_signature);
			this->tabPage2->Controls->Add(this->panel2);
			this->tabPage2->Controls->Add(this->richTextBox1);
			this->tabPage2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->tabPage2->Location = System::Drawing::Point(4, 37);
			this->tabPage2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPage2->Size = System::Drawing::Size(1521, 560);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Поверка кольца";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// checkBox_Re_red
			// 
			this->checkBox_Re_red->AutoSize = true;
			this->checkBox_Re_red->Location = System::Drawing::Point(1021, 318);
			this->checkBox_Re_red->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_Re_red->Name = L"checkBox_Re_red";
			this->checkBox_Re_red->Size = System::Drawing::Size(188, 29);
			this->checkBox_Re_red->TabIndex = 89;
			this->checkBox_Re_red->Text = L"редактировать";
			this->checkBox_Re_red->UseVisualStyleBackColor = true;
			this->checkBox_Re_red->CheckedChanged += gcnew System::EventHandler(this, &WorkForm::checkBox_Re_red_CheckedChanged);
			// 
			// checkBox_R_red
			// 
			this->checkBox_R_red->AutoSize = true;
			this->checkBox_R_red->Location = System::Drawing::Point(1021, 238);
			this->checkBox_R_red->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_R_red->Name = L"checkBox_R_red";
			this->checkBox_R_red->Size = System::Drawing::Size(188, 29);
			this->checkBox_R_red->TabIndex = 88;
			this->checkBox_R_red->Text = L"редактировать";
			this->checkBox_R_red->UseVisualStyleBackColor = true;
			this->checkBox_R_red->CheckedChanged += gcnew System::EventHandler(this, &WorkForm::checkBox_r_red_CheckedChanged);
			// 
			// checkBox_param_red
			// 
			this->checkBox_param_red->AutoSize = true;
			this->checkBox_param_red->Location = System::Drawing::Point(1021, 80);
			this->checkBox_param_red->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_param_red->Name = L"checkBox_param_red";
			this->checkBox_param_red->Size = System::Drawing::Size(188, 29);
			this->checkBox_param_red->TabIndex = 87;
			this->checkBox_param_red->Text = L"редактировать";
			this->checkBox_param_red->UseVisualStyleBackColor = true;
			this->checkBox_param_red->CheckedChanged += gcnew System::EventHandler(this, &WorkForm::checkBox_param_red_CheckedChanged);
			// 
			// tableLayoutPanel11
			// 
			this->tableLayoutPanel11->ColumnCount = 11;
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				120)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				83)));
			this->tableLayoutPanel11->Controls->Add(this->label20, 6, 8);
			this->tableLayoutPanel11->Controls->Add(this->label17, 0, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop2, 2, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop1, 1, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_10, 10, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_10, 10, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_10, 10, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С10, 10, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_signature, 7, 8);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc10, 10, 4);
			this->tableLayoutPanel11->Controls->Add(this->label15, 0, 0);
			this->tableLayoutPanel11->Controls->Add(this->label14, 0, 1);
			this->tableLayoutPanel11->Controls->Add(this->label13, 0, 2);
			this->tableLayoutPanel11->Controls->Add(this->label11, 0, 3);
			this->tableLayoutPanel11->Controls->Add(this->label12, 0, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc9, 9, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_1, 1, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С9, 9, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_1, 1, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_1, 1, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_9, 9, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С1, 1, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc1, 1, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_9, 9, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_9, 9, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc8, 8, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С8, 8, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_8, 8, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_8, 8, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_8, 8, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_7, 7, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_7, 7, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_7, 7, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С7, 7, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc7, 7, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_6, 6, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_5, 5, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_6, 6, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_4, 4, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_6, 6, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_3, 3, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С6, 6, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_2, 2, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc6, 6, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_5, 5, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_4, 4, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_2, 2, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_2, 2, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_3, 3, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_5, 5, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_4, 4, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С2, 2, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc2, 2, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С5, 5, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С4, 4, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_3, 3, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С3, 3, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc5, 5, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc4, 4, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc3, 3, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop1, 1, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop2, 2, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop3, 3, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop4, 4, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop5, 5, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop6, 6, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop7, 7, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop8, 8, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop9, 9, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop10, 10, 7);
			this->tableLayoutPanel11->Controls->Add(this->button_Z1, 1, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z2, 2, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z3, 3, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z4, 4, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z5, 5, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z6, 6, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z7, 7, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z8, 8, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z9, 9, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z10, 10, 6);
			this->tableLayoutPanel11->Controls->Add(this->label16, 0, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop3, 3, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop4, 4, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop5, 5, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop6, 6, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop7, 7, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop8, 8, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop9, 9, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_loop10, 10, 5);
			this->tableLayoutPanel11->Controls->Add(this->label18, 0, 8);
			this->tableLayoutPanel11->Controls->Add(this->textBox_radius, 1, 8);
			this->tableLayoutPanel11->Controls->Add(this->label19, 3, 8);
			this->tableLayoutPanel11->Controls->Add(this->textBox_R_wire, 4, 8);
			this->tableLayoutPanel11->Location = System::Drawing::Point(79, 38);
			this->tableLayoutPanel11->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel11->Name = L"tableLayoutPanel11";
			this->tableLayoutPanel11->RowCount = 10;
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				39)));
			this->tableLayoutPanel11->Size = System::Drawing::Size(923, 382);
			this->tableLayoutPanel11->TabIndex = 86;
			// 
			// label20
			// 
			this->label20->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label20->Location = System::Drawing::Point(523, 312);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(74, 39);
			this->label20->TabIndex = 100;
			this->label20->Text = L"sign";
			this->label20->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label17
			// 
			this->label17->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label17->Location = System::Drawing::Point(3, 204);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(63, 20);
			this->label17->TabIndex = 96;
			this->label17->Text = L"R_ om";
			// 
			// textBox_R_loop2
			// 
			this->textBox_R_loop2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop2->Location = System::Drawing::Point(203, 197);
			this->textBox_R_loop2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop2->Name = L"textBox_R_loop2";
			this->textBox_R_loop2->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop2->TabIndex = 87;
			this->textBox_R_loop2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop1
			// 
			this->textBox_R_loop1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop1->Location = System::Drawing::Point(123, 197);
			this->textBox_R_loop1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop1->Name = L"textBox_R_loop1";
			this->textBox_R_loop1->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop1->TabIndex = 87;
			this->textBox_R_loop1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_10
			// 
			this->textBox_L1_10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_10->Location = System::Drawing::Point(843, 2);
			this->textBox_L1_10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_10->Name = L"textBox_L1_10";
			this->textBox_L1_10->ReadOnly = true;
			this->textBox_L1_10->Size = System::Drawing::Size(77, 27);
			this->textBox_L1_10->TabIndex = 78;
			this->textBox_L1_10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_10
			// 
			this->textBox_L2_10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_10->Location = System::Drawing::Point(843, 41);
			this->textBox_L2_10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_10->Name = L"textBox_L2_10";
			this->textBox_L2_10->ReadOnly = true;
			this->textBox_L2_10->Size = System::Drawing::Size(77, 27);
			this->textBox_L2_10->TabIndex = 19;
			this->textBox_L2_10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_10
			// 
			this->textBox_f_10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_10->Location = System::Drawing::Point(843, 80);
			this->textBox_f_10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_10->Name = L"textBox_f_10";
			this->textBox_f_10->ReadOnly = true;
			this->textBox_f_10->Size = System::Drawing::Size(77, 27);
			this->textBox_f_10->TabIndex = 47;
			this->textBox_f_10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С10
			// 
			this->textBox_С10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С10->Location = System::Drawing::Point(843, 119);
			this->textBox_С10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С10->Name = L"textBox_С10";
			this->textBox_С10->ReadOnly = true;
			this->textBox_С10->Size = System::Drawing::Size(77, 27);
			this->textBox_С10->TabIndex = 47;
			this->textBox_С10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_signature
			// 
			this->textBox_signature->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_signature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_signature->Location = System::Drawing::Point(603, 314);
			this->textBox_signature->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_signature->Name = L"textBox_signature";
			this->textBox_signature->Size = System::Drawing::Size(74, 27);
			this->textBox_signature->TabIndex = 83;
			this->textBox_signature->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc10
			// 
			this->textBox_Z_calc10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc10->Location = System::Drawing::Point(843, 158);
			this->textBox_Z_calc10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc10->Name = L"textBox_Z_calc10";
			this->textBox_Z_calc10->ReadOnly = true;
			this->textBox_Z_calc10->Size = System::Drawing::Size(77, 27);
			this->textBox_Z_calc10->TabIndex = 47;
			this->textBox_Z_calc10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label15
			// 
			this->label15->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label15->Location = System::Drawing::Point(3, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(114, 39);
			this->label15->TabIndex = 35;
			this->label15->Text = L"L1 m";
			this->label15->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label14
			// 
			this->label14->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label14->Location = System::Drawing::Point(3, 39);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(114, 39);
			this->label14->TabIndex = 36;
			this->label14->Text = L"L2 m";
			this->label14->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label13
			// 
			this->label13->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label13->Location = System::Drawing::Point(0, 78);
			this->label13->Margin = System::Windows::Forms::Padding(0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(120, 39);
			this->label13->TabIndex = 37;
			this->label13->Text = L"F kHz";
			this->label13->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label11
			// 
			this->label11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label11->Location = System::Drawing::Point(3, 117);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(114, 39);
			this->label11->TabIndex = 47;
			this->label11->Text = L"C_req pF";
			this->label11->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label12
			// 
			this->label12->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label12->Location = System::Drawing::Point(3, 165);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(109, 20);
			this->label12->TabIndex = 64;
			this->label12->Text = L"RE_calk om";
			// 
			// textBox_Z_calc9
			// 
			this->textBox_Z_calc9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc9->Location = System::Drawing::Point(763, 158);
			this->textBox_Z_calc9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc9->Name = L"textBox_Z_calc9";
			this->textBox_Z_calc9->ReadOnly = true;
			this->textBox_Z_calc9->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc9->TabIndex = 47;
			this->textBox_Z_calc9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_1
			// 
			this->textBox_L1_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_1->Cursor = System::Windows::Forms::Cursors::No;
			this->textBox_L1_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_1->Location = System::Drawing::Point(123, 2);
			this->textBox_L1_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_1->Name = L"textBox_L1_1";
			this->textBox_L1_1->ReadOnly = true;
			this->textBox_L1_1->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_1->TabIndex = 2;
			this->textBox_L1_1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С9
			// 
			this->textBox_С9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С9->Location = System::Drawing::Point(763, 119);
			this->textBox_С9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С9->Name = L"textBox_С9";
			this->textBox_С9->ReadOnly = true;
			this->textBox_С9->Size = System::Drawing::Size(74, 27);
			this->textBox_С9->TabIndex = 47;
			this->textBox_С9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_1
			// 
			this->textBox_L2_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_1->Location = System::Drawing::Point(123, 41);
			this->textBox_L2_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_1->Name = L"textBox_L2_1";
			this->textBox_L2_1->ReadOnly = true;
			this->textBox_L2_1->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_1->TabIndex = 10;
			this->textBox_L2_1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_1
			// 
			this->textBox_f_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_1->Location = System::Drawing::Point(123, 80);
			this->textBox_f_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_1->Name = L"textBox_f_1";
			this->textBox_f_1->ReadOnly = true;
			this->textBox_f_1->Size = System::Drawing::Size(74, 27);
			this->textBox_f_1->TabIndex = 39;
			this->textBox_f_1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_9
			// 
			this->textBox_f_9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_9->Location = System::Drawing::Point(763, 80);
			this->textBox_f_9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_9->Name = L"textBox_f_9";
			this->textBox_f_9->ReadOnly = true;
			this->textBox_f_9->Size = System::Drawing::Size(74, 27);
			this->textBox_f_9->TabIndex = 47;
			this->textBox_f_9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С1
			// 
			this->textBox_С1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С1->Location = System::Drawing::Point(123, 119);
			this->textBox_С1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С1->Name = L"textBox_С1";
			this->textBox_С1->ReadOnly = true;
			this->textBox_С1->Size = System::Drawing::Size(74, 27);
			this->textBox_С1->TabIndex = 48;
			this->textBox_С1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc1
			// 
			this->textBox_Z_calc1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc1->Location = System::Drawing::Point(123, 158);
			this->textBox_Z_calc1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc1->Name = L"textBox_Z_calc1";
			this->textBox_Z_calc1->ReadOnly = true;
			this->textBox_Z_calc1->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc1->TabIndex = 56;
			this->textBox_Z_calc1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_9
			// 
			this->textBox_L2_9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_9->Location = System::Drawing::Point(763, 41);
			this->textBox_L2_9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_9->Name = L"textBox_L2_9";
			this->textBox_L2_9->ReadOnly = true;
			this->textBox_L2_9->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_9->TabIndex = 18;
			this->textBox_L2_9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_9
			// 
			this->textBox_L1_9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_9->Location = System::Drawing::Point(763, 2);
			this->textBox_L1_9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_9->Name = L"textBox_L1_9";
			this->textBox_L1_9->ReadOnly = true;
			this->textBox_L1_9->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_9->TabIndex = 77;
			this->textBox_L1_9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc8
			// 
			this->textBox_Z_calc8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc8->Location = System::Drawing::Point(683, 158);
			this->textBox_Z_calc8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc8->Name = L"textBox_Z_calc8";
			this->textBox_Z_calc8->ReadOnly = true;
			this->textBox_Z_calc8->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc8->TabIndex = 63;
			this->textBox_Z_calc8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С8
			// 
			this->textBox_С8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С8->Location = System::Drawing::Point(683, 119);
			this->textBox_С8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С8->Name = L"textBox_С8";
			this->textBox_С8->ReadOnly = true;
			this->textBox_С8->Size = System::Drawing::Size(74, 27);
			this->textBox_С8->TabIndex = 55;
			this->textBox_С8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_8
			// 
			this->textBox_f_8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_8->Location = System::Drawing::Point(683, 80);
			this->textBox_f_8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_8->Name = L"textBox_f_8";
			this->textBox_f_8->ReadOnly = true;
			this->textBox_f_8->Size = System::Drawing::Size(74, 27);
			this->textBox_f_8->TabIndex = 46;
			this->textBox_f_8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_8
			// 
			this->textBox_L2_8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_8->Location = System::Drawing::Point(683, 41);
			this->textBox_L2_8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_8->Name = L"textBox_L2_8";
			this->textBox_L2_8->ReadOnly = true;
			this->textBox_L2_8->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_8->TabIndex = 17;
			this->textBox_L2_8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_8
			// 
			this->textBox_L1_8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_8->Location = System::Drawing::Point(683, 2);
			this->textBox_L1_8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_8->Name = L"textBox_L1_8";
			this->textBox_L1_8->ReadOnly = true;
			this->textBox_L1_8->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_8->TabIndex = 9;
			this->textBox_L1_8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_7
			// 
			this->textBox_L1_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_7->Location = System::Drawing::Point(603, 2);
			this->textBox_L1_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_7->Name = L"textBox_L1_7";
			this->textBox_L1_7->ReadOnly = true;
			this->textBox_L1_7->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_7->TabIndex = 8;
			this->textBox_L1_7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_7
			// 
			this->textBox_L2_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_7->Location = System::Drawing::Point(603, 41);
			this->textBox_L2_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_7->Name = L"textBox_L2_7";
			this->textBox_L2_7->ReadOnly = true;
			this->textBox_L2_7->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_7->TabIndex = 16;
			this->textBox_L2_7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_7
			// 
			this->textBox_f_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_7->Location = System::Drawing::Point(603, 80);
			this->textBox_f_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_7->Name = L"textBox_f_7";
			this->textBox_f_7->ReadOnly = true;
			this->textBox_f_7->Size = System::Drawing::Size(74, 27);
			this->textBox_f_7->TabIndex = 45;
			this->textBox_f_7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С7
			// 
			this->textBox_С7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С7->Location = System::Drawing::Point(603, 119);
			this->textBox_С7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С7->Name = L"textBox_С7";
			this->textBox_С7->ReadOnly = true;
			this->textBox_С7->Size = System::Drawing::Size(74, 27);
			this->textBox_С7->TabIndex = 54;
			this->textBox_С7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc7
			// 
			this->textBox_Z_calc7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc7->Location = System::Drawing::Point(603, 158);
			this->textBox_Z_calc7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc7->Name = L"textBox_Z_calc7";
			this->textBox_Z_calc7->ReadOnly = true;
			this->textBox_Z_calc7->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc7->TabIndex = 62;
			this->textBox_Z_calc7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_6
			// 
			this->textBox_L1_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_6->Location = System::Drawing::Point(523, 2);
			this->textBox_L1_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_6->Name = L"textBox_L1_6";
			this->textBox_L1_6->ReadOnly = true;
			this->textBox_L1_6->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_6->TabIndex = 7;
			this->textBox_L1_6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_5
			// 
			this->textBox_L1_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_5->Location = System::Drawing::Point(443, 2);
			this->textBox_L1_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_5->Name = L"textBox_L1_5";
			this->textBox_L1_5->ReadOnly = true;
			this->textBox_L1_5->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_5->TabIndex = 6;
			this->textBox_L1_5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_6
			// 
			this->textBox_L2_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_6->Location = System::Drawing::Point(523, 41);
			this->textBox_L2_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_6->Name = L"textBox_L2_6";
			this->textBox_L2_6->ReadOnly = true;
			this->textBox_L2_6->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_6->TabIndex = 15;
			this->textBox_L2_6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_4
			// 
			this->textBox_L1_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_4->Location = System::Drawing::Point(363, 2);
			this->textBox_L1_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_4->Name = L"textBox_L1_4";
			this->textBox_L1_4->ReadOnly = true;
			this->textBox_L1_4->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_4->TabIndex = 5;
			this->textBox_L1_4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_6
			// 
			this->textBox_f_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_6->Location = System::Drawing::Point(523, 80);
			this->textBox_f_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_6->Name = L"textBox_f_6";
			this->textBox_f_6->ReadOnly = true;
			this->textBox_f_6->Size = System::Drawing::Size(74, 27);
			this->textBox_f_6->TabIndex = 44;
			this->textBox_f_6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_3
			// 
			this->textBox_L1_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_3->Location = System::Drawing::Point(283, 2);
			this->textBox_L1_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_3->Name = L"textBox_L1_3";
			this->textBox_L1_3->ReadOnly = true;
			this->textBox_L1_3->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_3->TabIndex = 4;
			this->textBox_L1_3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С6
			// 
			this->textBox_С6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С6->Location = System::Drawing::Point(523, 119);
			this->textBox_С6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С6->Name = L"textBox_С6";
			this->textBox_С6->ReadOnly = true;
			this->textBox_С6->Size = System::Drawing::Size(74, 27);
			this->textBox_С6->TabIndex = 53;
			this->textBox_С6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_2
			// 
			this->textBox_L1_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_2->Location = System::Drawing::Point(203, 2);
			this->textBox_L1_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_2->Name = L"textBox_L1_2";
			this->textBox_L1_2->ReadOnly = true;
			this->textBox_L1_2->Size = System::Drawing::Size(74, 27);
			this->textBox_L1_2->TabIndex = 3;
			this->textBox_L1_2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc6
			// 
			this->textBox_Z_calc6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc6->Location = System::Drawing::Point(523, 158);
			this->textBox_Z_calc6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc6->Name = L"textBox_Z_calc6";
			this->textBox_Z_calc6->ReadOnly = true;
			this->textBox_Z_calc6->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc6->TabIndex = 61;
			this->textBox_Z_calc6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_5
			// 
			this->textBox_L2_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_5->Location = System::Drawing::Point(443, 41);
			this->textBox_L2_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_5->Name = L"textBox_L2_5";
			this->textBox_L2_5->ReadOnly = true;
			this->textBox_L2_5->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_5->TabIndex = 14;
			this->textBox_L2_5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_4
			// 
			this->textBox_L2_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_4->Location = System::Drawing::Point(363, 41);
			this->textBox_L2_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_4->Name = L"textBox_L2_4";
			this->textBox_L2_4->ReadOnly = true;
			this->textBox_L2_4->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_4->TabIndex = 13;
			this->textBox_L2_4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_2
			// 
			this->textBox_L2_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_2->Location = System::Drawing::Point(203, 41);
			this->textBox_L2_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_2->Name = L"textBox_L2_2";
			this->textBox_L2_2->ReadOnly = true;
			this->textBox_L2_2->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_2->TabIndex = 11;
			this->textBox_L2_2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_2
			// 
			this->textBox_f_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_2->Location = System::Drawing::Point(203, 80);
			this->textBox_f_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_2->Name = L"textBox_f_2";
			this->textBox_f_2->ReadOnly = true;
			this->textBox_f_2->Size = System::Drawing::Size(74, 27);
			this->textBox_f_2->TabIndex = 40;
			this->textBox_f_2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_3
			// 
			this->textBox_L2_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_3->Location = System::Drawing::Point(283, 41);
			this->textBox_L2_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_3->Name = L"textBox_L2_3";
			this->textBox_L2_3->ReadOnly = true;
			this->textBox_L2_3->Size = System::Drawing::Size(74, 27);
			this->textBox_L2_3->TabIndex = 12;
			this->textBox_L2_3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_5
			// 
			this->textBox_f_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_5->Location = System::Drawing::Point(443, 80);
			this->textBox_f_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_5->Name = L"textBox_f_5";
			this->textBox_f_5->ReadOnly = true;
			this->textBox_f_5->Size = System::Drawing::Size(74, 27);
			this->textBox_f_5->TabIndex = 43;
			this->textBox_f_5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_4
			// 
			this->textBox_f_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_4->Location = System::Drawing::Point(363, 80);
			this->textBox_f_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_4->Name = L"textBox_f_4";
			this->textBox_f_4->ReadOnly = true;
			this->textBox_f_4->Size = System::Drawing::Size(74, 27);
			this->textBox_f_4->TabIndex = 42;
			this->textBox_f_4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С2
			// 
			this->textBox_С2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С2->Location = System::Drawing::Point(203, 119);
			this->textBox_С2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С2->Name = L"textBox_С2";
			this->textBox_С2->ReadOnly = true;
			this->textBox_С2->Size = System::Drawing::Size(74, 27);
			this->textBox_С2->TabIndex = 49;
			this->textBox_С2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc2
			// 
			this->textBox_Z_calc2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc2->Location = System::Drawing::Point(203, 158);
			this->textBox_Z_calc2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc2->Name = L"textBox_Z_calc2";
			this->textBox_Z_calc2->ReadOnly = true;
			this->textBox_Z_calc2->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc2->TabIndex = 57;
			this->textBox_Z_calc2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С5
			// 
			this->textBox_С5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С5->Location = System::Drawing::Point(443, 119);
			this->textBox_С5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С5->Name = L"textBox_С5";
			this->textBox_С5->ReadOnly = true;
			this->textBox_С5->Size = System::Drawing::Size(74, 27);
			this->textBox_С5->TabIndex = 52;
			this->textBox_С5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С4
			// 
			this->textBox_С4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С4->Location = System::Drawing::Point(363, 119);
			this->textBox_С4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С4->Name = L"textBox_С4";
			this->textBox_С4->ReadOnly = true;
			this->textBox_С4->Size = System::Drawing::Size(74, 27);
			this->textBox_С4->TabIndex = 51;
			this->textBox_С4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_3
			// 
			this->textBox_f_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_3->Location = System::Drawing::Point(283, 80);
			this->textBox_f_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_3->Name = L"textBox_f_3";
			this->textBox_f_3->ReadOnly = true;
			this->textBox_f_3->Size = System::Drawing::Size(74, 27);
			this->textBox_f_3->TabIndex = 41;
			this->textBox_f_3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С3
			// 
			this->textBox_С3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С3->Location = System::Drawing::Point(283, 119);
			this->textBox_С3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С3->Name = L"textBox_С3";
			this->textBox_С3->ReadOnly = true;
			this->textBox_С3->Size = System::Drawing::Size(74, 27);
			this->textBox_С3->TabIndex = 50;
			this->textBox_С3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc5
			// 
			this->textBox_Z_calc5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc5->Location = System::Drawing::Point(443, 158);
			this->textBox_Z_calc5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc5->Name = L"textBox_Z_calc5";
			this->textBox_Z_calc5->ReadOnly = true;
			this->textBox_Z_calc5->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc5->TabIndex = 60;
			this->textBox_Z_calc5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc4
			// 
			this->textBox_Z_calc4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc4->Location = System::Drawing::Point(363, 158);
			this->textBox_Z_calc4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc4->Name = L"textBox_Z_calc4";
			this->textBox_Z_calc4->ReadOnly = true;
			this->textBox_Z_calc4->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc4->TabIndex = 59;
			this->textBox_Z_calc4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc3
			// 
			this->textBox_Z_calc3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc3->Location = System::Drawing::Point(283, 158);
			this->textBox_Z_calc3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc3->Name = L"textBox_Z_calc3";
			this->textBox_Z_calc3->ReadOnly = true;
			this->textBox_Z_calc3->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_calc3->TabIndex = 58;
			this->textBox_Z_calc3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_loop1
			// 
			this->textBox_Z_loop1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop1->Location = System::Drawing::Point(123, 275);
			this->textBox_Z_loop1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop1->Name = L"textBox_Z_loop1";
			this->textBox_Z_loop1->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop1->TabIndex = 66;
			this->textBox_Z_loop1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop1_KeyUp);
			// 
			// textBox_Z_loop2
			// 
			this->textBox_Z_loop2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop2->Location = System::Drawing::Point(203, 275);
			this->textBox_Z_loop2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop2->Name = L"textBox_Z_loop2";
			this->textBox_Z_loop2->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop2->TabIndex = 67;
			this->textBox_Z_loop2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop2->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop2_KeyUp);
			// 
			// textBox_Z_loop3
			// 
			this->textBox_Z_loop3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop3->Location = System::Drawing::Point(283, 275);
			this->textBox_Z_loop3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop3->Name = L"textBox_Z_loop3";
			this->textBox_Z_loop3->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop3->TabIndex = 68;
			this->textBox_Z_loop3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop3->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop3_KeyUp);
			// 
			// textBox_Z_loop4
			// 
			this->textBox_Z_loop4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop4->Location = System::Drawing::Point(363, 275);
			this->textBox_Z_loop4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop4->Name = L"textBox_Z_loop4";
			this->textBox_Z_loop4->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop4->TabIndex = 69;
			this->textBox_Z_loop4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop4->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop4_KeyUp);
			// 
			// textBox_Z_loop5
			// 
			this->textBox_Z_loop5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop5->Location = System::Drawing::Point(443, 275);
			this->textBox_Z_loop5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop5->Name = L"textBox_Z_loop5";
			this->textBox_Z_loop5->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop5->TabIndex = 70;
			this->textBox_Z_loop5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop5->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop5->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop5_KeyUp);
			// 
			// textBox_Z_loop6
			// 
			this->textBox_Z_loop6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop6->Location = System::Drawing::Point(523, 275);
			this->textBox_Z_loop6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop6->Name = L"textBox_Z_loop6";
			this->textBox_Z_loop6->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop6->TabIndex = 71;
			this->textBox_Z_loop6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop6->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop6->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop6_KeyUp);
			// 
			// textBox_Z_loop7
			// 
			this->textBox_Z_loop7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop7->Location = System::Drawing::Point(603, 275);
			this->textBox_Z_loop7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop7->Name = L"textBox_Z_loop7";
			this->textBox_Z_loop7->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop7->TabIndex = 72;
			this->textBox_Z_loop7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop7->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop7->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop7_KeyUp);
			// 
			// textBox_Z_loop8
			// 
			this->textBox_Z_loop8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop8->Location = System::Drawing::Point(683, 275);
			this->textBox_Z_loop8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop8->Name = L"textBox_Z_loop8";
			this->textBox_Z_loop8->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop8->TabIndex = 73;
			this->textBox_Z_loop8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop8->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop8->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop8_KeyUp);
			// 
			// textBox_Z_loop9
			// 
			this->textBox_Z_loop9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop9->Location = System::Drawing::Point(763, 275);
			this->textBox_Z_loop9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop9->Name = L"textBox_Z_loop9";
			this->textBox_Z_loop9->Size = System::Drawing::Size(74, 27);
			this->textBox_Z_loop9->TabIndex = 74;
			this->textBox_Z_loop9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop9->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop9->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop9_KeyUp);
			// 
			// textBox_Z_loop10
			// 
			this->textBox_Z_loop10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop10->Location = System::Drawing::Point(843, 275);
			this->textBox_Z_loop10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop10->Name = L"textBox_Z_loop10";
			this->textBox_Z_loop10->Size = System::Drawing::Size(77, 27);
			this->textBox_Z_loop10->TabIndex = 74;
			this->textBox_Z_loop10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop10->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &WorkForm::digit2);
			this->textBox_Z_loop10->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &WorkForm::textBox_Z_loop10_KeyUp);
			// 
			// button_Z1
			// 
			this->button_Z1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z1->Location = System::Drawing::Point(130, 239);
			this->button_Z1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z1->Name = L"button_Z1";
			this->button_Z1->Size = System::Drawing::Size(60, 28);
			this->button_Z1->TabIndex = 18;
			this->button_Z1->Text = L"Z1";
			this->button_Z1->UseVisualStyleBackColor = true;
			this->button_Z1->Click += gcnew System::EventHandler(this, &WorkForm::button_Z1_Click);
			// 
			// button_Z2
			// 
			this->button_Z2->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z2->Location = System::Drawing::Point(210, 239);
			this->button_Z2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z2->Name = L"button_Z2";
			this->button_Z2->Size = System::Drawing::Size(60, 28);
			this->button_Z2->TabIndex = 19;
			this->button_Z2->Text = L"Z2";
			this->button_Z2->UseVisualStyleBackColor = true;
			this->button_Z2->Click += gcnew System::EventHandler(this, &WorkForm::button_Z2_Click);
			// 
			// button_Z3
			// 
			this->button_Z3->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z3->Location = System::Drawing::Point(290, 239);
			this->button_Z3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z3->Name = L"button_Z3";
			this->button_Z3->Size = System::Drawing::Size(60, 28);
			this->button_Z3->TabIndex = 20;
			this->button_Z3->Text = L"Z3";
			this->button_Z3->UseVisualStyleBackColor = true;
			this->button_Z3->Click += gcnew System::EventHandler(this, &WorkForm::button_Z3_Click);
			// 
			// button_Z4
			// 
			this->button_Z4->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z4->Location = System::Drawing::Point(370, 238);
			this->button_Z4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z4->Name = L"button_Z4";
			this->button_Z4->Size = System::Drawing::Size(60, 31);
			this->button_Z4->TabIndex = 21;
			this->button_Z4->Text = L"Z4";
			this->button_Z4->UseVisualStyleBackColor = true;
			this->button_Z4->Click += gcnew System::EventHandler(this, &WorkForm::button_Z4_Click);
			// 
			// button_Z5
			// 
			this->button_Z5->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z5->Location = System::Drawing::Point(450, 236);
			this->button_Z5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z5->Name = L"button_Z5";
			this->button_Z5->Size = System::Drawing::Size(60, 34);
			this->button_Z5->TabIndex = 22;
			this->button_Z5->Text = L"Z5";
			this->button_Z5->UseVisualStyleBackColor = true;
			this->button_Z5->Click += gcnew System::EventHandler(this, &WorkForm::button_Z5_Click);
			// 
			// button_Z6
			// 
			this->button_Z6->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z6->Location = System::Drawing::Point(530, 237);
			this->button_Z6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z6->Name = L"button_Z6";
			this->button_Z6->Size = System::Drawing::Size(60, 33);
			this->button_Z6->TabIndex = 23;
			this->button_Z6->Text = L"Z6";
			this->button_Z6->UseVisualStyleBackColor = true;
			this->button_Z6->Click += gcnew System::EventHandler(this, &WorkForm::button_Z6_Click);
			// 
			// button_Z7
			// 
			this->button_Z7->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z7->Location = System::Drawing::Point(610, 237);
			this->button_Z7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z7->Name = L"button_Z7";
			this->button_Z7->Size = System::Drawing::Size(60, 32);
			this->button_Z7->TabIndex = 24;
			this->button_Z7->Text = L"Z7";
			this->button_Z7->UseVisualStyleBackColor = true;
			this->button_Z7->Click += gcnew System::EventHandler(this, &WorkForm::button_Z7_Click);
			// 
			// button_Z8
			// 
			this->button_Z8->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z8->Location = System::Drawing::Point(690, 238);
			this->button_Z8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z8->Name = L"button_Z8";
			this->button_Z8->Size = System::Drawing::Size(60, 30);
			this->button_Z8->TabIndex = 25;
			this->button_Z8->Text = L"Z8";
			this->button_Z8->UseVisualStyleBackColor = true;
			this->button_Z8->Click += gcnew System::EventHandler(this, &WorkForm::button_Z8_Click);
			// 
			// button_Z9
			// 
			this->button_Z9->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z9->Location = System::Drawing::Point(770, 238);
			this->button_Z9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z9->Name = L"button_Z9";
			this->button_Z9->Size = System::Drawing::Size(60, 30);
			this->button_Z9->TabIndex = 26;
			this->button_Z9->Text = L"Z9";
			this->button_Z9->UseVisualStyleBackColor = true;
			this->button_Z9->Click += gcnew System::EventHandler(this, &WorkForm::button_Z9_Click);
			// 
			// button_Z10
			// 
			this->button_Z10->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z10->Location = System::Drawing::Point(851, 238);
			this->button_Z10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z10->Name = L"button_Z10";
			this->button_Z10->Size = System::Drawing::Size(60, 30);
			this->button_Z10->TabIndex = 26;
			this->button_Z10->Text = L"Z10";
			this->button_Z10->UseVisualStyleBackColor = true;
			this->button_Z10->Click += gcnew System::EventHandler(this, &WorkForm::button_Z10_Click);
			// 
			// label16
			// 
			this->label16->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label16->Location = System::Drawing::Point(3, 273);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(114, 39);
			this->label16->TabIndex = 65;
			this->label16->Text = L"Re meas";
			this->label16->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// textBox_R_loop3
			// 
			this->textBox_R_loop3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop3->Location = System::Drawing::Point(283, 197);
			this->textBox_R_loop3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop3->Name = L"textBox_R_loop3";
			this->textBox_R_loop3->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop3->TabIndex = 88;
			this->textBox_R_loop3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop4
			// 
			this->textBox_R_loop4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop4->Location = System::Drawing::Point(363, 197);
			this->textBox_R_loop4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop4->Name = L"textBox_R_loop4";
			this->textBox_R_loop4->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop4->TabIndex = 89;
			this->textBox_R_loop4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop5
			// 
			this->textBox_R_loop5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop5->Location = System::Drawing::Point(443, 197);
			this->textBox_R_loop5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop5->Name = L"textBox_R_loop5";
			this->textBox_R_loop5->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop5->TabIndex = 90;
			this->textBox_R_loop5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop6
			// 
			this->textBox_R_loop6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop6->Location = System::Drawing::Point(523, 197);
			this->textBox_R_loop6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop6->Name = L"textBox_R_loop6";
			this->textBox_R_loop6->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop6->TabIndex = 91;
			this->textBox_R_loop6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop7
			// 
			this->textBox_R_loop7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop7->Location = System::Drawing::Point(603, 197);
			this->textBox_R_loop7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop7->Name = L"textBox_R_loop7";
			this->textBox_R_loop7->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop7->TabIndex = 92;
			this->textBox_R_loop7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop8
			// 
			this->textBox_R_loop8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop8->Location = System::Drawing::Point(683, 197);
			this->textBox_R_loop8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop8->Name = L"textBox_R_loop8";
			this->textBox_R_loop8->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop8->TabIndex = 93;
			this->textBox_R_loop8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop9
			// 
			this->textBox_R_loop9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop9->Location = System::Drawing::Point(763, 197);
			this->textBox_R_loop9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop9->Name = L"textBox_R_loop9";
			this->textBox_R_loop9->Size = System::Drawing::Size(74, 27);
			this->textBox_R_loop9->TabIndex = 94;
			this->textBox_R_loop9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_R_loop10
			// 
			this->textBox_R_loop10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_loop10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_loop10->Location = System::Drawing::Point(843, 197);
			this->textBox_R_loop10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_loop10->Name = L"textBox_R_loop10";
			this->textBox_R_loop10->Size = System::Drawing::Size(77, 27);
			this->textBox_R_loop10->TabIndex = 95;
			this->textBox_R_loop10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label18
			// 
			this->label18->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label18->Location = System::Drawing::Point(3, 312);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(114, 39);
			this->label18->TabIndex = 97;
			this->label18->Text = L"Radius";
			this->label18->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label18->Visible = false;
			// 
			// textBox_radius
			// 
			this->textBox_radius->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_radius->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_radius->Location = System::Drawing::Point(123, 314);
			this->textBox_radius->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_radius->Name = L"textBox_radius";
			this->textBox_radius->Size = System::Drawing::Size(74, 27);
			this->textBox_radius->TabIndex = 98;
			this->textBox_radius->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_radius->Visible = false;
			// 
			// label19
			// 
			this->label19->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label19->Location = System::Drawing::Point(283, 312);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(74, 39);
			this->label19->TabIndex = 99;
			this->label19->Text = L"R_wire";
			this->label19->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label19->Visible = false;
			// 
			// textBox_R_wire
			// 
			this->textBox_R_wire->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_wire->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_wire->Location = System::Drawing::Point(363, 314);
			this->textBox_R_wire->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_wire->Name = L"textBox_R_wire";
			this->textBox_R_wire->Size = System::Drawing::Size(74, 27);
			this->textBox_R_wire->TabIndex = 100;
			this->textBox_R_wire->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_R_wire->Visible = false;
			// 
			// button_calk_C_need
			// 
			this->button_calk_C_need->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_calk_C_need->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_calk_C_need->Location = System::Drawing::Point(1021, 155);
			this->button_calk_C_need->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_calk_C_need->Name = L"button_calk_C_need";
			this->button_calk_C_need->Size = System::Drawing::Size(179, 66);
			this->button_calk_C_need->TabIndex = 85;
			this->button_calk_C_need->Text = L"РАССЧИТАТЬ ЕМКОСТИ";
			this->button_calk_C_need->UseVisualStyleBackColor = true;
			this->button_calk_C_need->Click += gcnew System::EventHandler(this, &WorkForm::button_calk_C_need_Click);
			// 
			// button_signature
			// 
			this->button_signature->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_signature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_signature->Location = System::Drawing::Point(861, 434);
			this->button_signature->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_signature->Name = L"button_signature";
			this->button_signature->Size = System::Drawing::Size(137, 59);
			this->button_signature->TabIndex = 84;
			this->button_signature->Text = L"ЗАПИСАТЬ  СИГНАТУРУ";
			this->button_signature->UseVisualStyleBackColor = true;
			this->button_signature->Click += gcnew System::EventHandler(this, &WorkForm::button_signature_Click);
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->button_read_loop_Z);
			this->panel2->Controls->Add(this->button_calc_z_to_loop);
			this->panel2->Controls->Add(this->button_write_Z);
			this->panel2->Controls->Add(this->button_erase);
			this->panel2->Location = System::Drawing::Point(79, 434);
			this->panel2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(749, 59);
			this->panel2->TabIndex = 82;
			// 
			// button_read_loop_Z
			// 
			this->button_read_loop_Z->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_read_loop_Z->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_read_loop_Z->Location = System::Drawing::Point(7, 4);
			this->button_read_loop_Z->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_read_loop_Z->Name = L"button_read_loop_Z";
			this->button_read_loop_Z->Size = System::Drawing::Size(167, 39);
			this->button_read_loop_Z->TabIndex = 76;
			this->button_read_loop_Z->Text = L"ПРОЧИТАТЬ";
			this->button_read_loop_Z->UseVisualStyleBackColor = true;
			this->button_read_loop_Z->Click += gcnew System::EventHandler(this, &WorkForm::button_read_loop_Z_Click);
			// 
			// button_calc_z_to_loop
			// 
			this->button_calc_z_to_loop->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_calc_z_to_loop->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->button_calc_z_to_loop->Location = System::Drawing::Point(477, 4);
			this->button_calc_z_to_loop->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_calc_z_to_loop->Name = L"button_calc_z_to_loop";
			this->button_calc_z_to_loop->Size = System::Drawing::Size(269, 39);
			this->button_calc_z_to_loop->TabIndex = 81;
			this->button_calc_z_to_loop->Text = L"записать_Z_расчетное";
			this->button_calc_z_to_loop->UseVisualStyleBackColor = true;
			this->button_calc_z_to_loop->Click += gcnew System::EventHandler(this, &WorkForm::button_calc_z_to_loop_Click);
			// 
			// button_write_Z
			// 
			this->button_write_Z->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_write_Z->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_write_Z->Location = System::Drawing::Point(181, 4);
			this->button_write_Z->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_write_Z->Name = L"button_write_Z";
			this->button_write_Z->Size = System::Drawing::Size(141, 39);
			this->button_write_Z->TabIndex = 79;
			this->button_write_Z->Text = L"ЗАПИСАТЬ ";
			this->button_write_Z->UseVisualStyleBackColor = true;
			this->button_write_Z->Click += gcnew System::EventHandler(this, &WorkForm::button_write_Z_Click);
			// 
			// button_erase
			// 
			this->button_erase->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_erase->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_erase->Location = System::Drawing::Point(329, 4);
			this->button_erase->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_erase->Name = L"button_erase";
			this->button_erase->Size = System::Drawing::Size(141, 39);
			this->button_erase->TabIndex = 80;
			this->button_erase->Text = L"СТЕРЕТЬ";
			this->button_erase->UseVisualStyleBackColor = true;
			this->button_erase->Click += gcnew System::EventHandler(this, &WorkForm::button_erase_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(1373, 646);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(44, 45);
			this->richTextBox1->TabIndex = 44;
			this->richTextBox1->Text = L"";
			// 
			// button_calk
			// 
			this->button_calk->Location = System::Drawing::Point(1756, 887);
			this->button_calk->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_calk->Name = L"button_calk";
			this->button_calk->Size = System::Drawing::Size(140, 46);
			this->button_calk->TabIndex = 34;
			this->button_calk->Text = L"РАССЧИТАТЬ";
			this->button_calk->UseVisualStyleBackColor = true;
			this->button_calk->Click += gcnew System::EventHandler(this, &WorkForm::button_calk_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &WorkForm::timer1_Tick);
			// 
			// comboBox_sonde_com
			// 
			this->comboBox_sonde_com->FormattingEnabled = true;
			this->comboBox_sonde_com->Location = System::Drawing::Point(291, 62);
			this->comboBox_sonde_com->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox_sonde_com->Name = L"comboBox_sonde_com";
			this->comboBox_sonde_com->Size = System::Drawing::Size(100, 24);
			this->comboBox_sonde_com->TabIndex = 35;
			this->toolTip1->SetToolTip(this->comboBox_sonde_com, L"Установка СОМ порта подключения прибора.");
			this->comboBox_sonde_com->DropDown += gcnew System::EventHandler(this, &WorkForm::comboBox_sonde_com_DropDown);
			this->comboBox_sonde_com->SelectedValueChanged += gcnew System::EventHandler(this, &WorkForm::comboBox_sonde_com_SelectedValueChanged);
			this->comboBox_sonde_com->TextChanged += gcnew System::EventHandler(this, &WorkForm::comboBox_sonde_com_SelectedValueChanged);
			// 
			// comboBox_loop_com
			// 
			this->comboBox_loop_com->FormattingEnabled = true;
			this->comboBox_loop_com->Location = System::Drawing::Point(405, 62);
			this->comboBox_loop_com->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox_loop_com->Name = L"comboBox_loop_com";
			this->comboBox_loop_com->Size = System::Drawing::Size(100, 24);
			this->comboBox_loop_com->TabIndex = 36;
			this->toolTip1->SetToolTip(this->comboBox_loop_com, L"Установка исходящего СОМ порта на Blurtooth соединении для кольца.");
			this->comboBox_loop_com->DropDown += gcnew System::EventHandler(this, &WorkForm::comboBox_loop_com_DropDown);
			this->comboBox_loop_com->SelectedValueChanged += gcnew System::EventHandler(this, &WorkForm::comboBox_loop_com_SelectedValueChanged);
			this->comboBox_loop_com->TextChanged += gcnew System::EventHandler(this, &WorkForm::comboBox_loop_com_SelectedValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(293, 42);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(66, 17);
			this->label1->TabIndex = 37;
			this->label1->Text = L"ПРИБОР";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(411, 42);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(69, 17);
			this->label2->TabIndex = 38;
			this->label2->Text = L"КОЛЬЦО";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(355, 16);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 17);
			this->label3->TabIndex = 39;
			this->label3->Text = L"COM PORT";
			// 
			// comboBox_aver
			// 
			this->comboBox_aver->FormattingEnabled = true;
			this->comboBox_aver->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"5", L"10", L"15", L"20" });
			this->comboBox_aver->Location = System::Drawing::Point(520, 62);
			this->comboBox_aver->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox_aver->Name = L"comboBox_aver";
			this->comboBox_aver->Size = System::Drawing::Size(100, 24);
			this->comboBox_aver->TabIndex = 40;
			this->toolTip1->SetToolTip(this->comboBox_aver, L"Количество измерений для усреднения.\r\nЕсли наблюдается большой разбос значений \r\n"
				L"на высоких сопротивлениях - увеличте.");
			this->comboBox_aver->SelectedIndexChanged += gcnew System::EventHandler(this, &WorkForm::comboBox_aver_SelectedValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(517, 42);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(103, 17);
			this->label4->TabIndex = 41;
			this->label4->Text = L"УСРЕДНЕНИЕ";
			// 
			// checkBox_express
			// 
			this->checkBox_express->AutoSize = true;
			this->checkBox_express->Location = System::Drawing::Point(739, 62);
			this->checkBox_express->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox_express->Name = L"checkBox_express";
			this->checkBox_express->Size = System::Drawing::Size(103, 21);
			this->checkBox_express->TabIndex = 42;
			this->checkBox_express->Text = L"ЭКСПРЕСС";
			this->toolTip1->SetToolTip(this->checkBox_express, L"Быстрые измерения на сопротивлениях от 20 ом и выше.\r\nВключено по умолчанию. При "
				L"отключеном увеличивается точность \r\nи возрастает время измерений.");
			this->checkBox_express->UseVisualStyleBackColor = true;
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->сохранитьРапортRTFToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(5, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1563, 28);
			this->menuStrip1->TabIndex = 43;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// сохранитьРапортRTFToolStripMenuItem
			// 
			this->сохранитьРапортRTFToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->uploadToFTPToolStripMenuItem,
					this->работаСФайломМетрологииToolStripMenuItem, this->пОМОЩЬToolStripMenuItem
			});
			this->сохранитьРапортRTFToolStripMenuItem->Name = L"сохранитьРапортRTFToolStripMenuItem";
			this->сохранитьРапортRTFToolStripMenuItem->Size = System::Drawing::Size(68, 24);
			this->сохранитьРапортRTFToolStripMenuItem->Text = L"МЕНЮ";
			// 
			// uploadToFTPToolStripMenuItem
			// 
			this->uploadToFTPToolStripMenuItem->Name = L"uploadToFTPToolStripMenuItem";
			this->uploadToFTPToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->uploadToFTPToolStripMenuItem->Text = L"Upload to FTP";
			this->uploadToFTPToolStripMenuItem->Click += gcnew System::EventHandler(this, &WorkForm::uploadToFTPToolStripMenuItem_Click);
			// 
			// работаСФайломМетрологииToolStripMenuItem
			// 
			this->работаСФайломМетрологииToolStripMenuItem->Name = L"работаСФайломМетрологииToolStripMenuItem";
			this->работаСФайломМетрологииToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->работаСФайломМетрологииToolStripMenuItem->Text = L"Работа с файлом метрологии";
			this->работаСФайломМетрологииToolStripMenuItem->Click += gcnew System::EventHandler(this, &WorkForm::работаСФайломМетрологииToolStripMenuItem_Click);
			// 
			// пОМОЩЬToolStripMenuItem
			// 
			this->пОМОЩЬToolStripMenuItem->Name = L"пОМОЩЬToolStripMenuItem";
			this->пОМОЩЬToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->пОМОЩЬToolStripMenuItem->Text = L"ПОМОЩЬ";
			this->пОМОЩЬToolStripMenuItem->Click += gcnew System::EventHandler(this, &WorkForm::пОМОЩЬToolStripMenuItem_Click);
			// 
			// comboBox_trashhold
			// 
			this->comboBox_trashhold->FormattingEnabled = true;
			this->comboBox_trashhold->Items->AddRange(gcnew cli::array< System::Object^  >(6) { L"5", L"6", L"7", L"8", L"9", L"10" });
			this->comboBox_trashhold->Location = System::Drawing::Point(635, 62);
			this->comboBox_trashhold->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox_trashhold->Name = L"comboBox_trashhold";
			this->comboBox_trashhold->Size = System::Drawing::Size(80, 24);
			this->comboBox_trashhold->TabIndex = 44;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(632, 42);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(73, 17);
			this->label9->TabIndex = 45;
			this->label9->Text = L"ПОРОГ %";
			// 
			// comboBox_tool_type
			// 
			this->comboBox_tool_type->FormattingEnabled = true;
			this->comboBox_tool_type->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"LWD_1DDS", L"SDR", L"AUTONOM" });
			this->comboBox_tool_type->Location = System::Drawing::Point(123, 62);
			this->comboBox_tool_type->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox_tool_type->Name = L"comboBox_tool_type";
			this->comboBox_tool_type->Size = System::Drawing::Size(121, 24);
			this->comboBox_tool_type->TabIndex = 47;
			this->comboBox_tool_type->SelectedIndexChanged += gcnew System::EventHandler(this, &WorkForm::comboBox_tool_type_SelectedIndexChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(125, 42);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(108, 17);
			this->label10->TabIndex = 48;
			this->label10->Text = L"ТИП ПРИБОРА";
			// 
			// toolTip1
			// 
			this->toolTip1->AutoPopDelay = 10000;
			this->toolTip1->InitialDelay = 500;
			this->toolTip1->ReshowDelay = 100;
			// 
			// WorkForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1563, 752);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->comboBox_tool_type);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->comboBox_trashhold);
			this->Controls->Add(this->checkBox_express);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_aver);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button_calk);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox_loop_com);
			this->Controls->Add(this->comboBox_sonde_com);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"WorkForm";
			this->Text = L"WorkForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &WorkForm::WorkForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &WorkForm::MyForm_Load);
			this->panel1->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_legend_up))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_graf10))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tableLayoutPanel11->ResumeLayout(false);
			this->tableLayoutPanel11->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

    // инициализация 
#define AIR_MODE 0
#define MEAS_MODE 1
	int tool_type = 0;

	const char* ConfName = "Conf.txt";
	Thread^ Sonde_Read_Thread = gcnew Thread(gcnew ThreadStart(this, &WorkForm::Sonde_Read));
	Thread^ Loop_Work_Thread =  gcnew Thread(gcnew ThreadStart(this, &WorkForm::Loop_work ));
	Thread^ Loop_Check_Thread = gcnew Thread(gcnew ThreadStart(this, &WorkForm::Loop_check));
	private:  System::Void  Loop_work() { ; }

	bool bt_on = false;
	bool on_off = false;
	bool collect_air_data = false;
	bool local_air_is_set = false;
	bool local_air_reset = false;
	bool start_R_measure = false;
	bool end_R_measure = false;
	bool Z_downloaded_from_loop = false;
	bool PH_Ro_is_calculated = false;
	bool auto_vork;
	int auto_R_IDX = 1;

	bool start = 0; const char* update = "err";
	uint32_t sonde_kojuh_serial = 0;
	int air_data_aver = 20;
	int trashhold = 5;
	int N_R_for_express = 2;//до какого сопротивления измеряем на всех конденсаторах
	//bool express = false;
	int R_IDX = 0;
	int C_IDX = 0;
	int N_averaging = 10;
	int aver_cntr = 0;
	int skip_cntr = 0;
	int skip = 2;
	int global_cntr = 0;
	int air_set_cntr = 0;
	
	
	//функции

	
void do_legend( int trashhold, int mode ) {
	pictureBox_legend_up;
	int width = pictureBox_legend_up->Width / 6;//ширина 5%
	int height = pictureBox_legend_up->Height;//высота полоски
	Bitmap^ bmp = gcnew Bitmap(6 * width, height, PixelFormat::Format32bppArgb);
	Graphics ^ g = Graphics::FromImage(bmp);
	String^ Text = "";
	Brush^ Кисть = gcnew SolidBrush(Color::Black);
	Font  = gcnew System::Drawing::Font("Microsoft Sans Serif", 8, FontStyle::Regular);
	//air
	if (mode == AIR_MODE) {
		Text = "-90mG";
		g->DrawString(Text, Font, Кисть, 0, 0); // Координаты размещения текста
		Text = "-60mG";
		g->DrawString(Text, Font, Кисть, width + 2, 0);
		Text = "-30mG";
		g->DrawString(Text, Font, Кисть, 2 * width + 2, 0);
		Text = "30mG";
		g->DrawString(Text, Font, Кисть, 4 * width - 30, 0);
		Text = Text = "60mG";
		g->DrawString(Text, Font, Кисть, 5 * width - 30, 0);
		Text = Text = "90mG";
		g->DrawString(Text, Font, Кисть, 6 * width - 30, 0);
	}
	if (mode == MEAS_MODE) {
		Text = "-" + 3 * trashhold + "%";
		g->DrawString(Text, Font, Кисть, 0, 0); // Координаты размещения текста
		Text = "-" + 2 * trashhold + "%";
		g->DrawString(Text, Font, Кисть, width + 2, 0);
		Text = "-" + 1 * trashhold + "%";
		g->DrawString(Text, Font, Кисть, 2 * width + 2, 0);
		Text = "" + 1 * trashhold + "%";
		g->DrawString(Text, Font, Кисть, 4 * width - 20, 0);
		Text = Text = "" + 2 * trashhold + "%";
		g->DrawString(Text, Font, Кисть, 5 * width - 24, 0);
		Text = Text = "" + 3 * trashhold + "%";
		g->DrawString(Text, Font, Кисть, 6 * width - 24, 0);
	}
	
	

	pictureBox_legend_up->Refresh();
	pictureBox_legend_up->Image = bmp;
		
}
	
void do_graf(int N_graf, int *X, int N, int trashhold) {
	if (N  > 0 && N_graf > 0 &&  N_graf < 11 ) {
		//if (N_graf == 5)
			//cout << "N_graf " << N_graf << endl;;
		//if (N_graf == 5)
			//cout << "N last points " << N << endl;
		// граф выводит первые N  процентные разницы из массива Х 
		//trashhold - процентный порог цвет
		extern vector <V10> PH_computed;
		extern vector <V10> Ro_computed;
		extern int PH_percent_from_sonde[10][20];
		extern int Ro_percent_from_sonde[10][20];
		extern vector <SONDE_PARAM> sonde_param;
		PictureBox^ GRAF = (PictureBox^)this->tableLayoutPanel1->Controls["pictureBox_graf" + (N_graf).ToString()];
		int cursor_width = 5;
		int width = GRAF->Width / 6;//ширина 5%
		int height = GRAF->Height;//высота полоски
		Bitmap^ bmp = gcnew Bitmap(6 * width, height, PixelFormat::Format32bppArgb);
		Graphics ^ g = Graphics::FromImage(bmp);
		Brush ^ Заливка;
		//если не воздух заливаем цветом	
		//if (R_IDX != 0 && C_IDX != 0) {
		Заливка = gcnew SolidBrush(Color::Red);
		g->FillRectangle(Заливка, 0, 0, width, height);
		Заливка = gcnew SolidBrush(Color::Yellow);
		g->FillRectangle(Заливка, width + 1, 0, width, height);
		Заливка = gcnew SolidBrush(Color::LimeGreen);
		g->FillRectangle(Заливка, 2 * width + 1, 0, 2 * width, height);
		Заливка = gcnew SolidBrush(Color::Yellow);
		g->FillRectangle(Заливка, 4 * width + 1, 0, width, height);
		Заливка = gcnew SolidBrush(Color::Red);
		g->FillRectangle(Заливка, 5 * width + 1, 0, width, height);
		//}

		////////////////////////////////////////////////////////
		//среднее выводимых	процентов			
		float summ = 0;
		
		for (int i = 0; i < N; i++) {
			summ += X[i];
			int x = width * X[i] / trashhold;
			if (x < -3 * width) x = -3 * width;
			if (x > 3 * width) x = 3 * width - 3;
			//серым выводим N измерений
			Заливка = gcnew SolidBrush(Color::RoyalBlue);
			if (R_IDX != 0 && C_IDX != 0)
				g->FillRectangle(Заливка, x + 3 * width, 0, cursor_width / 1, height);
			//if (N_graf == 5)
				//cout << i << " " << X[i] << endl;
		}
		summ /= (N);
		//if (N_graf == 5)
			//cout << " S " << summ << endl;
		//if (N_graf == 5)
			//cout << endl;

		int x_aver = width * summ / trashhold;
		if (x_aver <= -3 * width) x_aver = -3 * width;
		if (x_aver >= 3 * width) x_aver = 3 * width - 5;
		Заливка = gcnew SolidBrush(Color::Black);
		//черным выводим  среднее
		g->FillRectangle(Заливка, x_aver + 3*width, 0, cursor_width, height);

		GRAF->Refresh();
		GRAF->Image = bmp;

		//выводим номера передатчиков
		Label^ LABEL_T = (Label^)this->tableLayoutPanel1->Controls["labelT" + (N_graf).ToString()];
		if (N_graf <= sonde_param.size() / 2) {
			LABEL_T->Text = "T" + N_graf + "  " + (sonde_param[N_graf - 1].F / 1000).ToString("F0");
		}
		else if (N_graf > sonde_param.size() / 2 && N_graf <= sonde_param.size()) {
			LABEL_T->Text = "T" + (N_graf - sonde_param.size() / 2) + "  " + (sonde_param[N_graf - 1].F / 1000).ToString("F0");
		}

        //выводим проценты для R кольца
		if (R_IDX != 0) {
			TextBox^ TEXTBOX_R_T = (TextBox^)this->tableLayoutPanel1->Controls["textBox_R" + R_IDX + "_T" + (N_graf).ToString()];
			if (fabs(summ) <= float(trashhold))
				TEXTBOX_R_T->ForeColor = System::Drawing::Color::LimeGreen;
			if (fabs(summ) > float(trashhold) && fabs(summ) <= float(2 * trashhold))
				TEXTBOX_R_T->ForeColor = System::Drawing::Color::Gold;
			if (fabs(summ) > float(2 * trashhold) && fabs(summ) <= float(3 * trashhold))
				TEXTBOX_R_T->ForeColor = System::Drawing::Color::Red;
			if (fabs(summ) > float(3 * trashhold))
				TEXTBOX_R_T->ForeColor = System::Drawing::Color::DarkRed;
			TEXTBOX_R_T->Text = summ.ToString("F1");
			//TEXTBOX_R_T->Text = (PH_computed[R_IDX].VALUE[N_graf - 1] * mG).ToString("F0");

		}
	}
}

private: System::Void button_ON_OFF_Click(System::Object^  sender, System::EventArgs^  e) {		
	
	if (tool_type == AUTONOM) {
		int stop = 0, stop_ = 0;
		extern string Com_LWD_Name;
		extern vector <SONDE_PARAM> sonde_param;
		//длина вектора равна количеству сопротивлений
		extern vector <V10> PH_computed;
		extern vector <V10> Ro_computed;
		//длина вектора равна количеству зондов
		//отладка
		extern vector <LOOP_PARAM> loop_param;

		if (on_off == false) {
			AMK_DLL = LoadLibrary(TEXT("AMK_1DDS_AUTONOM.dll"));// Загружаем библиотеку EMK_90_1dds
			GP_TOOL_METROLOGY metrology_struct;
			_controlfp(_CW_DEFAULT, 0xfffff);
			AMK_StartComPort = (StartComPort)GetProcAddress(AMK_DLL, "AMK_StartComPort");
			AMK_GetCurrState = (GetCurrState)GetProcAddress(AMK_DLL, "AMK_GetCurrState");
			AMK_Init = (Init)GetProcAddress(AMK_DLL, "AMK_Init");
			AMK_BeginCyclogram = (BeginCyclogram)GetProcAddress(AMK_DLL, "AMK_BeginCyclogram");
			AMK_GetData = (GetData)GetProcAddress(AMK_DLL, "AMK_GetData");
			AMK_SetIdle = (SetIdle)GetProcAddress(AMK_DLL, "AMK_SetIdle");
			AMK_PwrOn = (PwrOn)GetProcAddress(AMK_DLL, "AMK_PwrOn");
			AMK_PwrOff = (PwrOff)GetProcAddress(AMK_DLL, "AMK_PwrOff");
			AMK_DeleteComPort = (DeleteComPort)GetProcAddress(AMK_DLL, "AMK_DeleteComPort");
			//AMK_CheckConnect = (CheckConnect)GetProcAddress(AMK_DLL, "AMK_CheckConnect");
			AMK_StopDDS3 = (StopDDS3)GetProcAddress(AMK_DLL, "AMK_StopDDS3");
			AMK_ResumeDDS3 = (ResumeDDS3)GetProcAddress(AMK_DLL, "AMK_ResumeDDS3");
			AMK_SetIQPhaseOffset = (SetIQPhaseOffset)GetProcAddress(AMK_DLL, "AMK_SetIQPhaseOffset");
			AMK_DDS3FreqOffset = (DDS3FreqOffset)GetProcAddress(AMK_DLL, "AMK_DDS3FreqOffset");
			AMK_GetSerial = (GetSerial)GetProcAddress(AMK_DLL, "AMK_GetSerial");
			AMK_GetMetroData = (GetMetroData)GetProcAddress(AMK_DLL, "AMK_GetMetroData");
			AMK_SetMetroData = (SetMetroData)GetProcAddress(AMK_DLL, "AMK_SetMetroData");

			if (AMK_DLL == NULL)MessageBox::Show("Положите файл 'DllProject.dll' в каталог программы!");
			if (!AMK_StartComPort || !AMK_GetCurrState || !AMK_Init || !AMK_BeginCyclogram || !AMK_GetData
				|| !AMK_SetIdle || !AMK_PwrOn || !AMK_PwrOff || !AMK_DeleteComPort
				//|| !AMK_CheckConnect
				|| !AMK_GetSerial || !AMK_GetMetroData || !AMK_SetMetroData) {
				MessageBox::Show("Файл 'DllProject.dll' испорчен !");
			}
			else {
				Sleep(2000);
				ComPort = AMK_StartComPort(&ComPort, NULL);
				cout << " ComPort " << ComPort << endl;
				if (ComPort != NULL) {
					cout << "idle " << AMK_SetIdle(ComPort) << endl;
					if (AMK_PwrOn(ComPort)) {
						Sleep(2000);
						//while (AMK_GetCurrState(ComPort) != 32 && AMK_GetCurrState(ComPort) != 1)
						//	Sleep(500);
						//cout << "idle " << AMK_SetIdle(ComPort) << endl;

						if (AMK_Init(ComPort)) {
							Sleep(500);
							if (!AMK_GetMetroData(ComPort, &metrology_struct)) {
								cout << "no metro1" << endl;
								if (!AMK_GetMetroData(ComPort, &metrology_struct)) {
									cout << "no metro2" << endl;
									if (!AMK_GetMetroData(ComPort, &metrology_struct)) {
										cout << "no metro3" << endl;
										AMK_PwrOff(ComPort);
										AMK_DeleteComPort(ComPort);
										FreeLibrary(AMK_DLL);
										MessageBox::Show(" Файл метрологии не выгружен, попробуйте еще раз!");
									}
								}
							}
							//файл метрологии выгружен

							metro_convert(&metrology_struct, sonde_param);

							/////////////////////////////////////////////////////////////////////////////////////////////////////
							Sleep(500);
							if (AMK_BeginCyclogram(ComPort)) {
								Sleep(500);
								cout << "autonom Start " << endl;
								on_off = true;
								this->button_ON_OFF->BackColor = System::Drawing::Color::Coral;
								Sonde_Read_Timer->Enabled = true;
								this->button_ON_OFF->Text = "ВЫКЛЮЧИТЬ АВТОНОМНИК";
								comboBox_tool_type->SelectedIndex = 2;//AUTONOM
							}
							else { AMK_PwrOff(ComPort); AMK_DeleteComPort(ComPort); FreeLibrary(AMK_DLL);  MessageBox::Show(" прибор не стартовал, попробуйте еще раз!"); }
						}
						else { AMK_PwrOff(ComPort); AMK_DeleteComPort(ComPort); FreeLibrary(AMK_DLL);  MessageBox::Show(" прибор неиницилизирован, попробуйте еще раз!"); }
					}
					else { AMK_DeleteComPort(ComPort);  FreeLibrary(AMK_DLL);  MessageBox::Show("Проверьте правильность подключения прибора!"); }
				}
				else { FreeLibrary(AMK_DLL);   MessageBox::Show("Подключите УСИ!"); }

			}

		}

		else {
			on_off = false;
			this->button_ON_OFF->BackColor = DefaultBackColor;
			serialPort_sonde->Close();
			Sonde_Read_Timer->Enabled = false;
			Sleep(100);
			this->button_ON_OFF->Text = "ВКЛЮЧИТЬ АВТОНОМНИК";
		}

	}
	
	if (tool_type == SDR || tool_type == LWD1DDS) {
		int stop = 0, stop_ = 0;
		extern string Com_LWD_Name;
		extern string Com_Name;
		extern vector <SONDE_PARAM> sonde_param;
		//длина вектора равна количеству сопротивлений
		extern vector <V10> PH_computed;
		extern vector <V10> Ro_computed;
		//длина вектора равна количеству зондов
		//отладка
		extern vector <LOOP_PARAM> loop_param;

		if (on_off == false) {
			serialPort_sonde->Close();
			Sleep(100);
			//serialPort_sonde->PortName = "COM17";
			if (tool_type == SDR)
				serialPort_sonde->BaudRate = 115200;
			if (tool_type == LWD1DDS)
				serialPort_sonde->BaudRate = 125000;
			serialPort_sonde->ReadTimeout = 100;
			serialPort_sonde->WriteTimeout = 100;
			Com_LWD_Name = (char*)Marshal::StringToHGlobalAnsi(serialPort_sonde->PortName).ToPointer();
			Console::WriteLine(serialPort_sonde->PortName);
			try {
				serialPort_sonde->Open();
				serialPort_sonde->DiscardOutBuffer();
				Sleep(150);
				//читаем метрологию
				if (tool_type == SDR) {
					uint8_t DATA[2] = { 0x05 ,0x00 };
					uint8_t CMD = 0; uint8_t DATA_IN[242];
					if (send_to_tool(serialPort_sonde, 0x13, DATA, 2) == 0) {
						Sleep(50);
						receive_from_tool(serialPort_sonde, CMD, DATA_IN, 242);
						metro_convert(DATA_IN + 2, sonde_param);
					}
				}
				if (tool_type == LWD1DDS) {
					
					int mode = 0; int res = 0;
					get_ph_from_1DDS(serialPort_sonde, mode, res);
					Sleep(1000);
					get_ph_from_1DDS(serialPort_sonde, mode, res);
					Sleep(1000);
					serialPort_sonde->Open();
					
					uint8_t DATA[3];
					DATA[0] = 0x00; DATA[1] = 0x04; DATA[2] = 240;
					send_to_tool(serialPort_sonde, CMD_READ_EE, DATA, 3);
					Sleep(150);
					uint8_t CMD = 0; uint8_t DATA_IN[256];
					receive_from_tool(serialPort_sonde, CMD, DATA_IN, 240);
					metro_convert(DATA_IN, sonde_param);
					sonde_kojuh_serial = *(uint32_t*)(DATA_IN + 4);
								
			
				}

			}
			catch (...) {
				stop = 1;
			}
			if (!stop) {
				serialPort_sonde->DiscardOutBuffer();
				Sleep(100);
				on_off = true;
				this->button_ON_OFF->BackColor = System::Drawing::Color::Coral;
				Sonde_Read_Timer->Enabled = true;
				this->button_ON_OFF->Text = "ВЫКЛЮЧИТЬ ПРИБОР";
			}
			else MessageBox::Show("Не удется открыть порт !");
		}
		else {
			on_off = false;
			this->button_ON_OFF->BackColor = DefaultBackColor;
			serialPort_sonde->Close();
			Sonde_Read_Timer->Enabled = false;
			Sleep(100);
			this->button_ON_OFF->Text = "ВКЛЮЧИТЬ ПРИБОР";
		}
	}
	
	
}

private: System::Void button_local_air_Click(System::Object^  sender, System::EventArgs^  e) {
	//eсли прибор работает отсчитываем 20 измерений и забираем 
	if (on_off == true && bt_on == true) {
		button_AIR->PerformClick();
		if (local_air_is_set == false) {
			if (!Loop_work(0, 0)) {
				air_set_cntr = 0;
				collect_air_data = true;
			}
		}
		else{
			local_air_reset = true;
		}
	}
	else MessageBox::Show("Включите прибор и кольцо!");
}

private:  System::Void  Sonde_Read() {
	Sonde_Read_Timer = gcnew System::Timers::Timer(1000);
	Sonde_Read_Timer->Elapsed += gcnew ElapsedEventHandler(this, &WorkForm::sonde_read_timer);
	Sonde_Read_Timer->AutoReset = true;
	Sonde_Read_Timer->Enabled = false;
}
    
private:void sonde_read_timer(System::Object^ source, ElapsedEventArgs^ e) {
	extern bool metro_check;
	
	if (metro_check == false) {
		extern string Com_Name;
		//длина вектора равна количеству сопротивлений
		extern vector <V10> PH_computed;
		extern vector <V10> Ro_computed;
		//длина вектора равна количеству зондов
		extern vector <SONDE_PARAM> sonde_param;
		// 10 - зонды 20 - усреднение
		extern int PH_percent_from_sonde[10][20];
		extern int Ro_percent_from_sonde[10][20];
		extern V10 AIR;
		extern V10 ph_data;
		extern V10 ro_data;

		bool stop = false;
		int res = 0;
		//получаем набор фаз от прибора

		int mode = 0;
		if (tool_type == SDR)
			ph_data = get_ph_from_SDR(serialPort_sonde, mode, res);
		if (tool_type == LWD1DDS)
			ph_data = get_ph_from_1DDS(serialPort_sonde, mode, res);
		if (tool_type == AUTONOM)
			ph_data = get_ph_from_AUTONOM(ComPort, mode, res);

		if (res == 0)
			global_cntr++;
		//Console::WriteLine("global_cntr " + global_cntr);

		/*/отладка ////////////////////////////////////////
		if (global_cntr > 20)global_cntr = 0;
		int add;
		if (global_cntr % 2 == 0)add = global_cntr;// чет
		else add = -global_cntr;// нечет
		//Console::WriteLine( "add " + add + " " + (1.0f + 0.01f*add));
		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
			if (R_IDX != 0)
				ph_data.VALUE[N_sonde] = PH_computed[R_IDX].VALUE[N_sonde] * (1.0f + 0.01f*add);
			if (R_IDX == 0)
				ph_data.VALUE[N_sonde] = 0.0001745*(N_sonde+1) * (1.0f + 0.01f*add);
		}

		*////////////////////////////////////////////////

		// локальный воздух ////////////////////////////////////
		// собираем данные и усредняем
		if (res == 0 && collect_air_data == true) {
			if (air_set_cntr < air_data_aver) {
				for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
					AIR.VALUE[N_sonde] += (ph_data.VALUE[N_sonde] / (float)(air_data_aver));
				}
				air_set_cntr++;
			}
			if (air_set_cntr == air_data_aver) {
				collect_air_data = false;
				local_air_is_set = true;
				air_set_cntr = 0;
			}
		}
		//сброс локального воздуха
		if (local_air_reset == true) {
			for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
				AIR.VALUE[N_sonde] = 0;
			}

			local_air_reset = false;
			local_air_is_set = false;
		}

		//Console::WriteLine("air_set_cntr " + air_set_cntr);
		//////////////////////////////////////////////////////////

		//вычитаем локальный воздух из фаз
		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
			ph_data.VALUE[N_sonde] -= AIR.VALUE[N_sonde];
		}

		//проталкиваем буфер на единицу

		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
			for (int aver = 19; aver > 0; aver--) {
				PH_percent_from_sonde[N_sonde][aver] = PH_percent_from_sonde[N_sonde][aver - 1];
				Ro_percent_from_sonde[N_sonde][aver] = Ro_percent_from_sonde[N_sonde][aver - 1];
			}
		}

		//вычисляем процентную разницу между вычисленными значениями фаз всех зондов VALUE[N_sonde] 
		//для включенного сейчас сопротивления R_IDX и записываем в начало буфера PH_percent_from_sonde[N_sonde][0]
		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++)
			PH_percent_from_sonde[N_sonde][0] =
			100 * (ph_data.VALUE[N_sonde] - PH_computed[R_IDX].VALUE[N_sonde]) / (PH_computed[R_IDX].VALUE[N_sonde] + 0.000000001f);

		/*/отладка////
		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
			cout << "aver arr "<< N_sonde << " ";
			for (int i = 0; i < 20; i++)
				cout << PH_percent_from_sonde[0][i] << " ";
			cout << endl;
		}
		/*////////////////

		//вычисляем кажущееся УЭС от сигналов прибора
		ro_data = ro_dfi(sonde_param, ph_data);
		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++)
			Ro_percent_from_sonde[N_sonde][0] =
			100 * (ro_data.VALUE[N_sonde] - Ro_computed[R_IDX].VALUE[N_sonde]) / (Ro_computed[R_IDX].VALUE[N_sonde] + 0.000000001f);


		//измерения от конкретного сопротивления
		//переключаем конденсаторы  через N_averaging точек изм.
		if (res == 0 && start_R_measure == true) {
			//переключение конденсатора для каждого зонда 
			if (checkBox_express->Checked == false ||
				(checkBox_express->Checked == true && R_IDX <= N_R_for_express)
				) {
				if (C_IDX <= sonde_param.size()) {
					skip_cntr++;
					//cout << " skip_cntr " << skip_cntr << endl;
					if (skip_cntr > skip)
						aver_cntr++;
					Console::WriteLine(sonde_param.size() + " R_idx " + R_IDX + " C_idx " + C_IDX + " skip_cntr " + skip_cntr + " aver_cntr " + aver_cntr);
					if (aver_cntr > N_averaging) {
						aver_cntr = 0;
						C_IDX++;
						if (C_IDX > sonde_param.size()) {
							Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (R_IDX).ToString()];
							BUTTON_R->BackColor = DefaultBackColor;
							C_IDX = 0;
							R_IDX = 0;
							start_R_measure = false;
							end_R_measure = true;

							button_AIR->BackColor = Color::Red;
						}
						int work_result = Loop_work(R_IDX, C_IDX);
						if (!work_result)
							skip_cntr = 0;

						if (work_result) {
							Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (R_IDX).ToString()];
							BUTTON_R->BackColor = DefaultBackColor;
						}

					}
				}

			}
			//переключение конденсатора для всех зондов, начиная с 3go сопротивления
			if (checkBox_express->Checked == true && R_IDX > N_R_for_express) {
				if (C_IDX <= sonde_param.size()) {
					skip_cntr++;
					if (skip_cntr > skip)
						aver_cntr++;
					//Console::WriteLine(sonde_param.size() + " R_idx " + R_IDX + " C_idx " + C_IDX + " aver_cntr " + aver_cntr);
					if (aver_cntr > N_averaging) {
						aver_cntr = 0;
						C_IDX += (int)((sonde_param.size() / 2));
						if (C_IDX > sonde_param.size()) {
							Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (R_IDX).ToString()];
							BUTTON_R->BackColor = DefaultBackColor;
							C_IDX = 0;
							R_IDX = 0;
							start_R_measure = false;
							end_R_measure = true;
							button_AIR->BackColor = Color::Red;
						}
						int work_result = Loop_work(R_IDX, C_IDX);
						if (!work_result)
							skip_cntr = 0;

						if (work_result) {
							Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (R_IDX).ToString()];
							BUTTON_R->BackColor = DefaultBackColor;
						}
					}
				}
			}
		}
	}
	
	
}

private: System::Void BT_ON(System::Object^  sender, System::EventArgs^  e) {
	int stop = 0, stop_ = 0;
	extern string Com_Name;
	String^ Str = "";
	Byte send = 0;
	Byte resive = 0;
	bt_on = false;
	this->button_BT_ON->BackColor = DefaultBackColor;
	
	if (serialPort_loop->IsOpen == true) {
		serialPort_loop->Close();		
			//MessageBox::Show("уже включен");
	}			

	//serialPort_loop->PortName = gcnew String(Com_Name.c_str());
	serialPort_loop->BaudRate = 9600;
	serialPort_loop->ReadTimeout = 500;
	serialPort_loop->WriteTimeout = 500;
	Console::WriteLine(serialPort_loop->PortName);

	try { serialPort_loop->Open(); }
	catch (...) { stop_ = 1; }
	if (!stop_) {		
		serialPort_loop->DiscardInBuffer();
		serialPort_loop->Write("$\r\n");
		try { Str = serialPort_loop->ReadLine(); }
		catch (...) { stop = 1; }
		if (!stop) {
			if (Str->StartsWith("Unknown")) {
				bt_on = true;
				button_AIR->PerformClick();
				this->button_BT_ON->BackColor = Color::DodgerBlue;
				this->button_BT_ON->Text = "КОЛЬЦО ВКЛЮЧЕНО";
				Loop_Check_Timer->Enabled = true;
			}
			else { serialPort_loop->Close(); MessageBox::Show("Не удается установить соединение !"); }
		}
		else {
			serialPort_loop->Close(); MessageBox::Show("Нет ответа от кольца !");
		}	
	}
	else MessageBox::Show("Не удается открыть порт кольца!");
	
}

int  Loop_work(int R_idx, int C_idx) {
	int result = -1;
	if (bt_on == true) {
		Loop_Check_Timer->Enabled = false;
		Sleep(200);
	    int stop = 0, stop_ = 0;
		String^ resive = ""; String^ resive1 = "";
		String^ send_R = ""; String^ send_C = "";
	    String^ resive_R = ""; String^ resive_C = "";

		send_R = "RES " + (R_idx) + "\r\n"; resive_R = "RES=" + (R_idx) + " OK";
		send_C = "CAP " + (C_idx) + "\r\n"; resive_C = "CAP=" + (C_idx) + " OK";
		
		Sleep(50);
		serialPort_loop->DiscardInBuffer();
	    try {			    
		    serialPort_loop->Write(send_R);
			Sleep(50);
			serialPort_loop->Write(send_C);
			Sleep(50);
		}
		catch (...) { stop_ = 1; }
		if (stop_) {
		    result = 1;
		}

		try {
			resive = serialPort_loop->ReadLine();
			resive1 = serialPort_loop->ReadLine();
		}
		catch (...) { stop = 1; }
		if (!stop) {			    
			if (resive->StartsWith(resive_R) && resive1->StartsWith(resive_C)) 								
				result = 0;			    
			else result = 2;		    
		}
		else result = 3;	
		Loop_Check_Timer->Enabled = true;
	}
	else  result = 4; 

	return result;
}
    
private:  System::Void  Loop_check() {
	Loop_Check_Timer = gcnew System::Timers::Timer(3000);
	Loop_Check_Timer->Elapsed += gcnew ElapsedEventHandler(this, &WorkForm::loop_check_timer);
	Loop_Check_Timer->AutoReset = true;			
}
//проверка работы кольца		  		  
private:void loop_check_timer(System::Object^ source, ElapsedEventArgs^ e) {
	bool stop = 0; 
	String^ Str = "";	
	try {
		serialPort_loop->DiscardInBuffer();
		serialPort_loop->Write("$\r\n");
		Str = serialPort_loop->ReadLine(); 
	}
	catch (...) {
		stop = 1; 
		//Loop_Check_Timer->Enabled = false;
		this->button_BT_ON->BackColor = DefaultBackColor;
		this->button_BT_ON->Text = " ВКЛЮЧИТЬ КОЛЬЦО";
		bt_on = false;
		//Console::WriteLine("no_BT_port");		
		try {
			if (serialPort_loop->IsOpen == true) 
				serialPort_loop->Close();										
			serialPort_loop->Open();
			if (serialPort_loop->IsOpen == true) {
				this->button_BT_ON->BackColor = Color::DodgerBlue;
				this->button_BT_ON->Text = "КОЛЬЦО ВКЛЮЧЕНО";
				bt_on = true;
				//Console::WriteLine("BT_port_is_on");
			}
		}
		catch (...) {;}
	}
	if (!stop){ 
		this->button_BT_ON->BackColor = Color::DodgerBlue;
		this->button_BT_ON->Text = "КОЛЬЦО ВКЛЮЧЕНО";
		bt_on = true;
		//Console::WriteLine("BT_port_is_on");
	}	
	
}

///loop////////////////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void button_AIR_Click(System::Object^  sender, System::EventArgs^  e) {
	if (on_off == true && bt_on == true) {
		R_IDX = 0;
		C_IDX = 0;
		int work_result = Loop_work(R_IDX, C_IDX);
		if (!work_result) {
			button_AIR->BackColor = Color::Red;
			for (int n = 1; n <= 8; n++) {
				Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (n).ToString()];
				BUTTON_R->BackColor = DefaultBackColor;
			}
		}
	}
	
}

void R_Click(int R_idx, int C_idx) {
	if (on_off == true && bt_on == true && start_R_measure == false) {
		R_IDX = R_idx; C_IDX = C_idx;
		Loop_Check_Timer->Enabled = false;
		
		Sleep(200);
		int work_result = Loop_work(R_idx, C_idx);
		if (!work_result) {
			for (int n = 1; n <= 8; n++) {
				Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (n).ToString()];
				if (n == R_idx)
					BUTTON_R->BackColor = Color::Red;
				else
					BUTTON_R->BackColor = DefaultBackColor;				
			}

			button_AIR->BackColor = DefaultBackColor;

			CheckBox^ CHECKBOX_R = (CheckBox^)this->tableLayoutPanel1->Controls["checkBox_R" + (R_idx).ToString()];
			CHECKBOX_R->Checked = false;
			//очищаем графики
			for (int N_sonde = 1; N_sonde <= 10; N_sonde++) {
				Label^ LABEL_T = (Label^)this->tableLayoutPanel1->Controls["labelT" + (N_sonde).ToString()];
				LABEL_T->Text = "Tx ";
				PictureBox^ GRAF = (PictureBox^)this->tableLayoutPanel1->Controls["pictureBox_graf" + (N_sonde).ToString()];
				GRAF->Refresh();
				GRAF->Image = nullptr;	
				TextBox^ TEXTBOX_R_T = (TextBox^)this->tableLayoutPanel1->Controls["textBox_R" + R_IDX + "_T" + (N_sonde).ToString()];
				TEXTBOX_R_T->Text = "";
			}			
		
			start_R_measure = true;
			end_R_measure = false;
			aver_cntr = 0;
			skip_cntr = 0;
		}
		Loop_Check_Timer->Enabled = true;
	}
}

private: System::Void button_R1_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(1, 1);
}
private: System::Void button_R2_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(2, 1);
}
private: System::Void button_R3_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(3, 1);
}
private: System::Void button_R4_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(4, 1);
}
private: System::Void button_R5_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(5, 1);
}
private: System::Void button_R6_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(6, 1);
}
private: System::Void button_R7_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
	    R_Click(7, 1);
}
private: System::Void button_R8_Click(System::Object^  sender, System::EventArgs^  e) {
	if (auto_vork == false)
		R_Click(8, 1);
}

void Z_Click(int button_n) {
	if (bt_on == true) {
		if (!Loop_work(1, button_n)) {
			for (int n = 1; n <= 10; n++) {
				Button^ BUTTON_Z = (Button^)this->tableLayoutPanel11->Controls["button_Z" + (n).ToString()];
				if (n == button_n)
					BUTTON_Z->BackColor = Color::Red;
				else
					BUTTON_Z->BackColor = DefaultBackColor;
			}
		}
	}	
}

private: System::Void button_Z1_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 1;
	Z_Click(button_n);	
}
private: System::Void button_Z2_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 2;
	Z_Click(button_n);	
}
private: System::Void button_Z3_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 3;
	Z_Click(button_n);
}
private: System::Void button_Z4_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 4;
	Z_Click(button_n);
}
private: System::Void button_Z5_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 5;
	Z_Click(button_n);
}
private: System::Void button_Z6_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 6;
	Z_Click(button_n);
}
private: System::Void button_Z7_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 7;
	Z_Click(button_n);
}
private: System::Void button_Z8_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 8;
	Z_Click(button_n);
}
private: System::Void button_Z9_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 9;
	Z_Click(button_n);
}
private: System::Void button_Z10_Click(System::Object^  sender, System::EventArgs^  e) {
	int button_n = 10;
	Z_Click(button_n);
}

private: System::Void textBox_Z_loop1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop1->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop2_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop2->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop3_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop3->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop4_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop4->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop5_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop5->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop6_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop6->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop7_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop7->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop8_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop8->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop9_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop9->BackColor = Color::LightYellow;
}
private: System::Void textBox_Z_loop10_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	textBox_Z_loop10->BackColor = Color::LightYellow;
}
//загрузить параметры кольца
private: System::Void button_read_loop_Z_Click(System::Object^  sender, System::EventArgs^  e) {
	int stop = 0;
	String^ Str = "";
	String^ send_Z = "";
	String^ resive_Z = "";
	Byte resive = 0;
	float R[32];
	float Re[32];
	extern vector <V10> PH_computed;
	extern vector <V10> Ro_computed;
	extern vector <LOOP_PARAM> loop_param;
	extern vector <SONDE_PARAM> sonde_param;
	if (bt_on == true) {
		Loop_Check_Timer->Enabled = false;
		
		String^ resive = "";
		for (int n = 0; n < 10; n++) {
			int stop = 0;
			send_Z = "ZREAD " + (n)+"\r\n"; resive_Z = "Z" + (n)+" ";
			serialPort_loop->DiscardInBuffer();
			serialPort_loop->Write(send_Z);
			Sleep(50);
			try { resive = serialPort_loop->ReadLine(); }
			catch (...) { stop = 1; }
			if (!stop) {
				if (resive->StartsWith(resive_Z)) {
					Str = resive->Substring(3);
					int index_0 = Str->IndexOf(" ");
					R[n] = Double::Parse((Str->Substring(0, index_0))->Replace(".", ","));
					Re[n] = Double::Parse((Str->Substring(index_0 + 1))->Replace(".", ","));
					Console::WriteLine(n + " R " + R[n] + " Re " + Re[n]);
					TextBox^ TEXTBOX_Re = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];
					TextBox^ TEXTBOX_R = (TextBox^)this->tableLayoutPanel11->Controls["textBox_R_loop" + (n + 1).ToString()];
					TEXTBOX_Re->Text = Re[n].ToString("F4");
					TEXTBOX_R->Text = R[n].ToString("F0");
					TEXTBOX_Re->BackColor = Color::White;
					Z_downloaded_from_loop = true;

				}
				else {
					Z_downloaded_from_loop = false;
					//button_read_loop->BackColor = DefaultBackColor;
					//this->button_read_loop->Text = L"ПОЛУЧИТЬ НАСТРОЙКИ КОЛЬЦА";
					MessageBox::Show("Нет ответа от кольца " + n);

				}
			}
			//else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 3!"); }
		}
		Loop_Check_Timer->Enabled = true;
	}
	if (bt_on == false) { MessageBox::Show("ВКЛЮЧИТЬ КОЛЬЦО"); }
	//заполняем вектор параметров кольца
	if (Z_downloaded_from_loop == true) {
		loop_param.clear();
		for (int n = 0; n < 10; n++) {
			if (Re[n] > 0.000001f) {
				LOOP_PARAM buff;
				buff.R = R[n];
				buff.r = 0.5;
				buff.Re = Re[n];
				buff.R_Add_Wire = 0.5;
				loop_param.push_back(buff);
			}
		}
		//если прибор включен рассчитываем фазы и УЭС от всех сопротивлений кольца для всех зондов
	}

}

private: System::Void button_write_Z_Click(System::Object^  sender, System::EventArgs^  e) {
	bool write_ok = false;
	int stop = 0;
	String^ Str = "";
	String^ send_Z = "";
	String^ resive_Z = "";
	Byte resive = 0;
	double R[32] = { 1.0, };
	double Re[32] = { 0.0, };
	//Команда: ZWRITE 0 1.0 2.0\r\n
	//Ответ : WRITE Z0 OK!\r\n
	if (bt_on == true) {
		Loop_Check_Timer->Enabled = false;
		Sleep(200);
	    for (int n = 0; n < 10; n++) {
			int stop = 0;
			String^ resive = "";
			TextBox^ TEXTBOX_Re = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];
			TextBox^ TEXTBOX_R = (TextBox^)this->tableLayoutPanel11->Controls["textBox_R_loop" + (n + 1).ToString()];
			if (TEXTBOX_Re->Text != "") Re[n] = System::Double::Parse((TEXTBOX_Re->Text));
			if (TEXTBOX_R->Text != "")   R[n] = System::Double::Parse((TEXTBOX_R->Text));
			send_Z = "ZWRITE " + n + " " + ((R[n]).ToString())->Replace(",", ".") + " " + ((Re[n]).ToString())->Replace(",", ".") + "\r\n";
			Console::WriteLine("send_Z " + send_Z);
			resive_Z = "WRITE Z" + (n) +" OK!";
			serialPort_loop->DiscardInBuffer();
			serialPort_loop->Write(send_Z);
			Sleep(50);
			try { resive = serialPort_loop->ReadLine(); }
			catch (...) { stop = 1; }
			if (!stop) {
				if (resive->StartsWith(resive_Z)) {
					TEXTBOX_Re->BackColor = Color::White;
					write_ok = true;
				}
				else { 
					write_ok = false;
					MessageBox::Show("Нет ответа от кольца , нажмите  еще раз !"); 
				}
			}
			else { write_ok = false; }
	    }

		textBox_radius; textBox_R_wire;


		if(write_ok == true )
			MessageBox::Show("запись прошла успешно");
		else
			MessageBox::Show("ошибка, перезапишите");
		
		Loop_Check_Timer->Enabled = true;
	}
	if (bt_on == false) { MessageBox::Show("Включите кольцо"); }
}

private: System::Void button_erase_Click(System::Object^  sender, System::EventArgs^  e) {
	bool erase_ok = false;
	int stop = 0;
	String^ Str = "";
	String^ send_Z = "";
	String^ resive_Z = "";
	Byte resive = 0;
	double R = 1.0;
	double Re = 0.0;
	//Команда: ZERASE\r\n
	//Ответ : ERASE OK!\r\n
	if (bt_on == true) {
		Loop_Check_Timer->Enabled = false;
		Sleep(200);
		int stop = 0;
		String^ resive = "";
		if (textBox_Z_loop1->Text != "") Re = System::Double::Parse((textBox_Z_loop1->Text));
		send_Z = "ZERASE\r\n";
		resive_Z = "ERASE OK!";
		serialPort_loop->DiscardInBuffer();
		serialPort_loop->Write(send_Z);
		Sleep(50);
		try { resive = serialPort_loop->ReadLine(); }
		catch (...) { stop = 1; }
		if (!stop) {
			if (resive->StartsWith(resive_Z)) {
				button_read_loop_Z->PerformClick();
				erase_ok = true;
			}
			else { 
				MessageBox::Show("ошибка, сотрите еще раз"); 
				erase_ok = false;
			}
		}
		else {  
			MessageBox::Show("Соединение с кольцом прервано "); 
			erase_ok = false;
		}		
		if (erase_ok == true)
			MessageBox::Show("запись прошла успешно");
		else
			MessageBox::Show("ошибка, перезапишите");
	
		Loop_Check_Timer->Enabled = true;
	}
	if (bt_on == false) { MessageBox::Show("Включите кольцо"); }
}

private: System::Void button_signature_Click(System::Object^  sender, System::EventArgs^  e) {
	int signature_to_loop = 0;
	if (textBox_signature->Text != "") signature_to_loop = System::Double::Parse((textBox_signature->Text));

	int stop = 0;
	String^ Str = "";
	String^ send_Z = "";
	String^ resive_Z = "";
	Byte resive = 0;

	if (bt_on == true) {
		Loop_Check_Timer->Enabled = false;
		Sleep(200);
		int stop = 0;
		String^ resive = "";
		send_Z = "ZWRITE " + 31 + " " + signature_to_loop + " " + signature_to_loop + "\r\n";
		Console::WriteLine("send_Z " + send_Z);
		resive_Z = "WRITE Z" + (31) + " OK!";
		serialPort_loop->DiscardInBuffer();
		serialPort_loop->Write(send_Z);
		Sleep(50);
		try { resive = serialPort_loop->ReadLine(); }
		catch (...) { stop = 1; }
		if (!stop) {
			if (resive->StartsWith(resive_Z)) {
					MessageBox::Show("сигнатура записана успешно");
			}
			else MessageBox::Show("ошибка, перезапишите");
		}
		else {  MessageBox::Show("ошибка, перезапишите"); }

		Loop_Check_Timer->Enabled = true;
	}
	if (bt_on == false) { MessageBox::Show("Включите кольцо"); }


}

private: System::Void button_calc_z_to_loop_Click(System::Object^  sender, System::EventArgs^  e) {
	bool calc_z_exist = false;
	for (int i = 0; i < 10; i++) {
		TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];
		TextBox^ TEXTBOX_Z_LOOP = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (i + 1).ToString()];
		if (TEXTBOX_Z_CALC->Text != "") { 
			TEXTBOX_Z_LOOP->Text = (System::Double::Parse((TEXTBOX_Z_CALC->Text))).ToString("F4");
			calc_z_exist = true;
		}
		else
			calc_z_exist = false;
	}
	if(!calc_z_exist)
		MessageBox::Show("Требуется рассчитать реактивные сопротивления");
	else {
		button_write_Z->PerformClick();
		//MessageBox::Show("теперь запишите");
	}
}

private: System::Void button_calk_C_need_Click(System::Object^  sender, System::EventArgs^  e) {
	double L_loop = 4.6*1e-6;
	double F_rez[10];
	double C[10];
	const char* Metrology_in;
	vector <SONDE_PARAM> param;

	ifstream metro_in;
	GP_TOOL_METROLOGY metrology_struct;
	openFileDialog1->Filter = "text files (*.bin)|*.bin";
	if (checkBox_param_red->Checked == false) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			Metrology_in = (char*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer();
			metro_in.open(Metrology_in, ios::binary);
			metro_in.read((char*)&metrology_struct, sizeof(metrology_struct));
			metro_in.close();
			metro_convert(&metrology_struct, param);
			
			//расчетные значения емкостей и резонансных частот для кольца
			for (int i = 0; i < param.size(); i++) {
				//F_rez[i] = (int(F(param[i].F, 0.5, L_loop, param[i].L1, param[i].L2)));
				C[i] = (C_max_fi(param[i].F, 0.5, L_loop, param[i].L1, param[i].L2));
				//cout << "C" << i << " " << C[i] << endl;
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (i + 1).ToString()];
				TEXTBOX_L1->Text = (param[i].L1).ToString("F3");
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (i + 1).ToString()];
				TEXTBOX_L2->Text = (param[i].L2).ToString("F3");
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (i + 1).ToString()];
				TEXTBOX_F->Text = (param[i].F / 1000).ToString();
				TextBox^ TEXTBOX_C = (TextBox^)this->tableLayoutPanel11->Controls["textBox_С" + (i + 1).ToString()];
				TEXTBOX_C->Text = ((int)(1e12*C[i])).ToString("F0");
				TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];
				TEXTBOX_Z_CALC->Text = (imag(Z(1.0, L_loop, C[i], param[i].F))).ToString("F3");
			}
		}
		else {
			for (int i = 0; i < 10; i++) {
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (i + 1).ToString()];
				TEXTBOX_L1->Text = "";
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (i + 1).ToString()];
				TEXTBOX_L2->Text = "";
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (i + 1).ToString()];
				TEXTBOX_F->Text = "";
				TextBox^ TEXTBOX_C = (TextBox^)this->tableLayoutPanel11->Controls["textBox_С" + (i + 1).ToString()];
				TEXTBOX_C->Text = "";
				TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];
				TEXTBOX_Z_CALC->Text = "";
			}
		}
	}
	if (checkBox_param_red->Checked == true) {
		SONDE_PARAM param[10];
		for (int i = 0; i < 10; i++) {
			TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (i + 1).ToString()];
			if (TEXTBOX_L1->Text != "") param[i].L1 = System::Double::Parse((TEXTBOX_L1->Text));
			TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (i + 1).ToString()];
			if (TEXTBOX_L1->Text != "") param[i].L2 = System::Double::Parse((TEXTBOX_L2->Text));
			TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (i + 1).ToString()];
			if (TEXTBOX_F->Text != "") param[i].F = 1000*System::Double::Parse((TEXTBOX_F->Text));
			C[i] = (C_max_fi(param[i].F, 0.5, L_loop, param[i].L1, param[i].L2));
			TextBox^ TEXTBOX_C = (TextBox^)this->tableLayoutPanel11->Controls["textBox_С" + (i + 1).ToString()];
			TEXTBOX_C->Text = ((int)(1e12*C[i])).ToString("F0");
			TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];
			TEXTBOX_Z_CALC->Text = (imag(Z(1.0, L_loop, C[i], param[i].F))).ToString("F3");			
		}
	}
}

private: System::Void checkBox_Re_red_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (checkBox_Re_red->Checked == true) {
		for (int n = 0; n < 10; n++) {
			TextBox^ TEXTBOX_Re = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];
			TEXTBOX_Re->ReadOnly = false;
		}
	}
	if (checkBox_Re_red->Checked == false) {
		for (int n = 0; n < 10; n++) {
			TextBox^ TEXTBOX_Re = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];
			TEXTBOX_Re->ReadOnly = true;
		}
	}
}
private: System::Void checkBox_r_red_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (checkBox_R_red->Checked == true){
		for (int n = 0; n < 10; n++) {
			TextBox^ TEXTBOX_R = (TextBox^)this->tableLayoutPanel11->Controls["textBox_R_loop" + (n + 1).ToString()];
			TEXTBOX_R->ReadOnly = false;
		}
	}
	if (checkBox_R_red->Checked == false) {
		for (int n = 0; n < 10; n++) {
			TextBox^ TEXTBOX_R = (TextBox^)this->tableLayoutPanel11->Controls["textBox_R_loop" + (n + 1).ToString()];
			TEXTBOX_R->ReadOnly = true;
		}
	}
}
private: System::Void checkBox_param_red_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (checkBox_param_red->Checked == true) {
		for (int n = 0; n < 10; n++) {
			TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (n + 1).ToString()];
			TEXTBOX_L1->ReadOnly = false;
			TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (n + 1).ToString()];
			TEXTBOX_L2->ReadOnly = false;
			TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (n + 1).ToString()];
			TEXTBOX_F->ReadOnly = false;
			
		}
	}
	if (checkBox_param_red->Checked == false) {
		for (int n = 0; n < 10; n++) {
			TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (n + 1).ToString()];
			TEXTBOX_L1->ReadOnly = true;
			TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (n + 1).ToString()];
			TEXTBOX_L2->ReadOnly = true;
			TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (n + 1).ToString()];
			TEXTBOX_F->ReadOnly = true;
		}
	}
}
//убрать после отладки
private: System::Void button_calk_Click(System::Object^  sender, System::EventArgs^  e) {
	extern bool metro_check;
	extern vector <V10> PH_computed;
	extern vector <V10> Ro_computed;
	extern vector <SONDE_PARAM> sonde_param;
	extern vector <LOOP_PARAM> loop_param;

	const char* Metrology_in;
	ifstream metro_in;
	GP_TOOL_METROLOGY *metrology_struct;
	
	
	
	cout << endl;
	cout << endl;

	calc_PH_from_loop(sonde_param, loop_param,  PH_computed);
	calc_Ro_from_loop(sonde_param, PH_computed, Ro_computed);

	for (int R_idx = 0; R_idx < 8; R_idx++) {
		cout << "PH ";
		for (int n_sonde = 0; n_sonde < sonde_param.size(); n_sonde++)
			 cout << PH_computed[R_idx].VALUE[n_sonde] << " ";
		cout << endl;
	}
	cout << endl;
	for (int R_idx = 0; R_idx < PH_computed.size(); R_idx++) {
		cout << "Ro ";
		for (int n_sonde = 0; n_sonde < sonde_param.size(); n_sonde++)
			cout << Ro_computed[R_idx].VALUE[n_sonde] << " ";
		cout << endl;
	}
	
	
	/*
	cout << endl;
	double Re[8];
	for (int i = 0; i < 8; i++) {
		Re[i] = loop_param[i].Re;
	}
	for(int R_idx = 0; R_idx < 8; R_idx++) {
		V10 PH_old = calc_phase_from_loop(metrology_struct, loop_param[R_idx].R, 0.01, Re, 0, 0, 0.5, 1);
		V10 Ro_old = R0_adecvat_r_loop(metrology_struct, &PH_old);
		cout  << " Ro_old ";
		for (int i = 0; i < 8; i++)
			cout << Ro_old.VALUE[i] << " ";
		cout << endl;
	}
	*/

}

private: System::Void button_auto_Click(System::Object^  sender, System::EventArgs^  e) {
	extern vector <SONDE_PARAM> sonde_param;
	if (on_off == true && bt_on == true) {
		if (auto_vork == false) {
			button_auto->BackColor = Color::Red;
			auto_vork = true;
			R_IDX = 1;
			C_IDX = 1;
			auto_R_IDX = 1;
			for (int R_idx = 1; R_idx <= 8; R_idx++) {
				Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (R_idx).ToString()];
				BUTTON_R->BackColor = DefaultBackColor;
				CheckBox^ CHECKBOX_R = (CheckBox^)this->tableLayoutPanel1->Controls["checkBox_R" + (R_idx).ToString()];
				CHECKBOX_R->Checked = false;
				for (int N_sonde = 1; N_sonde <= sonde_param.size(); N_sonde++) {
					TextBox^ TEXTBOX_R_T = (TextBox^)this->tableLayoutPanel1->Controls["textBox_R" + R_idx + "_T" + (N_sonde).ToString()];
					TEXTBOX_R_T->Text = "";
				}
			}
		}

		else {
			button_auto->BackColor = DefaultBackColor;
			auto_vork = false;
			start_R_measure = false;
			end_R_measure = true;
			//auto_R_IDX = 10;
			int work_result = Loop_work(0, 0);
			R_IDX = 0;
			C_IDX = 0;
			if (!work_result) {
				button_AIR->BackColor = Color::Red;
				for (int R_idx = 1; R_idx <= 8; R_idx++) {
					Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (R_idx).ToString()];
					BUTTON_R->BackColor = DefaultBackColor;
				}
			}
					
		}


	}

}
//отображение графиков и текста		 
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	extern V10 ph_data;
	extern V10 ro_data;
	//длина вектора равна количеству зондов
	extern vector <SONDE_PARAM> sonde_param;
	//длина вектора равна количеству сопротивлений
	extern vector <LOOP_PARAM> loop_param;
	// 10 - зонды 20 - усреднение
	extern int PH_percent_from_sonde[10][20];
	extern int Ro_percent_from_sonde[10][20];
	extern vector <V10> PH_computed;
	extern vector <V10> Ro_computed;

	//кнопка локального воздуха////////////
	int countdown = air_data_aver - air_set_cntr;
	if (countdown < 0) countdown = 0;
	if (collect_air_data == true) {
		this->button_local_metro->BackColor = System::Drawing::Color::Coral;
		button_local_metro->Text = "Идет сбор данных " + countdown + "C";
	}
	else {
		if (local_air_is_set == true) {
			this->button_local_metro->BackColor = System::Drawing::Color::Coral;
			this->button_local_metro->Text = "Локальный воздух установлен";
		}
		else {
			this->button_local_metro->BackColor = DefaultBackColor;
			this->button_local_metro->Text = L"ЛОКАЛЬНЫЙ ВОЗДУХ ";
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	// выводим фазы - воздух
	for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
		TextBox^ TEXTBOX_DFI = (TextBox^)this->tableLayoutPanel1->Controls["textBox_dfi" + (N_sonde + 1).ToString()];
		TEXTBOX_DFI->Text = ((ph_data.VALUE[N_sonde])*mG).ToString("F0");
	}

	//выводим фазы на графики
	if (R_IDX == 0 && C_IDX == 0 && on_off == 1) {
		pictureBox_legend_up->Image = nullptr;
		do_legend(trashhold, AIR_MODE);
		for (int N_sonde = 1; N_sonde <= sonde_param.size(); N_sonde++) {
			int ph_for_graf = (int)((ph_data.VALUE[N_sonde-1]*mG) /6.0f)  ;
			do_graf(N_sonde, &ph_for_graf, 1, trashhold);
		}
	}

	//если не на воздухе,то выводим УЭС
	if (R_IDX != 0 && C_IDX != 0) {
		pictureBox_legend_up->Image = nullptr;
		do_legend(trashhold, MEAS_MODE);
		for (int N_sonde = 0; N_sonde < sonde_param.size(); N_sonde++) {
			TextBox^ TEXTBOX_Ro = (TextBox^)this->tableLayoutPanel1->Controls["textBox_ro" + (N_sonde + 1).ToString()];
			if (ro_data.VALUE[N_sonde] < 10)
				TEXTBOX_Ro->Text = ((ro_data.VALUE[N_sonde])).ToString("F2");
			else
				TEXTBOX_Ro->Text = ((ro_data.VALUE[N_sonde])).ToString("F0");
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////
	
	int N_last_points = aver_cntr;
	if (N_last_points > 20)N_last_points = 20;

	//переключение конденсатора для каждого зонда - отображаем по одному графику
	if (checkBox_express->Checked == false ||
		(checkBox_express->Checked == true && R_IDX <= N_R_for_express)) {
		if (C_IDX != 0 ) {
			    do_graf(C_IDX, &PH_percent_from_sonde[C_IDX - 1][0], N_last_points, trashhold);
		}
			
	}
	//переключение конденсатора для всех зондов, начиная с 3го сопротивления
	if (checkBox_express->Checked == true && R_IDX > N_R_for_express) {
		//отображаем 1е 4 или 5
		if (C_IDX == 1 ){
			for (int N_sonde = 1; N_sonde <= (int)((sonde_param.size() / 2)); N_sonde++) {
				do_graf(N_sonde, &PH_percent_from_sonde[N_sonde-1][0], N_last_points, trashhold);
			}
		}
		if (C_IDX == (int)((sonde_param.size() / 2) + 1)) {
			for (int N_sonde = (int)((sonde_param.size() / 2) + 1); N_sonde <= sonde_param.size(); N_sonde++) {
				do_graf(N_sonde, &PH_percent_from_sonde[N_sonde-1][0], N_last_points, trashhold);
			}
		}
	}
	////////////////////////////////////////////////////////////////////

	//рассчет фазы и УЭС от всех сопротивлений кольца для всех зондов 
	//
	if (on_off == true && bt_on == true && PH_Ro_is_calculated == false) {
		//получаем параметры от кольца//////////////////////////////////////////////////////////////////////////////
		int stop = 0;
		String^ Str = "";
		String^ send_Z = "";
		String^ resive_Z = "";
		Byte resive = 0;
		float R[32];
		float Re[32];

		if (bt_on == true) {
			Loop_Check_Timer->Enabled = false;
			int stop = 0;
			String^ resive = "";
			for (int n = 0; n < 10; n++) {
				send_Z = "ZREAD " + (n)+"\r\n"; resive_Z = "Z" + (n)+" ";
				serialPort_loop->DiscardInBuffer();
				serialPort_loop->Write(send_Z);
				Sleep(50);
				try { resive = serialPort_loop->ReadLine(); }
				catch (...) { stop = 1; }
				if (!stop) {
					if (resive->StartsWith(resive_Z)) {
						Str = resive->Substring(3);
						int index_0 = Str->IndexOf(" ");
						R[n] = Double::Parse((Str->Substring(0, index_0))->Replace(".", ","));
						Re[n] = Double::Parse((Str->Substring(index_0 + 1))->Replace(".", ","));
						Console::WriteLine(n + " R " + R[n] + " Re " + Re[n]);
						TextBox^ TEXTBOX_Re = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];
						TextBox^ TEXTBOX_R = (TextBox^)this->tableLayoutPanel11->Controls["textBox_R_loop" + (n + 1).ToString()];
						TEXTBOX_Re->Text = Re[n].ToString("F4");
						TEXTBOX_R->Text = R[n].ToString("F0");
						TEXTBOX_Re->BackColor = Color::White;
						Z_downloaded_from_loop = true;

					}
					else {
						Z_downloaded_from_loop = false;				
						MessageBox::Show("Нет ответа от кольца " + n);
					}
				}
				//else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 3!"); }
			}
			Loop_Check_Timer->Enabled = true;
		}
		if (bt_on == false) { MessageBox::Show("ВКЛЮЧИТЬ КОЛЬЦО"); }
		//заполняем вектор параметров кольца
		if (Z_downloaded_from_loop == true) {
			loop_param.clear();
			for (int n = 0; n < 10; n++) {
				if (Re[n] > 0.000001f) {
					LOOP_PARAM buff;
					buff.R = R[n];
					buff.r = 0.5;
					buff.Re = Re[n];
					buff.R_Add_Wire = 0.5;
					loop_param.push_back(buff);
				}
			}
		}
		else
			MessageBox::Show("Параметры от кольца не получены");
/////////////////////////////////////////////////////////////////////////////////
		if(Z_downloaded_from_loop == true){
			calc_PH_from_loop(sonde_param, loop_param, PH_computed);
			calc_Ro_from_loop(sonde_param, PH_computed, Ro_computed);

			for (int n = 0; n < 8; n++) {
				Button^ BUTTON_R = (Button^)this->tableLayoutPanel1->Controls["button_R" + (n+1).ToString()];
	            BUTTON_R->Text = R[n].ToString("F0");
			}


			//отладка/////////////////////////////////////////
			for (int R_idx = 0; R_idx < 8; R_idx++) {
				cout << "PH ";
				for (int n_sonde = 0; n_sonde < sonde_param.size(); n_sonde++)
					cout << PH_computed[R_idx].VALUE[n_sonde] << " ";
				cout << endl;
			}
			cout << endl;
			for (int R_idx = 0; R_idx < PH_computed.size(); R_idx++) {
				cout << "Ro ";
				for (int n_sonde = 0; n_sonde < sonde_param.size(); n_sonde++)
					cout << Ro_computed[R_idx].VALUE[n_sonde] << " ";
				cout << endl;
			}
			////////////////////////////////////////////////////////
			PH_Ro_is_calculated = true;
        }	
	}
	//если был выключен прибор или кольцо - заново читаем параметры кольца при следующем включении
	if (on_off == false || bt_on == false) {
		PH_Ro_is_calculated = false;
	}

	// автоматические измерения
	if(auto_vork == true){
		if (start_R_measure == false) {
			Sleep(200);
			cout << "auto_R_IDX "<< auto_R_IDX << endl;
			if (auto_R_IDX <= 8)
				R_Click(auto_R_IDX, 1);
			auto_R_IDX++;
		}
		if (auto_R_IDX > 8) {
			//auto_R_IDX = 1;
			auto_vork = false;
			button_auto->BackColor = DefaultBackColor;
		}
	}

	//ставим галочку, если измерения в пределах нормы
	if (end_R_measure == true) {
		for (int R_idx = 1; R_idx <= 8; R_idx++){

			bool measure_is_good = false;

			for (int N_sonde = 1; N_sonde <= sonde_param.size(); N_sonde++) {
				TextBox^ TEXTBOX_R_T = (TextBox^)this->tableLayoutPanel1->Controls["textBox_R" + R_idx + "_T" + (N_sonde).ToString()];
				if (TEXTBOX_R_T->Text != "") {
					if (TEXTBOX_R_T->ForeColor == System::Drawing::Color::LimeGreen ||
						TEXTBOX_R_T->ForeColor == System::Drawing::Color::Gold) {
						measure_is_good = true;
					}
					else
						measure_is_good = false;
				}
				else
					measure_is_good = false;
			}

			CheckBox^ CHECKBOX_R = (CheckBox^)this->tableLayoutPanel1->Controls["checkBox_R" + (R_idx).ToString()];
			if (measure_is_good == true)
				CHECKBOX_R->Checked = true;
			else
				CHECKBOX_R->Checked = false;
		}
		end_R_measure = false;
	}
	
	if (comboBox_trashhold->Text != "") trashhold = System::Int32::Parse((comboBox_trashhold->Text));
	
	


}

//сохранить рапорт
private: System::Void сохранитьРапортRtfToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	extern vector <LOOP_PARAM> loop_param;
	extern vector <SONDE_PARAM> sonde_param;
	extern int Ro_percent_from_sonde[10][20];
	String^ serial = sonde_kojuh_serial.ToString();
	time_t now = time(NULL);
	struct tm  t = *localtime(&now);
	String^ month;
	extern  double R_loop_physical[9];
	int otstup_sverhu = 60;
	String^ R_index;

	if(sonde_param.size()!= 0 && loop_param.size() != 0){
		if (on_off == true) {
			Sonde_Read_Timer->Enabled = false;
		}

		if (t.tm_mon + 1 == 1)month = " Января ";
		else if (t.tm_mon + 1 == 2)month = " Февраля ";
		else if (t.tm_mon + 1 == 3)month = " Марта ";
		else if (t.tm_mon + 1 == 4)month = " Апреля ";
		else if (t.tm_mon + 1 == 5)month = " Мая ";
		else if (t.tm_mon + 1 == 6)month = " Июня ";
		else if (t.tm_mon + 1 == 7)month = " Июля ";
		else if (t.tm_mon + 1 == 8)month = " Августа ";
		else if (t.tm_mon + 1 == 9)month = " Сентября ";
		else if (t.tm_mon + 1 == 10)month = " Октября ";
		else if (t.tm_mon + 1 == 11)month = " Ноября ";
		else if (t.tm_mon + 1 == 12)month = " Декабря ";
		auto font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
		richTextBox1->SelectionFont = font;
		richTextBox1->SelectionColor = Color::Red;
		richTextBox1->AppendText("Протокол проверки прибора ЭМК-90 № " + serial + Environment::NewLine);
		font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
		richTextBox1->SelectionFont = font;
		richTextBox1->SelectionColor = Color::Black;

		richTextBox1->AppendText("\r\n \r\n Расчет НЕсимметризованных фаз и УЭС на компьютере " + Environment::NewLine);

		//УЭС
		richTextBox1->AppendText("\r\n  ЗОНД        Погрешность % " + Environment::NewLine);
		//номер сопротивления 
		for (int R_idx = 1; R_idx <= 8; R_idx++) {
			if (R_idx <= 8)R_index = "\r\n Мера импеданса № " + R_idx.ToString() + "  " + loop_param[R_idx-1].R + " Ом  \r\n";
			else R_index = "\r\n Воздух________________________________________________________________________________ \r\n";
			richTextBox1->AppendText(R_index + Environment::NewLine);

			CheckBox^ CHECKBOX_R = (CheckBox^)this->tableLayoutPanel1->Controls["checkBox_R" + (R_idx).ToString()];
			if (CHECKBOX_R->Checked == true) {
				String^ frq; String^ nomer;
				//номер передатчика 
				for (int N_sonde = 1; N_sonde <= sonde_param.size(); N_sonde++) {
					TextBox^ TEXTBOX_R_T = (TextBox^)this->tableLayoutPanel1->Controls["textBox_R" + R_idx + "_T" + (N_sonde).ToString()];

					if (TEXTBOX_R_T->Text != "") {
						if (TEXTBOX_R_T->ForeColor == System::Drawing::Color::LimeGreen) richTextBox1->SelectionColor = Color::Green;
						if (TEXTBOX_R_T->ForeColor == System::Drawing::Color::Gold) richTextBox1->SelectionColor = Color::Yellow;
						if (TEXTBOX_R_T->ForeColor == System::Drawing::Color::Red) richTextBox1->SelectionColor = Color::Red;
						if (TEXTBOX_R_T->ForeColor == System::Drawing::Color::DarkRed) richTextBox1->SelectionColor = Color::DarkRed;

						frq = (sonde_param[N_sonde - 1].F / 1000).ToString();
						nomer = (N_sonde).ToString();

						String^ to_print;
						to_print = "T_" + nomer + "   " + frq + "kGz " + "            " +  TEXTBOX_R_T->Text ;
						richTextBox1->AppendText(to_print + Environment::NewLine);
						richTextBox1->SelectionColor = Color::Black;
					}
					else {
						richTextBox1->SelectionColor = Color::Black;
						richTextBox1->AppendText("Нет данных \r\n");
					}
				}

			}
			else {
				richTextBox1->SelectionColor = Color::Black;
				richTextBox1->AppendText("Нет данных \r\n");
			}
		}

		richTextBox1->SelectionColor = Color::Black;
		richTextBox1->AppendText("\r\n\r\n\r\n");
		richTextBox1->AppendText("Заключение ____________________________________________________________________________\r\n");
		richTextBox1->AppendText(" ______________________________________________________________________________________\r\n");
		richTextBox1->AppendText(" ______________________________________________________________________________________\r\n");
		richTextBox1->AppendText(" ______________________________________________________________________________________\r\n");

		richTextBox1->AppendText("Ф И О, должность, подпись______________________________________________________" + t.tm_mday + month + (t.tm_year + 1900) + " г. " + t.tm_hour + " :" + t.tm_min);

		// сохраняем рапорт	
		saveFileDialog1->Filter = "text files (*.rtf)|*.rtf";
		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			try {
				richTextBox1->SaveFile(saveFileDialog1->FileName);

			}
			catch (Exception^ e) {
				MessageBox::Show(L"Ошибка сохранения файла");
			}
			for (int R_idx = 1; R_idx <= 8; R_idx++) {
				for (int N_sonde = 1; N_sonde <= sonde_param.size(); N_sonde++) {
					TextBox^ TEXTBOX_R_T = (TextBox^)this->tableLayoutPanel1->Controls["textBox_R" + R_idx + "_T" + (N_sonde).ToString()];
					TEXTBOX_R_T->Text = "";
				}
			}
			//button_ON_OFF->PerformClick();
			//on_off = false;
			//return;
		}
	
		if (on_off == true) {
			Sonde_Read_Timer->Enabled = true;
		}
	
    }

}
//выбор com портов
private: System::Void comboBox_sonde_com_DropDown(System::Object^  sender, System::EventArgs^  e) {
	comboBox_sonde_com->Items->Clear();
	cli::array<String^>^ ports = nullptr;
	ports = serialPort_sonde->GetPortNames();
	comboBox_sonde_com->Items->AddRange(ports);
}
private: System::Void comboBox_sonde_com_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e) {
	serialPort_sonde->PortName = comboBox_sonde_com->Text;
	cout << (char*)Marshal::StringToHGlobalAnsi(comboBox_sonde_com->Text).ToPointer() << endl;
}
private: System::Void comboBox_loop_com_DropDown(System::Object^  sender, System::EventArgs^  e) {
	comboBox_loop_com->Items->Clear();
	cli::array<String^>^ ports = nullptr;
	ports = serialPort_loop->GetPortNames();
	comboBox_loop_com->Items->AddRange(ports);
}
private: System::Void comboBox_loop_com_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e) {
	serialPort_loop->PortName = comboBox_loop_com->Text;
	cout << (char*)Marshal::StringToHGlobalAnsi(comboBox_loop_com->Text).ToPointer() << endl;
}
private: System::Void digit2(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
	if ((e->KeyChar) == '-') e->Handled = false; if ((e->KeyChar) == ',') e->Handled = false;
}

private: System::Void WorkForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	extern bool express;
	ofstream conf_out;
	conf_out.open(ConfName, ios::out);
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox_sonde_com->Text).ToPointer() << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox_loop_com->Text).ToPointer() << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox_aver->Text).ToPointer() << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox_trashhold->Text).ToPointer() << endl;
	conf_out << checkBox_express->Checked << endl;
	conf_out << (char*)Marshal::StringToHGlobalAnsi(comboBox_tool_type->Text).ToPointer() << endl;	
	conf_out.close();
}
//update
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	start = 1;//убрать 
	if (start == 0) {
		remove("LOOP_TEST.exe");
		MoveFile("LOOP_TEST_NEW.exe", "LOOP_TEST.exe");
		CHAR path[MAX_PATH];
		GetModuleFileNameA(NULL, path, MAX_PATH);
		string path_str(path);
		std::cout << "path_str " << path_str << endl;
		path_str = path_str.substr(0, path_str.find_last_of("\\"));
		std::cout << "path_str1 " << path_str << endl;

		int version_now = 0, version_new = 0;
		string v;
		ifstream fin;
		ofstream fout;

		fin.open("version.txt", ios::in);
		fin >> v;
		if (v == "version") {
			fin >> version_now;
			cout << "version_now " << version_now << endl;
		}
		fin.close();

		HINTERNET hInet = NULL, hUrl;
		ULONG timeout = 2000; // миллисекунды.
		HRESULT res = -1;
		InternetSetOption(hInet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
		hInet = InternetOpen(
			"Microsoft Internet Explorer",  // agent
			INTERNET_OPEN_TYPE_PRECONFIG,   // access
			NULL, NULL, 0);                 // defaults

		hUrl = InternetOpenUrl(hInet,
			"ftp://AMK_VORONEJ:DaJa1305@89.189.156.184/VORONEJ_FTP/UPDATE/LOOP_TEST",
			NULL,
			NULL,
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE,
			NULL);
		InternetCloseHandle(hInet);

		cout << "hInet " << hInet << " hUrl " << hUrl << endl;
		if (hUrl != 0) {
			res = DeleteUrlCacheEntryA("ftp://89.189.156.184/VORONEJ_FTP/UPDATE/LOOP_TEST/version.txt");//вводить без пароля
			cout << "DeleteUrlCache " << res << endl;
			res = URLDownloadToFile(NULL, "ftp://AMK_VORONEJ:DaJa1305@89.189.156.184/VORONEJ_FTP/UPDATE/LOOP_TEST/version.txt", "version_url.txt", BINDF_GETNEWESTVERSION, NULL);

		}

		cout << "URLDownloadToFile " << res << endl;
		if (res == 0) {
			fin.open("version_url.txt", ios::in);
			fin >> v;
			if (v == "version")fin >> version_new;
			cout << "version_new " << version_new << endl;
			if (version_new - version_now > 0) {

				if (MessageBox::Show("Появилось обновление, скачать? ", "", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes) {
					MessageBox::Show("В папке программы старые \r\n файлы будут заменены на новые");

					String ^download_path = gcnew System::String(path_str.c_str());
					Console::WriteLine("Path " + download_path);
					String ^download_LOOPexe;
					String ^download_Dll;
					String ^download_version;

					const char* Download_LOOPexe;
					const char* Download_Dll;
					const char* Download_version;

					download_LOOPexe = download_path + "/LOOP_TEST_NEW.exe";
					download_Dll = download_path + "/AMK_1DDS_AUTONOM.dll";
					download_version = download_path + "/version.txt";
					Download_LOOPexe = (char*)Marshal::StringToHGlobalAnsi(download_LOOPexe).ToPointer();
					Download_Dll = (char*)Marshal::StringToHGlobalAnsi(download_Dll).ToPointer();
					Download_version = (char*)Marshal::StringToHGlobalAnsi(download_version).ToPointer();

					/*
					HRESULT res0 = URLDownloadToFile(NULL, "ftp://amkg:amkg@89.189.156.184/voronej/update/loop/LOOP_TEST.exe", Download_LOOPexe, 0, NULL);
					HRESULT res1 = URLDownloadToFile(NULL, "ftp://amkg:amkg@89.189.156.184/voronej/update/loop/AMK_1DDS_AUTONOM.dll", Download_Dll, 0, NULL);
					HRESULT res2 = URLDownloadToFile(NULL, "ftp://amkg:amkg@89.189.156.184/voronej/update/loop/version.txt", Download_version, 0, NULL);
					*/
					HRESULT res0 = URLDownloadToFile(NULL, "ftp://AMK_VORONEJ:DaJa1305@89.189.156.184/VORONEJ_FTP/UPDATE/LOOP_TEST/LOOP_TEST.exe", Download_LOOPexe, 0, NULL);
					HRESULT res1 = URLDownloadToFile(NULL, "ftp://AMK_VORONEJ:DaJa1305@89.189.156.184/VORONEJ_FTP/UPDATE/LOOP_TEST/AMK_1DDS_AUTONOM.dll", Download_Dll, 0, NULL);
					HRESULT res2 = URLDownloadToFile(NULL, "ftp://AMK_VORONEJ:DaJa1305@89.189.156.184/VORONEJ_FTP/UPDATE/LOOP_TEST/version.txt", Download_version, 0, NULL);

					cout << "res " << res0 << " " << res1 << " " << res2 << endl;

					if (res0 == 0 && res1 == 0 && res2 == 0) {
						MessageBox::Show("обновленные файлы скачаны в папку программы");
						version_now = version_new;
						time_t seconds = time(NULL);
						fout.open("version.txt", ios::out);
						fout << "version " << version_now << endl;
						fout << "time " << seconds << endl;
						fout.close();
						cout << "time_t " << seconds << endl;
						ShellExecute(0, 0, "LOOP_TEST_NEW.exe", 0, 0, SW_SHOW);
						exit(0);
					}

					else
						MessageBox::Show("скачивание не удалось попробуйте в следующий раз!");



				}
			}
			fin.close();
			remove("version_url.txt");

			time_t seconds = time(NULL);
			fout.open("version.txt", ios::out);
			fout << "version " << version_now << endl;
			fout << "time " << seconds << endl;
			fout.close();
			cout << "time_t " << seconds << endl;
		}
		else {
			time_t seconds, seconds_now = time(NULL);
			fin.open("version.txt", ios::in);
			fin >> v >> version_now >> v >> seconds;
			fin.close();
			if (seconds_now - seconds > 700000)
				MessageBox::Show("Давно не было информации об обновлениях проверьте подключение к интернету!");

			fout.open("version.txt", ios::out);
			fout << "version " << version_now << endl;
			fout << "time " << seconds_now << endl;
			fout.close();
		}
		start = 1;
	}

}
private: System::Void uploadToFTPToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (MessageBox::Show("если вы не разработчик, нажмите 'Нет' ", "", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes) {
		int version_now;
		string v;
		ifstream fin;
		ofstream fout;

		fin.open("version.txt", ios::in);
		fin >> v;
		if (v == "version") {
			fin >> version_now;
			cout << "version_now " << version_now << endl;
		}
		fin.close();

		fout.open("version_to_ftp.txt", ios::out);
		fout << "version ";
		fout << (version_now + 1);
		fout.close();


		HINTERNET hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		HINTERNET hFtpSession = InternetConnect(hInternet, "89.189.156.184", 21, "AMK_VORONEJ", "DaJa1305", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

		std::cout << "FtpSettDirectory "
			//<< FtpSetCurrentDirectoryA(hFtpSession, "/voronej/update/loop")
			<< FtpSetCurrentDirectoryA(hFtpSession, "VORONEJ_FTP/UPDATE/LOOP_TEST")
			<< std::endl;

		BOOL Transfer0 = FtpPutFile(hFtpSession, "version_to_ftp.txt", "version.txt", FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_RELOAD);
		BOOL Transfer1 = FtpPutFile(hFtpSession, "C:\\WORK\\repos\\LOOP_NEW\\Release\\LOOP.exe", "LOOP_TEST.exe", FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_RELOAD);
		BOOL Transfer2 = FtpPutFile(hFtpSession, "C:\\WORK\\repos\\LOOP_NEW\\Release\\AMK_1DDS_AUTONOM.dll", "AMK_1DDS_AUTONOM.dll", FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_RELOAD);
		if (Transfer0 == 1 && Transfer1 == 1 && Transfer2 == 1)
			MessageBox::Show("файлы программы записаны на FTP сервер");
		else
			MessageBox::Show("файлы НЕ БЫЛИ записаны, попробуйте еще раз");

		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
	}




}
private: System::Void работаСФайломМетрологииToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	AIRTEMP10::MyForm1^ Metro = gcnew AIRTEMP10::MyForm1;
	Metro->Show();
}
private: System::Void пОМОЩЬToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	LOOP::Help_form^ Help = gcnew LOOP::Help_form;
	Help->Show();
}

private: System::Void comboBox_aver_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e) {
	N_averaging = Int32::Parse(comboBox_aver->Text);
}

private: System::Void comboBox_tool_type_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	tool_type = comboBox_tool_type->SelectedIndex;
}
};
}
