#pragma once

namespace ПрограммарасчетауглеродногоследаBETA {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Reflection;
    using namespace System::IO;

    public ref class MyForm : public Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            InitMaterials();
            if (LoadImageToPictureBox())
            {
                SetDarkThemeWithWhiteText(); // Применяем темную тему с белым текстом
            }
            SetupVisualStyles();
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::IContainer^ components;
        System::Collections::Generic::Dictionary<System::String^, double>^ emissionFactors;

    protected:
        System::Windows::Forms::ComboBox^ comboBoxMaterial;
        System::Windows::Forms::TextBox^ textBoxMass;
        System::Windows::Forms::Button^ buttonCalculate;
        System::Windows::Forms::Label^ labelResult;
        System::Windows::Forms::Label^ labelInstruction;
        System::Windows::Forms::Label^ labelSource;
        System::Windows::Forms::Label^ labelTheory;
        System::Windows::Forms::Label^ labelResultExplanation;
        System::Windows::Forms::PictureBox^ pictureBox1;

    private:
        System::Void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Text = L"Программа расчёта углеродного следа";
            this->ClientSize = System::Drawing::Size(600, 400);
            this->DoubleBuffered = true;

            // Инициализация элементов управления
            this->pictureBox1 = gcnew System::Windows::Forms::PictureBox();
            this->comboBoxMaterial = gcnew System::Windows::Forms::ComboBox();
            this->textBoxMass = gcnew System::Windows::Forms::TextBox();
            this->buttonCalculate = gcnew System::Windows::Forms::Button();
            this->labelResult = gcnew System::Windows::Forms::Label();
            this->labelInstruction = gcnew System::Windows::Forms::Label();
            this->labelSource = gcnew System::Windows::Forms::Label();
            this->labelTheory = gcnew System::Windows::Forms::Label();
            this->labelResultExplanation = gcnew System::Windows::Forms::Label();

            // Настройка PictureBox
            this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBox1->BackColor = System::Drawing::Color::FromArgb(30, 30, 30); // Темный фон по умолчанию

            // Настройка ComboBox
            this->comboBoxMaterial->Location = System::Drawing::Point(30, 30);
            this->comboBoxMaterial->Size = System::Drawing::Size(200, 21);
            this->comboBoxMaterial->FlatStyle = System::Windows::Forms::FlatStyle::Flat;

            // Настройка TextBox
            this->textBoxMass->Location = System::Drawing::Point(30, 70);
            this->textBoxMass->Size = System::Drawing::Size(200, 20);

            // Настройка Button
            this->buttonCalculate->Location = System::Drawing::Point(30, 110);
            this->buttonCalculate->Size = System::Drawing::Size(200, 30);
            this->buttonCalculate->Text = L"Рассчитать";
            this->buttonCalculate->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->buttonCalculate->Click += gcnew System::EventHandler(this, &MyForm::buttonCalculate_Click);

            // Настройка Label'ов
            SetupLabel(this->labelResult, System::Drawing::Point(30, 160), System::Drawing::Size(400, 40));
            this->labelResult->Font = gcnew System::Drawing::Font("Arial", 14, System::Drawing::FontStyle::Bold);

            SetupLabel(this->labelInstruction, System::Drawing::Point(260, 30), System::Drawing::Size(300, 150));
            this->labelInstruction->Text = L"🔍 Инструкция:\n1. Выберите материал.\n2. Введите массу в кг.\n3. Нажмите 'Рассчитать'.";

            SetupLabel(this->labelSource, System::Drawing::Point(30, 350), System::Drawing::Size(550, 30));
            this->labelSource->Text = L"Источник: IPCC Emission Factors (https://www.ipcc-nggip.iges.or.jp/)";

            SetupLabel(this->labelTheory, System::Drawing::Point(30, 200), System::Drawing::Size(550, 80));
            this->labelTheory->Text = L"Теория:\nРасчёт углеродного следа позволяет оценить выбросы CO2 от использования материалов. Это важно для анализа экологического воздействия производства, транспортировки и утилизации. Применяется в экологическом менеджменте, проектировании устойчивых продуктов и отчётности по выбросам.";

            SetupLabel(this->labelResultExplanation, System::Drawing::Point(30, 290), System::Drawing::Size(550, 60));
            this->labelResultExplanation->Text = L"Что означает результат:\nЧисло показывает количество CO2 (в кг), выделяемое при использовании указанной массы материала. Используйте для сравнения экологичности материалов, планирования сокращения выбросов или отчётности.";

            // Добавление элементов на форму
            this->Controls->Add(this->pictureBox1);
            this->Controls->Add(this->comboBoxMaterial);
            this->Controls->Add(this->textBoxMass);
            this->Controls->Add(this->buttonCalculate);
            this->Controls->Add(this->labelResult);
            this->Controls->Add(this->labelInstruction);
            this->Controls->Add(this->labelSource);
            this->Controls->Add(this->labelTheory);
            this->Controls->Add(this->labelResultExplanation);

