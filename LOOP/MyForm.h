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
#include <typeinfo.h>
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
//#include "sondedll.h"
//#pragma comment(lib,"C:\\WORK\\repos\\SONDE_DLL\\SONDE_DLL_4.0\\Release\\SONDE_DLL_1.0.lib") 

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
extern CheckConnect AMK_CheckConnect;
extern SetIQPhaseOffset AMK_SetIQPhaseOffset;
extern DDS3FreqOffset AMK_DDS3FreqOffset;
extern GetMetroData AMK_GetMetroData;
extern SetMetroData AMK_SetMetroData;
extern GetSerial AMK_GetSerial;
extern void *ComPort;
extern uint32_t DDS_3FreqOffset;
extern struct PhaseOffset phaseOffset;

extern void *ComPort;


namespace LOOP {
	using namespace std;
	using namespace System;
	using namespace System::Timers;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Printing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Windows::Forms::DataVisualization::Charting;
	using namespace System::IO::Ports;
	using namespace System::Threading;
	using namespace System::Windows;
	
	

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		ofstream *fout;
		V8 *Ro_chart;
		V8 *Ro_aver;
		V8 *Phase_chart;
		V8 *Phase_aver;
		V8 *Ro_for_print;
		V8 *Phase_for_print;
		//V8 **Ro_percent_temperature;
		V8 **Phase_temperature;
		V8 *Ro_percent_for_print;
		V8 *Phase_percent_for_print;
		
		V8 *Phase_raw;
		V8 *Phase_raw_aver;
		V8 *Data_Cal;
		V8 *Ro_adecvat_computed;//
		V8 *Phase_loop_computed;//
		
		Thread^ Read_Port_Thread;
		Thread^ Chart_Thread;
		Thread^ Loop_Port_Thread;
		double *C;
		double *Re_from_loop;
		double *C_meas;
		double *F_rez;
		double *L1;
		double *L2;
		double *f_;
		bool *green;
		double L_loop = 4.6*1e-6;
		double * R_Add_Wire;
		int loop_version = 0;

	private: System::Timers::Timer^ ToolTimer;

	private: System::Windows::Forms::Button^  buttonR9;
	public:

	public:

	private: System::Windows::Forms::Button^  buttonR1;
	private: System::Windows::Forms::Button^  buttonR2;
	private: System::Windows::Forms::Button^  buttonR3;
	private: System::Windows::Forms::Button^  buttonR4;
	public:


	private: System::Windows::Forms::ToolStripMenuItem^  параметрыToolStripMenuItem;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart3;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart4;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart5;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart6;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart7;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart8;
	private: System::Windows::Forms::TextBox^  textBox16;
	private: System::Windows::Forms::TextBox^  textBox15;
	private: System::Windows::Forms::TextBox^  textBox14;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::TextBox^  textBox12;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::TextBox^  textBox9;


	private: System::Windows::Forms::Button^  button_Report;

	private: System::Windows::Forms::PictureBox^  pictureBoxAir;
	private: System::Windows::Forms::PictureBox^  pictureBoxR1;
	private: System::Windows::Forms::PictureBox^  pictureBoxR2;
	private: System::Windows::Forms::PictureBox^  pictureBoxR3;
	private: System::Windows::Forms::PictureBox^  pictureBoxR4;


	private: System::Drawing::Printing::PrintDocument^  docToPrint;


			 SerialPort^ serialPort;
	private: System::Windows::Forms::Button^  button_BT_ON;



	private: System::Windows::Forms::ComboBox^  comboBox1;


	private: System::Windows::Forms::TextBox^  textBox17;
	private: System::Windows::Forms::TextBox^  textBox18;
	private: System::Windows::Forms::TextBox^  textBox19;

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
	private: System::Windows::Forms::TextBox^  textBox21;
	private: System::Windows::Forms::TextBox^  textBox20;
	private: System::Windows::Forms::TextBox^  textBox22;
	private: System::Windows::Forms::TextBox^  textBox23;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::TextBox^  textBox7;

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel3;
	private: System::Windows::Forms::TextBox^  textBox24;
	private: System::Windows::Forms::TextBox^  textBox25;
	private: System::Windows::Forms::TextBox^  textBox26;
	private: System::Windows::Forms::TextBox^  textBox27;
	private: System::Windows::Forms::TextBox^  textBox28;
	private: System::Windows::Forms::TextBox^  textBox29;
	private: System::Windows::Forms::TextBox^  textBox30;
	private: System::Windows::Forms::TextBox^  textBox31;

private: System::Windows::Forms::TextBox^  textBox55;
private: System::Windows::Forms::TextBox^  textBox48;
private: System::Windows::Forms::TextBox^  textBox32;
private: System::Windows::Forms::TextBox^  textBox33;
private: System::Windows::Forms::TextBox^  textBox34;
private: System::Windows::Forms::TextBox^  textBox35;
private: System::Windows::Forms::TextBox^  textBox36;
private: System::Windows::Forms::TextBox^  textBox37;
private: System::Windows::Forms::TextBox^  textBox38;
private: System::Windows::Forms::TextBox^  textBox39;
private: System::Windows::Forms::TextBox^  textBox40;
private: System::Windows::Forms::TextBox^  textBox41;
private: System::Windows::Forms::TextBox^  textBox42;
private: System::Windows::Forms::TextBox^  textBox43;
private: System::Windows::Forms::TextBox^  textBox44;
private: System::Windows::Forms::TextBox^  textBox45;
private: System::Windows::Forms::TextBox^  textBox46;
private: System::Windows::Forms::TextBox^  textBox47;
private: System::Windows::Forms::TextBox^  textBox54;
private: System::Windows::Forms::TextBox^  textBox49;
private: System::Windows::Forms::TextBox^  textBox50;
private: System::Windows::Forms::TextBox^  textBox51;
private: System::Windows::Forms::TextBox^  textBox52;
private: System::Windows::Forms::TextBox^  textBox53;
private: System::Windows::Forms::TextBox^  textBox88;
private: System::Windows::Forms::TextBox^  textBox89;
private: System::Windows::Forms::TextBox^  textBox90;
private: System::Windows::Forms::TextBox^  textBox91;
private: System::Windows::Forms::TextBox^  textBox92;
private: System::Windows::Forms::TextBox^  textBox93;
private: System::Windows::Forms::TextBox^  textBox95;


private: System::Windows::Forms::TextBox^  textBox94;
private: System::Windows::Forms::TextBox^  textBox128;


private: System::Windows::Forms::TextBox^  textBox120;
private: System::Windows::Forms::TextBox^  textBox121;
private: System::Windows::Forms::TextBox^  textBox122;
private: System::Windows::Forms::TextBox^  textBox123;

private: System::Windows::Forms::TextBox^  textBox103;
private: System::Windows::Forms::TextBox^  textBox129;

private: System::Windows::Forms::TextBox^  textBox96;
private: System::Windows::Forms::TextBox^  textBox97;
private: System::Windows::Forms::TextBox^  textBox99;



private: System::Windows::Forms::TextBox^  textBox98;
private: System::Windows::Forms::TextBox^  textBox101;


private: System::Windows::Forms::TextBox^  textBox100;
private: System::Windows::Forms::TextBox^  textBox102;


private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel4;
private: System::Windows::Forms::ToolStripMenuItem^  открытьToolStripMenuItem;


private: System::Windows::Forms::Panel^  panel1;
private: System::Windows::Forms::TabControl^  tabControl1;
private: System::Windows::Forms::TabPage^  main;


private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;


private: System::Windows::Forms::TextBox^  textBox56;
private: System::Windows::Forms::TextBox^  textBox57;
private: System::Windows::Forms::TextBox^  textBox58;
private: System::Windows::Forms::TextBox^  textBox59;
private: System::Windows::Forms::TextBox^  textBox60;
private: System::Windows::Forms::TextBox^  textBox61;
private: System::Windows::Forms::TextBox^  textBox62;
private: System::Windows::Forms::TextBox^  textBox63;
private: System::Windows::Forms::TextBox^  textBox64;
private: System::Windows::Forms::TextBox^  textBox65;
private: System::Windows::Forms::TextBox^  textBox66;
private: System::Windows::Forms::TextBox^  textBox67;
private: System::Windows::Forms::TextBox^  textBox68;
private: System::Windows::Forms::TextBox^  textBox69;
private: System::Windows::Forms::TextBox^  textBox70;
private: System::Windows::Forms::TextBox^  textBox71;
private: System::Windows::Forms::TextBox^  textBox72;
private: System::Windows::Forms::TextBox^  textBox73;
private: System::Windows::Forms::TextBox^  textBox74;
private: System::Windows::Forms::TextBox^  textBox75;
private: System::Windows::Forms::TextBox^  textBox76;
private: System::Windows::Forms::TextBox^  textBox77;
private: System::Windows::Forms::TextBox^  textBox78;
private: System::Windows::Forms::TextBox^  textBox79;
private: System::Windows::Forms::TextBox^  textBox80;
private: System::Windows::Forms::TextBox^  textBox81;
private: System::Windows::Forms::TextBox^  textBox82;
private: System::Windows::Forms::TextBox^  textBox83;
private: System::Windows::Forms::TextBox^  textBox84;
private: System::Windows::Forms::TextBox^  textBox85;
private: System::Windows::Forms::TextBox^  textBox86;
private: System::Windows::Forms::TextBox^  textBox87;
private: System::Windows::Forms::TextBox^  textBox127;

private: System::Windows::Forms::TextBox^  textBox126;
private: System::Windows::Forms::TextBox^  textBox125;
private: System::Windows::Forms::TextBox^  textBox124;
private: System::Windows::Forms::Button^  buttonR5;
private: System::Windows::Forms::Button^  buttonR6;
private: System::Windows::Forms::Button^  buttonR7;
private: System::Windows::Forms::Button^  buttonR8;
private: System::Windows::Forms::PictureBox^  pictureBoxR5;
private: System::Windows::Forms::PictureBox^  pictureBoxR6;
private: System::Windows::Forms::PictureBox^  pictureBoxR7;
private: System::Windows::Forms::PictureBox^  pictureBoxR8;


private: System::Windows::Forms::ToolTip^  toolTip1;
private: System::Windows::Forms::HelpProvider^  helpProvider1;
private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;

private: System::Windows::Forms::ToolStripMenuItem^  сохранитьРапортRtfToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  работаСФайломМетрологииToolStripMenuItem;


private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel7;

private: System::Windows::Forms::Button^  buttonLocalMetro;


private: System::Windows::Forms::TextBox^  textBox8;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_0;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_1;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_2;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_3;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_4;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_5;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_6;
private: System::Windows::Forms::TextBox^  textBox_AM_Tx_7;
private: System::Windows::Forms::CheckBox^  checkBox_С;

private: System::Windows::Forms::Timer^  timer_Temperature;


private: System::Windows::Forms::Button^  button_Auto;
private: System::Windows::Forms::Button^  button_Clear;


private: System::IO::Ports::SerialPort^  serialPort1;

private: System::Windows::Forms::ToolStripMenuItem^  помощьToolStripMenuItem;

private: System::Windows::Forms::Button^  button_ON_OFF;


private: System::Windows::Forms::ComboBox^  comboBox_sonde_type;
private: System::IO::Ports::SerialPort^  serialPort_LWD_1DDS;

private: System::Windows::Forms::TabPage^  impedance;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel11;
private: System::Windows::Forms::Label^  label11;

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
private: System::Windows::Forms::Button^  button_Z1;
private: System::Windows::Forms::Button^  button_Z2;
private: System::Windows::Forms::Button^  button_Z3;
private: System::Windows::Forms::Button^  button_Z4;
private: System::Windows::Forms::Button^  button_Z5;
private: System::Windows::Forms::Button^  button_Z6;
private: System::Windows::Forms::Button^  button_Z7;
private: System::Windows::Forms::Button^  button_Z8;

private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::TextBox^  textBox_f_1;
private: System::Windows::Forms::TextBox^  textBox_f_2;
private: System::Windows::Forms::TextBox^  textBox_f_3;
private: System::Windows::Forms::TextBox^  textBox_f_4;
private: System::Windows::Forms::TextBox^  textBox_f_5;
private: System::Windows::Forms::TextBox^  textBox_f_6;
private: System::Windows::Forms::TextBox^  textBox_f_7;
private: System::Windows::Forms::TextBox^  textBox_f_8;

private: System::Windows::Forms::ToolStripMenuItem^  Файл_метрологии_открыть;
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
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::TextBox^  textBox_Z_loop1;
private: System::Windows::Forms::TextBox^  textBox_Z_loop2;
private: System::Windows::Forms::TextBox^  textBox_Z_loop3;
private: System::Windows::Forms::TextBox^  textBox_Z_loop4;
private: System::Windows::Forms::TextBox^  textBox_Z_loop5;
private: System::Windows::Forms::TextBox^  textBox_Z_loop6;
private: System::Windows::Forms::TextBox^  textBox_Z_loop7;
private: System::Windows::Forms::TextBox^  textBox_Z_loop8;

private: System::Windows::Forms::Button^  button_read_loop_Z;

private: System::Windows::Forms::Button^  button_erase;
private: System::Windows::Forms::TextBox^  textBox_R_activ;
private: System::Windows::Forms::Button^  button_write_Z;
private: System::Windows::Forms::RichTextBox^  richTextBox1;
private: System::Windows::Forms::ToolStripMenuItem^  uploadToFTPToolStripMenuItem;
private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
private: System::Windows::Forms::Button^  button_calc_z_to_loop;
private: System::Windows::Forms::Button^  button_signature;
private: System::Windows::Forms::TextBox^  textBox_signature;
private: System::Windows::Forms::TextBox^  textBox_Z_loop10;
private: System::Windows::Forms::TextBox^  textBox_Z_loop9;
private: System::Windows::Forms::TextBox^  textBox_Z_loop11;


	public:

		
	private: System::Windows::Forms::Timer^  timer1;
	public:
		


