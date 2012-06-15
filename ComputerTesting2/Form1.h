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

			timeDomainData = (int*) malloc(DoubleGraph::graphWidth * sizeof(int));
			memset(timeDomainData, 0, DoubleGraph::graphWidth);
			drawingTimeDomainData = (int*) malloc(DoubleGraph::graphWidth * sizeof(int));
			
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

		static void onDataReceived(int n)
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

		static int* timeDomainData;
		static int dataIndex;
		static int* drawingTimeDomainData;

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
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 630);
			this->Controls->Add(this->pauseGraphsButton);
			this->Controls->Add(this->drawingUPS);
			this->Controls->Add(this->readingUPS);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->graphsPlaceholder);
			this->Name = L"Form1";
			this->Text = L"Form1";
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
};
}

