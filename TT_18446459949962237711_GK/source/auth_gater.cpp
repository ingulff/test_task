/**
 *  ᛝ
 */

#include <algorithm>
#include <fstream>
#include <iterator>

#include "utils/algorithm.hpp"
#include "auth_gater.hpp"

namespace tt_program
{

auth_gater::auth_gater() = default;

auth_gater::~auth_gater() = default;

void  auth_gater::run(const std::string & filename)
{
    std::ifstream file(filename, std::ios::in);
    file.exceptions(std::ifstream::badbit);
    if( file.is_open() == false )
    {
        std::string error_msg { "Can't open file: " };
        error_msg += filename;

        throw std::ios_base::failure(error_msg);
    }

    bool is_right_bound = false;
    for( std::int64_t left_bound, right_bound, bound; 
        file >> bound; )
    {
        if( is_right_bound )
        {
            right_bound = bound;
            m_tokens.emplace(right_bound, left_bound);
        }
        else
        {
            left_bound = bound;
        }

        is_right_bound = !is_right_bound;
    }

    file.close();
}

} // namespace tt_program

