#pragma once

#ifndef DATA_STRUCTURES_
#define DATA_STRUCTURES_

// Chrono lib
#include <chrono>

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIMESTAMP_ms std::chrono::time_point_cast<std::chrono::microseconds>(TIME_NOW).time_since_epoch().count()

struct RPAS_telemetry
{
    float latitude_deg;
    float longuitude_deg;
    float altitude_m;
    int n_sat;
    int fix_type;
    int64_t timestamp_ms;
    RPAS_telemetry()
        : latitude_deg(-200),
          longuitude_deg(-200),
          altitude_m(-200),
          n_sat(-200),
          fix_type(-200),
          timestamp_ms(TIMESTAMP_ms)
    {
    }
};

struct ILS_data
{
    float DDM;
    float SDM;
    float ILS_horizontal_angle;
    float calculated_horizontal_angle;
    float ILS_vertical_angle;
    float calculated_vertical_angle;
    bool couse;
    bool clearence;
    ILS_data()
        : DDM(-200),
          SDM(-200),
          ILS_horizontal_angle(-200),
          calculated_horizontal_angle(-200),
          ILS_vertical_angle(-200),
          calculated_vertical_angle(-200),
          couse(false),
          clearence(false)
    {
    }
};

struct DDM_SDM
{
    float course_DDM;
    float course_SDM;
    float course_index_modulation_90;
    float course_index_modulation_150;
    float course_modulation_percentage_90;
    float course_modulation_percentage_150;

    float clearance_DDM;
    float clearance_SDM;
    float clearance_index_modulation_90;
    float clearance_index_modulation_150;
    float clearance_modulation_percentage_90;
    float clearance_modulation_percentage_150;
};

#endif // DATA_STRUCTURES_