		MyForm(void)
		{

			Loop_Port_Thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Read_Port));
			
			InitializeComponent();
			this->Width = 1310;
			this->Height = 920;

			for (int i = 24; i <= 95; i++) {
					TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel3->Controls["textBox" + (i).ToString()];
					TEXTBOX->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
					TEXTBOX->ReadOnly = true;
			}

			for (int i = 96; i <= 103; i++) {
				TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel7->Controls["textBox" + (i).ToString()];
				TEXTBOX->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
				TEXTBOX->ReadOnly = true;
			}


			//TODO: добавьте код конструктора
			this->serialPort = gcnew SerialPort();
			extern string Com_Name;
			extern string Com1_Name;
			extern string Com_LWD_Name;;
			extern METROLOGY metrology_struct;

			fout = new ofstream ;
			fout->open(Dataout, ios::trunc);
			fout->close();
			//remove(Dataout);

			Ro_chart = new V8[20];
			Ro_aver = new V8;
			Ro_for_print = new V8[20];
			Phase_for_print = new V8[20];
			Ro_percent_for_print = new V8[20];
			Phase_percent_for_print = new V8[20];
		
			/*
			Ro_percent_temperature = new V8*[200]; // размещаем массив из 10 указателей -строки
			for (int i = 0; i < 200; ++i)
				Ro_percent_temperature[i] = new V8[10]; //столбцы
			for (int t = 0; t < 200; ++t) {
				for (int i = 0; i < 10; ++i) {
					for (int j = 0; j < 8; ++j) {
						Ro_percent_temperature[t][i].VALUE[j] = 0;
					}
				}
			}
			*/
			Phase_temperature = new V8*[200]; // размещаем массив из 10 указателей -строки
			for (int i = 0; i < 200; ++i)
				Phase_temperature[i] = new V8[10]; //столбцы
			
			
			green = new bool[9];
			extern V8 Air_Phase;
			Phase_chart = new V8[20];
			Phase_aver = new V8;
			Phase_raw = new V8[20];
			Phase_raw_aver = new V8;
			Data_Cal = new V8[800];
			Ro_adecvat_computed = new V8[10];//
			Phase_loop_computed = new V8[10];//
            extern int norm[10];
			extern int medium[10];
			extern int out_of_norm[10];
			extern  double R_loop_physical[9];
			extern double loop_radius;
			extern bool express;
			extern bool Ro_to_raport;
			extern bool PH_to_raport;
			extern int N_aver;
			extern bool AM_TX;
			extern bool sonde_ro_calk;
			extern bool comp_smt;
			extern bool mode;
			//extern string Metrology_Name;
			extern unsigned int Tick;
			const char* ConfName = "Config.txt";
			const char* Report_Name = "Report.rtf";
			comboBox1->SelectedIndex = 3;
			comboBox_sonde_type->SelectedIndex = 0;// EMK_90
			this->textBox21->Text = L"";
			
			C = new double[8];	
			Re_from_loop = new double[11];
			C_meas = new double[8];			
			L1 = new double[8];	
			L2 = new double[8];
			f_ = new double[8];
			F_rez = new double[8];
			R_Add_Wire = new double[8];
			for (int i = 0; i < 4; i++)
				R_Add_Wire[i] = 0.4;
			for (int i = 4; i < 8; i++)
				R_Add_Wire[i] = 0.6;

			ifstream conf_in;
			conf_in.open(ConfName, ios::in);
			for (int i = 0; i < 10; i++) {
				conf_in >> norm[i]; conf_in >> medium[i]; conf_in >> out_of_norm[i];
			}

			for (int i = 0; i < 8; i++) {
				conf_in >> R_loop_physical[i];
				//cout << R_loop_physical[i] << " ";
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

			
			for (int i = 0; i < 8; i++) {
				TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox_AM_Tx_" + (i).ToString()];
				if (AM_TX) TEXTBOX->Visible = true;
				if (!AM_TX) TEXTBOX->Visible = false;
			}
		
			R_loop_physical[8] = 7000; loop_radius = 0.5;

			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 8; j++) {
					Ro_chart[i].VALUE[j] = 0;
					Phase_chart[i].VALUE[j] = 0;
					Phase_raw[i].VALUE[j] = 0;
				}
			}

			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 8; j++) {
					Ro_aver->VALUE[j] = 0;
					Ro_for_print[i].VALUE[j] = 0;
				}
			}

			for (int i = 0; i < 9; i++) {
				green[i] = false;
			}

			this->timer1->Interval = Tick;
			//AIRTEMP10::MyForm1^ Metro = gcnew AIRTEMP10::MyForm1;
			//Metro->Show();	
			
			auto font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			//loops->Parent = tabControl1; //Показать
			//tabControl1->SelectedIndex = 2;
}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;


	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;



	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  файлToolStripMenuItem;



	private: System::ComponentModel::IContainer^  components;
	protected:

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
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series8 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series9 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series10 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series11 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series12 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series13 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series14 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series15 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series16 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series17 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series18 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series19 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series20 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series21 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series22 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series23 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series24 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea4 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series25 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series26 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series27 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series28 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series29 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series30 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series31 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series32 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea5 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series33 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series34 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series35 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series36 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series37 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series38 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series39 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series40 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea6 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series41 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series42 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series43 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series44 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series45 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series46 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series47 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series48 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea7 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series49 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series50 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series51 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series52 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series53 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series54 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series55 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series56 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea8 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend8 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series57 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series58 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series59 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series60 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series61 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series62 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series63 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series64 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->textBox16 = (gcnew System::Windows::Forms::TextBox());
			this->textBox15 = (gcnew System::Windows::Forms::TextBox());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart8 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_0 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_AM_Tx_7 = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->файлToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->открытьToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Файл_метрологии_открыть = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->параметрыToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->сохранитьРапортRtfToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->работаСФайломМетрологииToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->помощьToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->uploadToFTPToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->buttonR9 = (gcnew System::Windows::Forms::Button());
			this->buttonR1 = (gcnew System::Windows::Forms::Button());
			this->buttonR2 = (gcnew System::Windows::Forms::Button());
			this->buttonR3 = (gcnew System::Windows::Forms::Button());
			this->buttonR4 = (gcnew System::Windows::Forms::Button());
			this->button_Report = (gcnew System::Windows::Forms::Button());
			this->pictureBoxAir = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR4 = (gcnew System::Windows::Forms::PictureBox());
			this->docToPrint = (gcnew System::Drawing::Printing::PrintDocument());
			this->button_BT_ON = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->textBox17 = (gcnew System::Windows::Forms::TextBox());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			this->textBox19 = (gcnew System::Windows::Forms::TextBox());
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->textBox21 = (gcnew System::Windows::Forms::TextBox());
			this->textBox20 = (gcnew System::Windows::Forms::TextBox());
			this->textBox22 = (gcnew System::Windows::Forms::TextBox());
			this->textBox23 = (gcnew System::Windows::Forms::TextBox());
			this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->textBox127 = (gcnew System::Windows::Forms::TextBox());
			this->textBox126 = (gcnew System::Windows::Forms::TextBox());
			this->textBox125 = (gcnew System::Windows::Forms::TextBox());
			this->textBox124 = (gcnew System::Windows::Forms::TextBox());
			this->textBox31 = (gcnew System::Windows::Forms::TextBox());
			this->textBox39 = (gcnew System::Windows::Forms::TextBox());
			this->textBox55 = (gcnew System::Windows::Forms::TextBox());
			this->textBox47 = (gcnew System::Windows::Forms::TextBox());
			this->textBox54 = (gcnew System::Windows::Forms::TextBox());
			this->textBox46 = (gcnew System::Windows::Forms::TextBox());
			this->textBox38 = (gcnew System::Windows::Forms::TextBox());
			this->textBox30 = (gcnew System::Windows::Forms::TextBox());
			this->textBox29 = (gcnew System::Windows::Forms::TextBox());
			this->textBox37 = (gcnew System::Windows::Forms::TextBox());
			this->textBox45 = (gcnew System::Windows::Forms::TextBox());
			this->textBox53 = (gcnew System::Windows::Forms::TextBox());
			this->textBox28 = (gcnew System::Windows::Forms::TextBox());
			this->textBox36 = (gcnew System::Windows::Forms::TextBox());
			this->textBox44 = (gcnew System::Windows::Forms::TextBox());
			this->textBox52 = (gcnew System::Windows::Forms::TextBox());
			this->textBox27 = (gcnew System::Windows::Forms::TextBox());
			this->textBox35 = (gcnew System::Windows::Forms::TextBox());
			this->textBox43 = (gcnew System::Windows::Forms::TextBox());
			this->textBox51 = (gcnew System::Windows::Forms::TextBox());
			this->textBox26 = (gcnew System::Windows::Forms::TextBox());
			this->textBox34 = (gcnew System::Windows::Forms::TextBox());
			this->textBox42 = (gcnew System::Windows::Forms::TextBox());
			this->textBox50 = (gcnew System::Windows::Forms::TextBox());
			this->textBox25 = (gcnew System::Windows::Forms::TextBox());
			this->textBox33 = (gcnew System::Windows::Forms::TextBox());
			this->textBox41 = (gcnew System::Windows::Forms::TextBox());
			this->textBox49 = (gcnew System::Windows::Forms::TextBox());
			this->textBox24 = (gcnew System::Windows::Forms::TextBox());
			this->textBox32 = (gcnew System::Windows::Forms::TextBox());
			this->textBox40 = (gcnew System::Windows::Forms::TextBox());
			this->textBox48 = (gcnew System::Windows::Forms::TextBox());
			this->textBox120 = (gcnew System::Windows::Forms::TextBox());
			this->textBox121 = (gcnew System::Windows::Forms::TextBox());
			this->textBox122 = (gcnew System::Windows::Forms::TextBox());
			this->textBox128 = (gcnew System::Windows::Forms::TextBox());
			this->textBox88 = (gcnew System::Windows::Forms::TextBox());
			this->textBox89 = (gcnew System::Windows::Forms::TextBox());
			this->textBox90 = (gcnew System::Windows::Forms::TextBox());
			this->textBox91 = (gcnew System::Windows::Forms::TextBox());
			this->textBox92 = (gcnew System::Windows::Forms::TextBox());
			this->textBox93 = (gcnew System::Windows::Forms::TextBox());
			this->textBox94 = (gcnew System::Windows::Forms::TextBox());
			this->textBox95 = (gcnew System::Windows::Forms::TextBox());
			this->textBox56 = (gcnew System::Windows::Forms::TextBox());
			this->textBox57 = (gcnew System::Windows::Forms::TextBox());
			this->textBox58 = (gcnew System::Windows::Forms::TextBox());
			this->textBox59 = (gcnew System::Windows::Forms::TextBox());
			this->textBox60 = (gcnew System::Windows::Forms::TextBox());
			this->textBox61 = (gcnew System::Windows::Forms::TextBox());
			this->textBox62 = (gcnew System::Windows::Forms::TextBox());
			this->textBox63 = (gcnew System::Windows::Forms::TextBox());
			this->textBox64 = (gcnew System::Windows::Forms::TextBox());
			this->textBox65 = (gcnew System::Windows::Forms::TextBox());
			this->textBox66 = (gcnew System::Windows::Forms::TextBox());
			this->textBox67 = (gcnew System::Windows::Forms::TextBox());
			this->textBox68 = (gcnew System::Windows::Forms::TextBox());
			this->textBox69 = (gcnew System::Windows::Forms::TextBox());
			this->textBox70 = (gcnew System::Windows::Forms::TextBox());
			this->textBox71 = (gcnew System::Windows::Forms::TextBox());
			this->textBox72 = (gcnew System::Windows::Forms::TextBox());
			this->textBox73 = (gcnew System::Windows::Forms::TextBox());
			this->textBox74 = (gcnew System::Windows::Forms::TextBox());
			this->textBox75 = (gcnew System::Windows::Forms::TextBox());
			this->textBox76 = (gcnew System::Windows::Forms::TextBox());
			this->textBox77 = (gcnew System::Windows::Forms::TextBox());
			this->textBox78 = (gcnew System::Windows::Forms::TextBox());
			this->textBox79 = (gcnew System::Windows::Forms::TextBox());
			this->textBox80 = (gcnew System::Windows::Forms::TextBox());
			this->textBox81 = (gcnew System::Windows::Forms::TextBox());
			this->textBox82 = (gcnew System::Windows::Forms::TextBox());
			this->textBox83 = (gcnew System::Windows::Forms::TextBox());
			this->textBox84 = (gcnew System::Windows::Forms::TextBox());
			this->textBox85 = (gcnew System::Windows::Forms::TextBox());
			this->textBox123 = (gcnew System::Windows::Forms::TextBox());
			this->textBox86 = (gcnew System::Windows::Forms::TextBox());
			this->textBox87 = (gcnew System::Windows::Forms::TextBox());
			this->textBox129 = (gcnew System::Windows::Forms::TextBox());
			this->textBox96 = (gcnew System::Windows::Forms::TextBox());
			this->textBox97 = (gcnew System::Windows::Forms::TextBox());
			this->textBox98 = (gcnew System::Windows::Forms::TextBox());
			this->textBox99 = (gcnew System::Windows::Forms::TextBox());
			this->textBox100 = (gcnew System::Windows::Forms::TextBox());
			this->textBox101 = (gcnew System::Windows::Forms::TextBox());
			this->textBox102 = (gcnew System::Windows::Forms::TextBox());
			this->textBox103 = (gcnew System::Windows::Forms::TextBox());
			this->tableLayoutPanel4 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->main = (gcnew System::Windows::Forms::TabPage());
			this->impedance = (gcnew System::Windows::Forms::TabPage());
			this->button_signature = (gcnew System::Windows::Forms::Button());
			this->textBox_signature = (gcnew System::Windows::Forms::TextBox());
			this->button_calc_z_to_loop = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button_write_Z = (gcnew System::Windows::Forms::Button());
			this->textBox_R_activ = (gcnew System::Windows::Forms::TextBox());
			this->button_erase = (gcnew System::Windows::Forms::Button());
			this->button_read_loop_Z = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel11 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->textBox_L1_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L1_8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_L2_8 = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->textBox_f_1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_f_8 = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->textBox_С1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_С8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_calc8 = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->button_Z1 = (gcnew System::Windows::Forms::Button());
			this->button_Z2 = (gcnew System::Windows::Forms::Button());
			this->button_Z3 = (gcnew System::Windows::Forms::Button());
			this->button_Z4 = (gcnew System::Windows::Forms::Button());
			this->button_Z5 = (gcnew System::Windows::Forms::Button());
			this->button_Z6 = (gcnew System::Windows::Forms::Button());
			this->button_Z7 = (gcnew System::Windows::Forms::Button());
			this->button_Z8 = (gcnew System::Windows::Forms::Button());
			this->textBox_Z_loop1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Z_loop11 = (gcnew System::Windows::Forms::TextBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->buttonR5 = (gcnew System::Windows::Forms::Button());
			this->buttonR6 = (gcnew System::Windows::Forms::Button());
			this->buttonR7 = (gcnew System::Windows::Forms::Button());
			this->buttonR8 = (gcnew System::Windows::Forms::Button());
			this->pictureBoxR5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR6 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxR8 = (gcnew System::Windows::Forms::PictureBox());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->buttonLocalMetro = (gcnew System::Windows::Forms::Button());
			this->button_Auto = (gcnew System::Windows::Forms::Button());
			this->button_Clear = (gcnew System::Windows::Forms::Button());
			this->button_ON_OFF = (gcnew System::Windows::Forms::Button());
			this->helpProvider1 = (gcnew System::Windows::Forms::HelpProvider());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tableLayoutPanel7 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->checkBox_С = (gcnew System::Windows::Forms::CheckBox());
			this->timer_Temperature = (gcnew System::Windows::Forms::Timer(this->components));
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->comboBox_sonde_type = (gcnew System::Windows::Forms::ComboBox());
			this->serialPort_LWD_1DDS = (gcnew System::IO::Ports::SerialPort(this->components));
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart8))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxAir))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR4))->BeginInit();
			this->tableLayoutPanel2->SuspendLayout();
			this->tableLayoutPanel3->SuspendLayout();
			this->tableLayoutPanel4->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->main->SuspendLayout();
			this->impedance->SuspendLayout();
			this->tableLayoutPanel11->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR8))->BeginInit();
			this->tableLayoutPanel7->SuspendLayout();
			this->SuspendLayout();
			// 
			// chart1
			// 
			this->chart1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea1->AxisX->LabelStyle->Enabled = false;
			chartArea1->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Enabled = false;
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(4, 46);
			this->chart1->Margin = System::Windows::Forms::Padding(1);
			this->chart1->Name = L"chart1";
			series1->BorderWidth = 2;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Color = System::Drawing::Color::Black;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			series2->BorderWidth = 2;
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series2->Color = System::Drawing::Color::ForestGreen;
			series2->Legend = L"Legend1";
			series2->Name = L"Series2";
			series3->BorderWidth = 2;
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series3->Color = System::Drawing::Color::ForestGreen;
			series3->Legend = L"Legend1";
			series3->Name = L"Series3";
			series4->BorderWidth = 2;
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series4->Color = System::Drawing::Color::Gold;
			series4->Legend = L"Legend1";
			series4->Name = L"Series4";
			series5->BorderWidth = 2;
			series5->ChartArea = L"ChartArea1";
			series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series5->Color = System::Drawing::Color::Gold;
			series5->Legend = L"Legend1";
			series5->Name = L"Series5";
			series6->BorderWidth = 2;
			series6->ChartArea = L"ChartArea1";
			series6->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series6->Color = System::Drawing::Color::Red;
			series6->Legend = L"Legend1";
			series6->Name = L"Series6";
			series7->BorderWidth = 2;
			series7->ChartArea = L"ChartArea1";
			series7->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series7->Color = System::Drawing::Color::Red;
			series7->Legend = L"Legend1";
			series7->Name = L"Series7";
			series8->BorderWidth = 2;
			series8->ChartArea = L"ChartArea1";
			series8->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series8->Color = System::Drawing::Color::Blue;
			series8->Legend = L"Legend1";
			series8->Name = L"Series8";
			this->chart1->Series->Add(series1);
			this->chart1->Series->Add(series2);
			this->chart1->Series->Add(series3);
			this->chart1->Series->Add(series4);
			this->chart1->Series->Add(series5);
			this->chart1->Series->Add(series6);
			this->chart1->Series->Add(series7);
			this->chart1->Series->Add(series8);
			this->chart1->Size = System::Drawing::Size(206, 527);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chart1";
			this->toolTip1->SetToolTip(this->chart1, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->BackColor = System::Drawing::SystemColors::Control;
			this->tableLayoutPanel1->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::OutsetDouble;
			this->tableLayoutPanel1->ColumnCount = 8;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12.5F)));
			this->tableLayoutPanel1->Controls->Add(this->textBox16, 7, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox15, 6, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox14, 5, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox13, 4, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox12, 3, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox11, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox10, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->textBox9, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->chart1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart3, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart4, 3, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart5, 4, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart6, 5, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart7, 6, 1);
			this->tableLayoutPanel1->Controls->Add(this->chart8, 7, 1);
			this->tableLayoutPanel1->Controls->Add(this->textBox1, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox2, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox3, 2, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox4, 3, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox5, 4, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox6, 5, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox7, 6, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox8, 7, 2);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_0, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_1, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_2, 2, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_3, 3, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_4, 4, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_5, 5, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_6, 6, 3);
			this->tableLayoutPanel1->Controls->Add(this->textBox_AM_Tx_7, 7, 3);
			this->tableLayoutPanel1->Location = System::Drawing::Point(3, 2);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->tableLayoutPanel1->RowCount = 5;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 39)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 32)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 4)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(1691, 652);
			this->tableLayoutPanel1->TabIndex = 2;
			// 
			// textBox16
			// 
			this->textBox16->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox16->BackColor = System::Drawing::SystemColors::Window;
			this->textBox16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox16->Location = System::Drawing::Point(1483, 5);
			this->textBox16->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox16->Name = L"textBox16";
			this->textBox16->ReadOnly = true;
			this->textBox16->Size = System::Drawing::Size(202, 27);
			this->textBox16->TabIndex = 30;
			this->textBox16->Text = L"T4_2000";
			this->textBox16->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox15
			// 
			this->textBox15->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox15->BackColor = System::Drawing::SystemColors::Window;
			this->textBox15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox15->Location = System::Drawing::Point(1272, 5);
			this->textBox15->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox15->Name = L"textBox15";
			this->textBox15->ReadOnly = true;
			this->textBox15->Size = System::Drawing::Size(202, 27);
			this->textBox15->TabIndex = 29;
			this->textBox15->Text = L"T3_2000";
			this->textBox15->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox14
			// 
			this->textBox14->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox14->BackColor = System::Drawing::SystemColors::Window;
			this->textBox14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox14->Location = System::Drawing::Point(1061, 5);
			this->textBox14->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox14->Name = L"textBox14";
			this->textBox14->ReadOnly = true;
			this->textBox14->Size = System::Drawing::Size(202, 27);
			this->textBox14->TabIndex = 28;
			this->textBox14->Text = L"T2_2000";
			this->textBox14->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox13
			// 
			this->textBox13->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox13->BackColor = System::Drawing::SystemColors::Window;
			this->textBox13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox13->Location = System::Drawing::Point(850, 5);
			this->textBox13->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox13->Name = L"textBox13";
			this->textBox13->ReadOnly = true;
			this->textBox13->Size = System::Drawing::Size(202, 27);
			this->textBox13->TabIndex = 27;
			this->textBox13->Text = L"T1_2000";
			this->textBox13->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox12
			// 
			this->textBox12->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox12->BackColor = System::Drawing::SystemColors::Window;
			this->textBox12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox12->Location = System::Drawing::Point(639, 5);
			this->textBox12->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox12->Name = L"textBox12";
			this->textBox12->ReadOnly = true;
			this->textBox12->Size = System::Drawing::Size(202, 27);
			this->textBox12->TabIndex = 26;
			this->textBox12->Text = L"T4_875";
			this->textBox12->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox11
			// 
			this->textBox11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox11->BackColor = System::Drawing::SystemColors::Window;
			this->textBox11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox11->Location = System::Drawing::Point(428, 5);
			this->textBox11->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox11->Name = L"textBox11";
			this->textBox11->ReadOnly = true;
			this->textBox11->Size = System::Drawing::Size(202, 27);
			this->textBox11->TabIndex = 25;
			this->textBox11->Text = L"T3_875";
			this->textBox11->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox10
			// 
			this->textBox10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox10->BackColor = System::Drawing::SystemColors::Window;
			this->textBox10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox10->Location = System::Drawing::Point(217, 5);
			this->textBox10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox10->Name = L"textBox10";
			this->textBox10->ReadOnly = true;
			this->textBox10->Size = System::Drawing::Size(202, 27);
			this->textBox10->TabIndex = 24;
			this->textBox10->Text = L"T2_875";
			this->textBox10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox9
			// 
			this->textBox9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox9->BackColor = System::Drawing::SystemColors::Window;
			this->textBox9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox9->Location = System::Drawing::Point(6, 5);
			this->textBox9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox9->Name = L"textBox9";
			this->textBox9->ReadOnly = true;
			this->textBox9->Size = System::Drawing::Size(202, 27);
			this->textBox9->TabIndex = 23;
			this->textBox9->Text = L"T1_875";
			this->textBox9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// chart2
			// 
			this->chart2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea2->AxisX->LabelStyle->Enabled = false;
			chartArea2->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea2->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea2);
			legend2->Enabled = false;
			legend2->Name = L"Legend1";
			this->chart2->Legends->Add(legend2);
			this->chart2->Location = System::Drawing::Point(215, 46);
			this->chart2->Margin = System::Windows::Forms::Padding(1);
			this->chart2->Name = L"chart2";
			series9->BorderWidth = 2;
			series9->ChartArea = L"ChartArea1";
			series9->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series9->Color = System::Drawing::Color::Black;
			series9->Legend = L"Legend1";
			series9->Name = L"Series1";
			series10->BorderWidth = 2;
			series10->ChartArea = L"ChartArea1";
			series10->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series10->Color = System::Drawing::Color::ForestGreen;
			series10->Legend = L"Legend1";
			series10->Name = L"Series2";
			series11->BorderWidth = 2;
			series11->ChartArea = L"ChartArea1";
			series11->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series11->Color = System::Drawing::Color::ForestGreen;
			series11->Legend = L"Legend1";
			series11->Name = L"Series3";
			series12->BorderWidth = 2;
			series12->ChartArea = L"ChartArea1";
			series12->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series12->Color = System::Drawing::Color::Gold;
			series12->Legend = L"Legend1";
			series12->Name = L"Series4";
			series13->BorderWidth = 2;
			series13->ChartArea = L"ChartArea1";
			series13->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series13->Color = System::Drawing::Color::Gold;
			series13->Legend = L"Legend1";
			series13->Name = L"Series5";
			series14->BorderWidth = 2;
			series14->ChartArea = L"ChartArea1";
			series14->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series14->Color = System::Drawing::Color::Red;
			series14->Legend = L"Legend1";
			series14->Name = L"Series6";
			series15->BorderWidth = 2;
			series15->ChartArea = L"ChartArea1";
			series15->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series15->Color = System::Drawing::Color::Red;
			series15->Legend = L"Legend1";
			series15->Name = L"Series7";
			series16->BorderWidth = 2;
			series16->ChartArea = L"ChartArea1";
			series16->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series16->Color = System::Drawing::Color::Blue;
			series16->Legend = L"Legend1";
			series16->Name = L"Series8";
			this->chart2->Series->Add(series9);
			this->chart2->Series->Add(series10);
			this->chart2->Series->Add(series11);
			this->chart2->Series->Add(series12);
			this->chart2->Series->Add(series13);
			this->chart2->Series->Add(series14);
			this->chart2->Series->Add(series15);
			this->chart2->Series->Add(series16);
			this->chart2->Size = System::Drawing::Size(206, 527);
			this->chart2->TabIndex = 8;
			this->chart2->Text = L"chart2";
			this->toolTip1->SetToolTip(this->chart2, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// chart3
			// 
			this->chart3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea3->AxisX->LabelStyle->Enabled = false;
			chartArea3->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea3->Name = L"ChartArea1";
			this->chart3->ChartAreas->Add(chartArea3);
			legend3->Enabled = false;
			legend3->Name = L"Legend1";
			this->chart3->Legends->Add(legend3);
			this->chart3->Location = System::Drawing::Point(426, 46);
			this->chart3->Margin = System::Windows::Forms::Padding(1);
			this->chart3->Name = L"chart3";
			series17->BorderWidth = 2;
			series17->ChartArea = L"ChartArea1";
			series17->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series17->Color = System::Drawing::Color::Black;
			series17->Legend = L"Legend1";
			series17->Name = L"Series1";
			series18->BorderWidth = 2;
			series18->ChartArea = L"ChartArea1";
			series18->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series18->Color = System::Drawing::Color::ForestGreen;
			series18->Legend = L"Legend1";
			series18->Name = L"Series2";
			series19->BorderWidth = 2;
			series19->ChartArea = L"ChartArea1";
			series19->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series19->Color = System::Drawing::Color::ForestGreen;
			series19->Legend = L"Legend1";
			series19->Name = L"Series3";
			series20->BorderWidth = 2;
			series20->ChartArea = L"ChartArea1";
			series20->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series20->Color = System::Drawing::Color::Gold;
			series20->Legend = L"Legend1";
			series20->Name = L"Series4";
			series21->BorderWidth = 2;
			series21->ChartArea = L"ChartArea1";
			series21->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series21->Color = System::Drawing::Color::Gold;
			series21->Legend = L"Legend1";
			series21->Name = L"Series5";
			series22->BorderWidth = 2;
			series22->ChartArea = L"ChartArea1";
			series22->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series22->Color = System::Drawing::Color::Red;
			series22->Legend = L"Legend1";
			series22->Name = L"Series6";
			series23->BorderWidth = 2;
			series23->ChartArea = L"ChartArea1";
			series23->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series23->Color = System::Drawing::Color::Red;
			series23->Legend = L"Legend1";
			series23->Name = L"Series7";
			series24->BorderWidth = 2;
			series24->ChartArea = L"ChartArea1";
			series24->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series24->Color = System::Drawing::Color::Blue;
			series24->Legend = L"Legend1";
			series24->Name = L"Series8";
			this->chart3->Series->Add(series17);
			this->chart3->Series->Add(series18);
			this->chart3->Series->Add(series19);
			this->chart3->Series->Add(series20);
			this->chart3->Series->Add(series21);
			this->chart3->Series->Add(series22);
			this->chart3->Series->Add(series23);
			this->chart3->Series->Add(series24);
			this->chart3->Size = System::Drawing::Size(206, 527);
			this->chart3->TabIndex = 9;
			this->chart3->Text = L"chart3";
			this->toolTip1->SetToolTip(this->chart3, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// chart4
			// 
			this->chart4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea4->AxisX->LabelStyle->Enabled = false;
			chartArea4->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea4->Name = L"ChartArea1";
			this->chart4->ChartAreas->Add(chartArea4);
			legend4->Enabled = false;
			legend4->Name = L"Legend1";
			this->chart4->Legends->Add(legend4);
			this->chart4->Location = System::Drawing::Point(637, 46);
			this->chart4->Margin = System::Windows::Forms::Padding(1);
			this->chart4->Name = L"chart4";
			series25->BorderWidth = 2;
			series25->ChartArea = L"ChartArea1";
			series25->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series25->Color = System::Drawing::Color::Black;
			series25->Legend = L"Legend1";
			series25->Name = L"Series1";
			series26->BorderWidth = 2;
			series26->ChartArea = L"ChartArea1";
			series26->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series26->Color = System::Drawing::Color::ForestGreen;
			series26->Legend = L"Legend1";
			series26->Name = L"Series2";
			series27->BorderWidth = 2;
			series27->ChartArea = L"ChartArea1";
			series27->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series27->Color = System::Drawing::Color::ForestGreen;
			series27->Legend = L"Legend1";
			series27->Name = L"Series3";
			series28->BorderWidth = 2;
			series28->ChartArea = L"ChartArea1";
			series28->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series28->Color = System::Drawing::Color::Gold;
			series28->Legend = L"Legend1";
			series28->Name = L"Series4";
			series29->BorderWidth = 2;
			series29->ChartArea = L"ChartArea1";
			series29->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series29->Color = System::Drawing::Color::Gold;
			series29->Legend = L"Legend1";
			series29->Name = L"Series5";
			series30->BorderWidth = 2;
			series30->ChartArea = L"ChartArea1";
			series30->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series30->Color = System::Drawing::Color::Red;
			series30->Legend = L"Legend1";
			series30->Name = L"Series6";
			series31->BorderWidth = 2;
			series31->ChartArea = L"ChartArea1";
			series31->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series31->Color = System::Drawing::Color::Red;
			series31->Legend = L"Legend1";
			series31->Name = L"Series7";
			series32->BorderWidth = 2;
			series32->ChartArea = L"ChartArea1";
			series32->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series32->Color = System::Drawing::Color::Blue;
			series32->Legend = L"Legend1";
			series32->Name = L"Series8";
			this->chart4->Series->Add(series25);
			this->chart4->Series->Add(series26);
			this->chart4->Series->Add(series27);
			this->chart4->Series->Add(series28);
			this->chart4->Series->Add(series29);
			this->chart4->Series->Add(series30);
			this->chart4->Series->Add(series31);
			this->chart4->Series->Add(series32);
			this->chart4->Size = System::Drawing::Size(206, 527);
			this->chart4->TabIndex = 10;
			this->chart4->Text = L"chart4";
			this->toolTip1->SetToolTip(this->chart4, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// chart5
			// 
			this->chart5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea5->AxisX->LabelStyle->Enabled = false;
			chartArea5->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea5->Name = L"ChartArea1";
			this->chart5->ChartAreas->Add(chartArea5);
			legend5->Enabled = false;
			legend5->Name = L"Legend1";
			this->chart5->Legends->Add(legend5);
			this->chart5->Location = System::Drawing::Point(848, 46);
			this->chart5->Margin = System::Windows::Forms::Padding(1);
			this->chart5->Name = L"chart5";
			series33->BorderWidth = 2;
			series33->ChartArea = L"ChartArea1";
			series33->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series33->Color = System::Drawing::Color::Black;
			series33->Legend = L"Legend1";
			series33->Name = L"Series1";
			series34->BorderWidth = 2;
			series34->ChartArea = L"ChartArea1";
			series34->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series34->Color = System::Drawing::Color::ForestGreen;
			series34->Legend = L"Legend1";
			series34->Name = L"Series2";
			series35->BorderWidth = 2;
			series35->ChartArea = L"ChartArea1";
			series35->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series35->Color = System::Drawing::Color::ForestGreen;
			series35->Legend = L"Legend1";
			series35->Name = L"Series3";
			series36->BorderWidth = 2;
			series36->ChartArea = L"ChartArea1";
			series36->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series36->Color = System::Drawing::Color::Gold;
			series36->Legend = L"Legend1";
			series36->Name = L"Series4";
			series37->BorderWidth = 2;
			series37->ChartArea = L"ChartArea1";
			series37->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series37->Color = System::Drawing::Color::Gold;
			series37->Legend = L"Legend1";
			series37->Name = L"Series5";
			series38->BorderWidth = 2;
			series38->ChartArea = L"ChartArea1";
			series38->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series38->Color = System::Drawing::Color::Red;
			series38->Legend = L"Legend1";
			series38->Name = L"Series6";
			series39->BorderWidth = 2;
			series39->ChartArea = L"ChartArea1";
			series39->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series39->Color = System::Drawing::Color::Red;
			series39->Legend = L"Legend1";
			series39->Name = L"Series7";
			series40->BorderWidth = 2;
			series40->ChartArea = L"ChartArea1";
			series40->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series40->Color = System::Drawing::Color::Blue;
			series40->Legend = L"Legend1";
			series40->Name = L"Series8";
			this->chart5->Series->Add(series33);
			this->chart5->Series->Add(series34);
			this->chart5->Series->Add(series35);
			this->chart5->Series->Add(series36);
			this->chart5->Series->Add(series37);
			this->chart5->Series->Add(series38);
			this->chart5->Series->Add(series39);
			this->chart5->Series->Add(series40);
			this->chart5->Size = System::Drawing::Size(206, 527);
			this->chart5->TabIndex = 11;
			this->chart5->Text = L"chart5";
			this->toolTip1->SetToolTip(this->chart5, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// chart6
			// 
			this->chart6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea6->AxisX->LabelStyle->Enabled = false;
			chartArea6->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea6->Name = L"ChartArea1";
			this->chart6->ChartAreas->Add(chartArea6);
			legend6->Enabled = false;
			legend6->Name = L"Legend1";
			this->chart6->Legends->Add(legend6);
			this->chart6->Location = System::Drawing::Point(1059, 46);
			this->chart6->Margin = System::Windows::Forms::Padding(1);
			this->chart6->Name = L"chart6";
			series41->BorderWidth = 2;
			series41->ChartArea = L"ChartArea1";
			series41->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series41->Color = System::Drawing::Color::Black;
			series41->Legend = L"Legend1";
			series41->Name = L"Series1";
			series42->BorderWidth = 2;
			series42->ChartArea = L"ChartArea1";
			series42->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series42->Color = System::Drawing::Color::ForestGreen;
			series42->Legend = L"Legend1";
			series42->Name = L"Series2";
			series43->BorderWidth = 2;
			series43->ChartArea = L"ChartArea1";
			series43->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series43->Color = System::Drawing::Color::ForestGreen;
			series43->Legend = L"Legend1";
			series43->Name = L"Series3";
			series44->BorderWidth = 2;
			series44->ChartArea = L"ChartArea1";
			series44->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series44->Color = System::Drawing::Color::Gold;
			series44->Legend = L"Legend1";
			series44->Name = L"Series4";
			series45->BorderWidth = 2;
			series45->ChartArea = L"ChartArea1";
			series45->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series45->Color = System::Drawing::Color::Gold;
			series45->Legend = L"Legend1";
			series45->Name = L"Series5";
			series46->BorderWidth = 2;
			series46->ChartArea = L"ChartArea1";
			series46->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series46->Color = System::Drawing::Color::Red;
			series46->Legend = L"Legend1";
			series46->Name = L"Series6";
			series47->BorderWidth = 2;
			series47->ChartArea = L"ChartArea1";
			series47->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series47->Color = System::Drawing::Color::Red;
			series47->Legend = L"Legend1";
			series47->Name = L"Series7";
			series48->BorderWidth = 2;
			series48->ChartArea = L"ChartArea1";
			series48->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series48->Color = System::Drawing::Color::Blue;
			series48->Legend = L"Legend1";
			series48->Name = L"Series8";
			this->chart6->Series->Add(series41);
			this->chart6->Series->Add(series42);
			this->chart6->Series->Add(series43);
			this->chart6->Series->Add(series44);
			this->chart6->Series->Add(series45);
			this->chart6->Series->Add(series46);
			this->chart6->Series->Add(series47);
			this->chart6->Series->Add(series48);
			this->chart6->Size = System::Drawing::Size(206, 527);
			this->chart6->TabIndex = 12;
			this->chart6->Text = L"chart6";
			this->toolTip1->SetToolTip(this->chart6, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// chart7
			// 
			this->chart7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea7->AxisX->LabelStyle->Enabled = false;
			chartArea7->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea7->Name = L"ChartArea1";
			this->chart7->ChartAreas->Add(chartArea7);
			legend7->Enabled = false;
			legend7->Name = L"Legend1";
			this->chart7->Legends->Add(legend7);
			this->chart7->Location = System::Drawing::Point(1270, 46);
			this->chart7->Margin = System::Windows::Forms::Padding(1);
			this->chart7->Name = L"chart7";
			series49->BorderWidth = 2;
			series49->ChartArea = L"ChartArea1";
			series49->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series49->Color = System::Drawing::Color::Black;
			series49->Legend = L"Legend1";
			series49->Name = L"Series1";
			series50->BorderWidth = 2;
			series50->ChartArea = L"ChartArea1";
			series50->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series50->Color = System::Drawing::Color::ForestGreen;
			series50->Legend = L"Legend1";
			series50->Name = L"Series2";
			series51->BorderWidth = 2;
			series51->ChartArea = L"ChartArea1";
			series51->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series51->Color = System::Drawing::Color::ForestGreen;
			series51->Legend = L"Legend1";
			series51->Name = L"Series3";
			series52->BorderWidth = 2;
			series52->ChartArea = L"ChartArea1";
			series52->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series52->Color = System::Drawing::Color::Gold;
			series52->Legend = L"Legend1";
			series52->Name = L"Series4";
			series53->BorderWidth = 2;
			series53->ChartArea = L"ChartArea1";
			series53->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series53->Color = System::Drawing::Color::Gold;
			series53->Legend = L"Legend1";
			series53->Name = L"Series5";
			series54->BorderWidth = 2;
			series54->ChartArea = L"ChartArea1";
			series54->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series54->Color = System::Drawing::Color::Red;
			series54->Legend = L"Legend1";
			series54->Name = L"Series6";
			series55->BorderWidth = 2;
			series55->ChartArea = L"ChartArea1";
			series55->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series55->Color = System::Drawing::Color::Red;
			series55->Legend = L"Legend1";
			series55->Name = L"Series7";
			series56->BorderWidth = 2;
			series56->ChartArea = L"ChartArea1";
			series56->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series56->Color = System::Drawing::Color::Blue;
			series56->Legend = L"Legend1";
			series56->Name = L"Series8";
			this->chart7->Series->Add(series49);
			this->chart7->Series->Add(series50);
			this->chart7->Series->Add(series51);
			this->chart7->Series->Add(series52);
			this->chart7->Series->Add(series53);
			this->chart7->Series->Add(series54);
			this->chart7->Series->Add(series55);
			this->chart7->Series->Add(series56);
			this->chart7->Size = System::Drawing::Size(206, 527);
			this->chart7->TabIndex = 13;
			this->chart7->Text = L"chart7";
			this->toolTip1->SetToolTip(this->chart7, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// chart8
			// 
			this->chart8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea8->AxisX->LabelStyle->Enabled = false;
			chartArea8->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea8->Name = L"ChartArea1";
			this->chart8->ChartAreas->Add(chartArea8);
			legend8->Enabled = false;
			legend8->Name = L"Legend1";
			this->chart8->Legends->Add(legend8);
			this->chart8->Location = System::Drawing::Point(1481, 46);
			this->chart8->Margin = System::Windows::Forms::Padding(1);
			this->chart8->Name = L"chart8";
			series57->BorderWidth = 2;
			series57->ChartArea = L"ChartArea1";
			series57->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series57->Color = System::Drawing::Color::Black;
			series57->Legend = L"Legend1";
			series57->Name = L"Series1";
			series58->BorderWidth = 2;
			series58->ChartArea = L"ChartArea1";
			series58->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series58->Color = System::Drawing::Color::ForestGreen;
			series58->Legend = L"Legend1";
			series58->Name = L"Series2";
			series59->BorderWidth = 2;
			series59->ChartArea = L"ChartArea1";
			series59->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series59->Color = System::Drawing::Color::ForestGreen;
			series59->Legend = L"Legend1";
			series59->Name = L"Series3";
			series60->BorderWidth = 2;
			series60->ChartArea = L"ChartArea1";
			series60->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series60->Color = System::Drawing::Color::Gold;
			series60->Legend = L"Legend1";
			series60->Name = L"Series4";
			series61->BorderWidth = 2;
			series61->ChartArea = L"ChartArea1";
			series61->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series61->Color = System::Drawing::Color::Gold;
			series61->Legend = L"Legend1";
			series61->Name = L"Series5";
			series62->BorderWidth = 2;
			series62->ChartArea = L"ChartArea1";
			series62->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series62->Color = System::Drawing::Color::Red;
			series62->Legend = L"Legend1";
			series62->Name = L"Series6";
			series63->BorderWidth = 2;
			series63->ChartArea = L"ChartArea1";
			series63->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series63->Color = System::Drawing::Color::Red;
			series63->Legend = L"Legend1";
			series63->Name = L"Series7";
			series64->BorderWidth = 2;
			series64->ChartArea = L"ChartArea1";
			series64->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series64->Color = System::Drawing::Color::Blue;
			series64->Legend = L"Legend1";
			series64->Name = L"Series8";
			this->chart8->Series->Add(series57);
			this->chart8->Series->Add(series58);
			this->chart8->Series->Add(series59);
			this->chart8->Series->Add(series60);
			this->chart8->Series->Add(series61);
			this->chart8->Series->Add(series62);
			this->chart8->Series->Add(series63);
			this->chart8->Series->Add(series64);
			this->chart8->Size = System::Drawing::Size(206, 527);
			this->chart8->TabIndex = 14;
			this->chart8->Text = L"chart8";
			this->toolTip1->SetToolTip(this->chart8, L"На графике отображаются последние 20 \r\nизмерянных значений сопротивления,\r\nустано"
				L"вленного на иммитаторе.\r\nЗеленым, желтым и красным показаны\r\n границы процентных"
				L" коридоров.");
			// 
			// textBox1
			// 
			this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox1->Location = System::Drawing::Point(6, 579);
			this->textBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(202, 27);
			this->textBox1->TabIndex = 15;
			// 
			// textBox2
			// 
			this->textBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox2->Location = System::Drawing::Point(217, 579);
			this->textBox2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(202, 27);
			this->textBox2->TabIndex = 16;
			// 
			// textBox3
			// 
			this->textBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox3->Location = System::Drawing::Point(428, 579);
			this->textBox3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(202, 27);
			this->textBox3->TabIndex = 17;
			// 
			// textBox4
			// 
			this->textBox4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox4->Location = System::Drawing::Point(639, 579);
			this->textBox4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(202, 27);
			this->textBox4->TabIndex = 18;
			// 
			// textBox5
			// 
			this->textBox5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox5->Location = System::Drawing::Point(850, 579);
			this->textBox5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(202, 27);
			this->textBox5->TabIndex = 19;
			// 
			// textBox6
			// 
			this->textBox6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox6->Location = System::Drawing::Point(1061, 579);
			this->textBox6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(202, 27);
			this->textBox6->TabIndex = 20;
			// 
			// textBox7
			// 
			this->textBox7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox7->Location = System::Drawing::Point(1272, 579);
			this->textBox7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(202, 27);
			this->textBox7->TabIndex = 21;
			// 
			// textBox8
			// 
			this->textBox8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox8->Location = System::Drawing::Point(1483, 579);
			this->textBox8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(202, 27);
			this->textBox8->TabIndex = 22;
			// 
			// textBox_AM_Tx_0
			// 
			this->textBox_AM_Tx_0->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_0->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_0->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_0->Location = System::Drawing::Point(6, 612);
			this->textBox_AM_Tx_0->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_0->Name = L"textBox_AM_Tx_0";
			this->textBox_AM_Tx_0->ReadOnly = true;
			this->textBox_AM_Tx_0->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_0->TabIndex = 31;
			// 
			// textBox_AM_Tx_1
			// 
			this->textBox_AM_Tx_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_1->Location = System::Drawing::Point(217, 612);
			this->textBox_AM_Tx_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_1->Name = L"textBox_AM_Tx_1";
			this->textBox_AM_Tx_1->ReadOnly = true;
			this->textBox_AM_Tx_1->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_1->TabIndex = 32;
			// 
			// textBox_AM_Tx_2
			// 
			this->textBox_AM_Tx_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_2->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_2->Location = System::Drawing::Point(428, 612);
			this->textBox_AM_Tx_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_2->Name = L"textBox_AM_Tx_2";
			this->textBox_AM_Tx_2->ReadOnly = true;
			this->textBox_AM_Tx_2->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_2->TabIndex = 33;
			// 
			// textBox_AM_Tx_3
			// 
			this->textBox_AM_Tx_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_3->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_3->Location = System::Drawing::Point(639, 612);
			this->textBox_AM_Tx_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_3->Name = L"textBox_AM_Tx_3";
			this->textBox_AM_Tx_3->ReadOnly = true;
			this->textBox_AM_Tx_3->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_3->TabIndex = 34;
			// 
			// textBox_AM_Tx_4
			// 
			this->textBox_AM_Tx_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_4->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_4->Location = System::Drawing::Point(850, 612);
			this->textBox_AM_Tx_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_4->Name = L"textBox_AM_Tx_4";
			this->textBox_AM_Tx_4->ReadOnly = true;
			this->textBox_AM_Tx_4->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_4->TabIndex = 35;
			// 
			// textBox_AM_Tx_5
			// 
			this->textBox_AM_Tx_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_5->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_5->Location = System::Drawing::Point(1061, 612);
			this->textBox_AM_Tx_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_5->Name = L"textBox_AM_Tx_5";
			this->textBox_AM_Tx_5->ReadOnly = true;
			this->textBox_AM_Tx_5->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_5->TabIndex = 36;
			// 
			// textBox_AM_Tx_6
			// 
			this->textBox_AM_Tx_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_6->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_6->Location = System::Drawing::Point(1272, 612);
			this->textBox_AM_Tx_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_6->Name = L"textBox_AM_Tx_6";
			this->textBox_AM_Tx_6->ReadOnly = true;
			this->textBox_AM_Tx_6->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_6->TabIndex = 37;
			// 
			// textBox_AM_Tx_7
			// 
			this->textBox_AM_Tx_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_AM_Tx_7->BackColor = System::Drawing::SystemColors::Window;
			this->textBox_AM_Tx_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_AM_Tx_7->Location = System::Drawing::Point(1483, 612);
			this->textBox_AM_Tx_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_AM_Tx_7->Name = L"textBox_AM_Tx_7";
			this->textBox_AM_Tx_7->ReadOnly = true;
			this->textBox_AM_Tx_7->Size = System::Drawing::Size(202, 27);
			this->textBox_AM_Tx_7->TabIndex = 38;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->файлToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(5, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(70, 28);
			this->menuStrip1->TabIndex = 5;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// файлToolStripMenuItem
			// 
			this->файлToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->открытьToolStripMenuItem,
					this->параметрыToolStripMenuItem, this->сохранитьРапортRtfToolStripMenuItem, this->работаСФайломМетрологииToolStripMenuItem,
					this->помощьToolStripMenuItem, this->uploadToFTPToolStripMenuItem
			});
			this->файлToolStripMenuItem->Name = L"файлToolStripMenuItem";
			this->файлToolStripMenuItem->Size = System::Drawing::Size(63, 24);
			this->файлToolStripMenuItem->Text = L"Меню";
			// 
			// открытьToolStripMenuItem
			// 
			this->открытьToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->Файл_метрологии_открыть });
			this->открытьToolStripMenuItem->Name = L"открытьToolStripMenuItem";
			this->открытьToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->открытьToolStripMenuItem->Text = L"Открыть";
			// 
			// Файл_метрологии_открыть
			// 
			this->Файл_метрологии_открыть->Name = L"Файл_метрологии_открыть";
			this->Файл_метрологии_открыть->Size = System::Drawing::Size(208, 26);
			this->Файл_метрологии_открыть->Text = L"Файл метрологии";
			this->Файл_метрологии_открыть->Click += gcnew System::EventHandler(this, &MyForm::Файл_метрологии_открыть_Click);
			// 
			// параметрыToolStripMenuItem
			// 
			this->параметрыToolStripMenuItem->Name = L"параметрыToolStripMenuItem";
			this->параметрыToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->параметрыToolStripMenuItem->Text = L"Параметры";
			this->параметрыToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::параметрыToolStripMenuItem_Click);
			// 
			// сохранитьРапортRtfToolStripMenuItem
			// 
			this->сохранитьРапортRtfToolStripMenuItem->Name = L"сохранитьРапортRtfToolStripMenuItem";
			this->сохранитьРапортRtfToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->сохранитьРапортRtfToolStripMenuItem->Text = L"Сохранить рапорт rtf";
			this->сохранитьРапортRtfToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::сохранитьРапортRtfToolStripMenuItem_Click);
			// 
			// работаСФайломМетрологииToolStripMenuItem
			// 
			this->работаСФайломМетрологииToolStripMenuItem->Name = L"работаСФайломМетрологииToolStripMenuItem";
			this->работаСФайломМетрологииToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->работаСФайломМетрологииToolStripMenuItem->Text = L"Работа с файлом метрологии";
			this->работаСФайломМетрологииToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::работаСФайломМетрологииToolStripMenuItem_Click);
			// 
			// помощьToolStripMenuItem
			// 
			this->помощьToolStripMenuItem->Name = L"помощьToolStripMenuItem";
			this->помощьToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->помощьToolStripMenuItem->Text = L"Помощь";
			this->помощьToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::помощьToolStripMenuItem_Click);
			// 
			// uploadToFTPToolStripMenuItem
			// 
			this->uploadToFTPToolStripMenuItem->Name = L"uploadToFTPToolStripMenuItem";
			this->uploadToFTPToolStripMenuItem->Size = System::Drawing::Size(290, 26);
			this->uploadToFTPToolStripMenuItem->Text = L"Upload to FTP";
			this->uploadToFTPToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::uploadToFTPToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// buttonR9
			// 
			this->buttonR9->Location = System::Drawing::Point(324, 6);
			this->buttonR9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR9->Name = L"buttonR9";
			this->buttonR9->Size = System::Drawing::Size(60, 25);
			this->buttonR9->TabIndex = 7;
			this->buttonR9->Text = L"AIR";
			this->toolTip1->SetToolTip(this->buttonR9, L" При включенном кольце\r\nустанавливает положение \"воздух\" на иммитаторе. \r\n");
			this->buttonR9->UseVisualStyleBackColor = true;
			this->buttonR9->Click += gcnew System::EventHandler(this, &MyForm::Air_Click);
			// 
			// buttonR1
			// 
			this->buttonR1->Location = System::Drawing::Point(395, 6);
			this->buttonR1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR1->Name = L"buttonR1";
			this->buttonR1->Size = System::Drawing::Size(60, 25);
			this->buttonR1->TabIndex = 7;
			this->buttonR1->Text = L"R1";
			this->toolTip1->SetToolTip(this->buttonR1, L"При включенном кольце устанавливает сопротивление R1 \r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n");
			this->buttonR1->UseVisualStyleBackColor = true;
			this->buttonR1->Click += gcnew System::EventHandler(this, &MyForm::R1_Click);
			// 
			// buttonR2
			// 
			this->buttonR2->Location = System::Drawing::Point(467, 6);
			this->buttonR2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR2->Name = L"buttonR2";
			this->buttonR2->Size = System::Drawing::Size(60, 25);
			this->buttonR2->TabIndex = 7;
			this->buttonR2->Text = L"R2";
			this->toolTip1->SetToolTip(this->buttonR2, L"При включенном кольце устанавливает сопротивление R2 \r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n\r\n");
			this->buttonR2->UseVisualStyleBackColor = true;
			this->buttonR2->Click += gcnew System::EventHandler(this, &MyForm::R2_Click);
			// 
			// buttonR3
			// 
			this->buttonR3->Location = System::Drawing::Point(535, 6);
			this->buttonR3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR3->Name = L"buttonR3";
			this->buttonR3->Size = System::Drawing::Size(60, 25);
			this->buttonR3->TabIndex = 7;
			this->buttonR3->Text = L"R3";
			this->toolTip1->SetToolTip(this->buttonR3, L"При включенном кольце устанавливает сопротивление R13\r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n\r\n");
			this->buttonR3->UseVisualStyleBackColor = true;
			this->buttonR3->Click += gcnew System::EventHandler(this, &MyForm::R3_Click);
			// 
			// buttonR4
			// 
			this->buttonR4->Location = System::Drawing::Point(604, 6);
			this->buttonR4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR4->Name = L"buttonR4";
			this->buttonR4->Size = System::Drawing::Size(60, 25);
			this->buttonR4->TabIndex = 7;
			this->buttonR4->Text = L"R4";
			this->toolTip1->SetToolTip(this->buttonR4, L"При включенном кольце устанавливает сопротивление R4 \r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n\r\n\r\n");
			this->buttonR4->UseVisualStyleBackColor = true;
			this->buttonR4->Click += gcnew System::EventHandler(this, &MyForm::R4_Click);
			// 
			// button_Report
			// 
			this->button_Report->Location = System::Drawing::Point(189, -1);
			this->button_Report->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Report->Name = L"button_Report";
			this->button_Report->Size = System::Drawing::Size(124, 48);
			this->button_Report->TabIndex = 33;
			this->button_Report->Text = L"ОТПРАВИТЬ В РАПОРТ";
			this->toolTip1->SetToolTip(this->button_Report, L"При нажатии отправляет данные о текущем измерении \r\nв рапорт. При повторном нажат"
				L"ии данные в рапорте \r\nобновляются. Даные также отображаются в таблице под\r\nграфи"
				L"ком.");
			this->button_Report->UseVisualStyleBackColor = true;
			this->button_Report->BackColorChanged += gcnew System::EventHandler(this, &MyForm::button_Report_BackColorChanged);
			this->button_Report->Click += gcnew System::EventHandler(this, &MyForm::Report_Click);
			// 
			// pictureBoxAir
			// 
			this->pictureBoxAir->Location = System::Drawing::Point(324, 31);
			this->pictureBoxAir->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxAir->Name = L"pictureBoxAir";
			this->pictureBoxAir->Size = System::Drawing::Size(60, 7);
			this->pictureBoxAir->TabIndex = 36;
			this->pictureBoxAir->TabStop = false;
			// 
			// pictureBoxR1
			// 
			this->pictureBoxR1->Location = System::Drawing::Point(395, 31);
			this->pictureBoxR1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR1->Name = L"pictureBoxR1";
			this->pictureBoxR1->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR1->TabIndex = 37;
			this->pictureBoxR1->TabStop = false;
			// 
			// pictureBoxR2
			// 
			this->pictureBoxR2->Location = System::Drawing::Point(467, 31);
			this->pictureBoxR2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR2->Name = L"pictureBoxR2";
			this->pictureBoxR2->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR2->TabIndex = 38;
			this->pictureBoxR2->TabStop = false;
			// 
			// pictureBoxR3
			// 
			this->pictureBoxR3->Location = System::Drawing::Point(535, 31);
			this->pictureBoxR3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR3->Name = L"pictureBoxR3";
			this->pictureBoxR3->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR3->TabIndex = 39;
			this->pictureBoxR3->TabStop = false;
			// 
			// pictureBoxR4
			// 
			this->pictureBoxR4->Location = System::Drawing::Point(604, 31);
			this->pictureBoxR4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR4->Name = L"pictureBoxR4";
			this->pictureBoxR4->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR4->TabIndex = 40;
			this->pictureBoxR4->TabStop = false;
			// 
			// button_BT_ON
			// 
			this->button_BT_ON->Location = System::Drawing::Point(1177, 4);
			this->button_BT_ON->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_BT_ON->Name = L"button_BT_ON";
			this->button_BT_ON->Size = System::Drawing::Size(135, 49);
			this->button_BT_ON->TabIndex = 46;
			this->button_BT_ON->Text = L"СОЕДИНЕНИЕ С КОЛЬЦОМ";
			this->toolTip1->SetToolTip(this->button_BT_ON, L"Установка связи с иммитатором по Блютус или кабелю.\r\nПеред нажатием кнопки убедит"
				L"есь, что тумблер питания\r\nна иммитаторе включен и мигает синий светодиод\r\n");
			this->button_BT_ON->UseVisualStyleBackColor = true;
			this->button_BT_ON->Click += gcnew System::EventHandler(this, &MyForm::BT_ON);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"Ro_wiew", L"PH_wiew", L"температура", L"RAW_wiew" });
			this->comboBox1->Location = System::Drawing::Point(73, 4);
			this->comboBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(103, 24);
			this->comboBox1->TabIndex = 49;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// textBox17
			// 
			this->textBox17->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox17->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox17->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox17->ForeColor = System::Drawing::Color::LimeGreen;
			this->textBox17->Location = System::Drawing::Point(0, 0);
			this->textBox17->Margin = System::Windows::Forms::Padding(0);
			this->textBox17->Name = L"textBox17";
			this->textBox17->ReadOnly = true;
			this->textBox17->Size = System::Drawing::Size(100, 20);
			this->textBox17->TabIndex = 51;
			this->textBox17->Text = L"-----5%";
			// 
			// textBox18
			// 
			this->textBox18->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox18->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox18->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox18->ForeColor = System::Drawing::Color::Gold;
			this->textBox18->Location = System::Drawing::Point(100, 0);
			this->textBox18->Margin = System::Windows::Forms::Padding(0);
			this->textBox18->Name = L"textBox18";
			this->textBox18->ReadOnly = true;
			this->textBox18->Size = System::Drawing::Size(100, 20);
			this->textBox18->TabIndex = 52;
			this->textBox18->Text = L"-----10%";
			// 
			// textBox19
			// 
			this->textBox19->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox19->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox19->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox19->ForeColor = System::Drawing::Color::Red;
			this->textBox19->Location = System::Drawing::Point(200, 0);
			this->textBox19->Margin = System::Windows::Forms::Padding(0);
			this->textBox19->Name = L"textBox19";
			this->textBox19->ReadOnly = true;
			this->textBox19->Size = System::Drawing::Size(100, 20);
			this->textBox19->TabIndex = 53;
			this->textBox19->Text = L"---15%";
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel2->ColumnCount = 8;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				100)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				100)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				100)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				143)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				171)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				80)));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel2->Controls->Add(this->textBox17, 0, 0);
			this->tableLayoutPanel2->Controls->Add(this->textBox21, 4, 0);
			this->tableLayoutPanel2->Controls->Add(this->textBox20, 3, 0);
			this->tableLayoutPanel2->Controls->Add(this->textBox22, 3, 0);
			this->tableLayoutPanel2->Controls->Add(this->textBox19, 2, 0);
			this->tableLayoutPanel2->Controls->Add(this->textBox18, 1, 0);
			this->tableLayoutPanel2->Controls->Add(this->textBox23, 7, 0);
			this->tableLayoutPanel2->Location = System::Drawing::Point(0, 656);
			this->tableLayoutPanel2->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->tableLayoutPanel2->RowCount = 1;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(1697, 23);
			this->tableLayoutPanel2->TabIndex = 55;
			// 
			// textBox21
			// 
			this->textBox21->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox21->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox21->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox21->ForeColor = System::Drawing::Color::Red;
			this->textBox21->Location = System::Drawing::Point(614, 0);
			this->textBox21->Margin = System::Windows::Forms::Padding(0);
			this->textBox21->Name = L"textBox21";
			this->textBox21->ReadOnly = true;
			this->textBox21->Size = System::Drawing::Size(80, 20);
			this->textBox21->TabIndex = 54;
			this->textBox21->Text = L"-----0";
			// 
			// textBox20
			// 
			this->textBox20->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox20->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox20->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox20->ForeColor = System::Drawing::Color::Black;
			this->textBox20->Location = System::Drawing::Point(443, 0);
			this->textBox20->Margin = System::Windows::Forms::Padding(0);
			this->textBox20->Name = L"textBox20";
			this->textBox20->ReadOnly = true;
			this->textBox20->Size = System::Drawing::Size(171, 20);
			this->textBox20->TabIndex = 55;
			this->textBox20->Text = L"-----мгновенное";
			// 
			// textBox22
			// 
			this->textBox22->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox22->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox22->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox22->ForeColor = System::Drawing::Color::RoyalBlue;
			this->textBox22->Location = System::Drawing::Point(300, 0);
			this->textBox22->Margin = System::Windows::Forms::Padding(0);
			this->textBox22->Name = L"textBox22";
			this->textBox22->ReadOnly = true;
			this->textBox22->Size = System::Drawing::Size(143, 20);
			this->textBox22->TabIndex = 56;
			this->textBox22->Text = L"-----среднее";
			// 
			// textBox23
			// 
			this->textBox23->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox23->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox23->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox23->Location = System::Drawing::Point(697, 2);
			this->textBox23->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox23->Name = L"textBox23";
			this->textBox23->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->textBox23->Size = System::Drawing::Size(1031, 20);
			this->textBox23->TabIndex = 56;
			this->textBox23->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// tableLayoutPanel3
			// 
			this->tableLayoutPanel3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel3->ColumnCount = 9;
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				4)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				12)));
			this->tableLayoutPanel3->Controls->Add(this->textBox127, 0, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox126, 0, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox125, 0, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox124, 0, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox31, 8, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox39, 8, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox55, 8, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox47, 8, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox54, 7, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox46, 7, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox38, 7, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox30, 7, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox29, 6, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox37, 6, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox45, 6, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox53, 6, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox28, 5, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox36, 5, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox44, 5, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox52, 5, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox27, 4, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox35, 4, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox43, 4, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox51, 4, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox26, 3, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox34, 3, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox42, 3, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox50, 3, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox25, 2, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox33, 2, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox41, 2, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox49, 2, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox24, 1, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox32, 1, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox40, 1, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox48, 1, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox120, 0, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox121, 0, 1);
			this->tableLayoutPanel3->Controls->Add(this->textBox122, 0, 2);
			this->tableLayoutPanel3->Controls->Add(this->textBox128, 0, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox88, 1, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox89, 2, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox90, 3, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox91, 4, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox92, 5, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox93, 6, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox94, 7, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox95, 8, 8);
			this->tableLayoutPanel3->Controls->Add(this->textBox56, 1, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox57, 2, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox58, 3, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox59, 4, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox60, 5, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox61, 6, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox62, 7, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox63, 8, 4);
			this->tableLayoutPanel3->Controls->Add(this->textBox64, 1, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox65, 2, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox66, 3, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox67, 4, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox68, 5, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox69, 6, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox70, 7, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox71, 8, 5);
			this->tableLayoutPanel3->Controls->Add(this->textBox72, 1, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox73, 2, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox74, 3, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox75, 4, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox76, 5, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox77, 6, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox78, 7, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox79, 8, 6);
			this->tableLayoutPanel3->Controls->Add(this->textBox80, 1, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox81, 2, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox82, 3, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox83, 4, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox84, 5, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox85, 6, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox123, 0, 3);
			this->tableLayoutPanel3->Controls->Add(this->textBox86, 7, 7);
			this->tableLayoutPanel3->Controls->Add(this->textBox87, 8, 7);
			this->tableLayoutPanel3->Location = System::Drawing::Point(3, 704);
			this->tableLayoutPanel3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
			this->tableLayoutPanel3->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->tableLayoutPanel3->RowCount = 9;
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 11.11111F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));
			this->tableLayoutPanel3->Size = System::Drawing::Size(1691, 220);
			this->tableLayoutPanel3->TabIndex = 57;
			// 
			// textBox127
			// 
			this->textBox127->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox127->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox127->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox127->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox127->Location = System::Drawing::Point(0, 168);
			this->textBox127->Margin = System::Windows::Forms::Padding(0);
			this->textBox127->Name = L"textBox127";
			this->textBox127->Size = System::Drawing::Size(67, 15);
			this->textBox127->TabIndex = 116;
			this->textBox127->Text = L"R8";
			// 
			// textBox126
			// 
			this->textBox126->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox126->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox126->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox126->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox126->Location = System::Drawing::Point(0, 144);
			this->textBox126->Margin = System::Windows::Forms::Padding(0);
			this->textBox126->Name = L"textBox126";
			this->textBox126->Size = System::Drawing::Size(67, 15);
			this->textBox126->TabIndex = 115;
			this->textBox126->Text = L"R7";
			// 
			// textBox125
			// 
			this->textBox125->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox125->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox125->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox125->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox125->Location = System::Drawing::Point(0, 120);
			this->textBox125->Margin = System::Windows::Forms::Padding(0);
			this->textBox125->Name = L"textBox125";
			this->textBox125->Size = System::Drawing::Size(67, 15);
			this->textBox125->TabIndex = 114;
			this->textBox125->Text = L"R6";
			// 
			// textBox124
			// 
			this->textBox124->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox124->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox124->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox124->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox124->Location = System::Drawing::Point(0, 96);
			this->textBox124->Margin = System::Windows::Forms::Padding(0);
			this->textBox124->Name = L"textBox124";
			this->textBox124->Size = System::Drawing::Size(67, 15);
			this->textBox124->TabIndex = 113;
			this->textBox124->Text = L"R5";
			// 
			// textBox31
			// 
			this->textBox31->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox31->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox31->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox31->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox31->Location = System::Drawing::Point(1481, 0);
			this->textBox31->Margin = System::Windows::Forms::Padding(0);
			this->textBox31->Name = L"textBox31";
			this->textBox31->Size = System::Drawing::Size(210, 15);
			this->textBox31->TabIndex = 30;
			// 
			// textBox39
			// 
			this->textBox39->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox39->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox39->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox39->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox39->Location = System::Drawing::Point(1481, 24);
			this->textBox39->Margin = System::Windows::Forms::Padding(0);
			this->textBox39->Name = L"textBox39";
			this->textBox39->Size = System::Drawing::Size(210, 15);
			this->textBox39->TabIndex = 38;
			// 
			// textBox55
			// 
			this->textBox55->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox55->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox55->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox55->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox55->Location = System::Drawing::Point(1481, 72);
			this->textBox55->Margin = System::Windows::Forms::Padding(0);
			this->textBox55->Name = L"textBox55";
			this->textBox55->Size = System::Drawing::Size(210, 15);
			this->textBox55->TabIndex = 58;
			// 
			// textBox47
			// 
			this->textBox47->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox47->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox47->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox47->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox47->Location = System::Drawing::Point(1481, 48);
			this->textBox47->Margin = System::Windows::Forms::Padding(0);
			this->textBox47->Name = L"textBox47";
			this->textBox47->Size = System::Drawing::Size(210, 15);
			this->textBox47->TabIndex = 46;
			// 
			// textBox54
			// 
			this->textBox54->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox54->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox54->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox54->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox54->Location = System::Drawing::Point(1279, 72);
			this->textBox54->Margin = System::Windows::Forms::Padding(0);
			this->textBox54->Name = L"textBox54";
			this->textBox54->Size = System::Drawing::Size(202, 15);
			this->textBox54->TabIndex = 53;
			// 
			// textBox46
			// 
			this->textBox46->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox46->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox46->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox46->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox46->Location = System::Drawing::Point(1279, 48);
			this->textBox46->Margin = System::Windows::Forms::Padding(0);
			this->textBox46->Name = L"textBox46";
			this->textBox46->Size = System::Drawing::Size(202, 15);
			this->textBox46->TabIndex = 45;
			// 
			// textBox38
			// 
			this->textBox38->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox38->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox38->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox38->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox38->Location = System::Drawing::Point(1279, 24);
			this->textBox38->Margin = System::Windows::Forms::Padding(0);
			this->textBox38->Name = L"textBox38";
			this->textBox38->Size = System::Drawing::Size(202, 15);
			this->textBox38->TabIndex = 37;
			// 
			// textBox30
			// 
			this->textBox30->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox30->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox30->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox30->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox30->Location = System::Drawing::Point(1279, 0);
			this->textBox30->Margin = System::Windows::Forms::Padding(0);
			this->textBox30->Name = L"textBox30";
			this->textBox30->Size = System::Drawing::Size(202, 15);
			this->textBox30->TabIndex = 29;
			// 
			// textBox29
			// 
			this->textBox29->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox29->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox29->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox29->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox29->Location = System::Drawing::Point(1077, 0);
			this->textBox29->Margin = System::Windows::Forms::Padding(0);
			this->textBox29->Name = L"textBox29";
			this->textBox29->Size = System::Drawing::Size(202, 15);
			this->textBox29->TabIndex = 28;
			// 
			// textBox37
			// 
			this->textBox37->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox37->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox37->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox37->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox37->Location = System::Drawing::Point(1077, 24);
			this->textBox37->Margin = System::Windows::Forms::Padding(0);
			this->textBox37->Name = L"textBox37";
			this->textBox37->Size = System::Drawing::Size(202, 15);
			this->textBox37->TabIndex = 36;
			// 
			// textBox45
			// 
			this->textBox45->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox45->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox45->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox45->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox45->Location = System::Drawing::Point(1077, 48);
			this->textBox45->Margin = System::Windows::Forms::Padding(0);
			this->textBox45->Name = L"textBox45";
			this->textBox45->Size = System::Drawing::Size(202, 15);
			this->textBox45->TabIndex = 44;
			// 
			// textBox53
			// 
			this->textBox53->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox53->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox53->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox53->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox53->Location = System::Drawing::Point(1077, 72);
			this->textBox53->Margin = System::Windows::Forms::Padding(0);
			this->textBox53->Name = L"textBox53";
			this->textBox53->Size = System::Drawing::Size(202, 15);
			this->textBox53->TabIndex = 52;
			// 
			// textBox28
			// 
			this->textBox28->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox28->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox28->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox28->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox28->Location = System::Drawing::Point(875, 0);
			this->textBox28->Margin = System::Windows::Forms::Padding(0);
			this->textBox28->Name = L"textBox28";
			this->textBox28->Size = System::Drawing::Size(202, 15);
			this->textBox28->TabIndex = 27;
			// 
			// textBox36
			// 
			this->textBox36->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox36->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox36->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox36->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox36->Location = System::Drawing::Point(875, 24);
			this->textBox36->Margin = System::Windows::Forms::Padding(0);
			this->textBox36->Name = L"textBox36";
			this->textBox36->Size = System::Drawing::Size(202, 15);
			this->textBox36->TabIndex = 35;
			// 
			// textBox44
			// 
			this->textBox44->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox44->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox44->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox44->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox44->Location = System::Drawing::Point(875, 48);
			this->textBox44->Margin = System::Windows::Forms::Padding(0);
			this->textBox44->Name = L"textBox44";
			this->textBox44->Size = System::Drawing::Size(202, 15);
			this->textBox44->TabIndex = 43;
			// 
			// textBox52
			// 
			this->textBox52->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox52->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox52->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox52->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox52->Location = System::Drawing::Point(875, 72);
			this->textBox52->Margin = System::Windows::Forms::Padding(0);
			this->textBox52->Name = L"textBox52";
			this->textBox52->Size = System::Drawing::Size(202, 15);
			this->textBox52->TabIndex = 51;
			// 
			// textBox27
			// 
			this->textBox27->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox27->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox27->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox27->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox27->Location = System::Drawing::Point(673, 0);
			this->textBox27->Margin = System::Windows::Forms::Padding(0);
			this->textBox27->Name = L"textBox27";
			this->textBox27->Size = System::Drawing::Size(202, 15);
			this->textBox27->TabIndex = 26;
			// 
			// textBox35
			// 
			this->textBox35->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox35->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox35->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox35->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox35->Location = System::Drawing::Point(673, 24);
			this->textBox35->Margin = System::Windows::Forms::Padding(0);
			this->textBox35->Name = L"textBox35";
			this->textBox35->Size = System::Drawing::Size(202, 15);
			this->textBox35->TabIndex = 34;
			// 
			// textBox43
			// 
			this->textBox43->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox43->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox43->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox43->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox43->Location = System::Drawing::Point(673, 48);
			this->textBox43->Margin = System::Windows::Forms::Padding(0);
			this->textBox43->Name = L"textBox43";
			this->textBox43->Size = System::Drawing::Size(202, 15);
			this->textBox43->TabIndex = 42;
			// 
			// textBox51
			// 
			this->textBox51->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox51->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox51->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox51->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox51->Location = System::Drawing::Point(673, 72);
			this->textBox51->Margin = System::Windows::Forms::Padding(0);
			this->textBox51->Name = L"textBox51";
			this->textBox51->Size = System::Drawing::Size(202, 15);
			this->textBox51->TabIndex = 50;
			// 
			// textBox26
			// 
			this->textBox26->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox26->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox26->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox26->Location = System::Drawing::Point(471, 0);
			this->textBox26->Margin = System::Windows::Forms::Padding(0);
			this->textBox26->Name = L"textBox26";
			this->textBox26->Size = System::Drawing::Size(202, 15);
			this->textBox26->TabIndex = 25;
			// 
			// textBox34
			// 
			this->textBox34->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox34->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox34->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox34->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox34->Location = System::Drawing::Point(471, 24);
			this->textBox34->Margin = System::Windows::Forms::Padding(0);
			this->textBox34->Name = L"textBox34";
			this->textBox34->Size = System::Drawing::Size(202, 15);
			this->textBox34->TabIndex = 33;
			// 
			// textBox42
			// 
			this->textBox42->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox42->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox42->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox42->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox42->Location = System::Drawing::Point(471, 48);
			this->textBox42->Margin = System::Windows::Forms::Padding(0);
			this->textBox42->Name = L"textBox42";
			this->textBox42->Size = System::Drawing::Size(202, 15);
			this->textBox42->TabIndex = 41;
			// 
			// textBox50
			// 
			this->textBox50->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox50->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox50->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox50->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox50->Location = System::Drawing::Point(471, 72);
			this->textBox50->Margin = System::Windows::Forms::Padding(0);
			this->textBox50->Name = L"textBox50";
			this->textBox50->Size = System::Drawing::Size(202, 15);
			this->textBox50->TabIndex = 49;
			// 
			// textBox25
			// 
			this->textBox25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox25->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox25->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox25->Location = System::Drawing::Point(269, 0);
			this->textBox25->Margin = System::Windows::Forms::Padding(0);
			this->textBox25->Name = L"textBox25";
			this->textBox25->Size = System::Drawing::Size(202, 15);
			this->textBox25->TabIndex = 24;
			// 
			// textBox33
			// 
			this->textBox33->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox33->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox33->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox33->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox33->Location = System::Drawing::Point(269, 24);
			this->textBox33->Margin = System::Windows::Forms::Padding(0);
			this->textBox33->Name = L"textBox33";
			this->textBox33->Size = System::Drawing::Size(202, 15);
			this->textBox33->TabIndex = 32;
			// 
			// textBox41
			// 
			this->textBox41->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox41->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox41->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox41->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox41->Location = System::Drawing::Point(269, 48);
			this->textBox41->Margin = System::Windows::Forms::Padding(0);
			this->textBox41->Name = L"textBox41";
			this->textBox41->Size = System::Drawing::Size(202, 15);
			this->textBox41->TabIndex = 40;
			// 
			// textBox49
			// 
			this->textBox49->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox49->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox49->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox49->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox49->Location = System::Drawing::Point(269, 72);
			this->textBox49->Margin = System::Windows::Forms::Padding(0);
			this->textBox49->Name = L"textBox49";
			this->textBox49->Size = System::Drawing::Size(202, 15);
			this->textBox49->TabIndex = 48;
			// 
			// textBox24
			// 
			this->textBox24->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox24->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox24->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox24->Location = System::Drawing::Point(67, 0);
			this->textBox24->Margin = System::Windows::Forms::Padding(0);
			this->textBox24->Name = L"textBox24";
			this->textBox24->Size = System::Drawing::Size(202, 15);
			this->textBox24->TabIndex = 23;
			// 
			// textBox32
			// 
			this->textBox32->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox32->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox32->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox32->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox32->Location = System::Drawing::Point(67, 24);
			this->textBox32->Margin = System::Windows::Forms::Padding(0);
			this->textBox32->Name = L"textBox32";
			this->textBox32->Size = System::Drawing::Size(202, 15);
			this->textBox32->TabIndex = 31;
			// 
			// textBox40
			// 
			this->textBox40->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox40->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox40->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox40->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox40->Location = System::Drawing::Point(67, 48);
			this->textBox40->Margin = System::Windows::Forms::Padding(0);
			this->textBox40->Name = L"textBox40";
			this->textBox40->Size = System::Drawing::Size(202, 15);
			this->textBox40->TabIndex = 39;
			// 
			// textBox48
			// 
			this->textBox48->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox48->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox48->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox48->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox48->Location = System::Drawing::Point(67, 72);
			this->textBox48->Margin = System::Windows::Forms::Padding(0);
			this->textBox48->Name = L"textBox48";
			this->textBox48->Size = System::Drawing::Size(202, 15);
			this->textBox48->TabIndex = 47;
			// 
			// textBox120
			// 
			this->textBox120->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox120->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox120->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox120->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox120->Location = System::Drawing::Point(0, 0);
			this->textBox120->Margin = System::Windows::Forms::Padding(0);
			this->textBox120->Name = L"textBox120";
			this->textBox120->ReadOnly = true;
			this->textBox120->Size = System::Drawing::Size(67, 15);
			this->textBox120->TabIndex = 67;
			this->textBox120->Text = L"R1";
			// 
			// textBox121
			// 
			this->textBox121->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox121->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox121->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox121->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox121->Location = System::Drawing::Point(0, 24);
			this->textBox121->Margin = System::Windows::Forms::Padding(0);
			this->textBox121->Name = L"textBox121";
			this->textBox121->ReadOnly = true;
			this->textBox121->Size = System::Drawing::Size(67, 15);
			this->textBox121->TabIndex = 68;
			this->textBox121->Text = L"R2";
			// 
			// textBox122
			// 
			this->textBox122->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox122->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox122->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox122->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox122->Location = System::Drawing::Point(0, 48);
			this->textBox122->Margin = System::Windows::Forms::Padding(0);
			this->textBox122->Name = L"textBox122";
			this->textBox122->ReadOnly = true;
			this->textBox122->Size = System::Drawing::Size(67, 15);
			this->textBox122->TabIndex = 69;
			this->textBox122->Text = L"R3";
			// 
			// textBox128
			// 
			this->textBox128->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox128->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox128->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox128->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox128->Location = System::Drawing::Point(0, 192);
			this->textBox128->Margin = System::Windows::Forms::Padding(0);
			this->textBox128->Name = L"textBox128";
			this->textBox128->ReadOnly = true;
			this->textBox128->Size = System::Drawing::Size(67, 15);
			this->textBox128->TabIndex = 71;
			this->textBox128->Text = L"AIR";
			// 
			// textBox88
			// 
			this->textBox88->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox88->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox88->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox88->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox88->Location = System::Drawing::Point(67, 192);
			this->textBox88->Margin = System::Windows::Forms::Padding(0);
			this->textBox88->Name = L"textBox88";
			this->textBox88->Size = System::Drawing::Size(202, 16);
			this->textBox88->TabIndex = 59;
			// 
			// textBox89
			// 
			this->textBox89->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox89->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox89->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox89->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox89->Location = System::Drawing::Point(269, 192);
			this->textBox89->Margin = System::Windows::Forms::Padding(0);
			this->textBox89->Name = L"textBox89";
			this->textBox89->Size = System::Drawing::Size(202, 15);
			this->textBox89->TabIndex = 60;
			// 
			// textBox90
			// 
			this->textBox90->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox90->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox90->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox90->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox90->Location = System::Drawing::Point(471, 192);
			this->textBox90->Margin = System::Windows::Forms::Padding(0);
			this->textBox90->Name = L"textBox90";
			this->textBox90->Size = System::Drawing::Size(202, 15);
			this->textBox90->TabIndex = 61;
			// 
			// textBox91
			// 
			this->textBox91->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox91->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox91->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox91->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox91->Location = System::Drawing::Point(673, 192);
			this->textBox91->Margin = System::Windows::Forms::Padding(0);
			this->textBox91->Name = L"textBox91";
			this->textBox91->Size = System::Drawing::Size(202, 15);
			this->textBox91->TabIndex = 62;
			// 
			// textBox92
			// 
			this->textBox92->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox92->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox92->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox92->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox92->Location = System::Drawing::Point(875, 192);
			this->textBox92->Margin = System::Windows::Forms::Padding(0);
			this->textBox92->Name = L"textBox92";
			this->textBox92->Size = System::Drawing::Size(202, 15);
			this->textBox92->TabIndex = 63;
			// 
			// textBox93
			// 
			this->textBox93->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox93->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox93->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox93->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox93->Location = System::Drawing::Point(1077, 192);
			this->textBox93->Margin = System::Windows::Forms::Padding(0);
			this->textBox93->Name = L"textBox93";
			this->textBox93->Size = System::Drawing::Size(202, 15);
			this->textBox93->TabIndex = 64;
			// 
			// textBox94
			// 
			this->textBox94->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox94->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox94->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox94->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox94->Location = System::Drawing::Point(1279, 192);
			this->textBox94->Margin = System::Windows::Forms::Padding(0);
			this->textBox94->Name = L"textBox94";
			this->textBox94->Size = System::Drawing::Size(202, 15);
			this->textBox94->TabIndex = 65;
			// 
			// textBox95
			// 
			this->textBox95->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox95->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox95->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox95->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox95->Location = System::Drawing::Point(1481, 192);
			this->textBox95->Margin = System::Windows::Forms::Padding(0);
			this->textBox95->Name = L"textBox95";
			this->textBox95->Size = System::Drawing::Size(210, 15);
			this->textBox95->TabIndex = 66;
			// 
			// textBox56
			// 
			this->textBox56->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox56->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox56->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox56->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox56->Location = System::Drawing::Point(67, 96);
			this->textBox56->Margin = System::Windows::Forms::Padding(0);
			this->textBox56->Name = L"textBox56";
			this->textBox56->Size = System::Drawing::Size(202, 15);
			this->textBox56->TabIndex = 81;
			// 
			// textBox57
			// 
			this->textBox57->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox57->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox57->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox57->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox57->Location = System::Drawing::Point(269, 96);
			this->textBox57->Margin = System::Windows::Forms::Padding(0);
			this->textBox57->Name = L"textBox57";
			this->textBox57->Size = System::Drawing::Size(202, 15);
			this->textBox57->TabIndex = 82;
			// 
			// textBox58
			// 
			this->textBox58->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox58->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox58->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox58->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox58->Location = System::Drawing::Point(471, 96);
			this->textBox58->Margin = System::Windows::Forms::Padding(0);
			this->textBox58->Name = L"textBox58";
			this->textBox58->Size = System::Drawing::Size(202, 15);
			this->textBox58->TabIndex = 83;
			// 
			// textBox59
			// 
			this->textBox59->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox59->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox59->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox59->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox59->Location = System::Drawing::Point(673, 96);
			this->textBox59->Margin = System::Windows::Forms::Padding(0);
			this->textBox59->Name = L"textBox59";
			this->textBox59->Size = System::Drawing::Size(202, 15);
			this->textBox59->TabIndex = 84;
			// 
			// textBox60
			// 
			this->textBox60->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox60->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox60->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox60->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox60->Location = System::Drawing::Point(875, 96);
			this->textBox60->Margin = System::Windows::Forms::Padding(0);
			this->textBox60->Name = L"textBox60";
			this->textBox60->Size = System::Drawing::Size(202, 15);
			this->textBox60->TabIndex = 85;
			// 
			// textBox61
			// 
			this->textBox61->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox61->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox61->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox61->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox61->Location = System::Drawing::Point(1077, 96);
			this->textBox61->Margin = System::Windows::Forms::Padding(0);
			this->textBox61->Name = L"textBox61";
			this->textBox61->Size = System::Drawing::Size(202, 15);
			this->textBox61->TabIndex = 86;
			// 
			// textBox62
			// 
			this->textBox62->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox62->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox62->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox62->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox62->Location = System::Drawing::Point(1279, 96);
			this->textBox62->Margin = System::Windows::Forms::Padding(0);
			this->textBox62->Name = L"textBox62";
			this->textBox62->Size = System::Drawing::Size(202, 15);
			this->textBox62->TabIndex = 87;
			// 
			// textBox63
			// 
			this->textBox63->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox63->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox63->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox63->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox63->Location = System::Drawing::Point(1481, 96);
			this->textBox63->Margin = System::Windows::Forms::Padding(0);
			this->textBox63->Name = L"textBox63";
			this->textBox63->Size = System::Drawing::Size(210, 15);
			this->textBox63->TabIndex = 88;
			// 
			// textBox64
			// 
			this->textBox64->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox64->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox64->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox64->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox64->Location = System::Drawing::Point(67, 120);
			this->textBox64->Margin = System::Windows::Forms::Padding(0);
			this->textBox64->Name = L"textBox64";
			this->textBox64->Size = System::Drawing::Size(202, 15);
			this->textBox64->TabIndex = 89;
			// 
			// textBox65
			// 
			this->textBox65->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox65->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox65->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox65->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox65->Location = System::Drawing::Point(269, 120);
			this->textBox65->Margin = System::Windows::Forms::Padding(0);
			this->textBox65->Name = L"textBox65";
			this->textBox65->Size = System::Drawing::Size(202, 15);
			this->textBox65->TabIndex = 90;
			// 
			// textBox66
			// 
			this->textBox66->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox66->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox66->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox66->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox66->Location = System::Drawing::Point(471, 120);
			this->textBox66->Margin = System::Windows::Forms::Padding(0);
			this->textBox66->Name = L"textBox66";
			this->textBox66->Size = System::Drawing::Size(202, 15);
			this->textBox66->TabIndex = 91;
			// 
			// textBox67
			// 
			this->textBox67->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox67->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox67->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox67->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox67->Location = System::Drawing::Point(673, 120);
			this->textBox67->Margin = System::Windows::Forms::Padding(0);
			this->textBox67->Name = L"textBox67";
			this->textBox67->Size = System::Drawing::Size(202, 15);
			this->textBox67->TabIndex = 92;
			// 
			// textBox68
			// 
			this->textBox68->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox68->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox68->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox68->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox68->Location = System::Drawing::Point(875, 120);
			this->textBox68->Margin = System::Windows::Forms::Padding(0);
			this->textBox68->Name = L"textBox68";
			this->textBox68->Size = System::Drawing::Size(202, 15);
			this->textBox68->TabIndex = 93;
			// 
			// textBox69
			// 
			this->textBox69->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox69->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox69->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox69->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox69->Location = System::Drawing::Point(1077, 120);
			this->textBox69->Margin = System::Windows::Forms::Padding(0);
			this->textBox69->Name = L"textBox69";
			this->textBox69->Size = System::Drawing::Size(202, 15);
			this->textBox69->TabIndex = 94;
			// 
			// textBox70
			// 
			this->textBox70->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox70->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox70->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox70->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox70->Location = System::Drawing::Point(1279, 120);
			this->textBox70->Margin = System::Windows::Forms::Padding(0);
			this->textBox70->Name = L"textBox70";
			this->textBox70->Size = System::Drawing::Size(202, 15);
			this->textBox70->TabIndex = 95;
			// 
			// textBox71
			// 
			this->textBox71->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox71->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox71->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox71->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox71->Location = System::Drawing::Point(1481, 120);
			this->textBox71->Margin = System::Windows::Forms::Padding(0);
			this->textBox71->Name = L"textBox71";
			this->textBox71->Size = System::Drawing::Size(210, 15);
			this->textBox71->TabIndex = 96;
			// 
			// textBox72
			// 
			this->textBox72->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox72->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox72->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox72->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox72->Location = System::Drawing::Point(67, 144);
			this->textBox72->Margin = System::Windows::Forms::Padding(0);
			this->textBox72->Name = L"textBox72";
			this->textBox72->Size = System::Drawing::Size(202, 15);
			this->textBox72->TabIndex = 97;
			// 
			// textBox73
			// 
			this->textBox73->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox73->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox73->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox73->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox73->Location = System::Drawing::Point(269, 144);
			this->textBox73->Margin = System::Windows::Forms::Padding(0);
			this->textBox73->Name = L"textBox73";
			this->textBox73->Size = System::Drawing::Size(202, 15);
			this->textBox73->TabIndex = 98;
			// 
			// textBox74
			// 
			this->textBox74->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox74->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox74->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox74->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox74->Location = System::Drawing::Point(471, 144);
			this->textBox74->Margin = System::Windows::Forms::Padding(0);
			this->textBox74->Name = L"textBox74";
			this->textBox74->Size = System::Drawing::Size(202, 15);
			this->textBox74->TabIndex = 99;
			// 
			// textBox75
			// 
			this->textBox75->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox75->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox75->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox75->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox75->Location = System::Drawing::Point(673, 144);
			this->textBox75->Margin = System::Windows::Forms::Padding(0);
			this->textBox75->Name = L"textBox75";
			this->textBox75->Size = System::Drawing::Size(202, 15);
			this->textBox75->TabIndex = 100;
			// 
			// textBox76
			// 
			this->textBox76->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox76->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox76->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox76->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox76->Location = System::Drawing::Point(875, 144);
			this->textBox76->Margin = System::Windows::Forms::Padding(0);
			this->textBox76->Name = L"textBox76";
			this->textBox76->Size = System::Drawing::Size(202, 15);
			this->textBox76->TabIndex = 101;
			// 
			// textBox77
			// 
			this->textBox77->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox77->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox77->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox77->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox77->Location = System::Drawing::Point(1077, 144);
			this->textBox77->Margin = System::Windows::Forms::Padding(0);
			this->textBox77->Name = L"textBox77";
			this->textBox77->Size = System::Drawing::Size(202, 15);
			this->textBox77->TabIndex = 102;
			// 
			// textBox78
			// 
			this->textBox78->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox78->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox78->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox78->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox78->Location = System::Drawing::Point(1279, 144);
			this->textBox78->Margin = System::Windows::Forms::Padding(0);
			this->textBox78->Name = L"textBox78";
			this->textBox78->Size = System::Drawing::Size(202, 15);
			this->textBox78->TabIndex = 103;
			// 
			// textBox79
			// 
			this->textBox79->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox79->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox79->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox79->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox79->Location = System::Drawing::Point(1481, 144);
			this->textBox79->Margin = System::Windows::Forms::Padding(0);
			this->textBox79->Name = L"textBox79";
			this->textBox79->Size = System::Drawing::Size(210, 15);
			this->textBox79->TabIndex = 104;
			// 
			// textBox80
			// 
			this->textBox80->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox80->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox80->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox80->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox80->Location = System::Drawing::Point(67, 168);
			this->textBox80->Margin = System::Windows::Forms::Padding(0);
			this->textBox80->Name = L"textBox80";
			this->textBox80->Size = System::Drawing::Size(202, 16);
			this->textBox80->TabIndex = 105;
			// 
			// textBox81
			// 
			this->textBox81->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox81->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox81->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox81->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox81->Location = System::Drawing::Point(269, 168);
			this->textBox81->Margin = System::Windows::Forms::Padding(0);
			this->textBox81->Name = L"textBox81";
			this->textBox81->Size = System::Drawing::Size(202, 15);
			this->textBox81->TabIndex = 106;
			// 
			// textBox82
			// 
			this->textBox82->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox82->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox82->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox82->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox82->Location = System::Drawing::Point(471, 168);
			this->textBox82->Margin = System::Windows::Forms::Padding(0);
			this->textBox82->Name = L"textBox82";
			this->textBox82->Size = System::Drawing::Size(202, 15);
			this->textBox82->TabIndex = 107;
			// 
			// textBox83
			// 
			this->textBox83->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox83->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox83->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox83->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox83->Location = System::Drawing::Point(673, 168);
			this->textBox83->Margin = System::Windows::Forms::Padding(0);
			this->textBox83->Name = L"textBox83";
			this->textBox83->Size = System::Drawing::Size(202, 15);
			this->textBox83->TabIndex = 108;
			// 
			// textBox84
			// 
			this->textBox84->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox84->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox84->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox84->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox84->Location = System::Drawing::Point(875, 168);
			this->textBox84->Margin = System::Windows::Forms::Padding(0);
			this->textBox84->Name = L"textBox84";
			this->textBox84->Size = System::Drawing::Size(202, 15);
			this->textBox84->TabIndex = 109;
			// 
			// textBox85
			// 
			this->textBox85->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox85->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox85->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox85->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox85->Location = System::Drawing::Point(1077, 168);
			this->textBox85->Margin = System::Windows::Forms::Padding(0);
			this->textBox85->Name = L"textBox85";
			this->textBox85->Size = System::Drawing::Size(202, 15);
			this->textBox85->TabIndex = 110;
			// 
			// textBox123
			// 
			this->textBox123->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox123->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox123->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox123->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox123->Location = System::Drawing::Point(0, 72);
			this->textBox123->Margin = System::Windows::Forms::Padding(0);
			this->textBox123->Name = L"textBox123";
			this->textBox123->ReadOnly = true;
			this->textBox123->Size = System::Drawing::Size(67, 15);
			this->textBox123->TabIndex = 70;
			this->textBox123->Text = L"R4";
			// 
			// textBox86
			// 
			this->textBox86->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox86->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox86->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox86->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox86->Location = System::Drawing::Point(1279, 168);
			this->textBox86->Margin = System::Windows::Forms::Padding(0);
			this->textBox86->Name = L"textBox86";
			this->textBox86->Size = System::Drawing::Size(202, 15);
			this->textBox86->TabIndex = 111;
			// 
			// textBox87
			// 
			this->textBox87->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox87->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox87->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox87->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox87->Location = System::Drawing::Point(1481, 168);
			this->textBox87->Margin = System::Windows::Forms::Padding(0);
			this->textBox87->Name = L"textBox87";
			this->textBox87->Size = System::Drawing::Size(210, 15);
			this->textBox87->TabIndex = 112;
			// 
			// textBox129
			// 
			this->textBox129->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox129->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox129->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox129->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox129->Location = System::Drawing::Point(0, 0);
			this->textBox129->Margin = System::Windows::Forms::Padding(0);
			this->textBox129->Name = L"textBox129";
			this->textBox129->ReadOnly = true;
			this->textBox129->Size = System::Drawing::Size(1, 15);
			this->textBox129->TabIndex = 72;
			this->textBox129->Text = L"SHFIT";
			// 
			// textBox96
			// 
			this->textBox96->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox96->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox96->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox96->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox96->Location = System::Drawing::Point(0, 0);
			this->textBox96->Margin = System::Windows::Forms::Padding(0);
			this->textBox96->Name = L"textBox96";
			this->textBox96->Size = System::Drawing::Size(1, 20);
			this->textBox96->TabIndex = 73;
			// 
			// textBox97
			// 
			this->textBox97->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox97->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox97->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox97->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox97->Location = System::Drawing::Point(0, 0);
			this->textBox97->Margin = System::Windows::Forms::Padding(0);
			this->textBox97->Name = L"textBox97";
			this->textBox97->Size = System::Drawing::Size(204, 15);
			this->textBox97->TabIndex = 74;
			// 
			// textBox98
			// 
			this->textBox98->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox98->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox98->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox98->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox98->Location = System::Drawing::Point(204, 0);
			this->textBox98->Margin = System::Windows::Forms::Padding(0);
			this->textBox98->Name = L"textBox98";
			this->textBox98->Size = System::Drawing::Size(199, 15);
			this->textBox98->TabIndex = 75;
			// 
			// textBox99
			// 
			this->textBox99->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox99->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox99->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox99->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox99->Location = System::Drawing::Point(403, 0);
			this->textBox99->Margin = System::Windows::Forms::Padding(0);
			this->textBox99->Name = L"textBox99";
			this->textBox99->Size = System::Drawing::Size(197, 15);
			this->textBox99->TabIndex = 76;
			// 
			// textBox100
			// 
			this->textBox100->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox100->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox100->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox100->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox100->Location = System::Drawing::Point(600, 0);
			this->textBox100->Margin = System::Windows::Forms::Padding(0);
			this->textBox100->Name = L"textBox100";
			this->textBox100->Size = System::Drawing::Size(201, 15);
			this->textBox100->TabIndex = 77;
			// 
			// textBox101
			// 
			this->textBox101->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox101->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox101->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox101->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox101->Location = System::Drawing::Point(801, 0);
			this->textBox101->Margin = System::Windows::Forms::Padding(0);
			this->textBox101->Name = L"textBox101";
			this->textBox101->Size = System::Drawing::Size(204, 15);
			this->textBox101->TabIndex = 78;
			// 
			// textBox102
			// 
			this->textBox102->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox102->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox102->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox102->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox102->Location = System::Drawing::Point(1005, 0);
			this->textBox102->Margin = System::Windows::Forms::Padding(0);
			this->textBox102->Name = L"textBox102";
			this->textBox102->Size = System::Drawing::Size(192, 15);
			this->textBox102->TabIndex = 79;
			// 
			// textBox103
			// 
			this->textBox103->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox103->BackColor = System::Drawing::SystemColors::Menu;
			this->textBox103->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox103->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox103->Location = System::Drawing::Point(1197, 0);
			this->textBox103->Margin = System::Windows::Forms::Padding(0);
			this->textBox103->Name = L"textBox103";
			this->textBox103->Size = System::Drawing::Size(491, 15);
			this->textBox103->TabIndex = 80;
			// 
			// tableLayoutPanel4
			// 
			this->tableLayoutPanel4->BackColor = System::Drawing::SystemColors::Control;
			this->tableLayoutPanel4->ColumnCount = 1;
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel4->Controls->Add(this->tableLayoutPanel2, 0, 1);
			this->tableLayoutPanel4->Controls->Add(this->tableLayoutPanel1, 0, 0);
			this->tableLayoutPanel4->Controls->Add(this->tableLayoutPanel3, 0, 2);
			this->tableLayoutPanel4->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel4->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel4->Margin = System::Windows::Forms::Padding(0);
			this->tableLayoutPanel4->Name = L"tableLayoutPanel4";
			this->tableLayoutPanel4->RowCount = 3;
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 70)));
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 5)));
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25)));
			this->tableLayoutPanel4->Size = System::Drawing::Size(1697, 938);
			this->tableLayoutPanel4->TabIndex = 58;
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(1019, 796);
			this->panel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(200, 100);
			this->panel1->TabIndex = 60;
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->main);
			this->tabControl1->Controls->Add(this->impedance);
			this->tabControl1->Location = System::Drawing::Point(0, 60);
			this->tabControl1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1705, 967);
			this->tabControl1->TabIndex = 61;
			// 
			// main
			// 
			this->main->Controls->Add(this->tableLayoutPanel4);
			this->main->Location = System::Drawing::Point(4, 25);
			this->main->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->main->Name = L"main";
			this->main->Size = System::Drawing::Size(1697, 938);
			this->main->TabIndex = 0;
			this->main->Text = L"ГЛАВНАЯ";
			this->main->UseVisualStyleBackColor = true;
			// 
			// impedance
			// 
			this->impedance->Controls->Add(this->button_signature);
			this->impedance->Controls->Add(this->textBox_signature);
			this->impedance->Controls->Add(this->button_calc_z_to_loop);
			this->impedance->Controls->Add(this->richTextBox1);
			this->impedance->Controls->Add(this->button_write_Z);
			this->impedance->Controls->Add(this->textBox_R_activ);
			this->impedance->Controls->Add(this->button_erase);
			this->impedance->Controls->Add(this->button_read_loop_Z);
			this->impedance->Controls->Add(this->tableLayoutPanel11);
			this->impedance->Location = System::Drawing::Point(4, 25);
			this->impedance->Margin = System::Windows::Forms::Padding(4);
			this->impedance->Name = L"impedance";
			this->impedance->Padding = System::Windows::Forms::Padding(4);
			this->impedance->Size = System::Drawing::Size(1697, 938);
			this->impedance->TabIndex = 6;
			this->impedance->Text = L"ПОВЕРКА_КОЛЬЦА";
			this->impedance->UseVisualStyleBackColor = true;
			// 
			// button_signature
			// 
			this->button_signature->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_signature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_signature->Location = System::Drawing::Point(511, 591);
			this->button_signature->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_signature->Name = L"button_signature";
			this->button_signature->Size = System::Drawing::Size(333, 41);
			this->button_signature->TabIndex = 82;
			this->button_signature->Text = L"ЗАПИСАТЬ  СИГНАТУРУ";
			this->toolTip1->SetToolTip(this->button_signature, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.");
			this->button_signature->UseVisualStyleBackColor = true;
			this->button_signature->Click += gcnew System::EventHandler(this, &MyForm::button_signature_Click);
			// 
			// textBox_signature
			// 
			this->textBox_signature->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_signature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_signature->Location = System::Drawing::Point(344, 597);
			this->textBox_signature->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_signature->Name = L"textBox_signature";
			this->textBox_signature->Size = System::Drawing::Size(133, 27);
			this->textBox_signature->TabIndex = 81;
			this->textBox_signature->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// button_calc_z_to_loop
			// 
			this->button_calc_z_to_loop->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_calc_z_to_loop->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->button_calc_z_to_loop->Location = System::Drawing::Point(1000, 523);
			this->button_calc_z_to_loop->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_calc_z_to_loop->Name = L"button_calc_z_to_loop";
			this->button_calc_z_to_loop->Size = System::Drawing::Size(300, 41);
			this->button_calc_z_to_loop->TabIndex = 80;
			this->button_calc_z_to_loop->Text = L"записать_Z_расчетное";
			this->toolTip1->SetToolTip(this->button_calc_z_to_loop, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.");
			this->button_calc_z_to_loop->UseVisualStyleBackColor = true;
			this->button_calc_z_to_loop->Click += gcnew System::EventHandler(this, &MyForm::button_calc_z_to_loop_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(8, 823);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(1663, 96);
			this->richTextBox1->TabIndex = 79;
			this->richTextBox1->Text = L"";
			this->richTextBox1->Visible = false;
			// 
			// button_write_Z
			// 
			this->button_write_Z->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_write_Z->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_write_Z->Location = System::Drawing::Point(673, 523);
			this->button_write_Z->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_write_Z->Name = L"button_write_Z";
			this->button_write_Z->Size = System::Drawing::Size(132, 41);
			this->button_write_Z->TabIndex = 78;
			this->button_write_Z->Text = L"ЗАПИСАТЬ ";
			this->toolTip1->SetToolTip(this->button_write_Z, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.");
			this->button_write_Z->UseVisualStyleBackColor = true;
			this->button_write_Z->Click += gcnew System::EventHandler(this, &MyForm::button_write_Z_Click);
			// 
			// textBox_R_activ
			// 
			this->textBox_R_activ->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_R_activ->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_R_activ->Location = System::Drawing::Point(344, 529);
			this->textBox_R_activ->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_R_activ->Name = L"textBox_R_activ";
			this->textBox_R_activ->Size = System::Drawing::Size(133, 27);
			this->textBox_R_activ->TabIndex = 77;
			this->textBox_R_activ->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// button_erase
			// 
			this->button_erase->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_erase->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_erase->Location = System::Drawing::Point(811, 523);
			this->button_erase->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_erase->Name = L"button_erase";
			this->button_erase->Size = System::Drawing::Size(157, 41);
			this->button_erase->TabIndex = 76;
			this->button_erase->Text = L"СТЕРЕТЬ";
			this->toolTip1->SetToolTip(this->button_erase, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.");
			this->button_erase->UseVisualStyleBackColor = true;
			this->button_erase->Click += gcnew System::EventHandler(this, &MyForm::button_erase_Click);
			// 
			// button_read_loop_Z
			// 
			this->button_read_loop_Z->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_read_loop_Z->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_read_loop_Z->Location = System::Drawing::Point(511, 523);
			this->button_read_loop_Z->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_read_loop_Z->Name = L"button_read_loop_Z";
			this->button_read_loop_Z->Size = System::Drawing::Size(157, 41);
			this->button_read_loop_Z->TabIndex = 75;
			this->button_read_loop_Z->Text = L"ПРОЧИТАТЬ";
			this->toolTip1->SetToolTip(this->button_read_loop_Z, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.");
			this->button_read_loop_Z->UseVisualStyleBackColor = true;
			this->button_read_loop_Z->Click += gcnew System::EventHandler(this, &MyForm::button_read_loop_Z_Click);
			// 
			// tableLayoutPanel11
			// 
			this->tableLayoutPanel11->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->tableLayoutPanel11->ColumnCount = 9;
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				151)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				147)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				149)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				153)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				145)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				155)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				147)));
			this->tableLayoutPanel11->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				211)));
			this->tableLayoutPanel11->Controls->Add(this->label13, 0, 2);
			this->tableLayoutPanel11->Controls->Add(this->label14, 0, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_1, 1, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_2, 2, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_3, 3, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_4, 4, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_5, 5, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_6, 6, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_7, 7, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L1_8, 8, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_1, 1, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_2, 2, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_3, 3, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_4, 4, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_5, 5, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_6, 6, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_7, 7, 1);
			this->tableLayoutPanel11->Controls->Add(this->textBox_L2_8, 8, 1);
			this->tableLayoutPanel11->Controls->Add(this->label15, 0, 0);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_1, 1, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_2, 2, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_3, 3, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_4, 4, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_5, 5, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_6, 6, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_7, 7, 2);
			this->tableLayoutPanel11->Controls->Add(this->textBox_f_8, 8, 2);
			this->tableLayoutPanel11->Controls->Add(this->label11, 0, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С1, 1, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С2, 2, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С3, 3, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С4, 4, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С5, 5, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С6, 6, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С7, 7, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_С8, 8, 3);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc1, 1, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc2, 2, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc3, 3, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc4, 4, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc5, 5, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc6, 6, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc7, 7, 4);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_calc8, 8, 4);
			this->tableLayoutPanel11->Controls->Add(this->label12, 0, 4);
			this->tableLayoutPanel11->Controls->Add(this->label16, 0, 6);
			this->tableLayoutPanel11->Controls->Add(this->button_Z1, 1, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z2, 2, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z3, 3, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z4, 4, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z5, 5, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z6, 6, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z7, 7, 5);
			this->tableLayoutPanel11->Controls->Add(this->button_Z8, 8, 5);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop1, 1, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop2, 2, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop3, 3, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop4, 4, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop5, 5, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop6, 6, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop7, 7, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop8, 8, 6);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop10, 2, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop9, 1, 7);
			this->tableLayoutPanel11->Controls->Add(this->textBox_Z_loop11, 3, 7);
			this->tableLayoutPanel11->Location = System::Drawing::Point(133, 54);
			this->tableLayoutPanel11->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel11->Name = L"tableLayoutPanel11";
			this->tableLayoutPanel11->RowCount = 8;
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				37)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				37)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				37)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				37)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				44)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				37)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				217)));
			this->tableLayoutPanel11->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
				25)));
			this->tableLayoutPanel11->Size = System::Drawing::Size(1481, 464);
			this->tableLayoutPanel11->TabIndex = 3;
			// 
			// label13
			// 
			this->label13->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label13->Location = System::Drawing::Point(0, 82);
			this->label13->Margin = System::Windows::Forms::Padding(0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(20, 20);
			this->label13->TabIndex = 37;
			this->label13->Text = L"F";
			// 
			// label14
			// 
			this->label14->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label14->Location = System::Drawing::Point(3, 45);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(30, 20);
			this->label14->TabIndex = 36;
			this->label14->Text = L"L2";
			// 
			// textBox_L1_1
			// 
			this->textBox_L1_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_1->Cursor = System::Windows::Forms::Cursors::No;
			this->textBox_L1_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_1->Location = System::Drawing::Point(226, 2);
			this->textBox_L1_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_1->Name = L"textBox_L1_1";
			this->textBox_L1_1->ReadOnly = true;
			this->textBox_L1_1->Size = System::Drawing::Size(145, 27);
			this->textBox_L1_1->TabIndex = 2;
			this->textBox_L1_1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_2
			// 
			this->textBox_L1_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_2->Location = System::Drawing::Point(377, 2);
			this->textBox_L1_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_2->Name = L"textBox_L1_2";
			this->textBox_L1_2->ReadOnly = true;
			this->textBox_L1_2->Size = System::Drawing::Size(141, 27);
			this->textBox_L1_2->TabIndex = 3;
			this->textBox_L1_2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_3
			// 
			this->textBox_L1_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_3->Location = System::Drawing::Point(524, 2);
			this->textBox_L1_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_3->Name = L"textBox_L1_3";
			this->textBox_L1_3->ReadOnly = true;
			this->textBox_L1_3->Size = System::Drawing::Size(143, 27);
			this->textBox_L1_3->TabIndex = 4;
			this->textBox_L1_3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_4
			// 
			this->textBox_L1_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_4->Location = System::Drawing::Point(673, 2);
			this->textBox_L1_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_4->Name = L"textBox_L1_4";
			this->textBox_L1_4->ReadOnly = true;
			this->textBox_L1_4->Size = System::Drawing::Size(147, 27);
			this->textBox_L1_4->TabIndex = 5;
			this->textBox_L1_4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_5
			// 
			this->textBox_L1_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_5->Location = System::Drawing::Point(826, 2);
			this->textBox_L1_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_5->Name = L"textBox_L1_5";
			this->textBox_L1_5->ReadOnly = true;
			this->textBox_L1_5->Size = System::Drawing::Size(139, 27);
			this->textBox_L1_5->TabIndex = 6;
			this->textBox_L1_5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_6
			// 
			this->textBox_L1_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_6->Location = System::Drawing::Point(971, 2);
			this->textBox_L1_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_6->Name = L"textBox_L1_6";
			this->textBox_L1_6->ReadOnly = true;
			this->textBox_L1_6->Size = System::Drawing::Size(149, 27);
			this->textBox_L1_6->TabIndex = 7;
			this->textBox_L1_6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_7
			// 
			this->textBox_L1_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_7->Location = System::Drawing::Point(1126, 2);
			this->textBox_L1_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_7->Name = L"textBox_L1_7";
			this->textBox_L1_7->ReadOnly = true;
			this->textBox_L1_7->Size = System::Drawing::Size(141, 27);
			this->textBox_L1_7->TabIndex = 8;
			this->textBox_L1_7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L1_8
			// 
			this->textBox_L1_8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L1_8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L1_8->Location = System::Drawing::Point(1273, 2);
			this->textBox_L1_8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L1_8->Name = L"textBox_L1_8";
			this->textBox_L1_8->ReadOnly = true;
			this->textBox_L1_8->Size = System::Drawing::Size(205, 27);
			this->textBox_L1_8->TabIndex = 9;
			this->textBox_L1_8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_1
			// 
			this->textBox_L2_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_1->Location = System::Drawing::Point(226, 39);
			this->textBox_L2_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_1->Name = L"textBox_L2_1";
			this->textBox_L2_1->ReadOnly = true;
			this->textBox_L2_1->Size = System::Drawing::Size(145, 27);
			this->textBox_L2_1->TabIndex = 10;
			this->textBox_L2_1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_2
			// 
			this->textBox_L2_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_2->Location = System::Drawing::Point(377, 39);
			this->textBox_L2_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_2->Name = L"textBox_L2_2";
			this->textBox_L2_2->ReadOnly = true;
			this->textBox_L2_2->Size = System::Drawing::Size(141, 27);
			this->textBox_L2_2->TabIndex = 11;
			this->textBox_L2_2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_3
			// 
			this->textBox_L2_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_3->Location = System::Drawing::Point(524, 39);
			this->textBox_L2_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_3->Name = L"textBox_L2_3";
			this->textBox_L2_3->ReadOnly = true;
			this->textBox_L2_3->Size = System::Drawing::Size(143, 27);
			this->textBox_L2_3->TabIndex = 12;
			this->textBox_L2_3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_4
			// 
			this->textBox_L2_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_4->Location = System::Drawing::Point(673, 39);
			this->textBox_L2_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_4->Name = L"textBox_L2_4";
			this->textBox_L2_4->ReadOnly = true;
			this->textBox_L2_4->Size = System::Drawing::Size(147, 27);
			this->textBox_L2_4->TabIndex = 13;
			this->textBox_L2_4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_5
			// 
			this->textBox_L2_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_5->Location = System::Drawing::Point(826, 39);
			this->textBox_L2_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_5->Name = L"textBox_L2_5";
			this->textBox_L2_5->ReadOnly = true;
			this->textBox_L2_5->Size = System::Drawing::Size(139, 27);
			this->textBox_L2_5->TabIndex = 14;
			this->textBox_L2_5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_6
			// 
			this->textBox_L2_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_6->Location = System::Drawing::Point(971, 39);
			this->textBox_L2_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_6->Name = L"textBox_L2_6";
			this->textBox_L2_6->ReadOnly = true;
			this->textBox_L2_6->Size = System::Drawing::Size(149, 27);
			this->textBox_L2_6->TabIndex = 15;
			this->textBox_L2_6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_7
			// 
			this->textBox_L2_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_7->Location = System::Drawing::Point(1126, 39);
			this->textBox_L2_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_7->Name = L"textBox_L2_7";
			this->textBox_L2_7->ReadOnly = true;
			this->textBox_L2_7->Size = System::Drawing::Size(141, 27);
			this->textBox_L2_7->TabIndex = 16;
			this->textBox_L2_7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_L2_8
			// 
			this->textBox_L2_8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_L2_8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_L2_8->Location = System::Drawing::Point(1273, 39);
			this->textBox_L2_8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_L2_8->Name = L"textBox_L2_8";
			this->textBox_L2_8->ReadOnly = true;
			this->textBox_L2_8->Size = System::Drawing::Size(205, 27);
			this->textBox_L2_8->TabIndex = 17;
			this->textBox_L2_8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label15->Location = System::Drawing::Point(3, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(30, 20);
			this->label15->TabIndex = 35;
			this->label15->Text = L"L1";
			// 
			// textBox_f_1
			// 
			this->textBox_f_1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_1->Location = System::Drawing::Point(226, 76);
			this->textBox_f_1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_1->Name = L"textBox_f_1";
			this->textBox_f_1->ReadOnly = true;
			this->textBox_f_1->Size = System::Drawing::Size(145, 27);
			this->textBox_f_1->TabIndex = 39;
			this->textBox_f_1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_2
			// 
			this->textBox_f_2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_2->Location = System::Drawing::Point(377, 76);
			this->textBox_f_2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_2->Name = L"textBox_f_2";
			this->textBox_f_2->ReadOnly = true;
			this->textBox_f_2->Size = System::Drawing::Size(141, 27);
			this->textBox_f_2->TabIndex = 40;
			this->textBox_f_2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_3
			// 
			this->textBox_f_3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_3->Location = System::Drawing::Point(524, 76);
			this->textBox_f_3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_3->Name = L"textBox_f_3";
			this->textBox_f_3->ReadOnly = true;
			this->textBox_f_3->Size = System::Drawing::Size(143, 27);
			this->textBox_f_3->TabIndex = 41;
			this->textBox_f_3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_4
			// 
			this->textBox_f_4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_4->Location = System::Drawing::Point(673, 76);
			this->textBox_f_4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_4->Name = L"textBox_f_4";
			this->textBox_f_4->ReadOnly = true;
			this->textBox_f_4->Size = System::Drawing::Size(147, 27);
			this->textBox_f_4->TabIndex = 42;
			this->textBox_f_4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_5
			// 
			this->textBox_f_5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_5->Location = System::Drawing::Point(826, 76);
			this->textBox_f_5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_5->Name = L"textBox_f_5";
			this->textBox_f_5->ReadOnly = true;
			this->textBox_f_5->Size = System::Drawing::Size(139, 27);
			this->textBox_f_5->TabIndex = 43;
			this->textBox_f_5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_6
			// 
			this->textBox_f_6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_6->Location = System::Drawing::Point(971, 76);
			this->textBox_f_6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_6->Name = L"textBox_f_6";
			this->textBox_f_6->ReadOnly = true;
			this->textBox_f_6->Size = System::Drawing::Size(149, 27);
			this->textBox_f_6->TabIndex = 44;
			this->textBox_f_6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_7
			// 
			this->textBox_f_7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_7->Location = System::Drawing::Point(1126, 76);
			this->textBox_f_7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_7->Name = L"textBox_f_7";
			this->textBox_f_7->ReadOnly = true;
			this->textBox_f_7->Size = System::Drawing::Size(141, 27);
			this->textBox_f_7->TabIndex = 45;
			this->textBox_f_7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_f_8
			// 
			this->textBox_f_8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_f_8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_f_8->Location = System::Drawing::Point(1273, 76);
			this->textBox_f_8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_f_8->Name = L"textBox_f_8";
			this->textBox_f_8->ReadOnly = true;
			this->textBox_f_8->Size = System::Drawing::Size(205, 27);
			this->textBox_f_8->TabIndex = 46;
			this->textBox_f_8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label11
			// 
			this->label11->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label11->Location = System::Drawing::Point(3, 119);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(122, 20);
			this->label11->TabIndex = 47;
			this->label11->Text = L"Емкость пФ";
			// 
			// textBox_С1
			// 
			this->textBox_С1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С1->Location = System::Drawing::Point(226, 113);
			this->textBox_С1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С1->Name = L"textBox_С1";
			this->textBox_С1->ReadOnly = true;
			this->textBox_С1->Size = System::Drawing::Size(145, 27);
			this->textBox_С1->TabIndex = 48;
			this->textBox_С1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С2
			// 
			this->textBox_С2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С2->Location = System::Drawing::Point(377, 113);
			this->textBox_С2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С2->Name = L"textBox_С2";
			this->textBox_С2->ReadOnly = true;
			this->textBox_С2->Size = System::Drawing::Size(141, 27);
			this->textBox_С2->TabIndex = 49;
			this->textBox_С2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С3
			// 
			this->textBox_С3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С3->Location = System::Drawing::Point(524, 113);
			this->textBox_С3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С3->Name = L"textBox_С3";
			this->textBox_С3->ReadOnly = true;
			this->textBox_С3->Size = System::Drawing::Size(143, 27);
			this->textBox_С3->TabIndex = 50;
			this->textBox_С3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С4
			// 
			this->textBox_С4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С4->Location = System::Drawing::Point(673, 113);
			this->textBox_С4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С4->Name = L"textBox_С4";
			this->textBox_С4->ReadOnly = true;
			this->textBox_С4->Size = System::Drawing::Size(147, 27);
			this->textBox_С4->TabIndex = 51;
			this->textBox_С4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С5
			// 
			this->textBox_С5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С5->Location = System::Drawing::Point(826, 113);
			this->textBox_С5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С5->Name = L"textBox_С5";
			this->textBox_С5->ReadOnly = true;
			this->textBox_С5->Size = System::Drawing::Size(139, 27);
			this->textBox_С5->TabIndex = 52;
			this->textBox_С5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С6
			// 
			this->textBox_С6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С6->Location = System::Drawing::Point(971, 113);
			this->textBox_С6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С6->Name = L"textBox_С6";
			this->textBox_С6->ReadOnly = true;
			this->textBox_С6->Size = System::Drawing::Size(149, 27);
			this->textBox_С6->TabIndex = 53;
			this->textBox_С6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С7
			// 
			this->textBox_С7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С7->Location = System::Drawing::Point(1126, 113);
			this->textBox_С7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С7->Name = L"textBox_С7";
			this->textBox_С7->ReadOnly = true;
			this->textBox_С7->Size = System::Drawing::Size(141, 27);
			this->textBox_С7->TabIndex = 54;
			this->textBox_С7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_С8
			// 
			this->textBox_С8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_С8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_С8->Location = System::Drawing::Point(1273, 113);
			this->textBox_С8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_С8->Name = L"textBox_С8";
			this->textBox_С8->ReadOnly = true;
			this->textBox_С8->Size = System::Drawing::Size(205, 27);
			this->textBox_С8->TabIndex = 55;
			this->textBox_С8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc1
			// 
			this->textBox_Z_calc1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc1->Location = System::Drawing::Point(226, 150);
			this->textBox_Z_calc1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc1->Name = L"textBox_Z_calc1";
			this->textBox_Z_calc1->ReadOnly = true;
			this->textBox_Z_calc1->Size = System::Drawing::Size(145, 27);
			this->textBox_Z_calc1->TabIndex = 56;
			this->textBox_Z_calc1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc2
			// 
			this->textBox_Z_calc2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc2->Location = System::Drawing::Point(377, 150);
			this->textBox_Z_calc2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc2->Name = L"textBox_Z_calc2";
			this->textBox_Z_calc2->ReadOnly = true;
			this->textBox_Z_calc2->Size = System::Drawing::Size(141, 27);
			this->textBox_Z_calc2->TabIndex = 57;
			this->textBox_Z_calc2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc3
			// 
			this->textBox_Z_calc3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc3->Location = System::Drawing::Point(524, 150);
			this->textBox_Z_calc3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc3->Name = L"textBox_Z_calc3";
			this->textBox_Z_calc3->ReadOnly = true;
			this->textBox_Z_calc3->Size = System::Drawing::Size(143, 27);
			this->textBox_Z_calc3->TabIndex = 58;
			this->textBox_Z_calc3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc4
			// 
			this->textBox_Z_calc4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc4->Location = System::Drawing::Point(673, 150);
			this->textBox_Z_calc4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc4->Name = L"textBox_Z_calc4";
			this->textBox_Z_calc4->ReadOnly = true;
			this->textBox_Z_calc4->Size = System::Drawing::Size(147, 27);
			this->textBox_Z_calc4->TabIndex = 59;
			this->textBox_Z_calc4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc5
			// 
			this->textBox_Z_calc5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc5->Location = System::Drawing::Point(826, 150);
			this->textBox_Z_calc5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc5->Name = L"textBox_Z_calc5";
			this->textBox_Z_calc5->ReadOnly = true;
			this->textBox_Z_calc5->Size = System::Drawing::Size(139, 27);
			this->textBox_Z_calc5->TabIndex = 60;
			this->textBox_Z_calc5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc6
			// 
			this->textBox_Z_calc6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc6->Location = System::Drawing::Point(971, 150);
			this->textBox_Z_calc6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc6->Name = L"textBox_Z_calc6";
			this->textBox_Z_calc6->ReadOnly = true;
			this->textBox_Z_calc6->Size = System::Drawing::Size(149, 27);
			this->textBox_Z_calc6->TabIndex = 61;
			this->textBox_Z_calc6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc7
			// 
			this->textBox_Z_calc7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc7->Location = System::Drawing::Point(1126, 150);
			this->textBox_Z_calc7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc7->Name = L"textBox_Z_calc7";
			this->textBox_Z_calc7->ReadOnly = true;
			this->textBox_Z_calc7->Size = System::Drawing::Size(141, 27);
			this->textBox_Z_calc7->TabIndex = 62;
			this->textBox_Z_calc7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_calc8
			// 
			this->textBox_Z_calc8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_calc8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_calc8->Location = System::Drawing::Point(1273, 150);
			this->textBox_Z_calc8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_calc8->Name = L"textBox_Z_calc8";
			this->textBox_Z_calc8->ReadOnly = true;
			this->textBox_Z_calc8->Size = System::Drawing::Size(205, 27);
			this->textBox_Z_calc8->TabIndex = 63;
			this->textBox_Z_calc8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label12
			// 
			this->label12->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label12->Location = System::Drawing::Point(3, 148);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(144, 18);
			this->label12->TabIndex = 64;
			this->label12->Text = L"RE  расчетное";
			// 
			// label16
			// 
			this->label16->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label16->Location = System::Drawing::Point(3, 218);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(149, 20);
			this->label16->TabIndex = 65;
			this->label16->Text = L"Re измерянное";
			// 
			// button_Z1
			// 
			this->button_Z1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z1->Location = System::Drawing::Point(268, 174);
			this->button_Z1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z1->Name = L"button_Z1";
			this->button_Z1->Size = System::Drawing::Size(60, 28);
			this->button_Z1->TabIndex = 18;
			this->button_Z1->Text = L"Z1";
			this->toolTip1->SetToolTip(this->button_Z1, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.");
			this->button_Z1->UseVisualStyleBackColor = true;
			this->button_Z1->Click += gcnew System::EventHandler(this, &MyForm::button_Z1_Click);
			// 
			// button_Z2
			// 
			this->button_Z2->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z2->Location = System::Drawing::Point(417, 174);
			this->button_Z2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z2->Name = L"button_Z2";
			this->button_Z2->Size = System::Drawing::Size(60, 28);
			this->button_Z2->TabIndex = 19;
			this->button_Z2->Text = L"Z2";
			this->toolTip1->SetToolTip(this->button_Z2, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n");
			this->button_Z2->UseVisualStyleBackColor = true;
			this->button_Z2->Click += gcnew System::EventHandler(this, &MyForm::button_Z2_Click);
			// 
			// button_Z3
			// 
			this->button_Z3->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z3->Location = System::Drawing::Point(565, 174);
			this->button_Z3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z3->Name = L"button_Z3";
			this->button_Z3->Size = System::Drawing::Size(60, 28);
			this->button_Z3->TabIndex = 20;
			this->button_Z3->Text = L"Z3";
			this->toolTip1->SetToolTip(this->button_Z3, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n");
			this->button_Z3->UseVisualStyleBackColor = true;
			this->button_Z3->Click += gcnew System::EventHandler(this, &MyForm::button_Z3_Click);
			// 
			// button_Z4
			// 
			this->button_Z4->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z4->Location = System::Drawing::Point(716, 172);
			this->button_Z4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z4->Name = L"button_Z4";
			this->button_Z4->Size = System::Drawing::Size(60, 31);
			this->button_Z4->TabIndex = 21;
			this->button_Z4->Text = L"Z4";
			this->toolTip1->SetToolTip(this->button_Z4, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n\r\n");
			this->button_Z4->UseVisualStyleBackColor = true;
			this->button_Z4->Click += gcnew System::EventHandler(this, &MyForm::button_Z4_Click);
			// 
			// button_Z5
			// 
			this->button_Z5->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z5->Location = System::Drawing::Point(865, 171);
			this->button_Z5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z5->Name = L"button_Z5";
			this->button_Z5->Size = System::Drawing::Size(60, 34);
			this->button_Z5->TabIndex = 22;
			this->button_Z5->Text = L"Z5";
			this->toolTip1->SetToolTip(this->button_Z5, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n");
			this->button_Z5->UseVisualStyleBackColor = true;
			this->button_Z5->Click += gcnew System::EventHandler(this, &MyForm::button_Z5_Click);
			// 
			// button_Z6
			// 
			this->button_Z6->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z6->Location = System::Drawing::Point(1015, 171);
			this->button_Z6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z6->Name = L"button_Z6";
			this->button_Z6->Size = System::Drawing::Size(60, 33);
			this->button_Z6->TabIndex = 23;
			this->button_Z6->Text = L"Z6";
			this->toolTip1->SetToolTip(this->button_Z6, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n");
			this->button_Z6->UseVisualStyleBackColor = true;
			this->button_Z6->Click += gcnew System::EventHandler(this, &MyForm::button_Z6_Click);
			this->button_Z6->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			// 
			// button_Z7
			// 
			this->button_Z7->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z7->Location = System::Drawing::Point(1166, 172);
			this->button_Z7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z7->Name = L"button_Z7";
			this->button_Z7->Size = System::Drawing::Size(60, 32);
			this->button_Z7->TabIndex = 24;
			this->button_Z7->Text = L"Z7";
			this->toolTip1->SetToolTip(this->button_Z7, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n");
			this->button_Z7->UseVisualStyleBackColor = true;
			this->button_Z7->Click += gcnew System::EventHandler(this, &MyForm::button_Z7_Click);
			// 
			// button_Z8
			// 
			this->button_Z8->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button_Z8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Z8->Location = System::Drawing::Point(1345, 173);
			this->button_Z8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Z8->Name = L"button_Z8";
			this->button_Z8->Size = System::Drawing::Size(60, 30);
			this->button_Z8->TabIndex = 25;
			this->button_Z8->Text = L"Z8";
			this->toolTip1->SetToolTip(this->button_Z8, L"Нажмите и произведите подстройку одноименнным КПЕ \r\nна плате кольца. Требуется до"
				L"стичь минимума сигнала.\r\n");
			this->button_Z8->UseVisualStyleBackColor = true;
			this->button_Z8->Click += gcnew System::EventHandler(this, &MyForm::button_Z8_Click);
			// 
			// textBox_Z_loop1
			// 
			this->textBox_Z_loop1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop1->Location = System::Drawing::Point(226, 212);
			this->textBox_Z_loop1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop1->Name = L"textBox_Z_loop1";
			this->textBox_Z_loop1->Size = System::Drawing::Size(145, 27);
			this->textBox_Z_loop1->TabIndex = 66;
			this->textBox_Z_loop1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop1_KeyUp);
			// 
			// textBox_Z_loop2
			// 
			this->textBox_Z_loop2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop2->Location = System::Drawing::Point(377, 212);
			this->textBox_Z_loop2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop2->Name = L"textBox_Z_loop2";
			this->textBox_Z_loop2->Size = System::Drawing::Size(141, 27);
			this->textBox_Z_loop2->TabIndex = 67;
			this->textBox_Z_loop2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop2->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop2_KeyUp);
			// 
			// textBox_Z_loop3
			// 
			this->textBox_Z_loop3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop3->Location = System::Drawing::Point(524, 212);
			this->textBox_Z_loop3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop3->Name = L"textBox_Z_loop3";
			this->textBox_Z_loop3->Size = System::Drawing::Size(143, 27);
			this->textBox_Z_loop3->TabIndex = 68;
			this->textBox_Z_loop3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop3->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop3_KeyUp);
			// 
			// textBox_Z_loop4
			// 
			this->textBox_Z_loop4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop4->Location = System::Drawing::Point(673, 212);
			this->textBox_Z_loop4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop4->Name = L"textBox_Z_loop4";
			this->textBox_Z_loop4->Size = System::Drawing::Size(147, 27);
			this->textBox_Z_loop4->TabIndex = 69;
			this->textBox_Z_loop4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop4->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop4_KeyUp);
			// 
			// textBox_Z_loop5
			// 
			this->textBox_Z_loop5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop5->Location = System::Drawing::Point(826, 212);
			this->textBox_Z_loop5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop5->Name = L"textBox_Z_loop5";
			this->textBox_Z_loop5->Size = System::Drawing::Size(139, 27);
			this->textBox_Z_loop5->TabIndex = 70;
			this->textBox_Z_loop5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop5->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop5->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop5_KeyUp);
			// 
			// textBox_Z_loop6
			// 
			this->textBox_Z_loop6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop6->Location = System::Drawing::Point(971, 212);
			this->textBox_Z_loop6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop6->Name = L"textBox_Z_loop6";
			this->textBox_Z_loop6->Size = System::Drawing::Size(149, 27);
			this->textBox_Z_loop6->TabIndex = 71;
			this->textBox_Z_loop6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop6->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop6->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop6_KeyUp);
			// 
			// textBox_Z_loop7
			// 
			this->textBox_Z_loop7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop7->Location = System::Drawing::Point(1126, 212);
			this->textBox_Z_loop7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop7->Name = L"textBox_Z_loop7";
			this->textBox_Z_loop7->Size = System::Drawing::Size(141, 27);
			this->textBox_Z_loop7->TabIndex = 72;
			this->textBox_Z_loop7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop7->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop7->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop7_KeyUp);
			// 
			// textBox_Z_loop8
			// 
			this->textBox_Z_loop8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop8->Location = System::Drawing::Point(1273, 212);
			this->textBox_Z_loop8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop8->Name = L"textBox_Z_loop8";
			this->textBox_Z_loop8->Size = System::Drawing::Size(205, 27);
			this->textBox_Z_loop8->TabIndex = 73;
			this->textBox_Z_loop8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox_Z_loop8->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::digit2);
			this->textBox_Z_loop8->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox_Z_loop8_KeyUp);
			// 
			// textBox_Z_loop10
			// 
			this->textBox_Z_loop10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop10->Location = System::Drawing::Point(377, 249);
			this->textBox_Z_loop10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop10->Name = L"textBox_Z_loop10";
			this->textBox_Z_loop10->Size = System::Drawing::Size(141, 27);
			this->textBox_Z_loop10->TabIndex = 74;
			this->textBox_Z_loop10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_loop9
			// 
			this->textBox_Z_loop9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop9->Location = System::Drawing::Point(226, 249);
			this->textBox_Z_loop9->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop9->Name = L"textBox_Z_loop9";
			this->textBox_Z_loop9->Size = System::Drawing::Size(145, 27);
			this->textBox_Z_loop9->TabIndex = 75;
			this->textBox_Z_loop9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// textBox_Z_loop11
			// 
			this->textBox_Z_loop11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_Z_loop11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox_Z_loop11->Location = System::Drawing::Point(524, 249);
			this->textBox_Z_loop11->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox_Z_loop11->Name = L"textBox_Z_loop11";
			this->textBox_Z_loop11->Size = System::Drawing::Size(143, 27);
			this->textBox_Z_loop11->TabIndex = 76;
			this->textBox_Z_loop11->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->ImageScalingSize = System::Drawing::Size(18, 18);
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// buttonR5
			// 
			this->buttonR5->Location = System::Drawing::Point(675, 6);
			this->buttonR5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR5->Name = L"buttonR5";
			this->buttonR5->Size = System::Drawing::Size(60, 25);
			this->buttonR5->TabIndex = 62;
			this->buttonR5->Text = L"R5";
			this->toolTip1->SetToolTip(this->buttonR5, L"При включенном кольце устанавливает сопротивление R5\r\nна иммитаторе. Подсвечивает"
				L"ся зеленым при внесении в рапорт.\r\n\r\n");
			this->buttonR5->UseVisualStyleBackColor = true;
			this->buttonR5->Click += gcnew System::EventHandler(this, &MyForm::R5_Click);
			// 
			// buttonR6
			// 
			this->buttonR6->Location = System::Drawing::Point(747, 6);
			this->buttonR6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR6->Name = L"buttonR6";
			this->buttonR6->Size = System::Drawing::Size(60, 25);
			this->buttonR6->TabIndex = 63;
			this->buttonR6->Text = L"R6";
			this->toolTip1->SetToolTip(this->buttonR6, L"При включенном кольце устанавливает сопротивление R6 \r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n\r\n");
			this->buttonR6->UseVisualStyleBackColor = true;
			this->buttonR6->Click += gcnew System::EventHandler(this, &MyForm::R6_Click);
			// 
			// buttonR7
			// 
			this->buttonR7->Location = System::Drawing::Point(815, 6);
			this->buttonR7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR7->Name = L"buttonR7";
			this->buttonR7->Size = System::Drawing::Size(60, 25);
			this->buttonR7->TabIndex = 64;
			this->buttonR7->Text = L"R7";
			this->toolTip1->SetToolTip(this->buttonR7, L"При включенном кольце устанавливает сопротивление R7 \r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n\r\n\r\n");
			this->buttonR7->UseVisualStyleBackColor = true;
			this->buttonR7->Click += gcnew System::EventHandler(this, &MyForm::R7_Click);
			// 
			// buttonR8
			// 
			this->buttonR8->Location = System::Drawing::Point(884, 6);
			this->buttonR8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonR8->Name = L"buttonR8";
			this->buttonR8->Size = System::Drawing::Size(60, 25);
			this->buttonR8->TabIndex = 65;
			this->buttonR8->Text = L"R8";
			this->toolTip1->SetToolTip(this->buttonR8, L"При включенном кольце устанавливает сопротивление R8 \r\nна иммитаторе. Подсвечивае"
				L"тся зеленым при внесении в рапорт.\r\n\r\n");
			this->buttonR8->UseVisualStyleBackColor = true;
			this->buttonR8->Click += gcnew System::EventHandler(this, &MyForm::R8_Click);
			// 
			// pictureBoxR5
			// 
			this->pictureBoxR5->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBoxR5->Location = System::Drawing::Point(676, 31);
			this->pictureBoxR5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR5->Name = L"pictureBoxR5";
			this->pictureBoxR5->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR5->TabIndex = 66;
			this->pictureBoxR5->TabStop = false;
			// 
			// pictureBoxR6
			// 
			this->pictureBoxR6->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBoxR6->Location = System::Drawing::Point(747, 31);
			this->pictureBoxR6->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR6->Name = L"pictureBoxR6";
			this->pictureBoxR6->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR6->TabIndex = 67;
			this->pictureBoxR6->TabStop = false;
			// 
			// pictureBoxR7
			// 
			this->pictureBoxR7->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBoxR7->Location = System::Drawing::Point(816, 31);
			this->pictureBoxR7->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR7->Name = L"pictureBoxR7";
			this->pictureBoxR7->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR7->TabIndex = 68;
			this->pictureBoxR7->TabStop = false;
			// 
			// pictureBoxR8
			// 
			this->pictureBoxR8->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBoxR8->Location = System::Drawing::Point(884, 31);
			this->pictureBoxR8->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBoxR8->Name = L"pictureBoxR8";
			this->pictureBoxR8->Size = System::Drawing::Size(60, 7);
			this->pictureBoxR8->TabIndex = 69;
			this->pictureBoxR8->TabStop = false;
			// 
			// buttonLocalMetro
			// 
			this->buttonLocalMetro->Location = System::Drawing::Point(1319, 4);
			this->buttonLocalMetro->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->buttonLocalMetro->Name = L"buttonLocalMetro";
			this->buttonLocalMetro->Size = System::Drawing::Size(123, 49);
			this->buttonLocalMetro->TabIndex = 74;
			this->buttonLocalMetro->Text = L"ЛОКАЛЬНАЯ МЕТРОЛОГИЯ";
			this->toolTip1->SetToolTip(this->buttonLocalMetro, resources->GetString(L"buttonLocalMetro.ToolTip"));
			this->buttonLocalMetro->UseVisualStyleBackColor = true;
			this->buttonLocalMetro->Click += gcnew System::EventHandler(this, &MyForm::buttonLocalMetro_Click);
			// 
			// button_Auto
			// 
			this->button_Auto->Location = System::Drawing::Point(1455, 4);
			this->button_Auto->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Auto->Name = L"button_Auto";
			this->button_Auto->Size = System::Drawing::Size(72, 49);
			this->button_Auto->TabIndex = 77;
			this->button_Auto->Text = L"АВТО";
			this->toolTip1->SetToolTip(this->button_Auto, L"Поочередно переключает сопротивления на кольце\r\n и заносит данные о поверка в рап"
				L"орт.\r\nПо окончании авто поверки можно заново произвести\r\n измерение для любого и"
				L"з сопротивлений.\r\n\r\n");
			this->button_Auto->UseVisualStyleBackColor = true;
			this->button_Auto->Click += gcnew System::EventHandler(this, &MyForm::button_Auto_Click);
			// 
			// button_Clear
			// 
			this->button_Clear->Location = System::Drawing::Point(1713, 4);
			this->button_Clear->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Clear->Name = L"button_Clear";
			this->button_Clear->Size = System::Drawing::Size(72, 48);
			this->button_Clear->TabIndex = 78;
			this->button_Clear->Text = L"CLEAR";
			this->toolTip1->SetToolTip(this->button_Clear, L"Установка связи с иммитатором по Блютус.\r\nПеред нажатием кнопки убедитесь, что ту"
				L"мблер питания\r\nна иммитаторе включен и мигает зеленый светодиод\r\n");
			this->button_Clear->UseVisualStyleBackColor = true;
			this->button_Clear->Visible = false;
			this->button_Clear->Click += gcnew System::EventHandler(this, &MyForm::button_Clear_Click);
			// 
			// button_ON_OFF
			// 
			this->button_ON_OFF->Location = System::Drawing::Point(1068, 4);
			this->button_ON_OFF->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_ON_OFF->Name = L"button_ON_OFF";
			this->button_ON_OFF->Size = System::Drawing::Size(104, 49);
			this->button_ON_OFF->TabIndex = 45;
			this->button_ON_OFF->Text = L"ВКЛЮЧИТЬ ПРИБОР";
			this->toolTip1->SetToolTip(this->button_ON_OFF, L"Включение прибора. Перед нажатием кнопки\r\n подключите прибор ЭМК к компьютеру с п"
				L"омощью\r\nУСИ и включите питание.");
			this->button_ON_OFF->UseVisualStyleBackColor = true;
			this->button_ON_OFF->Click += gcnew System::EventHandler(this, &MyForm::ON_OFF_Click);
			// 
			// helpProvider1
			// 
			this->helpProvider1->HelpNamespace = L"C:\\WORK\\Documents\\отчет\\Модуль синхронизации прибора акустического каротажа.htm";
			// 
			// tableLayoutPanel7
			// 
			this->tableLayoutPanel7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel7->ColumnCount = 9;
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				29.47368F)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				70.52631F)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				204)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				199)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				197)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				201)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				204)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				192)));
			this->tableLayoutPanel7->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				491)));
			this->tableLayoutPanel7->Controls->Add(this->textBox129, 0, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox96, 1, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox97, 2, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox98, 3, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox99, 4, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox100, 5, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox101, 6, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox102, 7, 0);
			this->tableLayoutPanel7->Controls->Add(this->textBox103, 8, 0);
			this->tableLayoutPanel7->Location = System::Drawing::Point(3, 1034);
			this->tableLayoutPanel7->Margin = System::Windows::Forms::Padding(4);
			this->tableLayoutPanel7->Name = L"tableLayoutPanel7";
			this->tableLayoutPanel7->RowCount = 1;
			this->tableLayoutPanel7->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel7->Size = System::Drawing::Size(1688, 31);
			this->tableLayoutPanel7->TabIndex = 72;
			// 
			// checkBox_С
			// 
			this->checkBox_С->AutoSize = true;
			this->checkBox_С->Location = System::Drawing::Point(1729, 58);
			this->checkBox_С->Margin = System::Windows::Forms::Padding(4);
			this->checkBox_С->Name = L"checkBox_С";
			this->checkBox_С->Size = System::Drawing::Size(39, 21);
			this->checkBox_С->TabIndex = 75;
			this->checkBox_С->Text = L"С";
			this->checkBox_С->UseVisualStyleBackColor = true;
			this->checkBox_С->Visible = false;
			// 
			// timer_Temperature
			// 
			this->timer_Temperature->Interval = 1000;
			// 
			// comboBox_sonde_type
			// 
			this->comboBox_sonde_type->FormattingEnabled = true;
			this->comboBox_sonde_type->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"EMK_90", L"LWD_4Tx" });
			this->comboBox_sonde_type->Location = System::Drawing::Point(964, 7);
			this->comboBox_sonde_type->Margin = System::Windows::Forms::Padding(4);
			this->comboBox_sonde_type->Name = L"comboBox_sonde_type";
			this->comboBox_sonde_type->Size = System::Drawing::Size(93, 24);
			this->comboBox_sonde_type->TabIndex = 80;
			// 
			// serialPort_LWD_1DDS
			// 
			this->serialPort_LWD_1DDS->BaudRate = 125000;
			this->serialPort_LWD_1DDS->ReadTimeout = 2000;
			this->serialPort_LWD_1DDS->WriteTimeout = 2000;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(1701, 1040);
			this->Controls->Add(this->comboBox_sonde_type);
			this->Controls->Add(this->button_Clear);
			this->Controls->Add(this->button_Auto);
			this->Controls->Add(this->checkBox_С);
			this->Controls->Add(this->buttonLocalMetro);
			this->Controls->Add(this->tableLayoutPanel7);
			this->Controls->Add(this->pictureBoxR8);
			this->Controls->Add(this->pictureBoxR7);
			this->Controls->Add(this->pictureBoxR6);
			this->Controls->Add(this->pictureBoxR5);
			this->Controls->Add(this->buttonR8);
			this->Controls->Add(this->buttonR7);
			this->Controls->Add(this->buttonR6);
			this->Controls->Add(this->buttonR5);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button_BT_ON);
			this->Controls->Add(this->button_ON_OFF);
			this->Controls->Add(this->pictureBoxR4);
			this->Controls->Add(this->pictureBoxR3);
			this->Controls->Add(this->pictureBoxR2);
			this->Controls->Add(this->pictureBoxR1);
			this->Controls->Add(this->pictureBoxAir);
			this->Controls->Add(this->button_Report);
			this->Controls->Add(this->buttonR4);
			this->Controls->Add(this->buttonR3);
			this->Controls->Add(this->buttonR2);
			this->Controls->Add(this->buttonR1);
			this->Controls->Add(this->buttonR9);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"АМК \"ГОРИЗОНТ\" КАЛИБРОВКА НА КОЛЬЦАХ";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Shown += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResizeBegin += gcnew System::EventHandler(this, &MyForm::MyForm_ResizeBegin);
			this->ResizeEnd += gcnew System::EventHandler(this, &MyForm::MyForm_ResizeEnd);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart8))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxAir))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR4))->EndInit();
			this->tableLayoutPanel2->ResumeLayout(false);
			this->tableLayoutPanel2->PerformLayout();
			this->tableLayoutPanel3->ResumeLayout(false);
			this->tableLayoutPanel3->PerformLayout();
			this->tableLayoutPanel4->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->main->ResumeLayout(false);
			this->impedance->ResumeLayout(false);
			this->impedance->PerformLayout();
			this->tableLayoutPanel11->ResumeLayout(false);
			this->tableLayoutPanel11->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxR8))->EndInit();
			this->tableLayoutPanel7->ResumeLayout(false);
			this->tableLayoutPanel7->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		const char* LWD_Data = "";
		const char* LWD_Metro = "";
		const char* Data = ""; 
		const char* Dataout = "dataout.bin";
	
		int N = 50;
		int N_r = 0;
		int N_LocalMetro = 0;
		int N_R_loop = 9;// номер резистора на кольце
		int N_C_loop = 0;// номер конденсатора на кольце
		bool  Local_metro_is_click = false;
		//bool on_off = false;
		bool blink = 0;
		bool bt_on = false;
		bool dds_on = false;
		bool com_port = 0;
		bool PH_shift_calc = false;
		int wiew=0;
		bool metrology_is_captured_from_sonde = false;
		float Temp;
		int index_curve_1 = 0;// 
		int index_freq_1 = 1;//
		int Co2;
		int R_for_chart10;
		int N_sw = 0;
		bool working = 0;
		int N_aver_buff;
		int n_chart = 0;
		bool close = false;
		bool start = 0; const char* update = "err";
		bool temperature_loop = false;
		bool auto_loop = false;
		unsigned long ftdi_status; bool was_unpluged = 0, was_pluged = 0;
		bool lwd_chart_9_0_cal = 0 ;
		
		//bool mode = 1;


		void do_chart() {
			//8 chart 
			for (int i = 0; i < 8; i++) {
				Chart^ CHART = (Chart^)this->tableLayoutPanel1->Controls["chart" + (i+1).ToString()];
				TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox" + (i + 1).ToString()];
				int K = i;
				extern int norm[10];
				extern int medium[10];
				extern int out_of_norm[10];
				extern int N_aver;
				float computed;
				float to_chart[20], to_chart_aver, to_box_percent;
				String^ sine; 
				String^ mean;
				String^ frmt;
				CHART->ChartAreas["ChartArea1"]->AxisX->LabelStyle->Enabled = true;
				CHART->ChartAreas["ChartArea1"]->AxisX->LabelStyle->Format="F0";

				for (int i = 0; i < 8; i++) {
					CHART->Series[i]->Points->Clear();
				}
				TEXTBOX->Text = "";


				if (wiew == 0) {//Ro_wiew
					mean = " Om -> ";
					frmt = "F2";
                    computed = Ro_adecvat_computed[N_R_loop].VALUE[i];
					to_chart_aver = Ro_aver->VALUE[i];
					for (int n = 0; n < N_aver; n++) {
						to_chart[n] = Ro_chart[n].VALUE[i];
					}
				}
				if (wiew == 1) {//PH_wiew
					mean = " mG -> ";
					frmt = "F0";
					computed = Phase_loop_computed[N_R_loop].VALUE[i] * mG;
					to_chart_aver = Phase_aver->VALUE[i] * mG;
					for (int n = 0; n < N_aver; n++) {
						to_chart[n] = Phase_chart[n].VALUE[i] * mG;
					}
				}
				if (wiew == 3) {//PH_RAW_wiew
					mean = " mG -> ";
					frmt = "F0";
					computed = 0;
					to_chart_aver = Phase_raw_aver->VALUE[i] * mG;
					for (int n = 0; n < N_aver; n++) {
						to_chart[n] = Phase_raw[n].VALUE[i] * mG;
					}
				}


				for (int n = 0; n < N_aver; n++) {
					
					CHART->Series[0]->Points->AddXY(to_chart[n], n);
					CHART->Series[7]->Points->AddXY(to_chart_aver, n);

					//если есть связь с кольцом или кольцо переключается вручную
					
					if (bt_on == true ) {
						if (N_R_loop != 8) {//не работает на воздухе а на сопротивлении не работает этот чарт
							//- т е вообще никогда не работает
							CHART->Series[2]->Points->AddXY(computed + 0.01*norm[N_R_loop] * computed, n);
							CHART->Series[4]->Points->AddXY(computed + 0.01*medium[N_R_loop] * computed, n);
							CHART->Series[6]->Points->AddXY(computed + 0.01*out_of_norm[N_R_loop] * computed, n);

							CHART->Series[1]->Points->AddXY(computed - 0.01*norm[N_R_loop] * computed, n);
							CHART->Series[3]->Points->AddXY(computed - 0.01*medium[N_R_loop] * computed, n);
							CHART->Series[5]->Points->AddXY(computed - 0.01*out_of_norm[N_R_loop] * computed, n);

							to_box_percent = (100 * (to_chart_aver - computed) / (computed + 0.0000000000001));

							if (to_box_percent > 200)to_box_percent = 200;
							if (to_box_percent < -200)to_box_percent = -200;
							if (to_box_percent > 0) sine = "+"; else sine = "-";
							if (fabs(to_box_percent) <= norm[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::LimeGreen;
							if (fabs(to_box_percent) > norm[N_R_loop] && fabs(to_box_percent) <= medium[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::Gold;
							if (fabs(to_box_percent) > medium[N_R_loop] && fabs(to_box_percent) <= out_of_norm[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::Red;
							if (fabs(to_box_percent) > out_of_norm[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::DarkRed;
						}
					}
					if (N_R_loop != 8 && bt_on == true)TEXTBOX->Text = (to_chart_aver).ToString(frmt) + mean + sine + to_box_percent.ToString("F1") + " %";
					else TEXTBOX->Text = (to_chart_aver).ToString(frmt) + mean;

					///////////////////////////////////	

				}
			
			}
	
		}
		
		void do_chart_(int i) {

			Chart^ CHART = (Chart^)this->tableLayoutPanel1->Controls["chart" + (i + 1).ToString()];
			TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox" + (i + 1).ToString()];
			int K = i;
			extern int N_aver;
			extern int norm[10];
			extern int medium[10];
			extern int out_of_norm[10];
			float computed;
			float to_chart[20], to_chart_aver, to_box_percent;
			String^ sine;
			String^ mean;
			String^ frmt;

			CHART->ChartAreas["ChartArea1"]->AxisX->LabelStyle->Enabled = true;
			
			for (int i = 0; i < 8; i++) {
				CHART->Series[i]->Points->Clear();
			}
			TEXTBOX->Text = "";

			Ro_for_print[N_R_loop].VALUE[K] = Ro_aver->VALUE[K];
			Phase_for_print[N_R_loop].VALUE[K] = Phase_aver->VALUE[K] * mG;
			Ro_percent_for_print[N_R_loop].VALUE[K] = (100 * (Ro_aver->VALUE[K] - Ro_adecvat_computed[N_R_loop].VALUE[K]) / (Ro_adecvat_computed[N_R_loop].VALUE[K] + 0.0000000000001));
			Phase_percent_for_print[N_R_loop].VALUE[K] = (100 * (Phase_aver->VALUE[K] - Phase_loop_computed[N_R_loop].VALUE[i]) / Phase_loop_computed[N_R_loop].VALUE[K]);


			if (wiew == 0) {//Ro_wiew
				mean = " Om -> ";
				frmt = "F2";
				computed = Ro_adecvat_computed[N_R_loop].VALUE[K];
				to_chart_aver = Ro_aver->VALUE[K];
				to_box_percent = Ro_percent_for_print[N_R_loop].VALUE[K];
				for (int n = 0; n < N_aver; n++) {
					to_chart[n] = Ro_chart[n].VALUE[K];
				}
			}
			if (wiew == 1) {//PH_wiew
				//TEXTBOX->ForeColor = System::Drawing::Color::Black;
				mean = " mG -> ";
				frmt = "F0";
				computed = Phase_loop_computed[N_R_loop].VALUE[K] * mG;
				to_chart_aver = Phase_aver->VALUE[K] * mG;
				to_box_percent = Phase_percent_for_print[N_R_loop].VALUE[K];
				for (int n = 0; n < N_aver; n++) {
					to_chart[n] = Phase_chart[n].VALUE[K] * mG;
				}
			}
			CHART->ChartAreas["ChartArea1"]->AxisX->LabelStyle->Format = frmt;

			for (int n = 0; n < N_aver; n++) {
				if (to_chart[n]<180000 && to_chart[n] > -180000) {
					CHART->Series[0]->Points->AddXY(to_chart[n], n);
				}
				if (to_chart_aver < 180000 && to_chart_aver > -180000) {
					CHART->Series[7]->Points->AddXY(to_chart_aver, n);
				}
				

				//если есть связь с кольцом 
				if (bt_on == true ) {
					if (N_R_loop != 8) {
						CHART->Series[2]->Points->AddXY(computed + 0.01*norm[N_R_loop] * computed, n);
						CHART->Series[4]->Points->AddXY(computed + 0.01*medium[N_R_loop] * computed, n);
						CHART->Series[6]->Points->AddXY(computed + 0.01*out_of_norm[N_R_loop] * computed, n);

						CHART->Series[1]->Points->AddXY(computed - 0.01*norm[N_R_loop] * computed, n);
						CHART->Series[3]->Points->AddXY(computed - 0.01*medium[N_R_loop] * computed, n);
						CHART->Series[5]->Points->AddXY(computed - 0.01*out_of_norm[N_R_loop] * computed, n);

						if (to_box_percent > 200)to_box_percent= 200;
						if (to_box_percent < -200)to_box_percent = -200;
						if (to_box_percent > 0) sine = "+"; else sine = "-";
						if (fabs(to_box_percent) <= norm[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::LimeGreen;
						if (fabs(to_box_percent) > norm[N_R_loop] && fabs(to_box_percent) <= medium[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::Gold;
						if (fabs(to_box_percent) > medium[N_R_loop] && fabs(to_box_percent) <= out_of_norm[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::Red;
						if (fabs(to_box_percent) > out_of_norm[N_R_loop]) TEXTBOX->ForeColor = System::Drawing::Color::DarkRed;
					}
				}
				
				if (N_R_loop != 8 && bt_on == true) {
					TEXTBOX->Text = (to_chart_aver).ToString(frmt) + mean + sine + to_box_percent.ToString("F1") + " %";
					//TEXTBOX->Text = (to_chart_aver).ToString(frmt) + " " + computed.ToString(frmt);
					//Ro_for_print[N_R_loop].VALUE[K] = Ro_aver->VALUE[K];
					//Phase_for_print[N_R_loop].VALUE[K] = Phase_aver->VALUE[K];
					//Ro_for_print[N_R_loop].VALUE[i] = to_chart_aver;
				}
				else TEXTBOX->Text = (to_chart_aver).ToString(frmt) + mean;
				///////////////////////////////////	

			}

		}

		int n_for_c_switch;

		public:  System::Void  Read_Port() {
			C_switch(n_for_c_switch);
		}


		void C_switch(int n) {

			if (bt_on == true) {
				if (working == true) this->timer1->Enabled = false;
				
				N_C_loop = n-1;
				int  c = 0;
				
				if (n == 1)c = 1; if (n == 2)c = 2; if (n == 3)c = 3; if (n == 4)c = 4;
				if (n == 5)c = 5; if (n == 6)c = 6; if (n == 7)c = 7; if (n == 8)c = 8;

				//if (n == 1)c = 1; if (n == 2)c = 2; if (n == 3)c = 3; if (n == 4)c = 4;
				//if (n == 5)c = 6; if (n == 6)c = 7; if (n == 7)c = 9; if (n == 8)c = 9;

				String^ resive = ""; String^ resive1 = "";
				String^ send_R = ""; String^ send_C = "";
				String^ resive_R = ""; String^ resive_C = "";
				
				send_R = "RES " + (N_R_loop + 1) + "\r\n"; resive_R = "RES=" + (N_R_loop + 1)+" OK";
				send_C = "CAP " + (c) + "\r\n"; resive_C = "CAP=" + (c) + " OK";
				uint32_t Try = 0;
				uint32_t started = 0;

						
					int stop = 0, stop_ = 0;
					Sleep(50);
					serialPort->DiscardInBuffer();
					try {
						Console::WriteLine("send_R  " + send_R);
						Console::WriteLine("send_C " + send_C);
						serialPort->Write(send_R);
						Sleep(50);
						serialPort->Write(send_C);
						Sleep(150);
					}
					catch (...) { stop_ = 1; }
					if (stop_) {
						//bt_on = false;
						this->button_BT_ON->BackColor = DefaultBackColor;
						//MessageBox::Show("Соединение с  кольцом прервано переподключите BT_ON");
					}

					try {						
						resive = serialPort->ReadLine();
						resive1 = serialPort->ReadLine();
					}
					catch (...) { stop = 1; }
					if (!stop) {
						Console::WriteLine("resive " + resive);
						Console::WriteLine("resive1 " + resive1);
						if (resive->StartsWith(resive_R) && resive1->StartsWith(resive_C)) {
							started = true;
							resive = ""; resive1 = "";
							if (working == true) this->timer1->Enabled = true;							
						}
						else { 							
							bt_on = false;
							this->button_BT_ON->BackColor = DefaultBackColor;
							//MessageBox::Show("Нет ответа от кольца , нажмите С5 еще раз !");
														 
						}
					}
					else {
						
						bt_on = false;
						this->button_BT_ON->BackColor = DefaultBackColor;						
						MessageBox::Show("Нет ответа от кольца , нажмите С5 еще раз !");
						
					}
					if (working == true) this->timer1->Enabled = true;									
			}
			else { MessageBox::Show("Установите соединение с кольцом BT_ON или перейдите в ручной режим "); }
		}

		void R_switch(int n) {
			extern bool Ro_to_raport;
			extern bool PH_to_raport;
			extern  double R_loop_physical[9];
			int button = n;
			
			if (bt_on == true ) {
				if (button != 9) {
					//if (Ro_to_raport == true && PH_to_raport == false) { comboBox1->SelectedIndex = 0; }
					//if (Ro_to_raport == false && PH_to_raport == true) { comboBox1->SelectedIndex = 1; }
					//if (Ro_to_raport == true && PH_to_raport == true) { comboBox1->SelectedIndex = 0; }
				}
				
				
				N_R_loop = n - 1;
				N_r = 0;
				n_chart = 0;

				for (int i = 1; i < 10; i++) {
					Button^ BUTTON = (Button^)this->Controls["buttonR" + (i).ToString()];
					if (i != button) {
						if (i != 9) { BUTTON->BackColor = DefaultBackColor; BUTTON->Text = "R" + i; }
						else { BUTTON->BackColor = DefaultBackColor; BUTTON->Text = "AIR"; }
					}
					else {
						if (i != 9) { BUTTON->BackColor = Color::Red; BUTTON->Text = R_loop_physical[button - 1].ToString(); }
						else { BUTTON->BackColor = Color::Red; BUTTON->Text = "AIR"; }
					}
				}

				for (int i = 0; i < 8; i++) {
					Chart^ CHART = (Chart^)this->tableLayoutPanel1->Controls["chart" + (i + 1).ToString()];
					TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox" + (i + 1).ToString()];

					for (int j = 0; j < 8; j++) {
						CHART->Series[j]->Points->Clear();
						TEXTBOX->Text = "";
					}
				}
			}
			else { MessageBox::Show("Установите соединение с кольцом "); }
		}
		
		void CLEAR(int i) {
			int n = i - 1;
			if(n < 8){
				PictureBox^ PICTUREBOX = (PictureBox^)this->Controls["pictureBoxR" + (n+1).ToString()];
				PICTUREBOX->BackColor = DefaultBackColor; green[n] = false;
				for (int i = 24 + 8 * n; i < 32 + 8 * n; i++) {
					TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel3->Controls["textBox" + (i).ToString()];
					TEXTBOX->Text = "";
				}
			}
			else {
				this->pictureBoxAir->BackColor = DefaultBackColor; green[n] = false;
			}
			

			Ro_for_print[n] = { 0.0, };
			Phase_for_print[n] = { 0.0, };
			Ro_percent_for_print[n] = { 0.0, };
			Phase_percent_for_print[n] = { 0.0, };


		}

////////////////////////////////////////////////////////////////////////////////////////////
			
private: System::Void BT_ON(System::Object^  sender, System::EventArgs^  e) {
	comboBox1->SelectedIndex = 1;
	//tabControl1->SelectedIndex = 0;
	int stop = 0, stop_ = 0;
	extern string Com_Name;
	String^ Str = "";
	Byte send = 0;
	Byte resive = 0;
	//Com_Name = "COM5";
	this->button_BT_ON->BackColor = DefaultBackColor;
	if (Com_Name == "" || Com_Name == "COM_PORT") { MessageBox::Show("Установите Параметры -> COM_PORT !"); }
	else{

		//if (working == true) this->timer1->Enabled = false;
		serialPort->Close();
		Sleep(1000);
	    serialPort->PortName = gcnew String(Com_Name.c_str());
		serialPort->BaudRate = 9600;
		serialPort->ReadTimeout = 1000;
		Console::WriteLine(serialPort->PortName);
			
		while (permit == false) { ; }

			try { serialPort->Open(); }
			catch (...) { stop_ = 1; }
		    if (!stop_) {
				serialPort->DiscardInBuffer();
				serialPort->BaseStream->WriteByte(send);
			    Sleep(200);
				try { resive = serialPort->BaseStream->ReadByte(); }
				catch (...) { stop = 1; }
				if (!stop) {
					//Console::WriteLine("resive " + resive);
					if (resive == send) {
						loop_version = 1;
						bt_on = true;
						this->button_BT_ON->BackColor = Color::DodgerBlue;
						buttonR9->PerformClick();
						//if (working == true) this->timer1->Enabled = true;						
						MessageBox::Show("Соединение установлено!\r\n кольцо версия 1");
					}
					else { serialPort->Close(); MessageBox::Show("Не удается установить соединение !"); }
				}
				else {
					stop = 0;
					serialPort->DiscardInBuffer();
					serialPort->Write("$\r\n");
					try { Str = serialPort->ReadLine(); }
					catch (...) { stop = 1; }
					if (!stop) {
						if (Str->StartsWith("Unknown")) {
							loop_version = 2;
							bt_on = true;
							this->button_BT_ON->BackColor = Color::DodgerBlue;
							buttonR9->PerformClick();
							//if (working == true) this->timer1->Enabled = true;
							//MessageBox::Show("Соединение установлено!\r\n кольцо версия 2\r\n требуется пройти автоповерку кольца");
							//tabControl1->SelectedIndex = 2;
																	
						}
						else { serialPort->Close(); MessageBox::Show("Не удается установить соединение !"); }
					}
					else { 
						serialPort->Close(); MessageBox::Show("Нет ответа от кольца !"); 
					}
				}
		    }
		    else MessageBox::Show("Не удается открыть порт кольца!");
			if (working == true) this->timer1->Enabled = true;		
	}

}
 
private: System::Void ON_OFF_Click(System::Object^  sender, System::EventArgs^  e) {
	extern bool LWD_1DDS ;
	extern bool LocalMetro_working ;
	extern bool metrology_is_set;
	extern bool sonde_ro_calk;
	extern bool mode;
	const char* Metrology = "metrology_from_sonde_tmp.bin";
	extern METROLOGY metrology_struct;
	extern bool param_form_closed;
	
	tabControl1->SelectedIndex = 0;
	extern bool sonde_on_off;
	unsigned long AMK_result = 0;
	extern double R_loop_physical[9];
	extern double loop_radius;
	extern bool express;
	extern bool Z_downloaded_from_loop;
	extern string Com_LWD_Name;
	
	
	if (comboBox_sonde_type->SelectedIndex == 0) {
		//AMK_DLL = LoadLibrary(TEXT("DllProject.dll"));// Загружаем библиотеку EMK_90
		AMK_DLL = LoadLibrary(TEXT("AMK_1DDS_AUTONOM.dll"));// Загружаем библиотеку EMK_90_1dds
		
		LWD_1DDS = false;
	}
	
	else if (comboBox_sonde_type->SelectedIndex == 1) {
		LWD_1DDS = true;
	}

	if (LWD_1DDS == false) {
		if (sonde_on_off == 0) {
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
			AMK_CheckConnect = (CheckConnect)GetProcAddress(AMK_DLL, "AMK_CheckConnect");
			AMK_StopDDS3 = (StopDDS3)GetProcAddress(AMK_DLL, "AMK_StopDDS3");
			AMK_ResumeDDS3 = (ResumeDDS3)GetProcAddress(AMK_DLL, "AMK_ResumeDDS3");
			AMK_SetIQPhaseOffset = (SetIQPhaseOffset)GetProcAddress(AMK_DLL, "AMK_SetIQPhaseOffset");
			AMK_DDS3FreqOffset = (DDS3FreqOffset)GetProcAddress(AMK_DLL, "AMK_DDS3FreqOffset");
			AMK_GetSerial = (GetSerial)GetProcAddress(AMK_DLL, "AMK_GetSerial");
			AMK_GetMetroData = (GetMetroData)GetProcAddress(AMK_DLL, "AMK_GetMetroData");
			AMK_SetMetroData = (SetMetroData)GetProcAddress(AMK_DLL, "AMK_SetMetroData");

			if (AMK_DLL == NULL)MessageBox::Show("Положите файл 'DllProject.dll' в каталог программы!");
			if (!AMK_StartComPort || !AMK_GetCurrState || !AMK_Init || !AMK_BeginCyclogram || !AMK_GetData
				|| !AMK_SetIdle || !AMK_PwrOn || !AMK_PwrOff || !AMK_DeleteComPort || !AMK_CheckConnect
				|| !AMK_GetSerial || !AMK_GetMetroData || !AMK_SetMetroData)
				MessageBox::Show("Файл 'DllProject.dll' испорчен !");

			Sleep(2000);
			ComPort = AMK_StartComPort(&ComPort, NULL);
			cout << " ComPort " << ComPort << endl;
			if (ComPort != NULL) {
				//cout << "idle " << AMK_SetIdle(ComPort) << endl;
				if (AMK_PwrOn(ComPort)) {
					Sleep(2000);
					if (comboBox_sonde_type->SelectedIndex == 1) {
						while (AMK_GetCurrState(ComPort) != 32 && AMK_GetCurrState(ComPort) != 1) {
							Sleep(500);
						}
						//cout << "idle " << AMK_SetIdle(ComPort) << endl;
					}
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
						metrology_is_set = true;
						//переключаемся на просмотр фазы
						//wiew = 0;
						//comboBox1->SelectedIndex = 3;
					
						if ((metrology_struct.serial & (1 << 31)) != 0) {
							LocalMetro_working = true;
							this->buttonLocalMetro->BackColor = Color::Red;
							//cout << "local " << endl;
						}
						if ((metrology_struct.serial & (1 << 31)) == 0) {

							ofstream metro_out;
							metro_out.open(Metrology, ios::binary);
							metro_out.write((char*)&metrology_struct, sizeof(metrology_struct));
							metro_out.close();
							LocalMetro_working = false;
							this->buttonLocalMetro->BackColor = DefaultBackColor;
						}

						metrology_is_captured_from_sonde = true;
						// заполняем из файла метрологии
						for (int i = 0; i < 8; i++) {
							L1[i] = metrology_struct.L1[i] / 1000.0 ;
							L2[i] = metrology_struct.L2[i] / 1000.0;
							f_[i] = metrology_struct.F[i] * 1000.0;							
						}

						if (mode == 1 && Z_downloaded_from_loop == 0) {
							MessageBox::Show("Будет Установлена связь с кольцом и скачаны импедансы");
							button_BT_ON->PerformClick();
							tabControl1->SelectedIndex = 1;
							button_read_loop_Z->PerformClick();
							tabControl1->SelectedIndex = 0;
							
							uint32_t buff;
							uint32_t signature_for_pallete;
							cout << "sign " << std::hex << metrology_struct.signature << endl;
							buff = metrology_struct.signature << 12;	buff = buff >> 12;
							signature_for_pallete = buff / 1000;
							if (int(Re_from_loop[9]) != signature_for_pallete) {
								MessageBox::Show("подключите кольцо, соответствующее типу прибора \r\n программа будет закрыта");
								exit(0);
							}
							else
								Z_downloaded_from_loop = 1;
						}


						/////////////////////////////////////////////////////////////////////////////////////////////////					
					    //расчетные значения емкостей и резонансных частот для кольца
						for (int i = 0; i < 8; i++) {
							F_rez[i] = (int(F(f_[i], 0.5, L_loop, L1[i], L2[i])));														
							C[i] = (C_max_fi(f_[i], 0.5, L_loop, L1[i], L2[i]));
							//cout << "C" << i << " " << C[i] << endl;
							
							TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (i + 1).ToString()];
							TEXTBOX_L1->Text = metrology_struct.L1[i].ToString();
							TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (i + 1).ToString()];
							TEXTBOX_L2->Text = metrology_struct.L2[i].ToString();
							TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (i + 1).ToString()];
							TEXTBOX_F->Text = metrology_struct.F[i].ToString();
							TextBox^ TEXTBOX_C = (TextBox^)this->tableLayoutPanel11->Controls["textBox_С" + (i + 1).ToString()];
							TEXTBOX_C->Text = ((int)(1e12*C[i])).ToString("F0");
							TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];						
							TEXTBOX_Z_CALC->Text = (imag(Z(1.0, L_loop, C[i], f_[i]))).ToString("F3");

						}
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////

						if (express == false) {
							for (int r = 0; r < 8; r++) {
								for (int c = 0; c < 8; c++) {									
									Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[c], loop_radius, mode).VALUE[c];
								}
								Ro_adecvat_computed[r] = R0_adecvat_r_loop(&metrology_struct, &Phase_loop_computed[r]);
							}
							for (int r = 0; r < 8; r++) {
								for (int c = 0; c < 8; c++) {
								}
							}
						}

						if (express == true) {
							for (int r = 0; r < 8; r++) {
								if (r == 0 || r == 1) {
									for (int c = 0; c < 8; c++) {
										Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[c], loop_radius, mode).VALUE[c];
									}
									Ro_adecvat_computed[r] = R0_adecvat_r_loop(&metrology_struct, &Phase_loop_computed[r]);
								}
								else {
									for (int c = 0; c < 8; c++) {
										if (c < 4) Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[3], loop_radius, mode).VALUE[c];
										else       Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[7], loop_radius, mode).VALUE[c];
									}
									Ro_adecvat_computed[r] = R0_adecvat_r_loop(&metrology_struct, &Phase_loop_computed[r]);
								}
							}
						}

						/////////////////////////////////////////////////////////////////////////////////////////////////////
						Sleep(500);
						if (AMK_BeginCyclogram(ComPort)) {
							Sleep(500);
							cout << "Start !!!" << endl;
							sonde_on_off = 1;
							working = true;
							
							//////////////////////////////////////////////////////////
							this->timer1->Enabled = true;
							this->button_ON_OFF->Text = "ПРИБОР ВКЛЮЧЕН";
							this->comboBox_sonde_type->DropDownStyle = System::Windows::Forms::ComboBoxStyle::Simple;
						}
						else { AMK_PwrOff(ComPort); AMK_DeleteComPort(ComPort); FreeLibrary(AMK_DLL);  MessageBox::Show(" прибор не стартовал, попробуйте еще раз!"); }
					}
					else { AMK_PwrOff(ComPort); AMK_DeleteComPort(ComPort); FreeLibrary(AMK_DLL);  MessageBox::Show(" прибор неиницилизирован, попробуйте еще раз!"); }
				}
				else { AMK_DeleteComPort(ComPort);  FreeLibrary(AMK_DLL);  MessageBox::Show("Проверьте правильность подключения прибора!"); }

			}
			else { FreeLibrary(AMK_DLL);   MessageBox::Show("Подключите УСИ!"); }
		}

		else {
			fout->close();
			sonde_on_off = 0;
			working = false;			
			this->timer1->Enabled = false;
			this->button_ON_OFF->Text = "ВКЛЮЧИТЬ ПРИБОР";
			this->comboBox_sonde_type->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDown;
			this->button_ON_OFF->BackColor = DefaultBackColor;
			if (ComPort != NULL) {
				//cout << " 4 " << endl;
				if (AMK_SetIdle(ComPort)) {
					Sleep(1000);
					if (AMK_PwrOff(ComPort)) {
						Sleep(1000);
						AMK_DeleteComPort(ComPort);
						//cout << "Stop !!!" << endl;
					}
					else {
						AMK_DeleteComPort(ComPort);
						//cout << " !off " << endl; 
					}
				}
				else {
					AMK_DeleteComPort(ComPort);
					//cout << " !idle " << endl;
				}
			}
			else {
				FreeLibrary(AMK_DLL);
				//cout << "AMK_DLL" << AMK_DLL << endl; 
			}
		}
	}

	if (LWD_1DDS == true) {
		//_controlfp(_CW_DEFAULT, 0xfffff);
		int stop = 0, stop_ = 0;
		if (sonde_on_off == 0) {

			bool stop = false;
			try {
				serialPort_LWD_1DDS->PortName = gcnew String(Com_LWD_Name.c_str());
				Sleep(1000);
				serialPort_LWD_1DDS->ReadTimeout = 1000;
				serialPort_LWD_1DDS->WriteTimeout = 1000;
				serialPort_LWD_1DDS->BaudRate = 125000;
				serialPort_LWD_1DDS->Open();
			}
			catch (...) { stop = 1; }
			if (!stop) {		
				uint8_t DATA[3];	
				uint8_t CMD; uint8_t DATA_IN[256] = {0,};
				DATA[0] = 0x00; DATA[1] = 0x04; DATA[2] = 240;
				////////////прочистка////////////////////////////////////
				const uint8_t length = 252;
				uint8_t *DATA_ = new uint8_t;
				*DATA_ = length;
				send_to_tool(serialPort_LWD_1DDS, CMD_WORK, DATA_, 1);
				Sleep(2000);
				/////////////////////////////////////////////////////////
				send_to_tool(serialPort_LWD_1DDS, CMD_READ_EE, DATA, 3);
				Sleep(200);
				receive_from_tool(serialPort_LWD_1DDS, CMD, DATA_IN, 240);
				memcpy(&metrology_struct, DATA_IN, 256);	
				
				cout << std::hex << CMD << " " << metrology_struct.signature << endl;
				cout << std::hex << " " << metrology_struct.serial << endl;
				cout << std::dec;
				for (int i = 0; i < 8; i++) {
					cout << i << " L1= " << metrology_struct.L1[i] << " L2= " << metrology_struct.L2[i] << endl;
				}
				for (int i = 0; i < 8; i++) {
					cout << i << " F= " << metrology_struct.F[i] << endl;
				}
				for (int i = 0; i < 8; i++) {
					cout << i << " AIR= " << metrology_struct.Air_zz[i] << endl;
				}
					
			serialPort_LWD_1DDS->Close();				
				
				
			}
			else {
				stop = 0;
				Console::WriteLine("no port ");
				serialPort_LWD_1DDS->Close();
			}

			//файл метрологии выгружен
            metrology_is_set = true;		
			metrology_is_captured_from_sonde = true;
			// заполняем из файла метрологии
			for(int i = 0; i < 8; i++) {
				L1[i] = metrology_struct.L1[i] / 1000.0;
				L2[i] = metrology_struct.L2[i] / 1000.0;
				f_[i] = metrology_struct.F[i] * 1000.0;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////					
			//расчетные значения емкостей и резонансных частот для кольца
			for (int i = 0; i < 8; i++) {				        
				F_rez[i] = (int(F(f_[i], 0.5, L_loop, L1[i], L2[i])));
				C[i] = (C_max_fi(f_[i], 0.5, L_loop, L1[i], L2[i]));
				//cout << "C" << i << " " << C[i] << endl;
				TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (i + 1).ToString()];
				TEXTBOX_L1->Text = metrology_struct.L1[i].ToString();
				TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (i + 1).ToString()];
				TEXTBOX_L2->Text = metrology_struct.L2[i].ToString();
				TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (i + 1).ToString()];
				TEXTBOX_F->Text = metrology_struct.F[i].ToString();
				TextBox^ TEXTBOX_C = (TextBox^)this->tableLayoutPanel11->Controls["textBox_С" + (i + 1).ToString()];
				TEXTBOX_C->Text = ((int)(1e12*C[i])).ToString("F0");
				TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];
				TEXTBOX_Z_CALC->Text = (imag(Z(1.0, L_loop, C[i], f_[i]))).ToString("F3");
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (express == false) {
				for (int r = 0; r < 8; r++) {
					for (int c = 0; c < 8; c++) {
						Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[c], loop_radius, mode).VALUE[c];
					}
					Ro_adecvat_computed[r] = R0_adecvat_r_loop(&metrology_struct, &Phase_loop_computed[r]);
				}
				for (int r = 0; r < 8; r++) {
					for (int c = 0; c < 8; c++) {
					}
				}
			}
			loop_radius = 0.5;// почему не передается в функцию
			if (express == true) {
				for (int r = 0; r < 8; r++) {
					if (r == 0 || r == 1) {
						for (int c = 0; c < 8; c++) {
							Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[c], loop_radius, mode).VALUE[c];
							cout << "phase_loop " << r << " " << c << " " << Phase_loop_computed[r].VALUE[c]*mG << endl;

						}
						Ro_adecvat_computed[r] = R0_adecvat_r_loop(&metrology_struct, &Phase_loop_computed[r]);
					}
					else {
						for (int c = 0; c < 8; c++) {
							if (c < 4) Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], R_Add_Wire[c], Re_from_loop, L_loop, C[3], loop_radius, mode).VALUE[c];
							else Phase_loop_computed[r].VALUE[c] = calc_phase_from_loop(&metrology_struct, R_loop_physical[r], L_loop, Re_from_loop, R_Add_Wire[c], C[7], loop_radius, mode).VALUE[c];
							cout << "phase_loop " << r << " " << c << " " << Phase_loop_computed[r].VALUE[c]*mG << endl;

						}
						Ro_adecvat_computed[r] = R0_adecvat_r_loop(&metrology_struct, &Phase_loop_computed[r]);
					}
				}
			}

/////////////////////////////////////////////////////////////////////////////////////////////////////
			Sleep(1000);
			cout << "Start !!!" << endl;
			sonde_on_off = 1;
			working = true;
			this->timer1->Enabled = true;
			//Read_Port_Thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Read_Port));
			//Read_Port_Thread->Start();
			this->button_ON_OFF->Text = "ПРИБОР ВКЛЮЧЕН";
			this->comboBox_sonde_type->DropDownStyle = System::Windows::Forms::ComboBoxStyle::Simple;								
		}

		else {
			serialPort_LWD_1DDS->Close();
			fout->close();
			sonde_on_off = 0;
			working = false;
			this->timer1->Enabled = false;
			this->button_ON_OFF->Text = "ВКЛЮЧИТЬ ПРИБОР";
			this->comboBox_sonde_type->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDown;
			this->button_ON_OFF->BackColor = DefaultBackColor;
			//Read_Port_Thread->Sleep(200);
			//Read_Port_Thread->Abort();
		}
	}
	cout << "on_off " << sonde_on_off << endl;
}

