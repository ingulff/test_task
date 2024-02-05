#ifndef TT_AUTH_GATER_HPP
#define TT_AUTH_GATER_HPP

#include <iterator>
#include <map>
#include <string>

#include <cstdint>

#include <boost/beast/http/message_generator.hpp>

#include "utils/algorithm.hpp"

namespace tt_program
{

class auth_gater
{
public:
    using token_t = std::uint64_t;

public:
    auth_gater();

    ~auth_gater();

public:
    void  run(const std::string & filename);

public:
    template <class Body, class Allocator>
    bool is_auth_valid(const boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>& request) const
    {
        bool res = false;
        // Check auth=key
        std::optional<std::uint64_t> secret;
        for (const auto& header : request.base()) 
        {
            /***************1. get value and trim it *************/
            auto value = header.value();
            std::string_view value_view = tt_program::utils::trim( value );

            /***************2. get name-string and trim it *************/
            auto name_string = header.name_string();
            std::string_view name_string_view = tt_program::utils::trim(name_string);
            
            /***************4. check name-string="X-Protobuf-Message"  case-insinsetive*************/

            if (tt_program::utils::iequals(name_string_view, "secret")) 
            {
                secret.emplace(std::strtoull(value_view.data(), nullptr, 0));
            }
        }

        if( secret.has_value() )
        {
            auto pos = m_tokens.lower_bound(secret.value());

            bool is_in_tokens = pos != std::end(m_tokens);
            bool has_in_range  = (( secret.value() <= pos->first) && (secret.value() >= pos->second));

            if( is_in_tokens && has_in_range)
            {
                res = true;
            }
        }

        return res;
    }


private:
    std::map<token_t, token_t> m_tokens;
};

} // namespace tt_program

#endif // TT_AUTH_GATER_HPP
