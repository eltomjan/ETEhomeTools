#pragma once
#include "Cage.h"
#define WIDTH 320
#define HEIGHT 240

namespace ArduinoSimulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	[DllImport("gdi32.dll", EntryPoint = "SetPixel")]
	extern bool SetPixel(IntPtr hdc, int X, int Y, unsigned int crColor);

	/// <summary>
	/// Summary for ArduinoWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ArduinoWindow : public System::Windows::Forms::Form
	{
	public:
		ArduinoWindow(void)
		{
			InitializeComponent();
			bmp = gcnew array<UInt64>(WIDTH * HEIGHT);
			SetStyle(ControlStyles::OptimizedDoubleBuffer, true);
		}
	protected: array<UInt64>^ bmp;
	public: Void draw(Object^ sender, PaintEventArgs^ e)
		{
			IntPtr hdc = e->Graphics->GetHdc();

			for (int y = 0; y < HEIGHT; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					if (bmp[x + y * WIDTH] & (1 << 24)) {
						SetPixel(hdc, x, y, bmp[x + y * WIDTH] & 0xFFFFFF);
					}
				}
			}

			e->Graphics->ReleaseHdc(hdc);
		}

	private: System::Windows::Forms::Button^  pwrUp; 
	private: System::Windows::Forms::Button^  pwrDwn;
	private: System::Windows::Forms::HScrollBar^  hScrollBar1;
	public: static ArduinoSimulator::ArduinoWindow^ m_me;
		void setPixel(int x,int y,int r,int g,int b) {
			bmp[x + y * WIDTH] = ((1 << 24) | (b << 16) | (g << 8) | (r));
			pictureBox1->Invalidate();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ArduinoWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  buttonUp;
	private: System::Windows::Forms::Button^  buttonDwn;
	private: System::Windows::Forms::Button^  buttonSel;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		static System::Drawing::Bitmap^ m_image;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->buttonUp = (gcnew System::Windows::Forms::Button());
			this->buttonDwn = (gcnew System::Windows::Forms::Button());
			this->buttonSel = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->pwrUp = (gcnew System::Windows::Forms::Button());
			this->pwrDwn = (gcnew System::Windows::Forms::Button());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(WIDTH, HEIGHT);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ArduinoWindow::draw);

			// 
			// buttonUp
			// 
			this->buttonUp->Location = System::Drawing::Point(379, 12);
			this->buttonUp->Name = L"buttonUp";
			this->buttonUp->Size = System::Drawing::Size(99, 29);
			this->buttonUp->TabIndex = 1;
			this->buttonUp->Text = L"Up";
			this->buttonUp->UseVisualStyleBackColor = true;
			this->buttonUp->Click += gcnew System::EventHandler(this, &ArduinoWindow::buttonUp_Click);
			// 
			// buttonDwn
			// 
			this->buttonDwn->Location = System::Drawing::Point(379, 48);
			this->buttonDwn->Name = L"buttonDwn";
			this->buttonDwn->Size = System::Drawing::Size(99, 29);
			this->buttonDwn->TabIndex = 2;
			this->buttonDwn->Text = L"Down";
			this->buttonDwn->UseVisualStyleBackColor = true;
			this->buttonDwn->Click += gcnew System::EventHandler(this, &ArduinoWindow::buttonDwn_Click);
			// 
			// buttonSel
			// 
			this->buttonSel->Location = System::Drawing::Point(379, 143);
			this->buttonSel->Name = L"buttonSel";
			this->buttonSel->Size = System::Drawing::Size(99, 29);
			this->buttonSel->TabIndex = 3;
			this->buttonSel->Text = L"Select";
			this->buttonSel->UseVisualStyleBackColor = true;
			this->buttonSel->Click += gcnew System::EventHandler(this, &ArduinoWindow::buttonSel_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &ArduinoWindow::timer1_Tick);
			// 
			// pwrUp
			// 
			this->pwrUp->Location = System::Drawing::Point(379, 84);
			this->pwrUp->Name = L"pwrUp";
			this->pwrUp->Size = System::Drawing::Size(99, 23);
			this->pwrUp->TabIndex = 4;
			this->pwrUp->Text = L"pwrUp";
			this->pwrUp->UseVisualStyleBackColor = true;
			this->pwrUp->Click += gcnew System::EventHandler(this, &ArduinoWindow::pwrUp_Click);
			// 
			// pwrDwn
			// 
			this->pwrDwn->Location = System::Drawing::Point(379, 114);
			this->pwrDwn->Name = L"pwrDwn";
			this->pwrDwn->Size = System::Drawing::Size(99, 23);
			this->pwrDwn->TabIndex = 5;
			this->pwrDwn->Text = L"pwrDwn";
			this->pwrDwn->UseVisualStyleBackColor = true;
			this->pwrDwn->Click += gcnew System::EventHandler(this, &ArduinoWindow::pwrDwn_Click);
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Location = System::Drawing::Point(380, 205);
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(110, 17);
			this->hScrollBar1->TabIndex = 6;
			this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &ArduinoWindow::hScrollBar1_Scroll);
			// 
			// ArduinoWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(490, 300);
			this->Controls->Add(this->hScrollBar1);
			this->Controls->Add(this->pwrDwn);
			this->Controls->Add(this->pwrUp);
			this->Controls->Add(this->buttonSel);
			this->Controls->Add(this->buttonDwn);
			this->Controls->Add(this->buttonUp);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"ArduinoWindow";
			this->Text = L"Arduino Simulator";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			m_me = this;
		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 static bool first = true;
				 	if(first) setup();
					first = false;
				 	isr();
					neverEnd();
			 }

private: System::Void buttonUp_Click(System::Object^  sender, System::EventArgs^  e) {
			 portsState(1,0,0,0);
		 }
private: System::Void buttonDwn_Click(System::Object^  sender, System::EventArgs^  e) {
			 portsState(2,0,0,0);
		 }
private: System::Void pwrUp_Click(System::Object^  sender, System::EventArgs^  e) {
			 portsState(3,0,0,0);
		 }
private: System::Void pwrDwn_Click(System::Object^  sender, System::EventArgs^  e) {
			 portsState(4,0,0,0);
		 }
private: System::Void buttonSel_Click(System::Object^  sender, System::EventArgs^  e) {
			 portsState(5,0,0,0);
		 }
private: System::Void hScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
			 portsState(6,(e)->NewValue,0,0);
		 }
	};
}

