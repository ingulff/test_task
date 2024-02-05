/**
 *  ᛝ
 */

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <ios>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "error/error.hpp"
#include "http_server.hpp"
#include "settings.hpp"


int main(int argc, char* argv[])
{
    std::int32_t errc = error::to_int(error::errc::ok);

    try
    {
        tt_program::http_server server;

        server.run(tt_program::make_settings(argc, argv));
    }
    catch(const std::logic_error & e)
    {
        std::cerr << e.what() << std::endl;
        errc = error::to_int(error::errc::invalid_argument);
    }
    catch(std::ios_base::failure e)
    {
        std::cerr << e.what() << std::endl;
        errc = error::to_int(error::errc::file_operation_abort);
    }
    catch(...)
    {
        std::cerr << "Unknow Error" << std::endl;
        errc = error::to_int(error::errc::unknown);
    }

    return errc;
}
