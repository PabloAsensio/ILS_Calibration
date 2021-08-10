#pragma once

#ifndef FFT_H_
#define FFT_H_

#include <iostream>
#include <complex> // std::complex
#include <valarray>
#include <vector>
#include <math.h>

#include "global.h"

#include <fftw3.h>

#define REAL 0
#define IMAG 1

typedef std::complex<float> Complex;
typedef std::valarray<Complex> CArray;

// const double PI = 3.141592653589793238460;

namespace fft
{
void fft(fftwf_complex *x, const long unsigned int n, fftwf_complex *outFftw_);
void fft(const Complex *buf, const long unsigned int n, fftwf_complex *outFftw_);
void fft(CArray &buf, long unsigned int n, fftwf_complex *outFftw_);
void fft(CArray &x);
void _fft(Complex buf[], Complex out[], int n, int step);
} // namespace fft

#endif // FFT_H_