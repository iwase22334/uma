cmake_minimum_required(VERSION 2.8.3)
###########
## Build ##
###########
add_definitions("-Wall -std=c++11")
add_library(ModBuyer STATIC
	buyer.cpp
)

#target_link_libraries(
#)


