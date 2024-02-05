#ifndef TT_UTILS_ALGO_HPP
#define TT_UTILS_ALGO_HPP

namespace tt_program::utils
{

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

inline std::string_view trim(std::string_view string) {
    while (!string.empty() && std::isspace(static_cast<int>(static_cast<unsigned char>( string[0] ) ) ) )
    {
        string.remove_prefix(1);
    }
    while (!string.empty() && std::isspace(static_cast<int>(static_cast<unsigned char>(string.back() ) ) ) )
    {
        string.remove_suffix(1);
    }
    return string;
}

inline bool iequals(const std::string_view lhs, const std::string_view rhs)
{
    if (lhs.size() != rhs.size()) return false;
    std::string_view::size_type  lhs_size = lhs.size();
    for (std::string_view::size_type index = 0; index != lhs_size; ++index) {
        if (std::tolower(static_cast<int>(static_cast<unsigned char>(lhs[index])))  != 
            std::tolower(static_cast<int>(static_cast<unsigned char>(rhs[index]))) 
           )
           {
                return false;
           }
    }
    return true;
}

inline bool istarts_with(const std::string_view string, const std::string_view beg)
{
    if (string.size() < beg.size()) return false;
    auto sub = string.substr(0, beg.size());
    return iequals(sub, beg);
}

}

#endif // TT_UTILS_ALGO_HPP