bool permit = true;

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	extern bool LWD_1DDS;
	int result = 0;
	AllData_1DDS *P = new AllData_1DDS;
	const uint8_t length = 252;
	uint8_t *DATA = new uint8_t;
	*DATA = length;
	uint8_t DATA_IN[length] = { 0, };
	uint8_t CMD;
	V8 Air_for_smt;

	extern int N_aver;
	extern bool LocalMetro_working;
	extern bool stop_working;
	extern bool express;
	extern bool AM_TX;
	extern bool sonde_ro_calk;
	extern bool comp_smt;
	extern bool param_form_closed;
	extern METROLOGY metrology_struct;
	extern string Com_LWD_Name;
	
	if (LWD_1DDS == false) {
		if (stop_working == false) {

			extern double loop_radius;
			extern unsigned int Points;
			extern V8 Air_Phase;
			AllData_1DDS G_1DDS;			
			V8 Phase, Phase_smt;
			unsigned long AMK_result = 0;
			extern bool metrology_is_set;

			AMK_GetData = (GetData)GetProcAddress(AMK_DLL, "AMK_GetData");
			_controlfp(_CW_DEFAULT, 0xfffff);
			if (blink == 0) { blink = 1; this->button_ON_OFF->BackColor = DefaultBackColor; }
			else { blink = 0; this->button_ON_OFF->BackColor = Color::PaleGreen; }
			//если работаем на локальной метрологии то красное
			if (LocalMetro_working == false)this->buttonLocalMetro->BackColor = DefaultBackColor;
			else {
				if (sonde_ro_calk == true)
					this->buttonLocalMetro->BackColor = Color::Red;
				else if (sonde_ro_calk == false)
					this->buttonLocalMetro->BackColor = Color::PaleGreen;
			}


			for (int i = 0; i < N_aver - 1; i++) {
				Ro_chart[i] = Ro_chart[i + 1];
				Phase_chart[i] = Phase_chart[i + 1];
				Phase_raw[i] = Phase_raw[i + 1];
			}


			//сначала проверяем УСИ
			ftdi_status = AMK_CheckConnect();
			if (ftdi_status != 1) {
				was_unpluged = true;
			}
			if (was_unpluged == true) {
				textBox23->ForeColor = Color::Red;
				textBox23->Text = "Потеряна связь с УСИ    ";
				cout << " No USI " << endl;
				if (ftdi_status == 1)  was_pluged = true;
			}

			if (was_pluged) {
				do {
					ComPort = AMK_StartComPort(&ComPort, NULL);
					cout << " No COM " << ComPort << endl;
				} while (ComPort == NULL);
				was_pluged = false; was_unpluged = false;
				//Sleep(500);
			}
			permit = false;
			if (AMK_GetData(ComPort, &G_1DDS)) {

				if (Local_metro_is_click == true) {
					N_aver = 20;
					textBox23->ForeColor = Color::Red;
					textBox23->Text = "ИДЕТ СБОР ДАННЫХ О СМЕЩЕНИИ ФАЗ " + (24 - N_LocalMetro).ToString() + " СЕК   ";
					//cout << "Phase_shift " << (24 - N_LocalMetro) << endl;
					if (N_LocalMetro == 24) {
						N_LocalMetro = 0;
						Local_metro_is_click = false;
						comboBox1->SelectedIndex = 1;
						this->buttonR9->Enabled = true;
						this->buttonR1->Enabled = true;
						this->buttonR2->Enabled = true;
						this->buttonR3->Enabled = true;
						this->buttonR4->Enabled = true;
						this->buttonR5->Enabled = true;
						this->buttonR6->Enabled = true;
						this->buttonR7->Enabled = true;
						this->buttonR8->Enabled = true;
						this->button_Report->Enabled = true;
						//меняем воздух в зонде на локальный

						for (int i = 0; i < 8; i++) {
							if (sonde_ro_calk == true) {
								Air_for_smt = get_air_smt(Phase_raw_aver, &metrology_struct);
							    //metrology_struct.Air_zz[i] = Phase_raw_aver->VALUE[i] * mG;
							}
								
							if (sonde_ro_calk == false)
							    metrology_struct.Air_zz[i] = Phase_raw_aver->VALUE[i] * mG;
							//cout << "Air " << i << " " << metrology_struct.Air_zz[i] << endl;
							//cout << "raw " << i << " " << Phase_raw_aver->VALUE[i]*mG << endl;
						}

						//если пользуемся расчетами на контроллере зонда

						/*
						if (sonde_ro_calk == true) {
							//Устанавливаем 32й бит в серийном зонда, ка маркер того,
							//что зонд работает на временном файле метрологии
							metrology_struct.serial = metrology_struct.serial | (1 << 31);
							if (AMK_SetIdle(ComPort)) {
								this->timer1->Enabled = false;
								Sleep(200);
								if (AMK_SetMetroData(ComPort, &metrology_struct)) {
									//Sleep(200);
									//AMK_PwrOff(ComPort);
									//Sleep(500);
									//AMK_PwrOn(ComPort);
									//Sleep(2000);
									//AMK_Init(ComPort);
									Sleep(500);
									AMK_BeginCyclogram(ComPort);
									LocalMetro_working = true;
									this->buttonLocalMetro->BackColor = Color::Red;
									MessageBox::Show("локальные поправки на воздух установлены!");
									N_aver = N_aver_buff;
									this->timer1->Enabled = true;
									//comboBox1->SelectedIndex = 3;
									//comboBox1->SelectedIndex = 1;
								}
								else MessageBox::Show("локальный воздух не установлен, попробуйте еще раз");
							}
							else MessageBox::Show("локальный воздух не установлен, попробуйте еще раз");
						}
						*/

						//если пользуемся расчетами компьютере
						//удалить после проверки
						if (sonde_ro_calk == false || sonde_ro_calk == true) {
							this->timer1->Enabled = false;
							//Sleep(200);
							LocalMetro_working = true;
							this->buttonLocalMetro->BackColor = Color::PaleGreen;
							MessageBox::Show(" поправки на воздух для расчета на компьютере установлены!");
							N_aver = N_aver_buff;
							this->timer1->Enabled = true;
						}

					}
					N_LocalMetro++;
				}

				Temp = G_1DDS.temperature;
				if (Local_metro_is_click == false) {
					textBox23->ForeColor = Color::Black;
					textBox23->Text = "Temp " + Temp.ToString("F1") + " C ";
				}

				fout->open(Dataout, ios::binary | ios::app);
				fout->write((char*)&G_1DDS, sizeof(G_1DDS));
				fout->close();

				///////////////////////////////////////////////////////////////
				//амплитуды
				for (int i = 0; i < 8; i++) {
					TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox_AM_Tx_" + (i).ToString()];
					if (AM_TX) {
						TEXTBOX->Visible = true;
						TEXTBOX->Text = "AM_Tx " + ((G_1DDS.AM_RX_1[i])*mV).ToString("F0") + " mV";
					}
					if (!AM_TX) {
						TEXTBOX->Visible = false;
					}
				}

				Phase_raw[N_aver - 1] = get_phase_raw(&G_1DDS);
				
				//если пользуемся расчетами на контроллере зонда
				/*
				if (sonde_ro_calk == true) {
					//cout << "sonde_ro_calk " << endl;
					for (int i = 0; i < 8; i++) {
						Phase_chart[N_aver - 1].VALUE[i] = G_1DDS.PH_smt[i];
						Ro_chart[N_aver - 1].VALUE[i] = G_1DDS.Ro_smt[i];
					}
				}
				*/

				//если считаем на компе
				if (sonde_ro_calk == false) {
					//cout << "comp_ro_calk " << endl;
					if (comp_smt == true) {//для симметрзованных берем из зонда
						//cout << "smt " << endl;
						for (int i = 0; i < 8; i++) 
							Phase_chart[N_aver - 1].VALUE[i] = G_1DDS.PH_smt[i] - Air_for_smt.VALUE[i];
						Ro_chart[N_aver - 1] = R0_adecvat_r_loop(&metrology_struct, &Phase_chart[N_aver - 1]);
					}
					if (comp_smt == false) { //для несимметризованных
						//cout << "no_smt " << endl;
						Phase_chart[N_aver - 1] = get_phase(&G_1DDS, &metrology_struct);
						Ro_chart[N_aver - 1] = R0_adecvat_r_loop(&metrology_struct, &Phase_chart[N_aver - 1]);
					}
				}


				//считаем средние значения 
				for (int j = 0; j < 8; j++) {
					Ro_aver->VALUE[j] = 0;
					Phase_aver->VALUE[j] = 0;
					Phase_raw_aver->VALUE[j] = 0;
				}
				for (int i = 0; i < N_aver; i++) {
					for (int j = 0; j < 8; j++) {
						Ro_aver->VALUE[j] += Ro_chart[i].VALUE[j];
						Phase_aver->VALUE[j] += (Phase_chart[i].VALUE[j]);
						Phase_raw_aver->VALUE[j] += (Phase_raw[i].VALUE[j]);
					}
				}
				for (int j = 0; j < 8; j++) {
					Ro_aver->VALUE[j] /= double(N_aver);
					Phase_aver->VALUE[j] /= double(N_aver);
					Phase_raw_aver->VALUE[j] /= double(N_aver);
				}

				N++;
				// если работаем на разных емкостях
				if (express == false) {
					if (N_R_loop < 8) {
						if (n_chart < 8 && N_r == 0) {
							C_switch(n_chart + 1);
							//n_for_c_switch = n_chart + 1;
							//Read_Port();
						}
							
						N_r++;
						if (N_r == N_aver + 3) {
							N_r = 0;
							n_chart++;
							if (n_chart == 15)n_chart = 0;
						}
						if (n_chart < 8) {
							do_chart_(n_chart);
							button_Report->BackColor = DefaultBackColor;
						}
						else button_Report->BackColor = Color::PaleGreen;
					}

					else {
						do_chart();
					}
				}
				// если работаем на одной емкости в пределах одной частоты 
				if (express == true) {
					if (N_R_loop < 8) {
						if (N_R_loop == 0 || N_R_loop == 1) {
							if (n_chart < 8 && N_r == 0) {
								C_switch(n_chart + 1);
								//n_for_c_switch = n_chart + 1;
								//Read_Port();
							}
								
							N_r++;
							if (N_r == N_aver + 3) {
								N_r = 0;
								n_chart++;
								if (n_chart == 15)n_chart = 0;
							}
							if (n_chart < 8) {
								do_chart_(n_chart);
								button_Report->BackColor = DefaultBackColor;
							}
							else button_Report->BackColor = Color::PaleGreen;
						}
						else {
							if (N_r == 0) {
								if (n_chart == 0) { 
									C_switch(4); 
									//n_for_c_switch = 4;
									//Read_Port();
								}
								if (n_chart == 1){
									C_switch(8); 
									//n_for_c_switch = 8;
									//Read_Port();
								}
							}

							N_r++;
							if (N_r == N_aver + 3) {
								N_r = 0;
								n_chart++;
								if (n_chart == 5)n_chart = 0;
							}
							if (n_chart == 0) {
								do_chart_(0); do_chart_(1); do_chart_(2); do_chart_(3);
								button_Report->BackColor = DefaultBackColor;
							}
							else if (n_chart == 1) {
								do_chart_(4); do_chart_(5); do_chart_(6); do_chart_(7);
								button_Report->BackColor = DefaultBackColor;
							}
							else button_Report->BackColor = Color::PaleGreen;
						}

					}

					else {
						do_chart();
					}
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////////////

			}

			else {
				if (was_pluged == false && was_unpluged == false) {
					if (AMK_PwrOff(ComPort)) {
						cout << " PWR_Off " << endl;
						int WIEW = comboBox1->SelectedIndex;
						button_ON_OFF->PerformClick();
						Sleep(500);///////////////////////////////////////////
						button_ON_OFF->PerformClick();
						comboBox1->SelectedIndex = WIEW;
						Sleep(500);
					}
					else {
						textBox23->ForeColor = Color::Red;
						textBox23->Text = "Потеряна связь прибора с УСИ       ";
						cout << " NO_PWR " << endl;
						//button_ON_OFF->PerformClick();///////////////////////////////
					}
				}

			}
			
			permit = true;

		}
	}//end LWD_1DDS == false
	
	if (LWD_1DDS == true) {
		if (stop_working == false) {

			extern double loop_radius;
			extern unsigned int Points;
			extern V8 Air_Phase;
			V8 Phase, Phase_smt;
			//unsigned long AMK_result = 0;

			extern bool metrology_is_set;

			if (blink == 0) { blink = 1; this->button_ON_OFF->BackColor = DefaultBackColor; }
			else { blink = 0; this->button_ON_OFF->BackColor = Color::PaleGreen; }
			//если работаем на локальной метрологии то красное
			if (LocalMetro_working == false)this->buttonLocalMetro->BackColor = DefaultBackColor;
			else {
				if (sonde_ro_calk == true)
					this->buttonLocalMetro->BackColor = Color::Red;
				else if (sonde_ro_calk == false)
					this->buttonLocalMetro->BackColor = Color::PaleGreen;
			}


			for (int i = 0; i < N_aver - 1; i++) {
				Ro_chart[i] = Ro_chart[i + 1];
				Phase_chart[i] = Phase_chart[i + 1];
				Phase_raw[i] = Phase_raw[i + 1];
			}


			//////////////////////////////////////////////////////////////////////////////////


			bool stop = false;
			try {
				serialPort_LWD_1DDS->PortName = gcnew String(Com_LWD_Name.c_str());
				serialPort_LWD_1DDS->ReadTimeout = 1000;
				serialPort_LWD_1DDS->WriteTimeout = 1000;
				serialPort_LWD_1DDS->BaudRate = 125000;
				serialPort_LWD_1DDS->Open();
			}
			catch (...) { stop = 1; }

			if (!stop) {
				if (send_to_tool(serialPort_LWD_1DDS, CMD_WORK, DATA, 1) == 1) {
					MessageBox::Show("Соединение с прибором прервано переподключите !");
				}
				Sleep(100);
				result = receive_from_tool(serialPort_LWD_1DDS, CMD, DATA_IN, length);
				serialPort_LWD_1DDS->Close();
			}

			else {
				stop = 0;
				result = 3;
				Console::Write("no port ");
				serialPort_LWD_1DDS->Close();
			}

			//cout << "result " << result << endl;

			if (result == 0) {

				P = (AllData_1DDS*)(DATA_IN + 12);

				if (Local_metro_is_click == true) {
					N_aver = 20;
					textBox23->ForeColor = Color::Red;
					textBox23->Text = "ИДЕТ СБОР ДАННЫХ О СМЕЩЕНИИ ФАЗ " + (24 - N_LocalMetro).ToString() + " СЕК   ";
					//cout << "Phase_shift " << (24 - N_LocalMetro) << endl;
					if (N_LocalMetro == 24) {
						N_LocalMetro = 0;
						Local_metro_is_click = false;
						comboBox1->SelectedIndex = 1;
						this->buttonR9->Enabled = true;
						this->buttonR1->Enabled = true;
						this->buttonR2->Enabled = true;
						this->buttonR3->Enabled = true;
						this->buttonR4->Enabled = true;
						this->buttonR5->Enabled = true;
						this->buttonR6->Enabled = true;
						this->buttonR7->Enabled = true;
						this->buttonR8->Enabled = true;
						this->button_Report->Enabled = true;
						
						//меняем воздух в зонде на локальный
						for (int i = 0; i < 8; i++) {
							if (sonde_ro_calk == true) {								
								Air_for_smt = get_air_smt(Phase_raw_aver, &metrology_struct);
								//metrology_struct.Air_zz[i] = Phase_raw_aver->VALUE[i] * mG;
							}

							if (sonde_ro_calk == false)								
								metrology_struct.Air_zz[i] = Phase_raw_aver->VALUE[i] * mG;
							//cout << "Air " << i << " " << metrology_struct.Air_zz[i] << endl;
							//cout << "raw " << i << " " << Phase_raw_aver->VALUE[i]*mG << endl;
						}

	
						//если пользуемся расчетами на компьютере
						//удалить после проверки
						if (sonde_ro_calk == false || sonde_ro_calk == true) {
							this->timer1->Enabled = false;
							//Sleep(200);
							LocalMetro_working = true;
							this->buttonLocalMetro->BackColor = Color::PaleGreen;
							MessageBox::Show(" поправки на воздух для расчета на компьютере установлены!");
							N_aver = N_aver_buff;
							this->timer1->Enabled = true;
						}

					}
					N_LocalMetro++;
				}

				//tm t = time_tm(G.TIME);
				Temp = P->temperature;			
				if (Local_metro_is_click == false) {
					textBox23->ForeColor = Color::Black;
					textBox23->Text = "Temp " + Temp.ToString("F1") + " C ";
				}

				fout->open(Dataout, ios::binary | ios::app);
				fout->write((char*)P, sizeof(AllData_1DDS));
				fout->close();

				///////////////////////////////////////////////////////////////

				//амплитуды
				for (int i = 0; i < 8; i++) {
					TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox_AM_Tx_" + (i).ToString()];
					if (AM_TX) {
						TEXTBOX->Visible = true;
						TEXTBOX->Text = "AM_Tx " + ((P->AM_RX_1[i])*mV).ToString("F0") + " mV";						
					}
					if (!AM_TX) {
						TEXTBOX->Visible = false;
					}
				}

				Phase_raw[N_aver - 1] = get_phase_raw(P);
				
				//если считаем на компе
				if (sonde_ro_calk == false) {
					//cout << "comp_ro_calk " << endl;
					if (comp_smt == true) {//для симметрзованных берем из зонда
						//cout << "smt " << endl;
						for (int i = 0; i < 8; i++)
							Phase_chart[N_aver - 1].VALUE[i] = P->PH_smt[i] - Air_for_smt.VALUE[i];
						Ro_chart[N_aver - 1] = R0_adecvat_r_loop(&metrology_struct, &Phase_chart[N_aver - 1]);
					}
					if (comp_smt == false) { //для несимметризованных
						//cout << "no_smt " << endl;
						Phase_chart[N_aver - 1] = get_phase(P, &metrology_struct);
						Ro_chart[N_aver - 1] = R0_adecvat_r_loop(&metrology_struct, &Phase_chart[N_aver - 1]);
					}
				}

				





				//считаем средние значения 
				for (int j = 0; j < 8; j++) {
					Ro_aver->VALUE[j] = 0;
					Phase_aver->VALUE[j] = 0;
					Phase_raw_aver->VALUE[j] = 0;
				}
				for (int i = 0; i < N_aver; i++) {
					for (int j = 0; j < 8; j++) {
						Ro_aver->VALUE[j] += Ro_chart[i].VALUE[j];
						Phase_aver->VALUE[j] += (Phase_chart[i].VALUE[j]);
						Phase_raw_aver->VALUE[j] += (Phase_raw[i].VALUE[j]);
					}
				}
				for (int j = 0; j < 8; j++) {
					Ro_aver->VALUE[j] /= double(N_aver);
					Phase_aver->VALUE[j] /= double(N_aver);
					Phase_raw_aver->VALUE[j] /= double(N_aver);
				}

				N++;
				// если работаем на разных емкостях
				if (express == false) {
					if (N_R_loop < 8) {
						if (n_chart < 8 && N_r == 0) {
							C_switch(n_chart + 1);
							//n_for_c_switch = n_chart + 1;
							//Read_Port();
						}
							
						N_r++;
						if (N_r == N_aver + 3) {
							N_r = 0;
							n_chart++;
							if (n_chart == 15)n_chart = 0;
						}
						if (n_chart < 8) {
							do_chart_(n_chart);
							button_Report->BackColor = DefaultBackColor;
						}
						else button_Report->BackColor = Color::PaleGreen;
					}

					else {
						do_chart();
					}
				}
				// если работаем на одной емкости в пределах одной частоты 
				if (express == true) {
					if (N_R_loop < 8) {
						if (N_R_loop == 0 || N_R_loop == 1) {
							if (n_chart < 8 && N_r == 0) {
								C_switch(n_chart + 1);
								//n_for_c_switch = n_chart + 1;
								//Read_Port();
							}
								
							N_r++;
							if (N_r == N_aver + 3) {
								N_r = 0;
								n_chart++;
								if (n_chart == 15)n_chart = 0;
							}
							if (n_chart < 8) {
								do_chart_(n_chart);
								button_Report->BackColor = DefaultBackColor;
							}
							else button_Report->BackColor = Color::PaleGreen;
						}
						else {
							if (N_r == 0) {
								if (n_chart == 0) { 
									C_switch(4);
									//n_for_c_switch = 4;
									//Read_Port();
								}
								if (n_chart == 1) {
									C_switch(8);
									//n_for_c_switch = 8;
									//Read_Port();
								}
							}

							N_r++;
							if (N_r == N_aver + 3) {
								N_r = 0;
								n_chart++;
								if (n_chart == 5)n_chart = 0;
							}
							if (n_chart == 0) {
								do_chart_(0); do_chart_(1); do_chart_(2); do_chart_(3);
								button_Report->BackColor = DefaultBackColor;
							}
							else if (n_chart == 1) {
								do_chart_(4); do_chart_(5); do_chart_(6); do_chart_(7);
								button_Report->BackColor = DefaultBackColor;
							}
							else button_Report->BackColor = Color::PaleGreen;
						}

					}

					else {
						do_chart();
					}
				}


			}//end result == 0
			if (result == 0) {
				/*
				P = (AllData_1DDS*)(DATA_IN + 12);
				cout << " CMD " << std::hex << CMD << " SGN " << P->signature << endl;
				cout << std::dec << std::setprecision(6);

				cout << "Ro " << P->Ro_smt[0] << " " << P->Ro_smt[1] << " " << P->Ro_smt[2] << " " << P->Ro_smt[3] << " " << P->Ro_smt[4] << " " << P->Ro_smt[5] << " " << P->Ro_smt[6] << " " << P->Ro_smt[7] << endl;
				cout << "Ro " << P->PH_smt[0] << " " << P->PH_smt[1] << " " << P->PH_smt[2] << " " << P->PH_smt[3] << " " << P->PH_smt[4] << " " << P->PH_smt[5] << " " << P->PH_smt[6] << " " << P->PH_smt[7] << endl;

				for (int i = 0; i < 4; i++) {
					cout << "AM_TX_400 " << i << " RX_1 " << P->AM_RX_1[i] << " RX_2 " << P->AM_RX_2[i] << endl;
				}
				for (int i = 4; i < 8; i++) {
					cout << "AM_TX_2000 " << i << " RX_1 " << P->AM_RX_1[i] << " RX_2 " << P->AM_RX_2[i] << endl;
				}

				cout << "AM_TX_400_0 " << " RX_1 " << P->AM_RX_1[8] << " RX_2 " << P->AM_RX_2[8] << endl;
				cout << "AM_TX_400_0 " << " RX_1 " << P->AM_RX_1[9] << " RX_2 " << P->AM_RX_2[9] << endl;

				for (int i = 0; i < 4; i++) {
					cout << "PH_TX_400 " << i << " RX_1 " << P->PH_RX_1[i] << " RX_2 " << P->PH_RX_2[i] << endl;
				}
				for (int i = 4; i < 8; i++) {
					cout << "PH_TX_2000 " << i << " RX_1 " << P->PH_RX_1[i] << " RX_2 " << P->PH_RX_2[i] << endl;
				}

				cout << "PH_TX_400_0 " << " RX_1 " << P->PH_RX_1[8] << " RX_2 " << P->PH_RX_2[8] << endl;
				cout << "PH_TX_400_0 " << " RX_1 " << P->PH_RX_1[9] << " RX_2 " << P->PH_RX_2[9] << endl;
                */
			}

		}//end stop_working == false

	}//end LWD_1DDS == true

}

