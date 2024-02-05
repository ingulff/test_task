#ifndef TT_HTTP_SESSION_HPP
#define TT_HTTP_SESSION_HPP

#include <functional>
#include <memory>
#include <string>
#include <type_traits>

#include <boost/asio/io_context.hpp>
#include <boost/beast/ssl.hpp>


namespace tt_tests
{
using namespace std::chrono_literals;
constexpr std::chrono::milliseconds default_inactive_timeout = 1000ms;
class http_session : public std::enable_shared_from_this<http_session>
{
public:
	using response_callback_t = std::function<void(std::string, std::uint32_t)>;

	enum class http_method
	{
		GET,
		POST
	};

	struct connect_t
	{
		std::string host;
		std::uint16_t port;
		std::uint64_t secret;
		std::string target;
		std::uint16_t version;
		http_method method;
	};

	struct ssl
	{
		using enabled_ssl = std::true_type;

		using disabled_ssl = std::false_type;
	};

	enum class status 
    {
        created,
        resolving,
        connecting,
        handshaking,
        connected,
        writing,
        reading,
        error_occurred,
        wait_next_request,
        null
    };

public:

	explicit http_session(boost::asio::io_context & io_context, ssl::enabled_ssl);

	explicit http_session(boost::asio::io_context & io_context, ssl::disabled_ssl);

	http_session(http_session & other) = delete;

	http_session & operator=(http_session & other) = delete;

	http_session(http_session && other) = delete;

	http_session & operator=(http_session && other) = delete;

	~http_session();

public:

	status get_status() const noexcept;


	status set_status(status new_status);


	std::chrono::steady_clock::time_point  last_activity() const;

public:

	void send(connect_t settings, response_callback_t callbask);

private:
	class http_session_impl;
	std::shared_ptr<http_session_impl> m_impl;
};

} // namespace tt_tests

#endif // TT_HTTP_SESSION_HPP

