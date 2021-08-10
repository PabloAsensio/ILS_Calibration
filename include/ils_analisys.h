#pragma once

#ifndef ILS_ANALISYS_H_
#define ILS_ANALISYS_H_

#include "fft.h"
#include "printers.h"
#include "calculations.h"

/*! 
 *  This function input is the signal, then it makes its fft, gets the tones, 
 *  computes DDM, SDM, horizontal and vertical desviation.
 */
DDM_SDM ils_analisys(const std::complex<float> *signal, unsigned int ret, float samps_hz);

#endif // ILS_ANALISYS_H_