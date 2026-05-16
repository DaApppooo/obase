cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(obase)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 23)
set(SRC
  src/app.cpp
  src/anchor.cpp
  src/layout.cpp
  src/widget.cpp
  src/text.cpp
  src/push_button.cpp
  src/toggle_button.cpp
  src/hull.cpp
  src/flexbox.cpp
  src/knob.cpp
  src/numbox.cpp
  src/slider.cpp
  src/sys_settings.cpp
)

add_executable(test
  src/test.cpp
  ${SRC}
)
add_executable(asan_test
  src/test.cpp
  ${SRC}
)
# add_library(lib
#   ${SRC}
# )

target_link_libraries(test -lraylib)
target_link_libraries(asan_test -lraylib)
target_compile_options(asan_test PRIVATE -fsanitize=address)
target_link_options(asan_test PRIVATE -fsanitize=address)

