cmake_minimum_required (VERSION 2.6)
project (NeuroEngine)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS "-O0 -g3 -Wall -Wextra -Werror")

set (CMAKE_C_FLAGS "-O0 -g3 -Wall -Wextra -Werror")

find_package (Threads REQUIRED)

include_directories (
				include
				contrib/lggm/include
				)

add_executable (NeuroEngine_cpp
				src/cppapp/main.cpp
				include/nnet/neuron_line.h
				include/nnet/neuron.h
				include/noptim/metrics.h
				include/noptim/extreme.h
				)

target_link_libraries ( NeuroEngine_cpp
    ${CMAKE_THREAD_LIBS_INIT}
	)

add_executable (NeuroEngine_c
				src/capp/main.c
				)
