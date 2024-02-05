cmake_minimum_required(VERSION 3.20)

project(test_task
	VERSION 0.0.1
	DESCRIPTION "Simple console test program"
	LANGUAGES CXX C
)

#-----------------------------------------------------------------------------------------------------------------------------------------
# General options
#-----------------------------------------------------------------------------------------------------------------------------------------

option(TT_BUILD_TESTS "Enable build unit tests" ON)
option(TT_TEST_WITH_LARGE_FILES "Enable large resorce files for tests" OFF)

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRING "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MAIN_TARGET_NAME "${PROJECT_NAME}")
add_custom_target(${MAIN_TARGET_NAME})

# other option

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

set(${MAIN_TARGET_NAME}_DEPENDENCIES
	program
)

if(TT_BUILD_TESTS)
	message(STATUS "Add tests to depends")
	list(APPEND
		${MAIN_TARGET_NAME}_DEPENDENCIES
		tests
	)
endif()

add_dependencies(${MAIN_TARGET_NAME}
	${${MAIN_TARGET_NAME}_DEPENDENCIES}
)

# other dependencies

#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

# other sources


#------------------------------------------------------------------------------------------------------------------------------------------
# Target
#------------------------------------------------------------------------------------------------------------------------------------------

add_subdirectory(source)

if(TT_BUILD_TESTS)
	message(STATUS "Add tests to build target and enable testing option")
	enable_testing()
	add_subdirectory(tests)
endif()
