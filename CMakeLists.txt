cmake_minimum_required(VERSION 3.20.0)

set(MR_CMAKE_MODULES "${CMAKE_SOURCE_DIR}/cmake/Modules")

include(${MR_CMAKE_MODULES}/MetarealVersion.cmake)

project(metareal
	VERSION "${MR_VERSION_MAJOR}.${MR_VERSION_MINOR}.${MR_VERSION_PATCH}"
	LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(PROJECT_LIST "mrc")

foreach(proj ${PROJECT_LIST})
	add_subdirectory(${proj})
endforeach()
