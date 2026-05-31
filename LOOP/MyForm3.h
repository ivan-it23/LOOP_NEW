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
#include "MyForm2.h"
#include "MyForm3.h"
#include <typeinfo>
#include <winuser.h>
#include <locale.h>


namespace LOOP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	/// <summary>
	/// Сводка для MyForm3
	/// </summary>
	public ref class MyForm3 : public System::Windows::Forms::Form
	{
	public:
		MyForm3(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			extern vector <DAT> Temp_data;
			extern int min_n, max_n;
			extern struct V8 Phase_shift_chart;
			extern string Metrology_Name;
			min_n = 0;
			max_n = Temp_data.size()-2;
			Co = Temp_data.size()-2;
			comboBox1->SelectedIndex = 2;
			comboBox2->SelectedIndex = 1;


			do_chart();
		
			


		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm3()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;





	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  comboBox2;

	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;

	private: System::Windows::Forms::ToolStripMenuItem^  start_point;
	private: System::Windows::Forms::ToolStripMenuItem^  end_point;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
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
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->start_point = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->end_point = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tableLayoutPanel1->SuspendLayout();
			this->tableLayoutPanel3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel3, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->chart1, 0, 1);
			this->tableLayoutPanel1->Location = System::Drawing::Point(16, 15);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 3;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 7.589286F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 92.41071F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 5)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(1341, 601);
			this->tableLayoutPanel1->TabIndex = 1;
			// 
			// tableLayoutPanel3
			// 
			this->tableLayoutPanel3->ColumnCount = 12;
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				27)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				210)));
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel3->Controls->Add(this->label6, 7, 0);
			this->tableLayoutPanel3->Controls->Add(this->button5, 6, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox1, 8, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox9, 5, 0);
			this->tableLayoutPanel3->Controls->Add(this->checkBox1, 1, 0);
			this->tableLayoutPanel3->Controls->Add(this->textBox8, 4, 0);
			this->tableLayoutPanel3->Controls->Add(this->comboBox1, 2, 0);
			this->tableLayoutPanel3->Controls->Add(this->comboBox2, 3, 0);
			this->tableLayoutPanel3->Controls->Add(this->button1, 11, 0);
			this->tableLayoutPanel3->Controls->Add(this->button2, 10, 0);
			this->tableLayoutPanel3->Location = System::Drawing::Point(3, 2);
			this->tableLayoutPanel3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
			this->tableLayoutPanel3->RowCount = 1;
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel3->Size = System::Drawing::Size(1335, 37);
			this->tableLayoutPanel3->TabIndex = 90;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(665, 0);
			this->label6->Margin = System::Windows::Forms::Padding(32, 0, 0, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 17);
			this->label6->TabIndex = 82;
			this->label6->Text = L"курсор";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// button5
			// 
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button5->Location = System::Drawing::Point(573, 0);
			this->button5->Margin = System::Windows::Forms::Padding(0, 0, 7, 0);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(53, 23);
			this->button5->TabIndex = 30;
			this->button5->Text = L">>";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm3::do_);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox1->Location = System::Drawing::Point(718, 0);
			this->textBox1->Margin = System::Windows::Forms::Padding(0, 0, 7, 0);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(100, 22);
			this->textBox1->TabIndex = 10;
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(490, 0);
			this->textBox9->Margin = System::Windows::Forms::Padding(0, 0, 7, 0);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(76, 22);
			this->textBox9->TabIndex = 16;
			this->textBox9->TextChanged += gcnew System::EventHandler(this, &MyForm3::max_n_change);
			this->textBox9->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm3::digit2);
			// 
			// checkBox1
			// 
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->checkBox1->Location = System::Drawing::Point(34, 0);
			this->checkBox1->Margin = System::Windows::Forms::Padding(7, 0, 0, 0);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(61, 23);
			this->checkBox1->TabIndex = 55;
			this->checkBox1->Text = L"f(T)";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(407, 0);
			this->textBox8->Margin = System::Windows::Forms::Padding(16, 0, 7, 0);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(76, 22);
			this->textBox8->TabIndex = 15;
			this->textBox8->TextChanged += gcnew System::EventHandler(this, &MyForm3::min_n_change);
			this->textBox8->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm3::digit2);
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownHeight = 230;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->IntegralHeight = false;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"PH Z1-Z2", L"AM Tx", L"Temp RZZ", L"PH_RX_Z1-Z2",
					L" DELTA FI"
			});
			this->comboBox1->Location = System::Drawing::Point(95, 0);
			this->comboBox1->Margin = System::Windows::Forms::Padding(0, 0, 7, 0);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(195, 24);
			this->comboBox1->TabIndex = 22;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm3::comboBox1_SelectedIndexChanged);
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"400 kGz", L"2000 kGz" });
			this->comboBox2->Location = System::Drawing::Point(297, 0);
			this->comboBox2->Margin = System::Windows::Forms::Padding(0, 0, 7, 0);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(87, 24);
			this->comboBox2->TabIndex = 31;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm3::comboBox2_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(1035, 4);
			this->button1->Margin = System::Windows::Forms::Padding(0, 4, 0, 4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(300, 28);
			this->button1->TabIndex = 83;
			this->button1->Text = L" ТЕМПЕРАТУРНАЯ  ПОГРЕШНОСТЬ";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm3::button1_Click);
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(825, 4);
			this->button2->Margin = System::Windows::Forms::Padding(0, 4, 0, 4);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(210, 28);
			this->button2->TabIndex = 91;
			this->button2->Text = L"ФАЗЫ НА ВОЗДУХЕ";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm3::Air_Phase_Click);
			// 
			// chart1
			// 
			this->chart1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea2->CursorX->IsUserEnabled = true;
			chartArea2->CursorX->IsUserSelectionEnabled = true;
			chartArea2->CursorY->IsUserEnabled = true;
			chartArea2->CursorY->IsUserSelectionEnabled = true;
			chartArea2->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea2);
			this->chart1->ContextMenuStrip = this->contextMenuStrip1;
			legend2->Name = L"Legend1";
			legend2->Position->Auto = false;
			legend2->Position->Height = 5;
			legend2->Position->Width = 40;
			legend2->Position->X = 30;
			legend2->Position->Y = 1;
			this->chart1->Legends->Add(legend2);
			this->chart1->Location = System::Drawing::Point(3, 47);
			this->chart1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->chart1->Name = L"chart1";
			series9->BorderWidth = 2;
			series9->ChartArea = L"ChartArea1";
			series9->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series9->Color = System::Drawing::Color::LimeGreen;
			series9->Legend = L"Legend1";
			series9->Name = L"T1";
			series10->BorderWidth = 2;
			series10->ChartArea = L"ChartArea1";
			series10->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series10->Color = System::Drawing::Color::Gold;
			series10->Legend = L"Legend1";
			series10->Name = L"T2";
			series11->BorderWidth = 2;
			series11->ChartArea = L"ChartArea1";
			series11->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series11->Color = System::Drawing::Color::Red;
			series11->Legend = L"Legend1";
			series11->Name = L"T3";
			series12->BorderWidth = 2;
			series12->ChartArea = L"ChartArea1";
			series12->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series12->Color = System::Drawing::Color::RoyalBlue;
			series12->Legend = L"Legend1";
			series12->Name = L"T4";
			series13->BorderWidth = 2;
			series13->ChartArea = L"ChartArea1";
			series13->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series13->Color = System::Drawing::Color::SaddleBrown;
			series13->IsVisibleInLegend = false;
			series13->Legend = L"Legend1";
			series13->Name = L"5";
			series14->BorderWidth = 2;
			series14->ChartArea = L"ChartArea1";
			series14->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series14->Color = System::Drawing::Color::Gold;
			series14->IsVisibleInLegend = false;
			series14->Legend = L"Legend1";
			series14->Name = L"6";
			series15->BorderWidth = 2;
			series15->ChartArea = L"ChartArea1";
			series15->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series15->Color = System::Drawing::Color::Red;
			series15->IsVisibleInLegend = false;
			series15->Legend = L"Legend1";
			series15->Name = L"7";
			series16->BorderWidth = 2;
			series16->ChartArea = L"ChartArea1";
			series16->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series16->Color = System::Drawing::Color::RoyalBlue;
			series16->IsVisibleInLegend = false;
			series16->Legend = L"Legend1";
			series16->Name = L"8";
			this->chart1->Series->Add(series9);
			this->chart1->Series->Add(series10);
			this->chart1->Series->Add(series11);
			this->chart1->Series->Add(series12);
			this->chart1->Series->Add(series13);
			this->chart1->Series->Add(series14);
			this->chart1->Series->Add(series15);
			this->chart1->Series->Add(series16);
			this->chart1->Size = System::Drawing::Size(1335, 546);
			this->chart1->TabIndex = 10;
			this->chart1->Text = L"CCCC";
			this->chart1->TextAntiAliasingQuality = System::Windows::Forms::DataVisualization::Charting::TextAntiAliasingQuality::Normal;
			this->chart1->CursorPositionChanged += gcnew System::EventHandler<System::Windows::Forms::DataVisualization::Charting::CursorEventArgs^ >(this, &MyForm3::cursor_position);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->ImageScalingSize = System::Drawing::Size(18, 18);
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->start_point,
					this->end_point
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(149, 52);
			// 
			// start_point
			// 
			this->start_point->Name = L"start_point";
			this->start_point->Size = System::Drawing::Size(148, 24);
			this->start_point->Text = L"start_point";
			this->start_point->Click += gcnew System::EventHandler(this, &MyForm3::start_point_Click);
			// 
			// end_point
			// 
			this->end_point->Name = L"end_point";
			this->end_point->Size = System::Drawing::Size(148, 24);
			this->end_point->Text = L"end_point";
			this->end_point->Click += gcnew System::EventHandler(this, &MyForm3::end_point_Click);
			// 
			// MyForm3
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1373, 629);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MyForm3";
			this->Text = L"MyForm3";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm3::MyForm3_FormClosed);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel3->ResumeLayout(false);
			this->tableLayoutPanel3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion


		int index_curve_1 = 0;// 
		int index_freq_1 = 1;//
		int Co;

		void do_chart() {
			extern vector <DAT> Temp_data;
			extern int min_n, max_n;
			extern struct V8 Phase_shift_chart ;
			for (int i = 0; i < 8; i++) {
				cout<<"1 " << Phase_shift_chart.VALUE[i] << endl;
			}


			if (max_n - min_n > 0) {
				chart1->ChartAreas["ChartArea1"]->AxisX->ScaleView->ZoomReset(10);
				chart1->ChartAreas["ChartArea1"]->AxisY->ScaleView->ZoomReset(10);
				
				for (int i = 0; i < 8; i++) {
					chart1->Series[i]->Points->Clear();
				}


				for (int n = min_n; n < max_n; n++)// количество циклов n, равное кол-ву кадров Co
				{

					double N;
					if (checkBox1->Checked == true) {
						if((int)temp_deg(Temp_data[n].TX[TX0_400].tR[0])<155) 
							N = (int)temp_deg(Temp_data[n].TX[TX0_400].tR[0]); 
						//else N = 200;
					}
					if (checkBox1->Checked == false) { N = n; }
					// выводим графики
					
					//PH Z2-Z1
					if (index_curve_1 == 0) {
						//this->label10->Text = L"Фаза";
						int k;
						if (index_freq_1 == 0)k = 1;
						if (index_freq_1 == 1)k = 6;
						for (int i = 0; i < 4; i++) {
							chart1->Series[i]->Points->AddXY(N, (arg(Temp_data[n].TX[i + k].R[Z2] / Temp_data[n].TX[i + k].R[Z1]) * mG));
						}
					}


					//AM Tx
					else if (index_curve_1 == 1) {
						int k;
						if (index_freq_1 == 0)k = 1;
						if (index_freq_1 == 1)k = 6;
						for (int i = 0; i < 4; i++) {
							chart1->Series[i]->Points->AddXY(N, real(Temp_data[n].TX[i + k].T)*mV_Tx);
						}
					}

					//Temp RZZ
					else if (index_curve_1 == 2) {
						int T = temp_deg(Temp_data[n].TX[0].tR[0]);
						if(T < 155)	
						chart1->Series[4]->Points->AddXY(N, temp_deg(Temp_data[n].TX[0].tR[0]));
						//chart1->Series[0]->Points->AddXY(N, temp_deg(Temp_data[n].TX[0].tT[0]));
						//chart1->Series[1]->Points->AddXY(N, temp_deg(Temp_data[n].TX[0].tT[1]));
						//chart1->Series[2]->Points->AddXY(N, temp_deg(Temp_data[n].TX[0].tT[2]));
						//chart1->Series[3]->Points->AddXY(N, temp_deg(Temp_data[n].TX[0].tT[3]));
					}

					//PH_RX_Z2-Z1
					else if (index_curve_1 == 3) {
						int k;
						if (index_freq_1 == 0)k = 18;
						if (index_freq_1 == 1)k = 22;
						for (int i = 0; i < 4; i++) {
							chart1->Series[i]->Points->AddXY(N, (arg(Temp_data[n].TX[i + k].R[Z2] / Temp_data[n].TX[i + k].R[Z1])*mG) );
						}
					}

					//DELTA FI
					else if (index_curve_1 == 4) {
						
						int k, k1, k2;
						if (index_freq_1 == 0) { k = 1; k1 = 18; k2 = 0;}
						if (index_freq_1 == 1) { k = 6; k1 = 22; k2 = 4;}

						double Z_Z_cal;
						for (int i = 0; i < 4; i++) {
							double Z_Z_cal = arg(Temp_data[n].TX[i + k].R[Z2] / Temp_data[n].TX[i + k].R[Z1]) 
								         - arg(Temp_data[n].TX[i + k1].R[Z2] / Temp_data[n].TX[i + k1].R[Z1]);
							chart1->Series[i]->Points->AddXY(N, Z_Z_cal* mG - Phase_shift_chart.VALUE[i+k2]);
						}
						
					}















				}
			}

		}

