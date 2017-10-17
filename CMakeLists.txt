cmake_minimum_required(VERSION 3.8)
project(flyhero_client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp src/screen.h src/screen.cpp src/udp_client.cpp src/udp_client.h src/crc.cpp src/crc.h src/shared_data.cpp src/shared_data.h)

add_executable(flyhero_client ${SOURCE_FILES})

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(flyhero_client PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(flyhero_client "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_link_libraries(flyhero_client "${CURSES_LIBRARIES}")