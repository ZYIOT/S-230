cmake_minimum_required(VERSION 2.8) 
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_HAL_DRIVER -DSTM32F407xx")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_HAL_DRIVER -DSTM32F407xx")


project(2G_test C CXX)
set(GTEST_ROOT libs/googletest)
include_directories(../Inc)
include_directories(../Common)
include_directories(../Hardware)
include_directories(../APP)
include_directories(../RS485)
include_directories(./BSP)


#include_directories(../Drivers/STM32F4xx_HAL_Driver/Inc)
#include_directories(../Drivers/CMSIS/Include)
#include_directories(../Drivers/CMSIS/Device/ST/STM32F4xx/Include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()
set(G2_PROJECT_SRCS "")
find_package(GTest REQUIRED)

macro(find_regexp_files result curdir globbing)    
    file(GLOB children ${curdir}/*/${globbing} ${curdir}/*/*/${globbing} ${curdir}/*/*/*/${globbing} ${curdir}/*/*/*/*/${globbing} ${curdir}/*/*/*/*/${globbing})
    set(filelist "")
    foreach(child ${children})          
        if( NOT IS_DIRECTORY ${child})
          if( NOT ${child}  MATCHES "build" )
            message(found: ${child})
            list(APPEND filelist ${child})
          endif()
        endif()
    endforeach()
    set(${result} ${filelist})
endmacro()

#list(APPEND G2_PROJECT_SRCS ../Common/utils.c)

if(GTEST_FOUND)
  find_package(Threads REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  
  #aux_source_directory(. G2_GTEST_SRCS)
  find_regexp_files(G2_GTEST_SRCS ".." "tests/*_test.cpp")
  find_regexp_files(G2_GTEST_BSP_SRCS "." "*.cpp")
  set(G2_GTEST_SRCS ${G2_GTEST_SRCS} ${G2_GTEST_BSP_SRCS} main.cpp )

  set(G2_GTEST_SRCS ${G2_GTEST_SRCS} ${G2_TESTS_GTEST_SRCS})
  add_executable(2G_test ${G2_PROJECT_SRCS} ${G2_GTEST_SRCS})
  target_link_libraries(2G_test ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

endif(GTEST_FOUND)
