#pragma once

#ifndef DO_WORK_H_
#define DO_WORK_H_

#include "data_structures.h"

#include <cstdio> //stdandard output
#include <cstdlib>

#include <iostream>
#include <string> // std::string
#include <vector> // std::vector<...>
#include <map>    // std::map< ... , ... >
#include <list>
#include <array>
#include <memory>
#include <algorithm> // std::copy, reinterpret_cast, ...
#include <iterator>

// Multiprocesing
#include <future>

// SDR lib
#include <SoapySDR/Device.hpp>
#include <SoapySDR/Types.hpp>
#include <SoapySDR/Formats.hpp>

// Analisys
#include "ils_analisys.h"

// To stop correctly by hand
#include <unistd.h>
#include <signal.h>

// pixhawk
#include "pixhawk.h"

// mqtt
#include "mqtt_client.h"

#define REAL 0
#define IMAG 1

struct MyException : public std::exception
{
    std::string s;
    MyException(std::string ss) : s(ss) {}
    ~MyException() throw() {} // Updated
    const char *what() const throw() { return s.c_str(); }
};

/*!
 *   This function update telemetry data
 */
// extern RPAS_telemetry telematry;
void get_telemetry(RPAS_telemetry *telemetry);
void do_ILS_calibration(const Complex *signal,
                        const size_t signal_size,
                        int ret,
                        int n_proc,
                        RPAS_telemetry telemetry,
                        mqtt_client *publisher_client);
int do_fromSDR();

void do_fromCF32(const char *filename, float sr, const unsigned int sampling);

void test_array1(std::array<Complex, 20> a);
void test_array2(Complex a[], const size_t n);
void do_ArryTest();

// void inthand(int signum) { stop = 1; };
void do_ctrlC_test();

int do_pixhawk_test(int argc, char *argv[]);
int test_sdr(int argc, char *argv[]);

// bool STOP_TELEMETRY = false;

#endif // DO_WORK_H_