cmake_minimum_required(VERSION 2.6)
project(lua51)

include_directories(../../luasrc/)
include_directories(./)
aux_source_directory(./ source) 
aux_source_directory(../../luasrc/ luasrc) 
set(EXECUTABLE_OUTPUT_PATH ../proto)

add_executable(lua51 ${source} ${luasrc})
target_link_libraries(lua51 m)
