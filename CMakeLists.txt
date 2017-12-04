cmake_minimum_required(VERSION 3.5)

project(CsvParserTask)

set(subdirectories
	main
	testutil
	csvparser
)

FOREACH(subdir ${subdirectories})
add_subdirectory(${subdir})
ENDFOREACH(subdir)

