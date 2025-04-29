cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_COMPILER mingw32-gcc)
set(CMAKE_CXX_COMPILER mingw32-g++)
project(librs485 C)
include_directories(../common)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ENABLE_RS485_DRIVER_SENSORS_ZYIOT_PH201 1)
set(ENABLE_RS485_DRIVER_SENSORS_ZYIOT_DOY100 1)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rs485_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/rs485_config.h)
set(RS485_DRIVER_SENSOR_SRCS 
rs485.c
rs485_packet.c
rs485_sensor.c
drivers/sensors/sensors.c)

if(${ENABLE_RS485_DRIVER_SENSORS_PROBEST_DOY121})
 set(RS485_DRIVER_SENSOR_SRCS ${RS485_DRIVER_SENSOR_SRCS}
    drivers/sensors/probest/doy121.c)
endif()

if(${ENABLE_RS485_DRIVER_SENSORS_SZKLT_PH230})
 set(RS485_DRIVER_SENSOR_SRCS ${RS485_DRIVER_SENSOR_SRCS}
    drivers/sensors/szklt/ph230.c)
endif()

if(${ENABLE_RS485_DRIVER_SENSORS_ZYIOT_DOY100})
 set(RS485_DRIVER_SENSOR_SRCS ${RS485_DRIVER_SENSOR_SRCS}
    drivers/sensors/zyiot/doy100.c)
endif()

if(${ENABLE_RS485_DRIVER_SENSORS_ZYIOT_PH201})
 set(RS485_DRIVER_SENSOR_SRCS ${RS485_DRIVER_SENSOR_SRCS}
    drivers/sensors/zyiot/ph201.c)
endif()


add_library(LIBRS485 ${RS485_DRIVER_SENSOR_SRCS})


add_executable(rs485_test examples/main.c ../common/utils.c)
target_link_libraries(rs485_test  LIBRS485)