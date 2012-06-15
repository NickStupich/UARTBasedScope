#pragma once
#include <math.h>
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

using namespace System;

ref class FastFourierTransform1
{
private:
	int useLogScale;

public:

	FastFourierTransform1()
	{
		this->useLogScale = 1;
	}

	void setLogScale(int logScale)
	{
		this->useLogScale = logScale;
	}

	void DoFFT(double* input, double* output, int size)
	{
		//code comes from http://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=9388

		//variables required by downloaded code:
		
		unsigned int sample_rate = size * 4;// * 2;
		unsigned int number_of_samples = size;
		int sign = 1;
		double pi = 3.141592654f;

		double* data = (double*) malloc(number_of_samples * 2 * sizeof(double));
		double* vector = (double*) malloc(sample_rate * 2 * sizeof(double));

		for(int i=0;i<size;i+=2)
		{
			data[i] = (float) input[i];
			data[i+1] = 0;
			vector[i] = 0;
			vector[i+1] = 0;
		}

		//actual code downloaded:
		
		//variables for the fft
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;

	//the complex array is real+complex so the array 
    //as a size n = 2* number of complex samples
    //real part is the data[index] and 
    //the complex part is the data[index+1]

	//new complex array of size n=2*sample_rate
	/*if(vector!=NULL)
        delete [] vector;

	vector=new float [2*sample_rate];
	*/
	//put the real array in a complex array
	//the complex part is filled with 0's
	//the remaining vector with no data is filled with 0's
	for(n=0; n<sample_rate;n++)
	{
		if(n<number_of_samples)
			vector[2*n]=data[n];
		else
			vector[2*n]=0;
		vector[2*n+1]=0;
	}

	//binary inversion (note that the indexes 
    //start from 0 witch means that the
    //real part of the complex is on the even-indexes 
    //and the complex part is on the odd-indexes)
	n=sample_rate << 1;
	j=0;
	for (i=0;i<n/2;i+=2) {
		if (j > i) {
			SWAP(vector[j],vector[i]);
			SWAP(vector[j+1],vector[i+1]);
			if((j/2)<(n/4)){
				SWAP(vector[(n-(i+2))],vector[(n-(j+2))]);
				SWAP(vector[(n-(i+2))+1],vector[(n-(j+2))+1]);
			}
		}
		m=n >> 1;
		while (m >= 2 && j >= m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	//end of the bit-reversed order algorithm

	//Danielson-Lanzcos routine
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=sign*(2*pi/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*vector[j-1]-wi*vector[j];
				tempi=wr*vector[j]+wi*vector[j-1];
				vector[j-1]=vector[i-1]-tempr;
				vector[j]=vector[i]-tempi;
				vector[i-1] += tempr;
				vector[i] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
	//end of the algorithm
	
	/*now copy stuff back into the provided return array*/

	double max = 0;
	for(int i=0;i<size;i++)
	{
		vector[i] = (vector[2*i] * vector[2*i] + vector[2*i+1] * vector[2*i+1]);
		if(vector[i] > max)
			max = vector[i];
	}

	if(this->useLogScale)
	{
		/*	
		double scaling = 20;
		double logMax = Math::Log10(max);
		double top = 255 * logMax / scaling;
		for(int i=0;i<size;i++)
		{
			output[i] = (int)(scaling * (Math::Log10(vector[i])) - top);
			if(output[i] < 0)
				output[i] = 0;
		}*/
		/*
		double logMax = Math::Log10(max);
		for(int i=0;i<size;i++)
		{
			output[i] = (int)(255 * (Math::Log10(vector[i]))/logMax);
			if(output[i] < 0)
				output[i] = 0;
		}*/

		for(int i=0;i<size;i++)
		{
			output[i] = Math::Log10(vector[i]);
		}

	}
	else
	{
		for(int i=0;i<size;i++)
		{
			output[i] = vector[i] / size;
		}
	}

	}
};