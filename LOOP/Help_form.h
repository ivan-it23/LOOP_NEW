#pragma once

namespace LOOP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для Help_form
	/// </summary>
	public ref class Help_form : public System::Windows::Forms::Form
	{
	public:
		Help_form(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			tabControl1->SelectedIndex = 1;
			auto font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			richTextBox_pic->Font = font;
			richTextBox_main->Font = font;
			richTextBox_metro->Font = font;
			richTextBox_poverka->Font = font;

			richTextBox_pic->Text = "Порядок работы : \r\n"
				+ "Установите прибор на деревянные козлы на высоте не менее 1.5 метра над землей, как можно дальше.\r\n"
				+ "от массивных металлических предметов. Установите поверочное кольцо на прибор  строго в точке записи. \r\n"
				;

			richTextBox_main->Text = "1) Подключите прибор через УСИ к компьютеру и подайте питание.\r\n"
				+ "2) Нажмите 'ВКЛЮЧИТЬ ПРИБОР' дождитесь надписи  'ПРИБОР ВКЛЮЧЕН' \r\n"		
				+ "проконтролируйте работу прибора по изменениям показаний фазы на графиках. \r\n"
				+ "3) Включите питание поверочного кольца тумблером.  \r\n"
				+ "4) Установите COM_PORT соединения с поверочным кольцом по BT или кабелю. \r\n"
				+ "5) Нажмите 'СОЕДИНЕНИЕ С КОЛЬЦОМ' дождитесь синей подсветки кнопки \r\n"
				+ "Проконтролируйте показания прибора при разомкнутом кольце - кнопка 'AIR'. \r\n"
				+ "Фазы должны быть близки к 0 (не более +- 50 mG) и находиться в зеленой или желтой зоне. \r\n"
				+ "6) Нажмите 'ЛОКАЛЬНАЯ МЕТРОЛОГИЯ' \r\n"
				+ "начнется 30ти секундный сбор данных для введения поправок на воздух в данных условиях нахожнения прибора. \r\n"
				+ "Кнопка 'ЛОКАЛЬНАЯ МЕТРОЛОГИЯ' будет подсвечена красным. \r\n"				
				+ "7) Поочередным нажатием кнопок 'R1'-'R8' произведите измерения на различных сопротивлениях поверочного кольца. \r\n"
				+ "После нажатия каждой из этих кнопок на кольце включается активное сопротивление, соответствующее данной кнопке. \r\n"
				+ "Далее, поочередно для каждого зонда, на кольце устанавливается необходимое реактивное сопротивление и проводится  \r\n"
				+ " серия из N  измерений, которые отображаются на соответствующем графике и усредняются.  \r\n"
				+  "Значение N устанавливается в выпадающем меню усреднение"
				+ "По окончании измерений для даннго сопротивления, результаты выводятся в таблицу.  \r\n"
				+ "Для отправки в рапорт колонка помечается галочкой."
				+ "При нажатии кнопки 'РАПОРТ' результаты измерений записываются в файл.  \r\n"
				+ "Измерения для любой кнопки можно производить многократно и в любой последовательности.  \r\n"
				+ "7 A) При нажатии кнопки 'АВТО' производятся поочередные нажатия кнопок 'R1'-'R8' в атоматическом режиме."
				+ "Все результаты также,как и в ручном режиме, отображаются в таблице ."
				+ "По окончании режима 'АВТО' кольцо переводится в 'AIR'."
				+ "Вы можете произвести любое из 'R1'-'R8'измерений, перезапишуться данные только для этого измерения."
				+ "8) По окончании всех измерений и сбора результатов нажмите 'РАПОРТ' . \r\n";
			
			richTextBox_metro->Text = "Порядок работы с файлом метрологии: \r\n"
				+ "При включенном приборе откройте форму Меню-Работа с файлом метрологии. \r\n"
				+ "Вы можете выгружать из прибора, изменять и загружать в прибор файл метрологии. \r\n"
				+ "При выгрузке данные отображаются в таблице. Здесь их можно изменить. Загружается то, что отображено в таблице \r\n"				
				+ "При закрытии формы в прибор ничего не записывается и в таблице не сохраняется. \r\n";
			/*
			richTextBox_poverka->Text = 
				"В память платы управления кольцом записаны активные и реактивные сопротивления кольца для каждого из зондов.\r\n"
				+ "Порядок поверки и настройки кольца-иммитатора: \r\n"
				+ "1) Включите кольцо с помощью кнопки 'Соединение с кольцом' .\r\n"
				+ "2) При нажатии кнопки 'ПРОЧИТАТЬ' данные считываются и отображаются в таблице.\r\n"
				+ " их можно отредактировать и записать обратно с помощью кнопки 'ЗАПИСАТЬ'.\r\n"
				+ "3)Кнопка 'СТЕРЕТЬ' стирает все данные (заполняет нулями).\r\n"
				+ "Для измерения импедансов подключите кольцо к измерителю импеданса с поиощью специального кабеля .\r\n"
				+ "4) Поочередным нажатием кнопок'Z1' - 'Z10' подключаются соответствующие конденсаторы, \r\n"
				+ "активное сопротивление для всех кнопок - 1 Ом. \r\n"
				+ "Производится измерение и реактивная часть записывается в соответствующую ячейку.\r\n"
				+ "После серии измерений данные записываются в кольцо с помощью кнопки 'ЗАПИСАТЬ'.\r\n"
				+ "При  нажатии кнопки 'РАССЧИТАТЬ ЕМКОСТИ' появляется диалоговое окно загрузки файла метрологии. \r\n"
				+ "При выборе файла метрологии производится расчет емкостей для данного типа прибора.\r\n"
				+ "Данные о длинах и частотах прибора,емкостях и рассчетных импедансах отображаются в таблице.\r\n"
				+ "Если поверка не производится, то можно записать расчетные реактивные сопротивления, вместо измеряных.\r\n"
				+ "С помощью кнопки 'Записать Z измеряное' переносятся в соответствующие ячейки.\r\n"
				+ "После записываются в прибор с помощью кнопки 'ЗАПИСАТЬ' .\r\n"
				+ "Сигнатура записывается отдельно .\r\n"
				;
				*/
		
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Help_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	protected:

	protected:

	protected:
	private: System::Windows::Forms::TabPage^  tabPage_main;
	private: System::Windows::Forms::TabPage^  tabPage_metro;
	private: System::Windows::Forms::TabPage^  tabPage_poverka;
	private: System::Windows::Forms::TabPage^  tabPage_pic;
	private: System::Windows::Forms::RichTextBox^  richTextBox_main;
	private: System::Windows::Forms::RichTextBox^  richTextBox_metro;
	private: System::Windows::Forms::RichTextBox^  richTextBox_poverka;
	private: System::Windows::Forms::RichTextBox^  richTextBox_pic;
private: System::Windows::Forms::PictureBox^  pictureBox1;


	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Help_form::typeid));
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_main = (gcnew System::Windows::Forms::TabPage());
			this->richTextBox_main = (gcnew System::Windows::Forms::RichTextBox());
			this->tabPage_metro = (gcnew System::Windows::Forms::TabPage());
			this->richTextBox_metro = (gcnew System::Windows::Forms::RichTextBox());
			this->tabPage_poverka = (gcnew System::Windows::Forms::TabPage());
			this->richTextBox_poverka = (gcnew System::Windows::Forms::RichTextBox());
			this->tabPage_pic = (gcnew System::Windows::Forms::TabPage());
			this->richTextBox_pic = (gcnew System::Windows::Forms::RichTextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->tabControl1->SuspendLayout();
			this->tabPage_main->SuspendLayout();
			this->tabPage_metro->SuspendLayout();
			this->tabPage_poverka->SuspendLayout();
			this->tabPage_pic->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabPage_pic);
			this->tabControl1->Controls->Add(this->tabPage_main);
			this->tabControl1->Controls->Add(this->tabPage_metro);
			this->tabControl1->Controls->Add(this->tabPage_poverka);
			this->tabControl1->Location = System::Drawing::Point(12, 51);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(621, 573);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage_main
			// 
			this->tabPage_main->Controls->Add(this->richTextBox_main);
			this->tabPage_main->Location = System::Drawing::Point(4, 22);
			this->tabPage_main->Name = L"tabPage_main";
			this->tabPage_main->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_main->Size = System::Drawing::Size(613, 547);
			this->tabPage_main->TabIndex = 0;
			this->tabPage_main->Text = L"Главная";
			this->tabPage_main->UseVisualStyleBackColor = true;
			// 
			// richTextBox_main
			// 
			this->richTextBox_main->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox_main->Location = System::Drawing::Point(6, 6);
			this->richTextBox_main->Name = L"richTextBox_main";
			this->richTextBox_main->Size = System::Drawing::Size(601, 535);
			this->richTextBox_main->TabIndex = 0;
			this->richTextBox_main->Text = L"";
			// 
			// tabPage_metro
			// 
			this->tabPage_metro->Controls->Add(this->richTextBox_metro);
			this->tabPage_metro->Location = System::Drawing::Point(4, 22);
			this->tabPage_metro->Name = L"tabPage_metro";
			this->tabPage_metro->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_metro->Size = System::Drawing::Size(613, 547);
			this->tabPage_metro->TabIndex = 1;
			this->tabPage_metro->Text = L"Файл Метрологии";
			this->tabPage_metro->UseVisualStyleBackColor = true;
			// 
			// richTextBox_metro
			// 
			this->richTextBox_metro->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox_metro->Location = System::Drawing::Point(6, 6);
			this->richTextBox_metro->Name = L"richTextBox_metro";
			this->richTextBox_metro->Size = System::Drawing::Size(601, 535);
			this->richTextBox_metro->TabIndex = 1;
			this->richTextBox_metro->Text = L"";			
			// 
			// tabPage_poverka
			// 
			this->tabPage_poverka->Controls->Add(this->richTextBox_poverka);
			this->tabPage_poverka->Location = System::Drawing::Point(4, 22);
			this->tabPage_poverka->Name = L"tabPage_poverka";
			this->tabPage_poverka->Size = System::Drawing::Size(613, 547);
			this->tabPage_poverka->TabIndex = 3;
			this->tabPage_poverka->Text = L"Поверка кольца";
			this->tabPage_poverka->UseVisualStyleBackColor = true;
			// 
			// richTextBox_poverka
			// 
			this->richTextBox_poverka->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox_poverka->Location = System::Drawing::Point(6, 6);
			this->richTextBox_poverka->Name = L"richTextBox_poverka";
			this->richTextBox_poverka->Size = System::Drawing::Size(601, 535);
			this->richTextBox_poverka->TabIndex = 1;
			this->richTextBox_poverka->Text = L"";
			// 
			// tabPage_pic
			// 
			this->tabPage_pic->Controls->Add(this->pictureBox1);
			this->tabPage_pic->Controls->Add(this->richTextBox_pic);
			this->tabPage_pic->Location = System::Drawing::Point(4, 22);
			this->tabPage_pic->Name = L"tabPage_pic";
			this->tabPage_pic->Size = System::Drawing::Size(613, 547);
			this->tabPage_pic->TabIndex = 4;
			this->tabPage_pic->Text = L"Установка кольца";
			this->tabPage_pic->UseVisualStyleBackColor = true;
			// 
			// richTextBox_pic
			// 
			this->richTextBox_pic->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox_pic->Location = System::Drawing::Point(6, 6);
			this->richTextBox_pic->Name = L"richTextBox_pic";
			this->richTextBox_pic->Size = System::Drawing::Size(601, 113);
			this->richTextBox_pic->TabIndex = 1;
			this->richTextBox_pic->Text = L"";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(6, 190);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(596, 293);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// Help_form
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(645, 628);
			this->Controls->Add(this->tabControl1);
			this->Name = L"Help_form";
			this->Text = L"СПРАВКА";
			this->tabControl1->ResumeLayout(false);
			this->tabPage_main->ResumeLayout(false);
			this->tabPage_metro->ResumeLayout(false);
			this->tabPage_poverka->ResumeLayout(false);
			this->tabPage_pic->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
