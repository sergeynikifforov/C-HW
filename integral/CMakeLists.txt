cmake_minimum_required (VERSION 2.6) 
 
set (PROJECT integral_calculation) 
 
project (${PROJECT}) 
set (SOURCES
	integral.cpp) 

add_executable (${PROJECT} ${SOURCES} integral.exe)