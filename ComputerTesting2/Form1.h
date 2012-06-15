#include "DoubleGraph.h"
#include "SerialCommunication.h"

#pragma once

namespace ComputerTesting2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		
		Form1(void)
		{
			InitializeComponent();
			
			updateGraphs = true;

			timeDomainData = (double*) malloc(DoubleGraph::graphWidth * sizeof(double));
			memset(timeDomainData, 0, DoubleGraph::graphWidth);
			drawingTimeDomainData = (double*) malloc(DoubleGraph::graphWidth * sizeof(double));
			
			this->Graphs = gcnew DoubleGraph(this, 10, 10);
			DataReceiveDelegate^ dataReceived = gcnew DataReceiveDelegate(ComputerTesting2::Form1::onDataReceived);
			this->SerialComm = gcnew SerialCommunication(dataReceived);
			
			this->OpenConnection();

			ThreadStart^ updateThreadstart = gcnew ThreadStart(this, &Form1::updateGraphFunction);
			this->updateGraphThread = gcnew Thread(updateThreadstart);
			this->updateGraphThread->Start();
		}

		void OpenConnection()
		{
			if(!this->SerialComm->Open())
			{
				MessageBox::Show("Failed to open serial connection");
			}
			dataIndex = 0;
			openConnectionTime = DateTime::Now;
			totalFrameCount = 0;
			totalUpdateCount = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				this->updateGraphThread->Abort();
				delete components;
				this->Graphs->~DoubleGraph();
			}
		}

		void updateGraphFunction()
		{
			drawStartTime = DateTime::Now;
			while(true)
			{
				if(updateGraphs)
					this->Invalidate();
	
				Thread::Sleep(graphUpdateDelayMs);
			}
		}

		virtual void OnPaint( PaintEventArgs^ e) override
		{
			Form::OnPaint(e);
			
			//copy data into an array that is used and manipulated by drawing stuff - don't block data collection
			//memcpy(drawingTimeDomainData, timeDomainData, (DoubleGraph::graphWidth - dataIndex) * sizeof(int));
			//memcpy((void*)drawingTimeDomainData[(DoubleGraph::graphWidth - dataIndex)], timeDomainData, dataIndex * sizeof(int));

			if(updateGraphs)
			{
				//shitty copy
				for(int i=0;i<DoubleGraph::graphWidth - dataIndex;i++)
				{
					drawingTimeDomainData[i] = timeDomainData[i + dataIndex];
				}

				for(int i=0; i < dataIndex;i++)
				{
					drawingTimeDomainData[i + DoubleGraph::graphWidth - dataIndex] = timeDomainData[i];
				}

				this->Graphs->Render(drawingTimeDomainData);
				this->Graphs->SwapOpenGLBuffers();

				//update the updates per second boxes
				int updatesPS = (int)(totalUpdateCount * 1000/ (DateTime::Now - openConnectionTime).TotalMilliseconds);
				int framesPS = (int)(totalFrameCount * 1000 / (DateTime::Now - drawStartTime).TotalMilliseconds);

				char buffer[10];

				_itoa_s(updatesPS, buffer, 10);
				this->readingUPS->Text = gcnew String(buffer);

				_itoa_s(framesPS, buffer, 10);
				this->drawingUPS->Text = gcnew String(buffer);

				totalFrameCount++;
			}
		}

		static void onDataReceived(double n)
		{
			timeDomainData[dataIndex] = n;
			dataIndex++;
			dataIndex %= DoubleGraph::graphWidth;
			totalUpdateCount++;
		}

	private:

		DoubleGraph ^Graphs;
		SerialCommunication ^SerialComm;
		Thread ^updateGraphThread;
		bool updateGraphs;

		static double* timeDomainData;
		static int dataIndex;
		static double* drawingTimeDomainData;

		static long totalUpdateCount;
		static long totalFrameCount;

		static int graphUpdateDelayMs = 50;
		static DateTime openConnectionTime;
		static DateTime drawStartTime;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  readingUPS;
	private: System::Windows::Forms::TextBox^  drawingUPS;
	private: System::Windows::Forms::Button^  pauseGraphsButton;
    //private: System::Windows::Forms::TrackBar^ timeTrackBar;
