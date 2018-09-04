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
		"$<INSTALL_INTERFACE:include>")

target_compile_definitions(NostraUtils
	PRIVATE
		"NOU_EXPORT_SYMBOLS")

if(MSVC)
	target_compile_options(NostraUtils
		PRIVATE
			"/wd4251"
			"/WX")
else()
	target_compile_options(NostraUtils
		PRIVATE
			"-Werror"
			"-Wall"
			"-Wextra"
			"-Wpedantic")
endif()

install(TARGETS NostraUtils EXPORT NostraUtilsTargets
	RUNTIME 
		DESTINATION "bin"
	COMPONENT 
		"Required"
	LIBRARY 
		DESTINATION "lib"
	COMPONENT 
		"Required"
	ARCHIVE	
		DESTINATION "lib"
	COMPONENT 
		"Required"
	INCLUDES 
		DESTINATION "include")

install(DIRECTORY "include" 
	DESTINATION 
		"."
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
	add_executable("${TEST_NAME}" "test/${TEST_NAME}.cpp")

	target_link_libraries("${TEST_NAME}" Nostra::Utils)

add_test(
	NAME 
		"${TEST_NAME}"
	COMMAND 
		"${TEST_NAME}"
	WORKING_DIRECTORY 
		"${TEST_NAME}")
endmacro()

enable_testing()

# Tests start
if(BUILD_TESTING)
	nou_add_test(cmakebuild.wt)
	nou_add_test("test.wt")
endif()
# Tests end

install(FILES "cmake/INSTALL_README" 
	DESTINATION	
		"." 
	COMPONENT 
		"Required")

install(FILES "LICENSE"
	DESTINATION	
		"." 
	COMPONENT 
		"Required")

install(FILES "CHANGELOG"
	DESTINATION	
		"." 
	COMPONENT 
		"Required")


include(CMakePackageConfigHelpers)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/NostraUtilsConfigVersion.cmake"
	VERSION "${PROJECT_VERSION}"
	COMPATIBILITY AnyNewerVersion)

export(EXPORT NostraUtilsTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/NostraUtilsTargets.cmake"
	NAMESPACE Nostra::)

configure_file("cmake/NostraUtilsConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/NostraUtilsConfig.cmake" @ONLY)

set(CONFIG_PACKAGE_LOCATION "lib/cmake/NostraUtils")

install(EXPORT NostraUtilsTargets
	FILE
		"NostraUtilsTargets.cmake"
	NAMESPACE
		Nostra::
	DESTINATION
		${CONFIG_PACKAGE_LOCATION}
	COMPONENT
		"Required")

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/NostraUtilsConfig.cmake"
  DESTINATION
    ${CONFIG_PACKAGE_LOCATION}
  COMPONENT
    "Develop")

configure_file("cmake/CPackConfig.cmake.in" "${CMAKE_BINARY_DIR}/cmake/CPackConfig.cmake" @ONLY)

include("${CMAKE_BINARY_DIR}/cmake/CPackConfig.cmake")

include(CPack)