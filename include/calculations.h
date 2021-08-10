#pragma once

#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <fftw3.h>

#include "global.h"

// Signal calculations

#define REAL 0
#define IMAG 1

#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

/*! 
 *  Compute module of fftwf_complex real domain complex signal. 
 */
void compute_module(const fftwf_complex *signal, int n, std::vector<float> *outModules);

/*! 
 *  Compute module of complex real domain signal. 
 */
void compute_module(const std::complex<float> *signal, int n, std::vector<float> *outModules);

/*! 
 *  Compute module of fftwf_complex complex freq domain signal.
 */
void compute_fft_module(const fftwf_complex *signal, int n, std::vector<float> *outModules);

/*! 
 *  For a given FFT, compute its freq. vector.
 */
void compute_freq_axis(std::vector<float> *outFreq_vector, const int n, const float d);

/*! 
 *  For a given FFT 'data' and its 'freq_vec', centered at 0, returns max value for a 'band' with a 
 *  a 'bw' tolerance.
 */
float max_tone_base_band_rectangle_filter(const std::vector<float> *data,
                                          const std::vector<float> *freq_vec,
                                          float band, float bw);

/*! 
 *  For a given FFT 'data' and its 'freq_vec', centered at 0, returns frequency of max value 
 *  for a 'band' with 'bw' tolerance.
 */
float freq_max_tone(const std::vector<float> *data,
                    const std::vector<float> *freq_vec,
                    float band, float bw);

/*!
 *  Calculates compass bearing (0-360) in geofraphics degrees between points A and B. 
 *  A is reference point.
 *  latX, lonX must be in degrees.
 */
float calculate_compass_bearing(const float &latA, const float &lonA,
                                const float &latB, const float &lonB);

/*!
 *  Calculates distance in plane for an Earth Radius=6371000m between points A and B.
 *  latX, lonX must be in degrees.
 */
float plane_distance_AB(const float &latA, const float &lonA,
                        const float &latB, const float &lonB);

/*!
 *  Given THW possition (A), RPAS possition (B), runway bearing and descent angle (by default 3º).
 *  Retuns GS height RPAS should have for DDM = 0 on center RWY plane.
 *  All angular units must be in degrees. 
 *  Runway bearing must be in geografic reference.
 */
float ILS_GS_height(const float &latA, const float &lonA, const float &hA,
                    const float &latB, const float &lonB, [[maybe_unused]] const float &hB,
                    const float &RWY_radial, float descent_angle = 3);

/*!
 *  Given THW possition (A), RPAS possition (B), runway bearing and descent angle (by default 3º).
 *  Retuns angle difference between RPAS and descent angle for DDM = 0 on center RWY plane.
 *  All angular units must be in degrees. 
 *  Runway bearing must be in geografic reference.
 *  All heights in meters.
 */
float angle_difference(const float &latA, const float &lonA, const float &hA,
                       const float &latB, const float &lonB, const float &hB,
                       const float &RWY_radial, float descent_angle = 3);

/*!
 *  Given THW possition (A), RPAS possition (B)
 *  Retuns angle difference between A and RPAS on its plane.
 *  All angular units must be in degrees. 
 *  Runway bearing must be in geografic reference.
 *  All heights in meters.
 */
float angle_Earth_RPAS(const float &latA, const float &lonA, const float &hA,
                       const float &latB, const float &lonB, const float &hB);

#endif // CALCULATIONS_H_