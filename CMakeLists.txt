cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Guarder2)

set(ARDUINO_C_FLAGS "-DFIBER_STACK_SIZE=64")
set(ARDUINO_CXX_FLAGS "${ARDUINO_C_FLAGS} -std=c++11 -fno-exceptions")
project(${PROJECT_NAME} C CXX)

set(${CMAKE_PROJECT_NAME}_HDRS
        button.h
        debouncer.h
        serial.h
        buzzer.h
        guarder.h
        guarder_module.h
        lamp.h
        threads.h
        sound.h
        util.h
        action_debouncer.h
        )
#set(${CMAKE_PROJECT_NAME}_SRCS Guarder2.cpp threads.cpp)
set(${CMAKE_PROJECT_NAME}_SRCS Guarder2.cpp)
#set(${CMAKE_PROJECT_NAME}_SKETCH Guarder2.ino)

link_directories(${ARDUINO_SDK_PATH}/libraries)
set(fiber_RECURSE True)
set(NewTone_RECURSE True)
set(rtttl_RECURSE True)
set(EEPROM_RECURSE True)

set(${CMAKE_PROJECT_NAME}_BOARD uno)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
