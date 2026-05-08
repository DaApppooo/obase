cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(obase)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 23)
set(SRC
  
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

target_compile_options(asan_test PRIVATE -fsanitize=address)
target_link_options(asan_test PRIVATE -fsanitize=address)

