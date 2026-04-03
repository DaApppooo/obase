cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(obase)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 23)
set(SRC
  # add .cpp files here
)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(test
  test.cpp
  ${SRC}
)
qt_add_library(test
  ${SRC}
)

target_link_libraries(test PRIVATE Qt6::Core)