private: System::Void min_n_change(System::Object^  sender, System::EventArgs^  e) {
	extern int min_n, max_n;
	if (textBox8->Text != "") min_n = System::Int64::Parse(textBox8->Text);
	if (textBox8->Text == "") min_n = 0;
	if (min_n >= Co) min_n = 1;
}

private: System::Void max_n_change(System::Object^  sender, System::EventArgs^  e) {
	extern int min_n, max_n;
	if (textBox9->Text != "") max_n = System::Int64::Parse(textBox9->Text);
	if (textBox9->Text == "") max_n = Co;
	if (max_n >= Co) max_n = Co;
}

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	index_curve_1 = comboBox1->SelectedIndex;
}

private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	index_freq_1 = comboBox2->SelectedIndex;
}

private: System::Void digit2(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (!Char::IsDigit(e->KeyChar) && !Char::IsControl(e->KeyChar)) e->Handled = true;
}

private: System::Void do_(System::Object^  sender, System::EventArgs^  e) {
	do_chart();
}

private: System::Void start_point_Click(System::Object^  sender, System::EventArgs^  e) {
	extern int min_n, max_n;
	textBox8->Clear();
	min_n = int(chart1->ChartAreas["ChartArea1"]->CursorX->Position);
	if(min_n >= 0 && min_n < Co)
	textBox8->Text = min_n.ToString();
}

