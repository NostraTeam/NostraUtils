cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

option(NOU_BUILD_SHARED "If enabled, the library will be build as a shared library.")

project(NostraUtils
	VERSION 1.1.0.0
	DESCRIPTION "A general purpose C++ utility library."
	LANGUAGES CXX)
	
include(CTest)

configure_file("cmake/config.hpp.in" "${PROJECT_BINARY_DIR}/include/nostrautils/config.hpp" @ONLY)

list(APPEND NOU_SRC_FILES "src/test.cpp")

if(NOU_BUILD_SHARED)
	add_library(NostraUtils SHARED ${NOU_SRC_FILES})
else()
	add_library(NostraUtils STATIC ${NOU_SRC_FILES})
endif()

add_library(Nostra::Utils ALIAS NostraUtils)

target_include_directories(NostraUtils
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		"$<INSTALL_INTERFACE:$include>")

target_compile_definitions(NostraUtils
	PRIVATE
		"NOU_EXPORT_SYMBOLS")

if(MSVC)
	target_compile_options(NostraUtils
		PRIVATE
			"/wd4251"
			"/WX"
			"/Wall")
else()
	target_compile_options(NostraUtils
		PRIVATE
			"-Werror"
			"-Wall"
			"-Wextra"
			"-Wpedantic")
endif()

install(TARGETS NostraUtils EXPORT NostraUtilsTargets
	RUNTIME DESTINATION "bin"
	LIBRARY DESTINATION "lib"
	ARCHIVE DESTINATION "lib"
	INCLUDES DESTINATION "include")

install(DIRECTORY "src/header/" 
	DESTINATION 
		"include"
	COMPONENT
		"Develop")
# Install the additionally configured file(s)
install(DIRECTORY "${PROJECT_BINARY_DIR}/include/" 
	DESTINATION 
		"include"
	COMPONENT
		"Develop")

# Macro for simpler addition of tests
macro(nou_add_test TEST_NAME)
	add_executable(${TEST_NAME} "test/${TEST_NAME}.cpp")
add_test(
	NAME 
		${TEST_NAME}
	COMMAND 
		${TEST_NAME}
	WORKING_DIRECTORY 
		${TEST_NAME})
endmacro()

enable_testing()

# Tests start
if(BUILD_TESTING)
	nou_add_test(cmakebuild.wt)
endif()
# Tests end

install(FILES "cmake/INSTALL_README" DESTINATION ".")
install(FILES "LICENSE" DESTINATION ".")
install(FILES "CHANGELOG" DESTINATION ".")
