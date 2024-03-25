cmake_minimum_required(VERSION 3.20)

project(game_of_life
	VERSION 0.0.1
	DESCRIPTION "Game of Life application"
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


set(DF_WINDOW_WIDTH 1900)
set(DF_WINDOW_HEIGHT 1000)
set(DF_CELL_SIDE 15)
set(DF_LOOP_BOARD 0)
set(DF_FULLSCREEN 0)

set(DEFAULT_WINDWOW_WIDTH ${DF_WINDOW_WIDTH})
if(DEFINED DEF_WIN_WIDTH)
	set(DEFAULT_WINDWOW_WIDTH ${DEF_WIN_WIDTH})
endif()

set(DEFAULT_WINDOW_HEIGHT ${DF_WINDOW_HEIGHT})
if(DEFINED DEF_WIN_HEIGHT)
	set(DEFAULT_WINDOW_HEIGHT ${DEF_WIN_HEIGHT})
endif()

set(DEFAULT_CELL_SIDE ${DF_CELL_SIDE})
if(DEFINED DEF_CELL_SIDE)
	set(DEFAULT_CELL_SIDE ${DEF_CELL_SIDE})
endif()

set(DEFAULT_LOOP_BOARD ${DF_LOOP_BOARD})
if(DEFINED DEF_LOOP_BOARD)
	set(DEFAULT_LOOP_BOARD ${DEF_LOOP_BOARD})
endif()

set(DEFAULT_FULLSCREEN ${DF_FULLSCREEN})
if(DEFINED DEF_FULLSCREEN)
	set(DEFAULT_FULLSCREEN ${DEF_FULLSCREEN})
endif()

message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option DEFAULT_WINDWOW_WIDTH=${DEFAULT_WINDWOW_WIDTH}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option DEFAULT_WINDOW_HEIGHT=${DEFAULT_WINDOW_HEIGHT}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option DEFAULT_CELL_SIDE=${DEFAULT_CELL_SIDE}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option DEFAULT_LOOP_BOARD=${DEFAULT_LOOP_BOARD}")
message(STATUS "taget:${EXECUTABLE_TARGET_NAME} option DEFAULT_FULLSCREEN=${DEFAULT_FULLSCREEN}")

# other options

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

# other dependecies


#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

set(${EXECUTABLE_TARGET_NAME}_source
	${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/board.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cli_options_parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/error/error.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/game_engine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/io_interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/sdl_mouse_handler.cpp
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
		DEFAULT_WINDWOW_WIDTH=${DEFAULT_WINDWOW_WIDTH}
		DEFAULT_WINDOW_HEIGHT=${DEFAULT_WINDOW_HEIGHT}
		DEFAULT_CELL_SIDE=${DEFAULT_CELL_SIDE}
		DEFAULT_LOOP_BOARD=${DEFAULT_LOOP_BOARD}
		DEFAULT_FULLSCREEN=${DEFAULT_FULLSCREEN}
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
		${SDL2_INCLUDE_DIR}
)

#if(UNIX)
#    target_link_libraries(${EXECUTABLE_TARGET_NAME} pthread)
#endif(UNIX)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		SDL2::SDL2
		SDL2::SDL2main
)


set_target_properties(${EXECUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)

