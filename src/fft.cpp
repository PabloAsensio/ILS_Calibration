#include "fft.h"
// #include "fft"

namespace fft
{

void fft(fftwf_complex *x, const long unsigned int n, fftwf_complex *outFftw_)
{
	PROFILE_FUNCTION();
	//------ Plan FFT and execute it. ------
	fftwf_plan plan = fftwf_plan_dft_1d(n, x, outFftw_, FFTW_FORWARD, FFTW_ESTIMATE);

	// Do plan
	fftwf_execute(plan);

	//------ Clean up plan ------
	fftwf_destroy_plan(plan);
	fftwf_cleanup();
}

void fft(const Complex *buf, const long unsigned int n, fftwf_complex *outFftw_)
{
	PROFILE_FUNCTION();

	fftwf_complex *x = new fftwf_complex[n];
	for (long unsigned int i = 0; i < n; ++i)
	{
		x[i][REAL] = buf[i].real();
		x[i][IMAG] = buf[i].imag();
	}
	fftwf_plan plan = fftwf_plan_dft_1d(n, x, outFftw_, FFTW_FORWARD, FFTW_ESTIMATE);

	// Do plan
	fftwf_execute(plan);

	//------ Clean up plan ------
	fftwf_destroy_plan(plan);
	fftwf_cleanup();

	// Free mem
	delete[] x;
}

void fft(CArray &buf, long unsigned int n, fftwf_complex *outFftw_)
{
	PROFILE_FUNCTION();
	fftwf_complex *x = new fftwf_complex[n];
	// fftwf_complex outFftw_[n];
	for (long unsigned int i = 0; i < n; ++i)
	{
		// std::cerr << i << '\n';
		x[i][REAL] = buf[i].real();
		x[i][IMAG] = buf[i].imag();
	}
	//------ Plan FFT and execute it. ------
	fftwf_plan plan = fftwf_plan_dft_1d(n, x, outFftw_, FFTW_FORWARD, FFTW_ESTIMATE);

	// Do plan
	fftwf_execute(plan);

	//------ Clean up plan ------
	fftwf_destroy_plan(plan);
	fftwf_cleanup();

	delete[] x;
}

void fft(CArray &x)
{
	PROFILE_FUNCTION();

	std::cerr << "VECTOR DIMENSION " << x.size() << std::endl;
	// DFT
	unsigned int counter = 0;
	unsigned int N = x.size(), k = N, n;
	float thetaT = 3.14159265358979323846264338328f / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0f;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				if (b >= N)
				{
					// b = N;
					counter++;
				}
				else
				{
					Complex t = x[a] - x[b];
					x[a] += x[b];
					x[b] = t * T;
				}
			}
			T *= phiT;
		}
	}
	std::cerr << "ERROR FFT: " << counter << '\n';
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
}
