#include "StdAfx.h"
#include "DoubleGraph.h"


DoubleGraph::DoubleGraph(System::Windows::Forms::Form ^ parentForm, int iX, int iY) : 
OpenGL::COpenGL(parentForm, graphWidth + 2 * padding, graphHeight*2 + 3 * padding, iX, iY)
{
	frequencyDomainData = (int*) malloc(graphWidth * sizeof(int));
	fft = gcnew FastFourierTransform1();
}

