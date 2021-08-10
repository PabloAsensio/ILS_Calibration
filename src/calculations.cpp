#include "calculations.h"

void compute_module(const fftwf_complex *signal, int n, std::vector<float> *outModules)
{
    PROFILE_FUNCTION();
    for (auto i = 0; i < n; i++)
    {
        float module = std::sqrt(
            signal[i][REAL] * signal[i][REAL] + signal[i][IMAG] * signal[i][IMAG]);
        outModules->emplace_back(module);
    }
}

void compute_module(const std::complex<float> *signal, int n, std::vector<float> *outModules)
{
    PROFILE_FUNCTION();
    for (auto i = 0; i < n; i++)
    {
        float module = std::sqrt(
            signal[i].real() * signal[i].real() + signal[i].imag() * signal[i].imag());
        outModules->emplace_back(module);
    }
}

void compute_fft_module(const fftwf_complex *signal, int n, std::vector<float> *outModules)
{
    PROFILE_FUNCTION();
    float factor = 2.0f / (float)(n);
    for (int i = 0; i < n; i++)
    {
        float module = std::sqrt(
            signal[i][REAL] * signal[i][REAL] + signal[i][IMAG] * signal[i][IMAG]);
        outModules->emplace_back(module * factor);
    }
}

void compute_freq_axis(std::vector<float> *outFreq_vector, const int n, const float d = 1.0)
{
    PROFILE_FUNCTION();
    // if n is even
    if (n % 2 == 0)
    {
        for (auto i = 0; i <= n / 2 - 1; ++i)
        {
            outFreq_vector->emplace_back(i / (d * n));
        }
        for (auto i = -(n / 2); i <= -1; ++i)
        {
            outFreq_vector->emplace_back(i / (d * n));
        }
    }
    // if n is odd
    else if (n % 2 != 0)
    {
        for (auto i = 0; i <= (n - 1) / 2; ++i)
        {
            outFreq_vector->emplace_back(i / (d * n));
        }
        for (auto i = -((n - 1) / 2); i <= -1; ++i)
        {
            outFreq_vector->emplace_back(i / (d * n));
        }
    }
}

float max_tone_base_band_rectangle_filter(
    const std::vector<float> *data, const std::vector<float> *freq_vec, float band, float bw)
{
    PROFILE_FUNCTION();
    float max = -1000000000.0f;

    for (unsigned int i = 0; i < freq_vec->size(); ++i)
    {
        if (freq_vec->operator[](i) >= band - bw && freq_vec->operator[](i) <= band + bw)
        {
            if (data->operator[](i) > max)
                max = data->operator[](i);
        }
    }

    return max;
}

float freq_max_tone(
    const std::vector<float> *data, const std::vector<float> *freq_vec, float band, float bw)
{
    PROFILE_FUNCTION();
    float max = -1000000000.0f;
    int band_pos;

    for (unsigned int i = 0; i < freq_vec->size(); ++i)
    {
        if (freq_vec->operator[](i) >= band - bw && freq_vec->operator[](i) <= band + bw)
        {
            if (data->operator[](i) > max)
            {
                max = data->operator[](i);
                band_pos = i;
            }
        }
    }
    return freq_vec->operator[](band_pos);
}

float calculate_compass_bearing(const float &latA, const float &lonA,
                                const float &latB, const float &lonB)
{
    PROFILE_FUNCTION();
    float diff_lon = (lonA - lonB) * DEG2RAD; // radians

    float x = sin(diff_lon) * cos(latB * DEG2RAD);
    float y = cos(latA * DEG2RAD) * sin(latB * DEG2RAD) -
              (sin(latA * DEG2RAD) * cos(latB * DEG2RAD) * cos(diff_lon));

    float bearing = atan2(x, y) * RAD2DEG; // degrees

    if (bearing > 360.0f)
        return bearing - 360.0f;
    if (bearing < 0.0f)
        return bearing + 360.0f;
    return bearing;
}

float plane_distance_AB(const float &latA, const float &lonA,
                        const float &latB, const float &lonB)
{
    PROFILE_FUNCTION();
    float R = 6371000; //m earth_radius

    float diff_lon = (lonB - lonA) * DEG2RAD; // radians
    float diff_lat = (latB - latA) * DEG2RAD; // radians

    float a = pow(sin(diff_lat / 2), 2) + cos(latA * DEG2RAD) *
                                              cos(latB * DEG2RAD) * pow(sin(diff_lon / 2), 2);

    float c = 2.0f * atan2(sqrt(a), sqrt(1 - a));
    return std::abs(R * c);
}

float ILS_GS_height(const float &latA, const float &lonA, const float &hA,
                    const float &latB, const float &lonB, [[maybe_unused]] const float &hB,
                    const float &RWY_radial, float descent_angle)
{
    PROFILE_FUNCTION();
    // bearing RPAS towards THW
    float bearing_THW_RPAS = calculate_compass_bearing(latA, lonA, latB, lonB);   // degrees
    float angle_RWY_RPAS = (RWY_radial - bearing_THW_RPAS) * DEG2RAD;             // radians
    float distance_TWR_RPAS = plane_distance_AB(latA, lonA, latB, lonB);          // meters
    float distance_proyection_RWY_axis = distance_TWR_RPAS * cos(angle_RWY_RPAS); // meters

    return hA + (distance_proyection_RWY_axis * tan(descent_angle * DEG2RAD)); // meters
}

float angle_difference(const float &latA, const float &lonA, const float &hA,
                       const float &latB, const float &lonB, const float &hB,
                       const float &RWY_radial, float descent_angle)
{
    PROFILE_FUNCTION();
    // bearing RPAS towards THW
    float bearing_THW_RPAS = calculate_compass_bearing(latA, lonA, latB, lonB);   // degrees
    float angle_RWY_RPAS = (RWY_radial - bearing_THW_RPAS) * DEG2RAD;             // radians
    float distance_TWR_RPAS = plane_distance_AB(latA, lonA, latB, lonB);          // meters
    float distance_proyection_RWY_axis = distance_TWR_RPAS * cos(angle_RWY_RPAS); // meters

    return (atan((hB - hA) / distance_proyection_RWY_axis) * RAD2DEG) - descent_angle; // degrees
}

float angle_Earth_RPAS(const float &latA, const float &lonA, const float &hA,
                       const float &latB, const float &lonB, const float &hB)
{
    PROFILE_FUNCTION();
    float distance_TWR_RPAS = plane_distance_AB(latA, lonA, latB, lonB); // meters

    return atan((hB - hA) / distance_TWR_RPAS) * RAD2DEG; // degrees
}