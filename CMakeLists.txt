cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

option(NOU_BUILD_SHARED   "If enabled, the library will be build as a shared library." OFF)
option(NOU_BUILD_DOC      "If enabled, and Doxygen is installed, the documentation will be built." ON)
option(NOU_BUILD_EXAMPLES "If enabled, the examples that come with the library will be build." ON)

project(NostraUtils
	VERSION 1.1.0.0
	DESCRIPTION "A general purpose C++ utility library."
	LANGUAGES CXX)
	
include(CTest)

configure_file("cmake/config.hpp.in" "include/nostrautils/config.hpp" @ONLY)

list(APPEND NOU_SRC_FILES "src/assert.cpp")

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

target_compile_features(NostraUtils
	PUBLIC
		cxx_std_17)

target_compile_definitions(NostraUtils
	PRIVATE
		"NOU_EXPORT_SYMBOLS")

if(NOU_BUILD_SHARED)
	target_compile_definitions(NostraUtils
		PRIVATE
			"NOU_DLL")
endif()

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
		COMPONENT "Required"
	LIBRARY 
		DESTINATION "lib"
		COMPONENT "Required"
	ARCHIVE	
		DESTINATION "lib"
		COMPONENT "Required"
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



if(NOU_BUILD_DOC)
	
	message(STATUS "NostraUtils: Attempting to generate Doxygen documentation.")

	find_package(Doxygen OPTIONAL_COMPONENTS dot)

	if(DOXYGEN_FOUND)
		
		message(STATUS "NostraUtils: Doxygen executable was found, documentation will be generated.")

		configure_file("doc/Doxyfile.in" "doc/Doxyfile")

		add_custom_target(NostraUtilsDoc
			ALL COMMAND Doxygen::doxygen "doc/Doxyfile"
			WORKING_DIRECTORY "."
			COMMENT "Generating Doxygen documentation."
			VERBATIM)

		install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/html/"
			DESTINATION
				"doc"
			COMPONENT
				"Documentation")

	else()

		message(WARNING "NostraUtils: Doxygen executable could not be found, documentation generation will be omitted.")

	endif()

else()

	message(STATUS "NostraUtils: Generation of documentation is disabled.")

endif()



# Function for simpler addition of tests
function(nou_add_test TEST_NAME)
	add_executable("nou.${TEST_NAME}" "test/${TEST_NAME}.cpp")
	
	target_link_libraries("nou.${TEST_NAME}" Nostra::Utils)

	install(TARGETS "nou.${TEST_NAME}" EXPORT NostraUtilsTargets
		RUNTIME 
			DESTINATION "test"
			COMPONENT "Test")

	add_test(
		NAME 
			"nou.${TEST_NAME}"
		COMMAND 
			"nou.${TEST_NAME}"
		WORKING_DIRECTORY 
			"nou.${TEST_NAME}")
endfunction()

enable_testing()

# Tests start
if(BUILD_TESTING)
	nou_add_test(types.wt)
	nou_add_test(stddef.wt)
	nou_add_test(assert.wt)
	nou_add_test(notnull.wt)
endif()
# Tests end



# Function for simpler addition of examples
function(nou_add_example EXAMPLE_NAME)
	add_executable("${EXAMPLE_NAME}" "examples/${EXAMPLE_NAME}.cpp")

	target_link_libraries("${EXAMPLE_NAME}" Nostra::Utils)

	install(TARGETS "${EXAMPLE_NAME}" EXPORT NostraUtilsTargets
		RUNTIME 
			DESTINATION "examples"
			COMPONENT "Examples")
endfunction()

# Examples start
if(NOU_BUILD_EXAMPLES)
	nou_add_example(stddef.ex)
	nou_add_example(assert.ex)
endif()
# Examples end



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

configure_file("cmake/NostraUtilsConfig.cmake.in" "NostraUtilsConfig.cmake" @ONLY)

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

configure_file("cmake/CPackConfig.cmake.in" "cmake/CPackConfig.cmake" @ONLY)

include("${CMAKE_BINARY_DIR}/cmake/CPackConfig.cmake")

include(CPack)