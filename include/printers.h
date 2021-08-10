#pragma once

#ifndef PRINTERS_H_
#define PRINTERS_H_

#include <iostream>
#include <vector>
#include <complex>
#include <iomanip>
#include <fftw3.h>

// Chrono lib
#include <chrono>

// Threads lib
#include <future>

#include "global.h"
#include "data_structures.h"
#include <matplotlibcpp.h>

#define REAL 0
#define IMAG 1

void print_hello_word(int i);

void print_fftwf_complex(const fftwf_complex *carray, int n);

void print_complex(const std::complex<float> *carray, int n);

void print_vector(const std::vector<float> *vector);

void plot_module(const std::vector<float> *module, const char *name);

void plot_fft(const std::vector<float> *fft_freq, const std::vector<float> *fft, const char *name);

void print_telemetry(RPAS_telemetry *telemetry);
void print_telemetry(RPAS_telemetry *telemetry, int n_proc);
#endif // PRINTERS_H_