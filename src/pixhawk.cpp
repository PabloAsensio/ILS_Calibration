#include "pixhawk.h"

// volatile sig_atomic_t MANUAL_STOP_PIX;
// void inthand_pix(int signum)
// {
//     MANUAL_STOP_PIX = 1;
// }

void usage(std::string bin_name)
{
    PROFILE_FUNCTION();
    std::cerr << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

int main_pixhawk(int argc, char **argv)
{
    PROFILE_FUNCTION();
    Mavsdk dc;

    {
        auto prom = std::make_shared<std::promise<void>>();
        auto future_result = prom->get_future();

        std::cerr << "Waiting to discover system..." << std::endl;
        dc.register_on_discover([prom](uint64_t uuid) {
            std::cerr << "Discovered system with UUID: " << uuid << std::endl;
            prom->set_value();
        });

        std::string connection_url;
        ConnectionResult connection_result;

        connection_url = "udp://127.0.0.1:14550";
        dc.add_any_connection(connection_url);

        if (connection_result != ConnectionResult::SUCCESS)
        {
            std::cerr << ERROR_CONSOLE_TEXT
                      << "Connection failed: " << connection_result_str(connection_result)
                      << NORMAL_CONSOLE_TEXT << std::endl;
            return 1;
        }
        future_result.get();
    }

    dc.register_on_timeout([](uint64_t uuid) {
        std::cerr << "System with UUID timed out: " << uuid << std::endl;
        std::cerr << "Exiting." << std::endl;
        exit(0);
    });

    // We don't need to specifiy the UUID if it's only one system anyway.
    // If there were multiple, we could specify it with:
    // dc.system(uint64_t uuid);
    System &system = dc.system();
    auto action = std::make_shared<Action>(system);
    auto mission = std::make_shared<Mission>(system);
    auto telemetry = std::make_shared<Telemetry>(system);
    // auto position = std::make_shared<Telemetry::Position>(system);
    auto geofence = std::make_shared<Geofence>(system);

    // Telemetry::Position *position = new Telemetry::Position;

    while (!telemetry->health_all_ok())
    {
        std::cerr << "Waiting for system to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    std::cerr << "System ready" << std::endl;

    return 0;
}


int test_telemetry(int argc, char **argv)
{
    PROFILE_FUNCTION();
    Mavsdk dc;

    bool discovered_system = false;
    //ConnectionResult connection_result = dc.add_udp_connection("udp://0.0.0.0:14540");
    // ConnectionResult connection_result = dc.add_serial_connection("COM7");
    // ConnectionResult connection_result = dc.add_tcp_connection("127.0.0.1", 5760);
    // ConnectionResult connection_result = dc.add_udp_connection(14540);
    ConnectionResult connection_result = dc.add_any_connection("udp://14540");
    // ConnectionResult connection_result = dc.add_any_connection("udp://127.0.0.1:14550");

    // Wait for the system to connect via heartbeat
    while (!dc.is_connected())
    {
        std::cerr << "Waiting for contection...\n";
        sleep_for(seconds(1));
    }

    System &system = dc.system();

    dc.register_on_discover([&discovered_system](uint64_t uuid) {
        std::cerr << "Discovered system with UUID: " << uuid << std::endl;
        discovered_system = true;
    });

    if (!discovered_system)
    {
        std::cerr << "No system found, exiting." << std::endl;
        return 1;
    }

    auto telemetry = std::make_shared<Telemetry>(system);

    // Set gps_info rate to default rate
    const Telemetry::Result set_rate_result_gps_info = telemetry->set_rate_gps_info(1);
    if (set_rate_result_gps_info != Telemetry::Result::SUCCESS)
    {
        std::cerr << "GPS_info->Setting rate failed:"
                  << Telemetry::result_str(set_rate_result_gps_info) << std::endl;
    }

    // Set position rate to default rate
    const Telemetry::Result set_rate_result_position = telemetry->set_rate_position(1);
    if (set_rate_result_position != Telemetry::Result::SUCCESS)
    {
        std::cerr << "Position->Setting rate failed:"
                  << Telemetry::result_str(set_rate_result_position) << std::endl;
    }

    while (dc.is_connected())
    {
        auto gps_data = telemetry->gps_info();
        auto position_info = telemetry->position();

        std::cerr << "Position Information" << std::endl
                  << "\t Longitude: " << position_info.longitude_deg << std::endl
                  << "\t Latitude: " << position_info.latitude_deg << std::endl
                  << "\t Absolute Altitude (m): " << position_info.absolute_altitude_m << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 2));
    }

    return 0;
}

void get_telemetry(RPAS_telemetry *telem_data)
{
    PROFILE_FUNCTION();
    // signal(SIGINT, inthand_pix);
    Mavsdk dc;

    bool discovered_system = false;
    //ConnectionResult connection_result = dc.add_udp_connection("udp://0.0.0.0:14540");
    // ConnectionResult connection_result = dc.add_serial_connection("COM7");
    // ConnectionResult connection_result = dc.add_tcp_connection("127.0.0.1", 5760);
    // ConnectionResult connection_result = dc.add_udp_connection(14540);
    ConnectionResult connection_result = dc.add_any_connection("udp://14540");
    // ConnectionResult connection_result = dc.add_any_connection("udp://127.0.0.1:14550");

    // Wait for the system to connect via heartbeat
    int counter = 0;
    while (!dc.is_connected() && counter < 10)
    {
        std::cerr << "Waiting for contection...\n";
        sleep_for(seconds(1));
        counter++;
        if (counter == 10)
            KILL_PROGRAM = true;
    }

    if (!KILL_PROGRAM)
    {

        System &system = dc.system();

        dc.register_on_discover([&discovered_system](uint64_t uuid) {
            std::cerr << "Discovered system with UUID: " << uuid << std::endl;
            discovered_system = true;
        });

        if (!discovered_system)
        {
            std::cerr << "No system found, exiting." << std::endl;
            KILL_PROGRAM = true;
        }

        auto telemetry = std::make_shared<Telemetry>(system);

        // Set gps_info rate to default rate
        const Telemetry::Result set_rate_result_gps_info = telemetry->set_rate_gps_info(0);
        if (set_rate_result_gps_info != Telemetry::Result::SUCCESS)
        {
            std::cerr << "GPS_info->Setting rate failed:"
                      << Telemetry::result_str(set_rate_result_gps_info) << std::endl;
        }

        // Set position rate to default rate
        const Telemetry::Result set_rate_result_position = telemetry->set_rate_position(0);
        if (set_rate_result_position != Telemetry::Result::SUCCESS)
        {
            std::cerr << "Position->Setting rate failed:"
                      << Telemetry::result_str(set_rate_result_position) << std::endl;
        }

        while (dc.is_connected() && !KILL_PROGRAM) // && !MANUAL_STOP_PIX)
        {
            auto gps_data = telemetry->gps_info();
            auto position_info = telemetry->position();

            telem_data->longuitude_deg = position_info.longitude_deg;
            telem_data->latitude_deg = position_info.latitude_deg;
            telem_data->altitude_m = position_info.absolute_altitude_m;

            telem_data->n_sat = gps_data.num_satellites;
            telem_data->fix_type = gps_data.fix_type;

            telem_data->timestamp_ms = TIMESTAMP_ms;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 6));
        }
    }
}