private: System::Void R1_Click(System::Object^  sender, System::EventArgs^  e) { 
	if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(1);
			CLEAR(1);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
	
}

private: System::Void R2_Click(System::Object^  sender, System::EventArgs^  e) {
	if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(2);
			CLEAR(2);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}	
}

private: System::Void R3_Click(System::Object^  sender, System::EventArgs^  e) {
	 if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(3);
			CLEAR(3);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
}

private: System::Void R4_Click(System::Object^  sender, System::EventArgs^  e) {
 if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(4);
			CLEAR(4);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
}

private: System::Void R5_Click(System::Object^  sender, System::EventArgs^  e) {
	 if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(5);
			CLEAR(5);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
}

private: System::Void R6_Click(System::Object^  sender, System::EventArgs^  e) {
	 if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(6);
			CLEAR(6);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
}

private: System::Void R7_Click(System::Object^  sender, System::EventArgs^  e) {
	 if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(7);
			CLEAR(7);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
}

private: System::Void R8_Click(System::Object^  sender, System::EventArgs^  e) {
	 if (tabControl1->SelectedIndex == 0) {
		extern bool metrology_is_set;
		extern bool LocalMetro_working;
		extern bool sonde_ro_calk;
		if (metrology_is_set == true) {
			if (LocalMetro_working == false && sonde_ro_calk == true) MessageBox::Show("локальная метрология не установлена !");
			//comboBox1->SelectedIndex = 0;
			R_switch(8);
			CLEAR(8);
		}
		else MessageBox::Show("Считайте файл метрологии из прибора !");
	}
	
}

