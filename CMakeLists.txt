cmake_minimum_required(VERSION 3.5)

project(CsvParser)

INCLUDE_DIRECTORIES(inc)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(_inc_root_path "${CMAKE_CURRENT_SOURCE_DIR}/inc")

file(
	GLOB_RECURSE _src_list 
	LIST_DIRECTORIES false
	"${_src_root_path}/*.cpp"
)

file(
	GLOB_RECURSE _inc_list 
	LIST_DIRECTORIES false
	"${_inc_root_path}/*.h"    
)

ADD_EXECUTABLE(CsvParser ${_src_list} ${_inc_list})

