cmake_minimum_required (VERSION 3.6.2)
project (Databtools)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(datab src/bmp.c src/datab.c src/raw.c)

