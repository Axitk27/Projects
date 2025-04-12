cmake_minimum_required(VERSION 3.26)
project(libmodbus_demo C)

set(CMAKE_C_STANDARD 11)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/configure
        COMMAND /usr/bin/autoreconf -i
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus
        COMMENT "Generating configure script")
add_custom_target(libmodbus-autoreconf DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/configure)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/Makefile
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/configure
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus
        COMMENT "Generating makefile"
        DEPENDS libmodbus-autoreconf)
add_custom_target(libmodbus-configure DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/Makefile)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/src/.libs/libmodbus.so
        COMMAND /usr/bin/make -f ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/Makefile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus
        COMMENT "Build libmodbus using GNU make"
        DEPENDS libmodbus-configure)
add_custom_target(libmodbus DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/src/.libs/libmodbus.so)

add_executable(libmodbus_demo main.c)
add_dependencies(libmodbus_demo libmodbus)

target_include_directories(libmodbus_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/src)
target_link_libraries(libmodbus_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libmodbus/src/.libs/libmodbus.so)
