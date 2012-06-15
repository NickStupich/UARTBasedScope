#include "StdAfx.h"
#include "DoubleGraph.h"


DoubleGraph::DoubleGraph(System::Windows::Forms::Form ^ parentForm, int iX, int iY) : 
OpenGL::COpenGL(parentForm, graphWidth + 2 * padding, graphHeight*2 + 3 * padding, iX, iY)
{
	this->timeDomainScaling = 1.0;
	this->frequencyDomainScaling = 1.0;
	this->timeDomainOffset = 128.0;
	this->frequencyDomainOffset = 1.0;
	this->fftSize = 1024;

	frequencyDomainData = (double*) malloc(graphWidth * sizeof(double));
	fft = gcnew FastFourierTransform1();
}

