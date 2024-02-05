cmake_minimum_required(VERSION 3.20)

project(tests
	VERSION 0.0.1
	DESCRIPTION "Executable target"
	LANGUAGES CXX C
)


#-----------------------------------------------------------------------------------------------------------------------------------------
# General options
#-----------------------------------------------------------------------------------------------------------------------------------------

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRING "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(EXECUTABLE_TARGET_NAME "${PROJECT_NAME}")
add_executable(${EXECUTABLE_TARGET_NAME})

set(DEFAULT_RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resource")

if(DEFINED TT_RESOURCE_DIR)
	set(RESOURCE_DIR "${TT_RESOURCE_DIR}")
else()
	set(RESOURCE_DIR "${DEFAULT_RESOURCE_DIR}")
endif()

if(TT_TEST_WITH_LARGE_FILES)
	set(ENABLE_LARGE_FILE_TEST true)
else()
	set(ENABLE_LARGE_FILE_TEST false)
endif()


# other option

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

# other dependencies

#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

set(${EXECUTABLE_TARGET_NAME}_source
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_SOURCE_DIR}/source/command_worker.cpp
	${CMAKE_SOURCE_DIR}/source/error.cpp
	${CMAKE_SOURCE_DIR}/source/task.cpp
	${CMAKE_SOURCE_DIR}/source/settings/cli_options_parser.cpp
	${CMAKE_SOURCE_DIR}/source/settings/options_validator.cpp
)


#------------------------------------------------------------------------------------------------------------------------------------------
# Target
#------------------------------------------------------------------------------------------------------------------------------------------

target_sources(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${${EXECUTABLE_TARGET_NAME}_source}
)


target_compile_definitions(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		RESOURCE_DIR="${RESOURCE_DIR}"
		ENABLE_LARGE_FILE_TESTS=${ENABLE_LARGE_FILE_TEST}
)

set(COMPILE_OPTIONS)
if(MSVC)
	list(APPEND
		COMPILE_OPTIONS
		/std:c++17
		/Zc:__cplusplus
	)
else()
	list(APPEND
		COMPILE_OPTIONS
		-std=c++17
	)
endif()

target_compile_options(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${COMPILE_OPTIONS}
)

# target_link_options()

target_include_directories(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${CMAKE_CURRNET_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
		${CMAKE_SOURCE_DIR}/source
)

# target_link_libraries()


set_target_properties(${EXECUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)