private: System::Void Air_Click(System::Object^  sender, System::EventArgs^  e) {
	 if (tabControl1->SelectedIndex == 0) {

		for (int i = 0; i < 9; i++) {
			TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel1->Controls["textBox" + (i + 1).ToString()];
			TEXTBOX->ForeColor = System::Drawing::Color::Black;
		}

        R_switch(9);
	
	    if (bt_on == true ) {
		    if(working==true)this->timer1->Enabled = false;
		    
			if (loop_version == 1) {
				int stop = 0;
				Byte send = 0;
				Byte resive = 0;
				serialPort->DiscardInBuffer();
				serialPort->BaseStream->WriteByte(send);
				Sleep(300);
				try { resive = serialPort->BaseStream->ReadByte(); }
				catch (...) { stop = 1; }
				if (!stop) {
					if (resive == send) {
						if (working == true)this->timer1->Enabled = true;
						}
					else MessageBox::Show("Нет ответа от кольца , нажмите С5 еще раз !");
				}
				else { MessageBox::Show("Соединение с кольцом прервано переподключите  !"); }
			}
			
			if (loop_version == 2) {
				int stop = 0;
				String^ send = "RES 0\r\n";
				String^ resive = "";
				String^ resive1 = "";
				serialPort->DiscardInBuffer();
				//serialPort->Write("GEN 0\r\n");
				Sleep(500);
				serialPort->DiscardInBuffer();
				serialPort->Write("RES 0\r\n");
				Sleep(500);
				serialPort->Write("CAP 0\r\n");
				Sleep(500);
				try {
					resive = serialPort->ReadLine();
					resive1 = serialPort->ReadLine();
				}
				catch (...) { stop = 1; }
				if (!stop) {
					//Console::WriteLine("resive " + resive);
					//Console::WriteLine("resive1 "+ resive1);
					if (resive->StartsWith("RES=0 OK!") && resive1->StartsWith("CAP=0 OK!")) {
						resive = ""; resive1 = "";
						if (working == true)this->timer1->Enabled = true;
						}
					else MessageBox::Show("Нет ответа от кольца , нажмите С5 еще раз !");
				}
				else MessageBox::Show("Соединение с кольцом прервано переподключите  !"); 	
			}
	    }
	}
	
}

