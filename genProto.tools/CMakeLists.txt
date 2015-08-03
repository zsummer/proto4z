cmake_minimum_required(VERSION 2.6)
project(genProto)


add_definitions(-Wall  -O0 -g -ggdb  -DNDEBUG -std=c++11 -D_GLIBCXX_USE_NANOSLEEP )

set(EXECUTABLE_OUTPUT_PATH ../test/bin)

aux_source_directory(./ source)

if (APPLE)
	add_executable(genProtoMac ${source})
	target_link_libraries(genProto pthread)
else()
	add_executable(genProto ${source})
	target_link_libraries(genProto pthread rt)
endif()