            this->pictureBox1->SendToBack();
        }

        void SetupLabel(System::Windows::Forms::Label^ label, System::Drawing::Point location, System::Drawing::Size size)
        {
            label->Location = location;
            label->Size = size;
            label->Font = gcnew System::Drawing::Font("Arial", 9);
        }

        bool LoadImageToPictureBox()
        {
            try
            {
                System::String^ imagePath = System::IO::Path::Combine(Application::StartupPath, "field.jpg");
                if (System::IO::File::Exists(imagePath))
                {
                    this->pictureBox1->Image = System::Drawing::Image::FromFile(imagePath);
                    return true;
                }

                System::Reflection::Assembly^ assembly = System::Reflection::Assembly::GetExecutingAssembly();
                for each(System::String ^ name in assembly->GetManifestResourceNames())
                {
                    if (name->ToLower()->EndsWith("field.jpg"))
                    {
                        this->pictureBox1->Image = gcnew System::Drawing::Bitmap(assembly->GetManifestResourceStream(name));
                        return true;
                    }
                }
            }
            catch (System::Exception^ ex)
            {
                System::Windows::Forms::MessageBox::Show("Ошибка загрузки изображения: " + ex->Message);
            }
            return false;
        }

        void SetDarkThemeWithWhiteText()
        {
            // Основные цвета темной темы
            System::Drawing::Color darkBackground = System::Drawing::Color::FromArgb(45, 45, 48);
            System::Drawing::Color darkerPanel = System::Drawing::Color::FromArgb(37, 37, 38);
            System::Drawing::Color accentColor = System::Drawing::Color::FromArgb(0, 122, 204);
            System::Drawing::Color whiteText = System::Drawing::Color::White;

            // Фон формы
            this->BackColor = darkBackground;

            // Настройка текстовых элементов
            this->labelResult->ForeColor = whiteText;
            this->labelResult->BackColor = System::Drawing::Color::FromArgb(70, 70, 70);

            this->labelInstruction->ForeColor = whiteText;
            this->labelInstruction->BackColor = darkerPanel;

            this->labelSource->ForeColor = whiteText;
            this->labelSource->BackColor = darkerPanel;

            this->labelTheory->ForeColor = whiteText;
            this->labelTheory->BackColor = darkerPanel;

            this->labelResultExplanation->ForeColor = whiteText;
            this->labelResultExplanation->BackColor = darkerPanel;

            // Настройка элементов ввода
            this->comboBoxMaterial->BackColor = System::Drawing::Color::FromArgb(63, 63, 65);
            this->comboBoxMaterial->ForeColor = whiteText;

            this->textBoxMass->BackColor = System::Drawing::Color::FromArgb(63, 63, 65);
            this->textBoxMass->ForeColor = whiteText;

            this->buttonCalculate->BackColor = accentColor;
            this->buttonCalculate->ForeColor = whiteText;
            this->buttonCalculate->FlatAppearance->BorderColor = accentColor;
        }

        void SetupVisualStyles()
        {
            this->SetStyle(System::Windows::Forms::ControlStyles::AllPaintingInWmPaint |
                System::Windows::Forms::ControlStyles::UserPaint |
                System::Windows::Forms::ControlStyles::DoubleBuffer |
                System::Windows::Forms::ControlStyles::SupportsTransparentBackColor, true);
        }

        void InitMaterials()
        {
            emissionFactors = gcnew System::Collections::Generic::Dictionary<System::String^, double>();
            emissionFactors->Add(L"Уголь", 2.93);
            emissionFactors->Add(L"Нефть", 3.15);
            emissionFactors->Add(L"Природный газ", 2.75);
            emissionFactors->Add(L"Древесина", 1.80);
            emissionFactors->Add(L"Биомасса", 1.65);
            emissionFactors->Add(L"Цемент", 0.93);
            emissionFactors->Add(L"Сталь", 1.85);
            emissionFactors->Add(L"Алюминий", 11.50);
            emissionFactors->Add(L"Пластмассы", 2.50);
            emissionFactors->Add(L"Бензин", 3.00);
            emissionFactors->Add(L"Дизель", 3.20);
            emissionFactors->Add(L"Керосин", 3.10);
            emissionFactors->Add(L"Сжиженный природный газ (СПГ)", 2.70);
            emissionFactors->Add(L"Аммиак (NH3)", 2.20);
            emissionFactors->Add(L"Метанол (СН3ОН)", 1.38);
            emissionFactors->Add(L"Лесные пожары", 1.70);
            emissionFactors->Add(L"Разложение органических материалов", 1.90);
            emissionFactors->Add(L"Твердые бытовые отходы", 1.50);
            emissionFactors->Add(L"Свалки", 1.60);

            for each(System::String ^ material in emissionFactors->Keys)
                comboBoxMaterial->Items->Add(material);
        }

    private:
        System::Void buttonCalculate_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (comboBoxMaterial->SelectedItem == nullptr || System::String::IsNullOrWhiteSpace(textBoxMass->Text))
            {
                System::Windows::Forms::MessageBox::Show("Пожалуйста, выберите материал и введите массу.", "Ошибка",
                    System::Windows::Forms::MessageBoxButtons::OK,
                    System::Windows::Forms::MessageBoxIcon::Warning);
                return;
            }

            double mass;
            if (!System::Double::TryParse(textBoxMass->Text, mass))
            {
                System::Windows::Forms::MessageBox::Show("Некорректная масса. Введите число.", "Ошибка",
                    System::Windows::Forms::MessageBoxButtons::OK,
                    System::Windows::Forms::MessageBoxIcon::Error);
                return;
            }

            System::String^ material = comboBoxMaterial->SelectedItem->ToString();
            double factor = emissionFactors[material];
            double result = mass * factor;
            labelResult->Text = "Выброс CO2: " + result.ToString("F2") + " кг";
        }
    };
}