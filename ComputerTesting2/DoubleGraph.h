#pragma once
#include "opengl.h"
#include "FastFourierTransform1.h"

ref class DoubleGraph :
public OpenGL::COpenGL
{
public:
	DoubleGraph(System::Windows::Forms::Form ^ parentForm, int iX, int iY);

	FastFourierTransform1^ fft;

	static const int graphWidth = 1024;
	static const int graphHeight = 256;
	static const int padding = 20;
	
	double* frequencyDomainData;

	double timeDomainScaling;
	double frequencyDomainScaling;

	double timeDomainOffset;
	double frequencyDomainOffset;

	System::Void SetFFTIsLogScale(int isLogScale)
	{
		this->fft->setLogScale(isLogScale);
	}
	
	virtual System::Void Render(double* timeDomainData)
	{
		//get a black screen
		COpenGL::Render();

		glEnable2D();

		//draw lines around each graph
		glColor3f(0, 255, 0);
		glBegin(GL_LINES);

		//lines around the time domain signal
		glVertex2d(padding-1, padding-1);
		glVertex2d(padding + graphWidth-1, padding-1);

		glVertex2d(padding-1, padding + graphHeight-1);
		glVertex2d(padding + graphWidth-1, padding + graphHeight-1);

		glVertex2d(padding-1, padding-1);
		glVertex2d(padding-1, padding + graphHeight -1);

		glVertex2d(padding + graphWidth - 1, padding -1);
		glVertex2d(padding + graphWidth - 1, padding + graphHeight - 1);

		//lines around the frequency domain signal
		glVertex2d(padding-1, padding*2-1 + graphHeight);
		glVertex2d(padding + graphWidth-1, padding * 2 + graphHeight-1);

		glVertex2d(padding-1, padding*2 + graphHeight*2-1);
		glVertex2d(padding + graphWidth-1, padding  * 2 + graphHeight*2 -1);

		glVertex2d(padding-1, padding*2 + graphHeight -1);
		glVertex2d(padding-1, padding * 2 + graphHeight*2 -1);

		glVertex2d(padding + graphWidth - 1, padding* 2 + graphHeight -1);
		glVertex2d(padding + graphWidth - 1, padding*2 + graphHeight*2 - 1);


		glColor3f(255, 0, 0);

		//time domain signal
		for(int i=0;i<graphWidth - 1;i++)
		{
			glVertex2d(i + padding, timeDomainData[i]*this->timeDomainScaling + this->timeDomainOffset + padding);
			glVertex2d(i+1 + padding, timeDomainData[i+1]*this->timeDomainScaling + this->timeDomainOffset + padding);
		}

		//frequency domain signal
		fft->DoFFT(timeDomainData, frequencyDomainData, graphWidth);

		for(int i=0;i<graphWidth - 1;i++)
		{
			glVertex2d(i + padding, frequencyDomainData[i]*this->frequencyDomainScaling + this->frequencyDomainOffset + padding * 2 + graphHeight);
			glVertex2d(i+1 + padding, frequencyDomainData[i+1]*this->frequencyDomainScaling + this->frequencyDomainOffset + padding * 2 + graphHeight);
		}

		//draw frquency labels on the frequency domain signal
		int frequenciesToLabel[] = {0, 60, 200, 400, 600, 800, 1000};
		int frequencyCount = 7;

		glColor3f(0, 255, 255);
		
		for(int i=0;i<frequencyCount;i++)
		{
			//draw the tick
			glVertex2d(padding + frequenciesToLabel[i], graphHeight + padding * 2 - 5);
			glVertex2d(padding + frequenciesToLabel[i], graphHeight + padding * 2 - 1);

			//draw the label
		}

		glEnd();

		glDisable2D();
	}

	void glEnable2D()
    {
	  
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glOrtho(0, graphWidth + padding*2, 0, graphHeight*2 + padding * 3, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    }

    void glDisable2D()
    {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
};

