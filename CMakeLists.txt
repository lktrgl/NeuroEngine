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
				include/noptim/quick_descent.h

				src/cppapp/smoke_test_all.cpp
				include/cppapp/smoke_test_all.h

				src/cppapp/smoke_test_neuron.cpp
				include/cppapp/smoke_test_neuron.h
				src/cppapp/smoke_test_find_minimum.cpp
				include/cppapp/smoke_test_find_minimum.h
				src/cppapp/smoke_test_quick_descent.cpp
				include/cppapp/smoke_test_quick_descent.h
				include/diffsolve/diffsolve.h
				src/cppapp/smoke_test_diffsolve.cpp
				include/cppapp/smoke_test_diffsolve.h
				include/integ/integral.h
				src/cppapp/smoke_test_integral.cpp
				include/cppapp/smoke_test_integral.h

				include/utils/tuple_utils.h
				include/utils/target_functions.h
				)

target_link_libraries ( NeuroEngine_cpp
    ${CMAKE_THREAD_LIBS_INIT}
	)

add_executable (NeuroEngine_c
				src/capp/main.c
				)