private: System::Void end_point_Click(System::Object^  sender, System::EventArgs^  e) {
	extern int min_n, max_n;
	textBox9->Clear();
	max_n = int(chart1->ChartAreas["ChartArea1"]->CursorX->Position);
	if (max_n >= 0 && max_n < Co)
	textBox9->Text = max_n.ToString();
}

private: System::Void cursor_position(System::Object^  sender, System::Windows::Forms::DataVisualization::Charting::CursorEventArgs^  e) {
	textBox1->Text = chart1->ChartAreas["ChartArea1"]->CursorX->Position.ToString() + "; " + int(chart1->ChartAreas["ChartArea1"]->CursorY->Position).ToString();
}

	  

bool close = false;
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	extern int min_n, max_n;
	close = true;
	MyForm3::Close();

}

private: System::Void MyForm3_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
	extern vector <DAT> Temp_data;
	extern int min_n, max_n;
	double max[8], min[8];
	if (close == false) {
		min_n = 0; max_n = 0;
		Temp_data.clear();
	}
}

private: System::Void Air_Phase_Click(System::Object^  sender, System::EventArgs^  e) {
	extern int min_n, max_n; 
	extern vector <DAT> Temp_data;
	int cntr = 0;
	extern V8 Air_Phase;
	for (int n = min_n; n < max_n; n++) {
		for (int i = 0; i < 8; i++) {
			int k;
			if (i < 4)  k = 1; else  k = 2; 
			//Air_Phase.Phase[i] += (arg(Temp_data[n].TX[i + k].R[Z2] / Temp_data[n].TX[i + k].R[Z1])
				//- arg(Temp_data[n].TX[i + 18].R[Z2] / Temp_data[n].TX[i + 18].R[Z1]));
			Air_Phase.VALUE[i] += (arg(Temp_data[n].TX[i + k].R[Z2] / Temp_data[n].TX[i + k].R[Z1]));
		
		}
		cntr++;
	}
	for (int i = 0; i < 8; i++) {
		Air_Phase.VALUE[i] /= cntr;
		cout << Air_Phase.VALUE[i]*mG << endl;
	}
	close = false;
	MyForm3::Close();
	

}