private: System::Windows::Forms::CheckBox^  checkBox1;
private: System::Windows::Forms::TrackBar^  trackBar1;
private: System::Windows::Forms::TrackBar^  trackBar2;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TrackBar^  trackBar3;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::TrackBar^  trackBar4;



	private: System::Windows::Forms::Panel^  graphsPlaceholder;



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->graphsPlaceholder = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->readingUPS = (gcnew System::Windows::Forms::TextBox());
			this->drawingUPS = (gcnew System::Windows::Forms::TextBox());
			this->pauseGraphsButton = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->trackBar4 = (gcnew System::Windows::Forms::TrackBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar4))->BeginInit();
			this->SuspendLayout();
			// 
			// graphsPlaceholder
			// 
			this->graphsPlaceholder->Location = System::Drawing::Point(10, 10);
			this->graphsPlaceholder->Name = L"graphsPlaceholder";
			this->graphsPlaceholder->Size = System::Drawing::Size(1064, 600);
			this->graphsPlaceholder->TabIndex = 0;
			this->graphsPlaceholder->Visible = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1080, 10);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(103, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Reading Updates/s:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(1080, 33);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(97, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Drawing Update/s:";
			// 
			// readingUPS
			// 
			this->readingUPS->Location = System::Drawing::Point(1189, 7);
			this->readingUPS->Name = L"readingUPS";
			this->readingUPS->Size = System::Drawing::Size(63, 20);
			this->readingUPS->TabIndex = 3;
			// 
			// drawingUPS
			// 
			this->drawingUPS->Location = System::Drawing::Point(1189, 30);
			this->drawingUPS->Name = L"drawingUPS";
			this->drawingUPS->Size = System::Drawing::Size(63, 20);
			this->drawingUPS->TabIndex = 4;
			// 
			// pauseGraphsButton
			// 
			this->pauseGraphsButton->Location = System::Drawing::Point(1083, 56);
			this->pauseGraphsButton->Name = L"pauseGraphsButton";
			this->pauseGraphsButton->Size = System::Drawing::Size(169, 23);
			this->pauseGraphsButton->TabIndex = 5;
			this->pauseGraphsButton->Text = L"Pause Graphs";
			this->pauseGraphsButton->UseVisualStyleBackColor = true;
			this->pauseGraphsButton->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Checked = true;
			this->checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox1->Location = System::Drawing::Point(1083, 140);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(80, 17);
			this->checkBox1->TabIndex = 6;
			this->checkBox1->Text = L"Logarithmic";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 10;
			this->trackBar1->Location = System::Drawing::Point(1083, 202);
			this->trackBar1->Maximum = 1000;
			this->trackBar1->Minimum = 1;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(169, 45);
			this->trackBar1->TabIndex = 7;
			this->trackBar1->Value = 700;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
			// 
			// trackBar2
			// 
			this->trackBar2->LargeChange = 10;
			this->trackBar2->Location = System::Drawing::Point(1083, 377);
			this->trackBar2->Maximum = 1000;
			this->trackBar2->Minimum = 1;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(169, 45);
			this->trackBar2->TabIndex = 8;
			this->trackBar2->Value = 700;
			this->trackBar2->Scroll += gcnew System::EventHandler(this, &Form1::trackBar2_Scroll);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1089, 186);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(64, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"FFT Scaling";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(1089, 361);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(107, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Time Domain Scaling";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(1089, 433);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(35, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Offset";
			// 
			// trackBar3
			// 
			this->trackBar3->LargeChange = 10;
			this->trackBar3->Location = System::Drawing::Point(1092, 461);
			this->trackBar3->Maximum = 1000;
			this->trackBar3->Minimum = -1000;
			this->trackBar3->Name = L"trackBar3";
			this->trackBar3->Size = System::Drawing::Size(160, 45);
			this->trackBar3->SmallChange = 20;
			this->trackBar3->TabIndex = 100;
			this->trackBar3->Value = 128;
			this->trackBar3->Scroll += gcnew System::EventHandler(this, &Form1::trackBar3_Scroll);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(1090, 547);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(57, 13);
			this->label6->TabIndex = 101;
			this->label6->Text = L"FFT Width";
			// 
			// trackBar4
			// 
			this->trackBar4->LargeChange = 10;
			this->trackBar4->Location = System::Drawing::Point(1093, 565);
			this->trackBar4->Maximum = 1024;
			this->trackBar4->Minimum = 950;
			this->trackBar4->Name = L"trackBar4";
			this->trackBar4->Size = System::Drawing::Size(159, 45);
			this->trackBar4->TabIndex = 10;
			this->trackBar4->Value = 1000;
			this->trackBar4->Scroll += gcnew System::EventHandler(this, &Form1::trackBar4_Scroll);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 630);
			this->Controls->Add(this->trackBar4);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->trackBar3);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->trackBar2);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->pauseGraphsButton);
			this->Controls->Add(this->drawingUPS);
			this->Controls->Add(this->readingUPS);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->graphsPlaceholder);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar4))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(this->updateGraphs)
				 {
					 this->pauseGraphsButton->Text = L"Start Graphs";
				 }
				 else 
				 {
					 this->pauseGraphsButton->Text = L"Pause Graphs";
					 totalFrameCount = 0;
					 drawStartTime = DateTime::Now;
				 }

				 this->updateGraphs = !this->updateGraphs;
			 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->Graphs->SetFFTIsLogScale(this->checkBox1->Checked);
		 }
private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 this->Graphs->frequencyDomainScaling = Math::Pow(10.0, ((double)this->trackBar1->Value) / 100.0 - 7.0);
		 }
private: System::Void trackBar2_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 this->Graphs->timeDomainScaling = Math::Pow(10.0, ((double)this->trackBar2->Value)/100.0 - 7.0);
		 }
private: System::Void trackBar3_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 this->Graphs->timeDomainOffset = this->trackBar3->Value;
		 }
private: System::Void trackBar4_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 this->Graphs->fftSize = this->trackBar4->Value;
		 }
};
}

