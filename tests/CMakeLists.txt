cmake_minimum_required(VERSION 3.20)

project(tests
	VERSION 0.0.1
	DESCRIPTION "Tests project"
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

set(DEFAULT_INTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/resource")
set(DEFAULT_INTERNAL_UPLOAD_SOURCE_DIR "${DEFAULT_INTERNAL_SOURCE_DIR}/upload")
set(DEFAULT_INTERNAL_DOWNLOAD_SOURCE_DIR "${DEFAULT_INTERNAL_SOURCE_DIR}/download")
set(DEFAULT_CONFIG_FILEPATH "${CMAKE_SOURCE_DIR}/configs/ids.txt")
set(DEFAULT_HOST_IP "127.0.0.1")
set(DEFAULT_PORT_IP 1234)

set(DOWNLOAD_SOURCE_DIR "${DEFAULT_INTERNAL_DOWNLOAD_SOURCE_DIR}")
set(UPLOAD_SOURCE_DIR "${DEFAULT_INTERNAL_UPLOAD_SOURCE_DIR}")
set(CONFIG_FILE_PATH "${DEFAULT_CONFIG_FILEPATH}")

if(DEFINED TT_EXTERNAL_UPLOAD_SOURCE_DIR)
	set(UPLOAD_SOURCE_DIR "${EXTERNAL_SOURCE_DIR}")
endif()

if(DEFINED TT_EXTERNAL_DOWNLOAD_SOURCE_DIR)
	set(DOWNLOAD_SOURCE_DIR "${TT_EXTERNAL_DOWNLOAD_SOURCE_DIR}")
endif()

if(DEFINED TT_EXTERNAL_CONFIG_FILEPATH)
	set(CONFIG_FILE_PATH "${TT_EXTERNAL_CONFIG_FILEPATH}")
endif()

set(SERVER_HOST_IP ${HOST_IP})
set(SERVER_PORT_IP ${PORT_IP})

message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option DOWNLOAD_SOURCE_DIR=${DOWNLOAD_SOURCE_DIR}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option UPLOAD_SOURCE_DIR=${UPLOAD_SOURCE_DIR}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option CONFIG_FILE_PATH=${CONFIG_FILE_PATH}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option SERVER_HOST_IP=${SERVER_HOST_IP}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option SERVER_PORT_IP=${SERVER_PORT_IP}")

# other option

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

# other option

#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

set(${EXECUTABLE_TARGET_NAME}_source
	${CMAKE_SOURCE_DIR}/source/error/error.cpp
	${CMAKE_SOURCE_DIR}/source/utils/filesystem_helpers.cpp
	${CMAKE_SOURCE_DIR}/source/utils/files_prepare.cpp
	${CMAKE_SOURCE_DIR}/tests/main.cpp
	${CMAKE_SOURCE_DIR}/tests/http_client.cpp
	${CMAKE_SOURCE_DIR}/tests/http_session.cpp
	${CMAKE_SOURCE_DIR}/tests/3rdparty/Crc32/Crc32.cpp
	${CMAKE_SOURCE_DIR}/tests/utils/file_generators.cpp
	${CMAKE_SOURCE_DIR}/tests/utils/stream_crc32.cpp
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
		UPLOAD_SOURCE_DIR="${UPLOAD_SOURCE_DIR}"
		DOWNLOAD_SOURCE_DIR="${DOWNLOAD_SOURCE_DIR}"
		CONFIG_FILE_PATH="${CONFIG_FILE_PATH}"
		SERVER_HOST_IP="${SERVER_HOST_IP}"
		SERVER_PORT_IP=${SERVER_PORT_IP}

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
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/tests
		${CMAKE_SOURCE_DIR}/tests/3rdparty
		${BOOST_INCLUDE}
		${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		OpenSSL::SSL
		OpenSSL::Crypto
)


set_target_properties(${EXXCUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)

