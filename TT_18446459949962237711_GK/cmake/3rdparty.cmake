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


#-----------------------------------------------------------------------------------------------------------------------------------------
# Boost
#-----------------------------------------------------------------------------------------------------------------------------------------

if(NOT DEFINED BOOST_INCLUDE OR (BOOST_INCLUDE STREQUAL ""))
    message(SEND_ERROR "BOOST_INCLUDE - not found")
    config_instructions()
else()
    message(STATUS "BOOST_INCLUDE - found: ${BOOST_INCLUDE}")
endif()

#-----------------------------------------------------------------------------------------------------------------------------------------------------------
# (Boring)SSL settings
#-----------------------------------------------------------------------------------------------------------------------------------------------------------


if(USE_OPEN_SSL)
    set(OPENSSL_ROOT_DIR ${SSL_INSTALL_PATH})
    find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL ) 
else()
# todo
endif()

