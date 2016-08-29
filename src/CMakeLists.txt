cmake_minimum_required(VERSION 2.8.3)
project(uma)
###########
## Build ##
###########
add_definitions("-Wall -std=c++11")
add_executable(uma
	main.cpp
	FileReader.cpp
	DataStructure.cpp
	UmaDataAnalyzer.cpp
	timer/timer.cpp
)
#target_link_libraries(
#)


