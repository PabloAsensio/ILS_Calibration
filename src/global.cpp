#include "global.h"

// template <class std::string>
std::string
set_init_parameters(const std::string &param)
{
  std::string aux;
  boost::property_tree::ptree pt;
  // std::cerr << "CONFIG_INI_PATH: " << CONFIG_INI_PATH << ' ' << param << '\n';
  boost::property_tree::ini_parser::read_ini(CONFIG_INI_PATH, pt);
  // aux = pt.get(param);
  return pt.get<std::string>(param);
}

bool to_bool(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  std::istringstream is(str);
  bool b;
  is >> std::boolalpha >> b;
  return b;
}

const float PI = 3.14159265358979323846;

std::string PIXHAWK_CONNECTION_STRING = set_init_parameters("PIXHAWK_CONNECTION_STRING");
int PIXHAWK_BAUD = ::atoi((set_init_parameters("PIXHAWK_BAUD")).c_str());
bool PIXHAWK_WAIT_READY = to_bool(set_init_parameters("PIXHAWK_WAIT_READY"));

int SDR_CENTER_FREQ_HZ = ::atoi((set_init_parameters("SDR_CENTER_FREQ_HZ")).c_str());
int SDR_SAMPLE_RATE = ::atoi((set_init_parameters("SDR_SAMPLE_RATE")).c_str());
int SDR_BUFFER_SIZE = ::atoi((set_init_parameters("SDR_BUFFER_SIZE")).c_str());
int SDR_BANDWIDTH_HZ = ::atoi((set_init_parameters("SDR_BANDWIDTH_HZ")).c_str());
int SDR_RX_CHAN = ::atoi((set_init_parameters("SDR_RX_CHAN")).c_str());
std::string SDR_RX_ANT = set_init_parameters("SDR_RX_ANT");
std::string SDR_FORMAT_STRING = set_init_parameters("SDR_FORMAT_STRING");
bool SDR_GAIN_AUTO = to_bool(set_init_parameters("SDR_GAIN_AUTO"));
int SDR_GAIN_IFGR = ::atoi((set_init_parameters("SDR_GAIN_IFGR")).c_str());
int SDR_GAIN_RFGR = ::atoi((set_init_parameters("SDR_GAIN_RFGR")).c_str());
int SDR_BUFFER_READ = ::atoi((set_init_parameters("SDR_BUFFER_READ")).c_str());
int SDR_DELAY_LOOP_RECORDING = ::atoi((set_init_parameters("PIXHAWK_BAUD")).c_str());

float ILS_LAT = ::atof((set_init_parameters("ILS_LAT")).c_str());
float ILS_LON = ::atof((set_init_parameters("ILS_LON")).c_str());
float ILS_ALT = ::atof((set_init_parameters("ILS_ALT")).c_str());
float THR_LAT = ::atof((set_init_parameters("THR_LAT")).c_str());
float THR_LON = ::atof((set_init_parameters("THR_LON")).c_str());
float THR_ALT = ::atof((set_init_parameters("THR_ALT")).c_str());

std::string MQTT_HOST = set_init_parameters("MQTT_HOST");
std::string MQTT_PORT = set_init_parameters("MQTT_PORT");
std::string MQTT_KEEPALIVE = set_init_parameters("MQTT_KEEPALIVE");
std::string MQTT_BASE_TOPIC_PUB = set_init_parameters("MQTT_BASE_TOPIC_PUB");
bool KILL_PROGRAM = false;
bool STOP_MQTT = true;
