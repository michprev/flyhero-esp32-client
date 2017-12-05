cmake_minimum_required(VERSION 3.5)
project(flyhero_client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/screen.h
        src/screen.cpp
        src/tcp_client.h
        src/tcp_client.cpp
        src/udp_client.h
        src/udp_client.cpp
        src/crc.h
        src/crc.cpp
        src/shared_data.h
        src/shared_data.cpp
        src/logger.h
        src/logger.cpp)

add_executable(flyhero_esp32_client ${SOURCE_FILES})

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(flyhero_client PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(flyhero_esp32_client "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_link_libraries(flyhero_esp32_client "${CURSES_LIBRARIES}")
target_link_libraries(flyhero_esp32_client menu)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()
