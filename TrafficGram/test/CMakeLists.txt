cmake_minimum_required(VERSION 3.14)
project(TrafficGramTest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  TrafficGramTest
  TrafficGramTest.cc
  CameraListManagerTest.cc
)

# Copy required dynamic libraries to the build folder as a post-build step.
if(DEFINED ArcGISRuntime_LIBRARIES)
  add_custom_command(TARGET TrafficGramTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${ArcGISRuntime_LIBRARIES}
    $<TARGET_FILE_DIR:TrafficGramTest>)
endif()

target_link_libraries(
  TrafficGramTest PRIVATE
  GTest::gtest_main
)
target_link_libraries(TrafficGramTest PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::Multimedia
  Qt6::Positioning
  Qt6::Sensors
  Qt6::WebSockets
  ArcGISRuntime::Cpp)

include(GoogleTest)
gtest_discover_tests(TrafficGramTest)
