#-----------------------------------------------------------------------------------------------------------------------------------------
# 3rdparty config helpers
#-----------------------------------------------------------------------------------------------------------------------------------------


macro(config_instructions)
    message(FATAL_ERROR "1) Create 3rdparty_config.cmake file if no exist\n"
            "2) Define following path variables that are used for building: \n"
            "BOOST_INCLUDE - include directory for boost headers\n"
            "SSL_INSTALL_PATH" - ss; install directory
            )
endmacro()

# other helpers


#-----------------------------------------------------------------------------------------------------------------------------------------
# 3rdparty config
#-----------------------------------------------------------------------------------------------------------------------------------------

include(3rdparty_config OPTIONAL RESULT_VARIABLE LOCAL_CONFIG_RESULT)

if(NOT LOCAL_CONFIG_RESULT)
    message(SEND_ERROR "3rdparty_config.cmake not exist\n")
    config_insturctions()
endif()

#other configs

#------------------------------------------------------------------------------------------------------------------------------------------
# SDL2
#------------------------------------------------------------------------------------------------------------------------------------------

if(NOT DEFINED SDL_INSTALL_PATH OR SDL_INSTALL_PATH STREQUAL "")
    message(SEND_ERROR "SDL_INSTALL_PATH - not found")
    config_insturctions()
else()
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2 PATHS ${SDL_INSTALL_PATH})
    message(STATUS "found SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
    message(STATUS "found SDL2_LIBDIR: ${SDL2_LIBDIR}")
endif()

