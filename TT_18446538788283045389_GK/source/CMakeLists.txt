cmake_minimum_required(VERSION 3.20)

project(program
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

if(TT_TEST_WITH_LARGE_FILES)
	message(STATUS "Enable tests with large file.")
	message(STATUS "Tests can take a long time")

	set(WORKER_BUFFER_SIZE 2048)
elseif(TT_BUILD_TESTS)
	set(WORKER_BUFFER_SIZE 12)
else()
	set(WORKER_BUFFER_SIZE 1024)
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
	${CMAKE_CURRENT_SOURCE_DIR}/error.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command_worker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/task.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/settings/cli_options_parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/settings/options_validator.cpp
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
		WORKER_BUFFER_SIZE=${WORKER_BUFFER_SIZE}
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
		${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries()


set_target_properties(${EXECUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)
