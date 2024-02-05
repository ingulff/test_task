cmake_minimum_required(VERSION 3.20)

project(test_task
	VERSION 0.0.1
	DESCRIPTION "Test Task"
	LANGUAGES C CXX
)

#-----------------------------------------------------------------------------------------------------------------------------------------
# General options
#-----------------------------------------------------------------------------------------------------------------------------------------

add_compile_definitions(_WIN32_WINNT=0x0601)

option(TT_BUILD_TESTS "Enable build unit tests" ON)

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRING "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MAIN_TARGET_NAME "${PROJECT_NAME}")
add_custom_target(${MAIN_TARGET_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(3rdparty)

set(DEFAULT_HOST_IP "127.0.0.1")
set(DEFAULT_PORT_IP 1234)

set(HOST_IP ${DEFAULT_HOST_IP})
if(DEFINED TT_HOST_IP)
	set(HOST_IP ${TT_HOST_IP})
endif()

set(PORT_IP ${DEFAULT_PORT_IP})
if(DEFINED TT_PORT_IP)
	set(PORT_IP ${TT_PORT_IP})
endif()

message(STATUS "taget:${MAIN_TARGET_NAME} option TT_BUILD_TESTS=${TT_BUILD_TESTS}")
message(STATUS "taget:${MAIN_TARGET_NAME} option HOST_IP=${HOST_IP}")
message(STATUS "taget:${MAIN_TARGET_NAME} option PORT_IP=${PORT_IP}")

# other option

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

set(${MAIN_TARGET_NAME}_depencies
	http_server
)

if(TT_BUILD_TESTS)
	list(APPEND 
		${MAIN_TARGET_NAME}_depencies
		tests
	)
endif()

add_dependencies(${MAIN_TARGET_NAME}
	${${MAIN_TARGET_NAME}_depencies}
)

# other dependencies

#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

# other source


#------------------------------------------------------------------------------------------------------------------------------------------
# Target
#------------------------------------------------------------------------------------------------------------------------------------------

add_subdirectory(source)

if(TT_BUILD_TESTS)
	message(STATUS "Tests add to build")
	add_subdirectory(tests)
	enable_testing()
endif()