private: System::Void Report_Click(System::Object^  sender, System::EventArgs^  e) {
	button_Report->BackColor = Color::PaleGreen;
	extern int norm[10];
	extern int medium[10];
	extern int out_of_norm[10];
	String^ sine;
	String^ mean;
	String^ frmt;
	V8 percent_for_print[10];
	V8 value_for_print[10];

	if (N_R_loop == 0) { this->pictureBoxR1->BackColor = Color::Green; green[N_R_loop] = true;}
	if (N_R_loop == 1) { this->pictureBoxR2->BackColor = Color::Green; green[N_R_loop] = true;}
	if (N_R_loop == 2) { this->pictureBoxR3->BackColor = Color::Green; green[N_R_loop] = true;}
	if (N_R_loop == 3) { this->pictureBoxR4->BackColor = Color::Green; green[N_R_loop] = true;}
	if (N_R_loop == 4) { this->pictureBoxR5->BackColor = Color::Green; green[N_R_loop] = true; }
	if (N_R_loop == 5) { this->pictureBoxR6->BackColor = Color::Green; green[N_R_loop] = true; }
	if (N_R_loop == 6) { this->pictureBoxR7->BackColor = Color::Green; green[N_R_loop] = true; }
	if (N_R_loop == 7) { this->pictureBoxR8->BackColor = Color::Green; green[N_R_loop] = true; }
	if (N_R_loop == 8) { this->pictureBoxAir->BackColor = Color::Green; green[N_R_loop] = true;}
	
	if (wiew == 0) {//Ro_wiew
		percent_for_print[N_R_loop] = Ro_percent_for_print[N_R_loop];
		value_for_print[N_R_loop] = Ro_for_print[N_R_loop];
		mean = " Om ";
		frmt = "F2";
	}
	if (wiew == 1) {//PH_wiew
		percent_for_print[N_R_loop] = Phase_percent_for_print[N_R_loop];
		value_for_print[N_R_loop] = Phase_for_print[N_R_loop];
		mean = " mG ";
		frmt = "F0";
	}
	
	for (int i = 0; i < 8; i++) {
		int j = N_R_loop;
		if (j < 8) {/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!AIR
			TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel3->Controls["textBox" + (i + j * 8 + 24).ToString()];
			if (percent_for_print[N_R_loop].VALUE[i] > 0) sine = "+"; else sine = "-";
			if (fabs(percent_for_print[N_R_loop].VALUE[i]) <= norm[j]) TEXTBOX->ForeColor = System::Drawing::Color::LimeGreen;
			if (fabs(percent_for_print[N_R_loop].VALUE[i]) > norm[j] && fabs(percent_for_print[N_R_loop].VALUE[i]) <= medium[j]) TEXTBOX->ForeColor = System::Drawing::Color::Gold;
			if (fabs(percent_for_print[N_R_loop].VALUE[i]) > medium[j] && fabs(percent_for_print[N_R_loop].VALUE[i]) <= out_of_norm[j]) TEXTBOX->ForeColor = System::Drawing::Color::Red;
			if (fabs(percent_for_print[N_R_loop].VALUE[i]) > out_of_norm[j]) TEXTBOX->ForeColor = System::Drawing::Color::DarkRed;

			if (fabs(percent_for_print[N_R_loop].VALUE[i]) > 200) percent_for_print[N_R_loop].VALUE[i] = 200;
			if (value_for_print[j].VALUE[i] != 0) {

				//cout << "N_C_loop " << N_C_loop << endl;
				if(i < N_C_loop+1)
				    TEXTBOX->Text = value_for_print[j].VALUE[i].ToString(frmt) + mean + sine + percent_for_print[N_R_loop].VALUE[i].ToString("F1") + " %   ";
				else
				    TEXTBOX->Text = "";
			
			}
			else TEXTBOX->Text = "nan";
		}

		if (j == 8) {
			TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel3->Controls["textBox" + (i + j * 8 + 24).ToString()];
			if (value_for_print[j].VALUE[i] != 0) {
			    TEXTBOX->Text = value_for_print[j].VALUE[i].ToString(frmt) + mean ;
			}
			else TEXTBOX->Text = "nan";
		}
	}
}