private: System::Void Button2_Click_(System::Object^  sender, System::EventArgs^  e) {
	V8 Data_Cal[800] = { 0, };
	V8 SUMM = { 0, };
	V8 WINDOW[16] = { 0, };
	V8 Data_Ma[800] = { 0, };
	int w = 0;



	for (int j = 0; j < 800; j++) {
		//Data_Cal[j].T = 0;
		for (int i = 0; i < 8; i++) {
			Data_Cal[j].VALUE[i] = 0;
		}
	}
	extern vector <DAT> Temp_data;
	V8 Buff;
	extern int min_n, max_n;
	Temp_data.clear();

	int n = 0;
	int t = 0;
	int t_cntr[800] = { 0, };
	int k;
	

	chart1->ChartAreas["ChartArea1"]->AxisX->ScaleView->ZoomReset(10);
	chart1->ChartAreas["ChartArea1"]->AxisY->ScaleView->ZoomReset(10);

	for (int i = 0; i < 8; i++) {
		chart1->Series[i]->Points->Clear();
	}
	


		for (int n = min_n; n < max_n; n++) {
			for (int i = 0; i < 8; i++) {
				if (i == 0 || i == 1 || i == 2 || i == 3) { k = 1; }
				else if (i == 4 || i == 5 || i == 6 || i == 7) { k = 2; }

				t = Temp_data[n].TX[TX0_400].tR[0];
				Data_Cal[t].VALUE[i] += (arg(Temp_data[n].TX[i + k].R[Z1] / Temp_data[n].TX[i + k].R[Z2])
					- arg(Temp_data[n].TX[i + 18].R[Z1] / Temp_data[n].TX[i + 18].R[Z2]))*mG;
				

			}
			t_cntr[t]++;
		}
		int start = 0, stop = 0;
		bool str=0, stp=0;
		for (int t = 0; t < 800; t++) {
			if (t_cntr[t] != 0 && str == false) { start = t; str = true;  }
			if (t_cntr[t] == 0 && str == true && stp == false) { stop = t-1; stp = true; }
			for (int i = 0; i < 8; i++) {
				if (t_cntr[t] != 0) {
					Data_Cal[t].VALUE[i] /= t_cntr[t];
				}
			}
		}

		
		 Data_Cal[800];
		 SUMM;
		 WINDOW[16];
		 Data_Ma[800];
		 
		 for (int t = start; t < stop; t++) {
			 w++; 
			 if (w > 15) w = 0; // 

			 for (int i = 0; i < 8; i++) {
				 SUMM.VALUE[i] -= WINDOW[w].VALUE[i];
			 }
			 for (int i = 0; i < 8; i++) {
				  WINDOW[w].VALUE[i] = Data_Cal[t].VALUE[i];
			 }
			 for (int i = 0; i < 8; i++) {
				 SUMM.VALUE[i] += WINDOW[w].VALUE[i];
			 }

			 for (int i = 0; i < 8; i++) {
				 Data_Ma[t].VALUE[i] = SUMM.VALUE[i]/16;
				 //cout << t << " " << Data_Ma[t].Phase[i] << endl;
			 }

		 }


		cout << "start " << start << " stop " << stop << endl;
		
		double  max[8], min[8];
		for (int i = 0; i < 8; i++) {
			max[i] = Data_Cal[start].VALUE[i]; min[i] = Data_Cal[start].VALUE[i];
		}

		for (int t = start ; t < stop ; t++) {
			for (int i = 0; i < 8; i++) {
				if (t_cntr[t] != 0) {
					if (Data_Cal[t].VALUE[i] > max[i])max[i] = Data_Cal[t].VALUE[i];
					if (Data_Cal[t].VALUE[i] < min[i])min[i] = Data_Cal[t].VALUE[i];
					
				}
			}
		}

		for (int i = 0; i < 8; i++) {
			cout << i << " min " << min[i] << " max " << max[i] << endl;
		}
		
		

		for (int t = 0; t < 800; t++) {
			for (int i = 4; i < 8; i++) {
				if (t_cntr[t] != 0) {
					chart1->Series[i]->Points->AddXY((int)temp_deg(t), Data_Cal[t].VALUE[i]);
				}
			}
		}
		

		cout << "min " << min_n << " max " << max_n << endl;




	

}





};
}
