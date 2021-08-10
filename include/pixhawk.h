#pragma once

#ifndef PIXHAWK_H_
#define PIXHAWK_H_

#include "data_structures.h"

// #include <mavlink/v2.0/common/mavlink_msg_altitude.h>
// #include <mavlink/v2.0/common/mavlink.h>
// #include <mavlink/v2.0/mavlink_get_info.h>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/geofence/geofence.h>

// #include <ros/ros.h>
// #include <geometry_msgs/PoseStamped.h>
// #include <mavros_msgs/CommandBool.h>
// #include <mavros_msgs/SetMode.h>
// #include <mavros_msgs/State.h>

#include <functional>
#include <future>
#include <iostream>
#include <memory>

#include "printers.h"

#define ERROR_CONSOLE_TEXT "\033[31m"     // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m"     // Restore normal console colour

using namespace mavsdk;
using namespace std::placeholders; // for `_1`
using namespace std::chrono;       // for seconds(), milliseconds()
using namespace std::this_thread;  // for sleep_for()

// static Geofence::Polygon::Point add_point(double latitude_deg, double longitude_deg);

void usage(std::string bin_name);

int main_pixhawk(int argc, char **argv);

// Geofence::Polygon::Point add_point(double latitude_deg, double longitude_deg);

// extern RPAS_telemetry data_telem();
int test_telemetry(int argc, char **argv);
void get_telemetry(RPAS_telemetry *telem_data);

// class Pixhawk
// {
// public:
//     Pixhawk();
//     ~Pixhawk();

// private:
// };

#endif // PIXHAWK_H_