private: System::Void сохранитьРапортRtfToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (working == true) this->timer1->Enabled = false;
	richTextBox1->Clear();
	extern METROLOGY metrology_struct;
	METROLOGY metrology_struct_temp;
	extern bool Ro_to_raport;
	extern bool PH_to_raport;
	extern double R_loop_physical[9];
	extern double loop_radius;
	extern bool LocalMetro_working;
	extern bool sonde_on_off;
	extern bool sonde_ro_calk;
	extern bool comp_smt;
	extern int norm[10];
	extern int medium[10];
	extern int out_of_norm[10];
	unsigned int sonde_serial = metrology_struct.serial;
	String^ serial = (sonde_serial &= 0xffff).ToString();
	time_t now = time(NULL);
	struct tm  t = *localtime(&now);
	String^ month;
	extern  double R_loop_physical[9];
	int otstup_sverhu = 60;
	String^ R_index;

	if (Ro_to_raport == false && PH_to_raport == false) {
		MessageBox::Show(L"Параметры УЭС -> Рапорт и/или Фазы -> Рапорт");
	}
	else {
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
		
		if (sonde_ro_calk == false) {
			if (comp_smt == true)
			    richTextBox1->AppendText("\r\n \r\n Расчет симметризованных фаз и УЭС на компьютере " + Environment::NewLine);
			if (comp_smt == false)
				richTextBox1->AppendText("\r\n \r\n Расчет НЕсимметризованных фаз и УЭС на компьютере " + Environment::NewLine);
		}
		if (sonde_ro_calk == true) {
			richTextBox1->AppendText("\r\n \r\n Расчет фаз и УЭС на приборе " + Environment::NewLine);
		}

		
		//УЭС
		if (Ro_to_raport == true) {
			richTextBox1->AppendText("\r\n  ЗОНД      УЭС расчетное Ом    УЭС измерянное Ом    Погрешность % " + Environment::NewLine);
			//номер сопротивления 
			for (int j = 0; j < 8; j++) {//изменить на 9 если хотим воздух

				if (j < 8)R_index = "\r\n Мера импеданса № " + (j + 1).ToString() + "  " + R_loop_physical[j] + " Ом_____________________________________________________________ \r\n";
				else R_index = "\r\n Воздух________________________________________________________________________________ \r\n";
				richTextBox1->AppendText(R_index + Environment::NewLine);
				//green[j] = true;
				if (green[j] == true) {
					String^ frq; String^ nomer;

					//номер передатчика и кондюка
					for (int i = 0; i < 8; i++) {
						if (Ro_for_print[j].VALUE[i] != 0) {
							if (fabs(Ro_percent_for_print[j].VALUE[i]) <= norm[j]) richTextBox1->SelectionColor = Color::Green;
							if (fabs(Ro_percent_for_print[j].VALUE[i]) > norm[j] && fabs(Ro_percent_for_print[j].VALUE[i]) <= medium[j]) richTextBox1->SelectionColor = Color::Yellow;
							if (fabs(Ro_percent_for_print[j].VALUE[i]) > medium[j] && fabs(Ro_percent_for_print[j].VALUE[i]) <= out_of_norm[j]) richTextBox1->SelectionColor = Color::Red;
							if (fabs(Ro_percent_for_print[j].VALUE[i]) > out_of_norm[j]) richTextBox1->SelectionColor = Color::DarkRed;

							if (i < 4) { frq = "__875 "; nomer = (i + 1).ToString(); }
							else { frq = "_2000 "; nomer = (i - 3).ToString(); }
							String^ to_print;
							to_print = "T_" + nomer + frq + "         " + Ro_adecvat_computed[j].VALUE[i].ToString("0000.00") + "                     " +
								Ro_for_print[j].VALUE[i].ToString("0000.00") + "                      "
								+ Ro_percent_for_print[j].VALUE[i].ToString("0000.00") + "    ";
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

		}

		//Фазы
		if (PH_to_raport == true) {
			richTextBox1->AppendText("\r\n \r\n ЗОНД      Фаза расчетная mD    Фаза измерянная mD    Погрешность % " + Environment::NewLine);
			//номер сопротивления 
			for (int j = 0; j < 8; j++) {//изменить на 9 если хотим воздух

				if (j < 8)R_index = "\r\n Мера импеданса № " + (j + 1).ToString() + "  " + R_loop_physical[j] + " Ом_____________________________________________________________ \r\n";
				else R_index = "\r\n Воздух________________________________________________________________________________ \r\n";
				richTextBox1->AppendText(R_index + Environment::NewLine);
				//green[j] = true;
				if (green[j] == true) {
					String^ frq; String^ nomer;

					//номер передатчика и кондюка
					for (int i = 0; i < 8; i++) {
						if (Phase_for_print[j].VALUE[i] != 0) {
							if (fabs(Phase_percent_for_print[j].VALUE[i]) <= norm[j]) richTextBox1->SelectionColor = Color::Green;
							if (fabs(Phase_percent_for_print[j].VALUE[i]) > norm[j] && fabs(Phase_percent_for_print[j].VALUE[i]) <= medium[j]) richTextBox1->SelectionColor = Color::Yellow;
							if (fabs(Phase_percent_for_print[j].VALUE[i]) > medium[j] && fabs(Phase_percent_for_print[j].VALUE[i]) <= out_of_norm[j]) richTextBox1->SelectionColor = Color::Red;
							if (fabs(Phase_percent_for_print[j].VALUE[i]) > out_of_norm[j]) richTextBox1->SelectionColor = Color::DarkRed;

							if (i < 4) { frq = "__875 "; nomer = (i + 1).ToString(); }
							else { frq = "_2000 "; nomer = (i - 3).ToString(); }
							String^ to_print;
							to_print = "T_" + nomer + frq + "         " + Phase_loop_computed[j].VALUE[i].ToString("0000.00") + "                     " +
								Phase_for_print[j].VALUE[i].ToString("0000.00") + "                      "
								+ Phase_percent_for_print[j].VALUE[i].ToString("0000.00") + "    ";
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
		}


		richTextBox1->SelectionColor = Color::Black;
		richTextBox1->AppendText("\r\n\r\n\r\n");
		richTextBox1->AppendText("Заключение ____________________________________________________________________________\r\n");
		richTextBox1->AppendText(" ______________________________________________________________________________________\r\n");
		richTextBox1->AppendText(" ______________________________________________________________________________________\r\n");
		richTextBox1->AppendText(" ______________________________________________________________________________________\r\n");

		richTextBox1->AppendText("Ф И О, должность, подпись______________________________________________________" + t.tm_mday + month + (t.tm_year + 1900) + " г. " + t.tm_hour + " :" + t.tm_min);


		if (LocalMetro_working == true && sonde_ro_calk == true) {//если работаем на временном файле
			
			if (sonde_on_off == true) {// если зонд включен
				const char* Metrology = "metrology_from_sonde_tmp.bin";
				ifstream metro_in;
				// читаем метрологию из файла-буфера во временную структуру
				metro_in.open(Metrology, ios::binary);
				if (metro_in.is_open()) {
					metro_in.read((char*)&metrology_struct_temp, sizeof(metrology_struct_temp));
					metro_in.close();
					// прописываем метрологию в прибор
					if (AMK_SetIdle(ComPort)) {
						this->timer1->Enabled = false;
						Sleep(200);
						if (AMK_SetMetroData(ComPort, &metrology_struct_temp)) {
							Sleep(200);
							AMK_BeginCyclogram(ComPort);
							LocalMetro_working = false;
							this->buttonLocalMetro->BackColor = DefaultBackColor;
							buttonR9->PerformClick();
							//this->timer1->Enabled = true;
							//уничтожаем файл-буфер
							for (int i = 0; i < 8; i++) {
								metrology_struct.Air_zz[i] = 0;
							}
							
							if (!remove(Metrology)) 
							MessageBox::Show("Файл метрологии возвращен в прибор!");
							else MessageBox::Show("Не удалось стереть файл временного хранения метрологии");

							// сохраняем рапорт
							saveFileDialog1->Filter = "text files (*.rtf)|*.rtf";
							if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
								try {
									richTextBox1->SaveFile(saveFileDialog1->FileName);
									for (int i = 24; i < 96; i++) {
										TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel3->Controls["textBox" + (i).ToString()];
										TEXTBOX->Text = "";
									}
									button_ON_OFF->PerformClick();
								}
								catch (Exception^ e) {
									MessageBox::Show(L"Ошибка сохранения файла");
								}
								return;
							}

						}
						else MessageBox::Show("Файл метрологии Не возвращен в прибор!, попробуйте еще раз");
					}
					else MessageBox::Show("Файл метрологии Не возвращен в прибор!!, попробуйте еще раз");

				}
				else MessageBox::Show("невозможно открыть файл");
			}
			
			else if (sonde_on_off == false ) {
				MessageBox::Show("Включите прибор !");
			}
						
		}

		else {// сохраняем рапорт
		    this->buttonLocalMetro->BackColor = DefaultBackColor;
			for (int i = 0; i < 8; i++) {
				metrology_struct.Air_zz[i] = 0;
			}
			saveFileDialog1->Filter = "text files (*.rtf)|*.rtf";
			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				try {
					richTextBox1->SaveFile(saveFileDialog1->FileName);
					
				}
				catch (Exception^ e) {
					MessageBox::Show(L"Ошибка сохранения файла");
				}
				for (int i = 24; i < 96; i++) {
					TextBox^ TEXTBOX = (TextBox^)this->tableLayoutPanel3->Controls["textBox" + (i).ToString()];
					TEXTBOX->Text = "";
				}
				button_ON_OFF->PerformClick();
				return;
				
			}
		}
	}
	

	//cout << "working " << working << "LWD_working " << LWD_working << endl;
	if (working == true) this->timer1->Enabled = true;
}
	
private: System::Void параметрыToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	PARAMETRS::MyForm1^ Config = gcnew PARAMETRS::MyForm1;
	Config->Show();
}

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	wiew = comboBox1->SelectedIndex;
	if (wiew == 0) {
		this->textBox17->Text = L"-----5%";
		this->textBox18->Text = L"-----10%";
		this->textBox19->Text = L"-----15%";
		this->textBox20->Text = L"-----мгновенное";
		this->textBox22->Text = L"-----среднее";
		this->textBox21->Text = L"";
	}
	if (wiew == 1) {
		this->textBox17->Text = L"";
		this->textBox18->Text = L"";
		this->textBox19->Text = L"";
		this->textBox20->Text = L"-----мгновенное";
		this->textBox22->Text = L"-----среднее";
		this->textBox21->Text = L"-----0";
	}
	
	if (wiew == 2) {
		this->textBox17->Text = L"";
		this->textBox18->Text = L"";
		this->textBox19->Text = L"границы";
		this->textBox20->Text = L"--разность фаз";
		this->textBox22->Text = L"";
		this->textBox21->Text = L"";
	}

	if (wiew == 3) {
		this->textBox17->Text = L"";
		this->textBox18->Text = L"";
		this->textBox19->Text = L"";
		this->textBox20->Text = L"--разность фаз";
		this->textBox22->Text = L"";
		this->textBox21->Text = L"";
	}

}

private: System::Void MyForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	fout->close();
}

private: System::Void digit(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
}

private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	if (start == 0) {
		remove("LOOP_TEST.exe");
		MoveFile("LOOP_TEST_NEW.exe", "LOOP_TEST.exe");

		CHAR path[MAX_PATH];
		GetModuleFileNameA(NULL, path, MAX_PATH);
		string path_str(path);
		std::cout << path_str << endl;
		path_str = path_str.substr(0, path_str.find_last_of("\\"));
		std::cout << path_str << endl;

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
		
		HINTERNET hInet, hUrl;
		ULONG timeout = 2000; // миллисекунды.
		HRESULT res=-1;
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

		cout << "hInet " << hInet<<" hUrl "<< hUrl << endl;
		if (hUrl!=0) {
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

						cout << "res " << res0 << " " << res1 << " " << res2  << endl;

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

private: System::Void работаСФайломМетрологииToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	AIRTEMP10::MyForm1^ Metro = gcnew AIRTEMP10::MyForm1;
	Metro->Show();
}

private: System::Void buttonLocalMetro_Click(System::Object^  sender, System::EventArgs^  e) {
	extern bool sonde_on_off;
	

	if (sonde_on_off == true) {
		if (this->timer1->Enabled == true){
			if (bt_on == true) {
				buttonR9->PerformClick();
			}
			else {
				button_BT_ON->PerformClick();
			}
			//MessageBox::Show("Убедитесь, что кольцо выключено, или находится в режиме AIR ");
			this->buttonLocalMetro->BackColor = DefaultBackColor;
		
		this->buttonR9->Enabled = false;
		this->buttonR1->Enabled = false;
		this->buttonR2->Enabled = false;
		this->buttonR3->Enabled = false;
		this->buttonR4->Enabled = false;
		this->buttonR5->Enabled = false;
		this->buttonR6->Enabled = false;
		this->buttonR7->Enabled = false;
		this->buttonR8->Enabled = false;
		this->button_Report->Enabled = false;
		
		extern int N_aver;
		N_aver_buff = N_aver;

		Local_metro_is_click = true;
	    }
		
	}
	else  MessageBox::Show("Включите зонд!");
	

}

private: System::Void button_Auto_Click(System::Object^  sender, System::EventArgs^  e) {
	extern bool sonde_on_off;
	if (sonde_on_off == true) {
		
		if (auto_loop == false) {
			for (int i = 1; i < 9; i++) {
				CLEAR(i);
			}
			
			//this->buttonR9->Enabled = false;
			//this->buttonR1->Enabled = false;
			this->buttonR2->Enabled = false;
			this->buttonR3->Enabled = false;
			this->buttonR4->Enabled = false;
			this->buttonR5->Enabled = false;
			this->buttonR6->Enabled = false;
			this->buttonR7->Enabled = false;
			this->buttonR8->Enabled = false;
			//this->button_Report->Enabled = false;
			
			button_Auto->BackColor = Color::Red;
			auto_loop = true;
			buttonR1->PerformClick();
		}
		else {

			//this->buttonR9->Enabled = true;
			//this->buttonR1->Enabled = true;
			this->buttonR2->Enabled = true;
			this->buttonR3->Enabled = true;
			this->buttonR4->Enabled = true;
			this->buttonR5->Enabled = true;
			this->buttonR6->Enabled = true;
			this->buttonR7->Enabled = true;
			this->buttonR8->Enabled = true;
			//this->button_Report->Enabled = true;

			button_Auto->BackColor = DefaultBackColor;
			auto_loop = false;
		}
	}
	else  MessageBox::Show("Включите зонд! ");
}

private: System::Void button_Report_BackColorChanged(System::Object^  sender, System::EventArgs^  e) {
	ofstream Temerature_loop_out;
	ifstream Temerature_loop_in;
	extern V8 Ro_percent_temperature[200][10];
	if (temperature_loop == true && button_Report->BackColor == Color::PaleGreen ) {
		button_Report->PerformClick();
		int t =  int(Temp) + 50;
		cout << " t " << t << " Temp " << Temp << endl;
		for (int i = 0; i < 10; ++i) {
			Ro_percent_temperature[t][i] = Ro_percent_for_print[i];
		}
		
		Temerature_loop_out.open("Temerature_loop", ios::binary);
		Temerature_loop_out.write((char*)Ro_percent_temperature, sizeof(Ro_percent_temperature));
		Temerature_loop_out.close();

		if (N_R_loop < 7)  N_sw = N_R_loop + 2; 
		if (N_R_loop == 7) N_sw = 1; 
		R_switch(N_sw);
		cout << " N_R_loop2 " << N_R_loop << endl;
	
	}

	if (auto_loop == true && button_Report->BackColor == Color::PaleGreen) {
		if (N_R_loop < 7) {
			button_Report->PerformClick();
			N_sw = N_R_loop + 2; 
			cout << " N_R_loop " << N_R_loop << endl;
			R_switch(N_sw);
			
		}
		else if (N_R_loop == 7) {
			button_Report->PerformClick();
			cout << " N_R_loop " << N_R_loop << endl;
			buttonR9->PerformClick();
			button_Auto->PerformClick();
		}
		
	}

}

private: System::Void button_Clear_Click(System::Object^  sender, System::EventArgs^  e) {
	METROLOGY metrology_struct;
	extern double R_loop_physical[9];
	extern double loop_radius;
	extern V8 Ro_percent_temperature[200][10];
	ofstream Temerature_loop_out;
	Temerature_loop_out.open("Temerature_loop", ios::binary);
	Temerature_loop_out.write((char*)Ro_percent_temperature, sizeof(Ro_percent_temperature));
	Temerature_loop_out.close();

	for (int i = 0; i < 8; i++) {
		CLEAR(i);
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////	 
		 double mV = 4000.0f / pow(2.0f, 24.0f);
		
		 void buttonC_Click_(int n) {			
			 int stop = 0;
			 String^ Str = "";
			 String^ send_C = "";
			 String^ resive_C = "";
			 Byte resive = 0;
						
			 send_C = "CAP " + (n) + "\r\n"; resive_C = "CAP=" + (n) + " OK";
			 
			 if (bt_on == true) {	
				 if (loop_version == 2) {
					 int stop = 0;
					 String^ resive = "";
					 serialPort->DiscardInBuffer();
					 serialPort->Write("RES 1\r\n");
					 Sleep(100);
					 serialPort->DiscardInBuffer();
					 serialPort->Write(send_C);
					 Sleep(100);
					 try { resive = serialPort->ReadLine(); }
					 catch (...) { stop = 1; }
					 if (!stop) {
						 if (resive->StartsWith(resive_C)) {
							 if (working == true)this->timer1->Enabled = true;
						 }
						 else MessageBox::Show("Нет ответа от кольца , нажмите С5 еще раз !");
					 }
					 else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 1!"); }
				 }
			 }
			 if (bt_on == false) { MessageBox::Show("Установите соединение с кольцом BT_ON "); }
		 }

private: System::Void помощьToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	//LOOP::Help_form Help;
	//Help.ShowDialog();
	LOOP::Help_form^ Help = gcnew LOOP::Help_form;
	Help->Show();
}

private: System::Void MyForm_ResizeBegin(System::Object^  sender, System::EventArgs^  e) {
	extern bool sonde_on_off;
	if (sonde_on_off == true) 
		 timer1->Enabled = false;	
}

private: System::Void MyForm_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
	extern bool sonde_on_off;
	if (sonde_on_off == true) 
		 timer1->Enabled = true;
}

private: System::Void button_Z1_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(1);
	button_Z1->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z2_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(2);
	button_Z2->BackColor = Color::Red;
	button_Z1->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z3_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(3);
	button_Z3->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z1->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z4_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(4);
	button_Z4->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z1->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z5_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(5);
	button_Z5->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z6_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(6);
	button_Z6->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z1->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z7_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(7);
	button_Z7->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z1->BackColor = DefaultBackColor;
	button_Z8->BackColor = DefaultBackColor;
}
private: System::Void button_Z8_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonC_Click_(8);
	button_Z8->BackColor = Color::Red;
	button_Z2->BackColor = DefaultBackColor;
	button_Z3->BackColor = DefaultBackColor;
	button_Z4->BackColor = DefaultBackColor;
	button_Z5->BackColor = DefaultBackColor;
	button_Z6->BackColor = DefaultBackColor;
	button_Z7->BackColor = DefaultBackColor;
	button_Z1->BackColor = DefaultBackColor;
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

private: System::Void Файл_метрологии_открыть_Click(System::Object^  sender, System::EventArgs^  e) {
	double L1[8];
	double L2[8];
	double f[8];
	double C[8];
	complex <double> Z_[8];
	const char* Metrology_in;
	unsigned int Sonde_serial;
	ifstream metro_in;
    METROLOGY metrology_struct;
	extern bool metrology_is_set;
	extern double L_R1_R2;
	openFileDialog1->Filter = "text files (*.bin)|*.bin";
	
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		Metrology_in = (char*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer();
		Text = L" METRO IN " + gcnew String(Metrology_in);
	}

	metro_in.open(Metrology_in, ios::binary);
	metro_in.read((char*)&metrology_struct, sizeof(metrology_struct));
	metro_in.close();

	for (int i = 0; i < 8; i++) {
		TextBox^ TEXTBOX_L1 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L1_" + (i + 1).ToString()];
		TEXTBOX_L1->Text = metrology_struct.L1[i].ToString();
		TextBox^ TEXTBOX_L2 = (TextBox^)this->tableLayoutPanel11->Controls["textBox_L2_" + (i + 1).ToString()];
		TEXTBOX_L2->Text = metrology_struct.L2[i].ToString();
		TextBox^ TEXTBOX_F = (TextBox^)this->tableLayoutPanel11->Controls["textBox_f_" + (i + 1).ToString()];
		TEXTBOX_F->Text = metrology_struct.F[i].ToString();
		TextBox^ TEXTBOX_C = (TextBox^)this->tableLayoutPanel11->Controls["textBox_С" + (i + 1).ToString()];
		TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];

		L1[i] = metrology_struct.L1[i] / 1000.0 ;
		L2[i] = metrology_struct.L2[i] / 1000.0;
		f[i] = metrology_struct.F[i] * 1000.0;
		//cout<< i << "L1 "  << L1[i] << " L2 " << L2[i] << " F " << f[i];			
		C[i] = (C_max_fi(f[i], 0.5, L_loop, L1[i], L2[i]));		
		//cout << " C "  << C[i] << endl;
		TEXTBOX_C->Text = ((int)(1e12*C[i])).ToString("F0");
		
		Z_[i] = Z(1.0, L_loop, C[i], f[i]);
		TEXTBOX_Z_CALC->Text =  (imag(Z_[i])).ToString("F3");

	}

}

private: System::Void button_write_Z_Click(System::Object^  sender, System::EventArgs^  e) {
	int stop = 0;
	String^ Str = "";
	String^ send_Z = "";
	String^ resive_Z = "";
	Byte resive = 0;
	double R[8] = { 1.0, };
	double Re[8] = { 0.0, };
	//Команда: ZWRITE 0 1.0 2.0\r\n
	//Ответ : WRITE Z0 OK!\r\n
	if (bt_on == true) {
		if (loop_version == 2) {
			for (int n = 0; n < 8; n++){
				int stop = 0;
				String^ resive = "";
				TextBox^ TEXTBOX_Z_LOOP = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];

				if (textBox_R_activ->Text != "") R[n] = System::Double::Parse((textBox_R_activ->Text));
				if (TEXTBOX_Z_LOOP->Text != "") Re[n] = System::Double::Parse((TEXTBOX_Z_LOOP->Text));
				send_Z = "ZWRITE " + n + " " + ((R[n]).ToString())->Replace(",", ".") + " " + ((Re[n]).ToString())->Replace(",", ".") + "\r\n";
				Console::WriteLine("send_Z " + send_Z);
				resive_Z = "WRITE Z" + (n) + " OK!";
				serialPort->DiscardInBuffer();
				serialPort->Write(send_Z);
				Sleep(100);
				try { resive = serialPort->ReadLine(); }
				catch (...) { stop = 1; }
				if (!stop) {					
					if (resive->StartsWith(resive_Z)) {
						if (working == true)this->timer1->Enabled = true;
						TEXTBOX_Z_LOOP->BackColor = Color::White;
					}
					else MessageBox::Show("Нет ответа от кольца , нажмите  еще раз !");
				}
				else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 2!"); }
			}
		}
	}
	if (bt_on == false) { MessageBox::Show("Установите соединение с кольцом BT_ON "); }
}

