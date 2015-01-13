cmake_minimum_required(VERSION 2.6)
project(lua)

aux_source_directory(./ source) 
set(EXECUTABLE_OUTPUT_PATH ./bin)

add_executable(lua ${source})



