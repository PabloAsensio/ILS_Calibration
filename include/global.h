#pragma once

#ifndef SRC_LIB_COMMON_H_
#define SRC_LIB_COMMON_H_

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Instrumentor.h"

#define PROFILING 1

#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION()
#endif

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

// #define CONFIG_INI_PATH "../include/config.ini"
#define CONFIG_INI_PATH "/home/pablo/Desktop/ineco/calibILS/include/config.ini"

extern const float PI;
// [PIXHAWK]
extern std::string PIXHAWK_CONNECTION_STRING;
extern int PIXHAWK_BAUD;
extern bool PIXHAWK_WAIT_READY;

// [SDR]
extern int SDR_CENTER_FREQ_HZ;
extern int SDR_SAMPLE_RATE;
extern int SDR_BUFFER_SIZE;
extern int SDR_BANDWIDTH_HZ;
extern int SDR_RX_CHAN;
extern std::string SDR_RX_ANT;
extern std::string SDR_FORMAT_STRING;
extern bool SDR_GAIN_AUTO;
extern int SDR_GAIN_IFGR;
extern int SDR_GAIN_RFGR;
extern int SDR_BUFFER_READ;
extern int SDR_DELAY_LOOP_RECORDING;

// [ILS]
extern float ILS_LAT;
extern float ILS_LON;
extern float ILS_ALT;
extern float THR_LAT;
extern float THR_LON;
extern float THR_ALT;

// [MQTT]
extern std::string MQTT_HOST;
extern std::string MQTT_PORT;
extern std::string MQTT_KEEPALIVE;
extern std::string MQTT_BASE_TOPIC_PUB;
extern bool KILL_PROGRAM;
extern bool STOP_MQTT;

#endif //SRC_LIB_COMMON_H_