private: System::Void button_read_loop_Z_Click(System::Object^  sender, System::EventArgs^  e) {
	int stop = 0;
	String^ Str = "";
	String^ send_Z = "";
	String^ resive_Z = "";
	Byte resive = 0;
	double R[11];
	double Re[11];
	extern bool Z_downloaded_from_loop ;
	//ZREAD 0\r\n
	//READ Z0 1.0 2.0\r\n
	if (bt_on == true) {
		if (loop_version == 2) {
			int stop = 0;
			String^ resive = "";
			for (int n = 0; n < 10; n++) {
				send_Z = "ZREAD " + (n) + "\r\n"; resive_Z = "Z" + (n) + " ";
				serialPort->DiscardInBuffer();
				serialPort->Write(send_Z);
				Sleep(100);
				try { resive = serialPort->ReadLine(); }
				catch (...) { stop = 1; }
				if (!stop) {
					//Console::WriteLine("resive " + resive);
					if (resive->StartsWith(resive_Z)) {
						Str = resive->Substring(3);//минусуем 	
						//Console::WriteLine("Str " + Str);
						int index_0 = Str->IndexOf(" ");
						R[n] = Double::Parse((Str->Substring(0, index_0))->Replace(".", ","));
						Re[n] = Double::Parse((Str->Substring(index_0 + 1))->Replace(".", ","));
						Re_from_loop[n] = Re[n];
						Console::WriteLine(n + "R " + R[n] + " Re " + Re[n]);
						TextBox^ TEXTBOX_Re = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (n + 1).ToString()];
						TEXTBOX_Re->Text = Re[n].ToString("F4");
						TEXTBOX_Re->BackColor = Color::White;
						if (working == true)this->timer1->Enabled = true;
						Z_downloaded_from_loop = 1;
					}
					else { 
						Z_downloaded_from_loop = 0;
						MessageBox::Show("Нет ответа от кольца , нажмите С5 еще раз !"); 
					}
				}
				else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 3!"); }
			}			
			 
		}
	}
	if (bt_on == false) { MessageBox::Show("Установите соединение с кольцом BT_ON "); }

}

private: System::Void button_erase_Click(System::Object^  sender, System::EventArgs^  e) {
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
		if (loop_version == 2) {
			int stop = 0;
			String^ resive = "";
			if (textBox_Z_loop1->Text != "") Re = System::Double::Parse((textBox_Z_loop1->Text));
			send_Z = "ZERASE\r\n";
			resive_Z = "ERASE OK!";

			serialPort->DiscardInBuffer();
			serialPort->Write(send_Z);
			Sleep(100);
			try { resive = serialPort->ReadLine(); }
			catch (...) { stop = 1; }
			if (!stop) {				
				if (resive->StartsWith(resive_Z)) {
					button_read_loop_Z->PerformClick();
					if (working == true)this->timer1->Enabled = true;
				}
				else MessageBox::Show("Нет ответа от кольца , нажмите  еще раз !");
			}
			else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 4!"); }
		}
	}
	if (bt_on == false) { MessageBox::Show("Установите соединение с кольцом BT_ON "); }



}

private: System::Void digit2(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
	if ((e->KeyChar) == '-') e->Handled = false; if ((e->KeyChar) == ',') e->Handled = false;
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
		BOOL Transfer1 = FtpPutFile(hFtpSession, "C:\\WORK\\repos\\LOOP\\Release\\LOOP.exe", "LOOP_TEST.exe", FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_RELOAD);
		BOOL Transfer2 = FtpPutFile(hFtpSession, "C:\\WORK\\repos\\LOOP\\Release\\AMK_1DDS_AUTONOM.dll", "AMK_1DDS_AUTONOM.dll", FTP_TRANSFER_TYPE_BINARY, INTERNET_FLAG_RELOAD);
		if (Transfer0 == 1 && Transfer1 == 1 && Transfer2 == 1)
			MessageBox::Show("файлы программы записаны на FTP сервер");
		else
			MessageBox::Show("файлы НЕ БЫЛИ записаны, попробуйте еще раз");

		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
	}


		

}

private: System::Void button_calc_z_to_loop_Click(System::Object^  sender, System::EventArgs^  e) {
	for (int i = 0; i < 8; i++) {
		TextBox^ TEXTBOX_Z_CALC = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_calc" + (i + 1).ToString()];
		TextBox^ TEXTBOX_Z_LOOP = (TextBox^)this->tableLayoutPanel11->Controls["textBox_Z_loop" + (i + 1).ToString()];

		if (TEXTBOX_Z_CALC->Text != "") TEXTBOX_Z_LOOP->Text  = (System::Double::Parse((TEXTBOX_Z_CALC->Text))).ToString("F4");
	}
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
		if (loop_version == 2) {
			int stop = 0;
			String^ resive = "";				
			send_Z = "ZWRITE " + 9 + " " + signature_to_loop + " " + signature_to_loop + "\r\n";
			Console::WriteLine("send_Z " + send_Z);
			resive_Z = "WRITE Z" + (9)+" OK!";
			serialPort->DiscardInBuffer();
			serialPort->Write(send_Z);
			Sleep(100);
			try { resive = serialPort->ReadLine(); }
			catch (...) { stop = 1; }
			if (!stop) {
				if (resive->StartsWith(resive_Z)) {
					if (working == true)this->timer1->Enabled = true;					
				}
					else MessageBox::Show("Нет ответа от кольца , нажмите  еще раз !");
			}
			else { this->button_BT_ON->BackColor = DefaultBackColor; MessageBox::Show("Соединение с кольцом прервано переподключите BT_ON 5!"); }
			
		}
	}
	if (bt_on == false) { MessageBox::Show("Установите соединение с кольцом BT_ON "); }


}
};
}
