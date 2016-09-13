cmake_minimum_required(VERSION 2.8.3)
project(uma)
###########
## Build ##
###########
add_definitions("-Wall -std=c++11")
add_executable(print_density_function
	print_density_function.cpp
	timer/timer.cpp
)
add_executable(uma
	uma.cpp
	timer/timer.cpp
)
set_target_properties(print_density_function PROPERTIES COMPILE_DEFINITIONS "BUILDTYPE=1")
set_target_properties(uma PROPERTIES COMPILE_DEFINITIONS "BUILDTYPE=2")

add_subdirectory(analyzer)
add_subdirectory(simulator)
add_subdirectory(buyer)

target_link_libraries(print_density_function
	ModAnalyzer
)
target_link_libraries(uma
	ModAnalyzer
	ModSimulator
	ModBuyer
)

