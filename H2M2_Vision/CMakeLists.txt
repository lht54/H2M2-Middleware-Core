cmake_minimum_required(VERSION 3.16)
project(h2m2_middle)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(iceoryx_posh CONFIG REQUIRED)
find_package(iceoryx_hoofs CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
add_executable(h2m2_pub publisher_node.cpp)
target_link_libraries(h2m2_pub PRIVATE iceoryx_posh::iceoryx_posh iceoryx_hoofs::iceoryx_hoofs ${OpenCV_LIBS})

add_executable(h2m2_sub subscriber_node.cpp)
target_link_libraries(h2m2_sub PRIVATE iceoryx_posh::iceoryx_posh iceoryx_hoofs::iceoryx_hoofs ${OpenCV_LIBS})