cmake_minimum_required (VERSION 3.5)
project (calibILS)

# ---------------- Compiler flags ----------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  # set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLGAS "${CMAKE_CXX_FLAGS} ${extra_flags}")

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-deprecated-declarations")

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra")
endif()

# ---------------- Building CALIBILS ----------------
include_directories(
  src/
  include/
  # include/mavsdk
  # include/mavsdk/plugins/telemetry
  )

set(CALIBILS_SRCS
  include/matplotlibcpp.h
  include/Instrumentor.h
  include/data_structures.h
  include/global.h          src/global.cpp
  include/fft.h             src/fft.cpp
  # include/signal.h         src/signal.cpp
  include/printers.h        src/printers.cpp
  include/calculations.h    src/calculations.cpp
  include/ils_analisys.h    src/ils_analisys.cpp
  include/do_work.h         src/do_work.cpp
  include/pixhawk.h         src/pixhawk.cpp
  include/mqtt_client.h     src/mqtt_client.cpp
  include/mosquittopp.h     src/mosquittopp.cpp
  # src/mavlink_upd.c
  # include/mavsdk/geometry.h
  # include/mavsdk/connection_result.h
  # include/mavsdk/plugin_base.h
  # include/mavsdk/system.h
  # include/mavsdk/mavsdk.h
  # include/mavsdk/plugins/telemetry/telemetry.h
  src/main.cpp
)

# add_library(calibILSLib SHARED ${CALIBILS_SRCS})

add_executable(${PROJECT_NAME}
  ${CALIBILS_SRCS})

# add_executable(mavlink_test
#   src/mavlink_upd.c)

# set(SoapySDR_LIBRARY "/usr/lib/x86_64-linux-gnu/libSoapySDR.so.0.6")
# set(HackRFSuport_LIBRARY "/usr/lib/x86_64-linux-gnu/SoapySDR/modules0.6/libHackRFSupport.so")

# ---------------- REQUIREMENTS ----------------
find_package(SoapySDR REQUIRED)
find_package(MAVSDK REQUIRED)
find_package(mavlink REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)
target_include_directories(${PROJECT_NAME} 
  PRIVATE ${PYTHON_INCLUDE_DIRS} 
  )

target_link_libraries(${PROJECT_NAME}
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_mission
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk_geofence
    MAVSDK::mavsdk
    

    # mavlink
    mosquitto
    pthread
    fftw3f
    # gnuradio
    SoapySDR
    ${PYTHON_LIBRARIES}
	  # calibILSLib
  )
  
  # # TESTING
  # include(cmake/gtest.cmake)
  # enable_testing()
  # include_directories(${GTEST_INCLUDE_DIRS})
  # file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
  # foreach(_test_file ${TEST_SRC_FILES})
  #     get_filename_component(_test_name ${_test_file} NAME_WE)
  #     add_executable(${_test_name} ${_test_file})
  #     add_dependencies(${_test_name} googletest)
  #     target_link_libraries(${_test_name} gtest gtest_main
  #       ${CMAKE_THREAD_LIBS_INIT}
  #       MAVSDK::mavsdk_action
  #       MAVSDK::mavsdk_mission
  #       MAVSDK::mavsdk_telemetry
  #       MAVSDK::mavsdk_geofence
  #       MAVSDK::mavsdk

  #       mosquitto
  #       pthread
  #       fftw3f
  #       # gnuradio
  #       SoapySDR
  #       ${PYTHON_LIBRARIES}
  #       # calibILSLib
            
  #       )
  #     add_test(${_test_name} ${_test_name})
  #     set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
  # endforeach()