cmake_minimum_required(VERSION 3.10)
project(EncryptedSystemMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib/local/poco")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Poco REQUIRED Foundation Util)

include_directories(${Poco_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all source files in the src directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(monitorTasks ${SOURCES})

link_directories(${Poco_LIBRARY_DIRS})

target_link_libraries(monitorTasks PRIVATE Poco::Foundation Poco::Util)

set_target_properties(monitorTasks PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/compiled")
