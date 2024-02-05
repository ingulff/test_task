cmake_minimum_required(VERSION 3.20)

project(http_server
	VERSION 0.0.1
	DESCRIPTION "Simple http server"
	LANGUAGES C CXX
)


#-----------------------------------------------------------------------------------------------------------------------------------------
# General options
#-----------------------------------------------------------------------------------------------------------------------------------------

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRING "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(EXECUTABLE_TARGET_NAME ${PROJECT_NAME})
add_executable(${EXECUTABLE_TARGET_NAME})


set(DEFAULT_THREADS_COUNT 4)

set(THREADS_COUNT ${DEFAULT_THREADS_COUNT})
if(DEFINED TT_THREADS_NUMBER)
	set(THRETHREADS_COUNTADS_NUMBER ${TT_THREADS_COUNT})
endif()

message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option TT_USE_PRESET_FILE_MIME=${TT_USE_PRESET_FILE_MIME}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option HOST_IP=${HOST_IP}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option PORT_IP=${PORT_IP}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option THREADS_COUNT=${THREADS_COUNT}")

# other options

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

# other dependecies


#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

set(${EXECUTABLE_TARGET_NAME}_source
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/error/error.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/listener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/http_server.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/http_session.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/auth_gater.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/files_prepare.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/filesystem_helpers.cpp


)

# other source

#------------------------------------------------------------------------------------------------------------------------------------------
# Target
#------------------------------------------------------------------------------------------------------------------------------------------

target_sources(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${${EXECUTABLE_TARGET_NAME}_source}
)

target_compile_definitions(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		HOST_IP="${HOST_IP}"
		HOST_PORT=${PORT_IP}
		THREADS_COUNT=${THREADS_COUNT}
)

set(COMPILE_OPTIONS)
if(MSVC)
	list(APPEND
		COMPILE_OPTIONS
		/std:c++17
		/Zc:__cplusplus
		/bigobj
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
		${BOOST_INCLUDE}
		${OPENSSL_INCLUDE_DIR}
)

if(UNIX)
    target_link_libraries(${EXECUTABLE_TARGET_NAME} pthread)
endif(UNIX)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		OpenSSL::SSL
		OpenSSL::Crypto
)


set_target_properties(${EXECUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)

