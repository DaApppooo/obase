cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(obase)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 23)
set(SRC
  src/common.cpp
  src/clip.cpp
  src/clipspace.cpp
  src/grip_scroll_bar.cpp
  src/timeline.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(test
  src/test.cpp
  ${SRC}
)
qt_add_executable(asan_test
  src/test.cpp
  ${SRC}
)
qt_add_library(lib
  ${SRC}
)

target_compile_options(asan_test PRIVATE -fsanitize=address)
target_link_options(asan_test PRIVATE -fsanitize=address)

target_link_libraries(test PRIVATE Qt6::Core)
target_link_libraries(test PRIVATE Qt6::Widgets)
target_link_libraries(asan_test PRIVATE Qt6::Core)
target_link_libraries(asan_test PRIVATE Qt6::Widgets)

