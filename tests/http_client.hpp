#ifndef TT_HTTP_CLIENT_HPP
#define TT_HTTP_CLIENT_HPP

#include <functional>
#include <string>
#include <unordered_map>

#include <cstdint>

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include "http_session.hpp"

namespace tt_tests
{

class http_client
{
public:
	using connect_t = tt_tests::http_session::connect_t;

	using http_method = tt_tests::http_session::http_method;

	using response_callback_t = tt_tests::http_session::response_callback_t;

	using status = tt_tests::http_session::status;

	using ssl = tt_tests::http_session::ssl;

public:
	explicit http_client( boost::asio::io_context & io_context, response_callback_t response_callback );

	~http_client();

	http_client(http_client & other) = delete;

	http_client & operator=(http_client & other) = delete;

	http_client(http_client && other) = delete;

	http_client & operator=(http_client && other) = delete;

public:

	void send_http(connect_t settings);

	void send_https(connect_t settings);

private:

	void on_receive(std::string response, std::uint32_t error_code);

	std::shared_ptr<tt_tests::http_session> find_http_session(ssl::disabled_ssl) const;
	std::shared_ptr<tt_tests::http_session> find_http_session(ssl::enabled_ssl) const;

	std::shared_ptr<tt_tests::http_session> create_http_session(ssl::disabled_ssl);
	std::shared_ptr<tt_tests::http_session> create_http_session(ssl::enabled_ssl);

	void map_session(std::shared_ptr<tt_tests::http_session> session);
	
	bool is_https_session(const std::string_view & session_id) const;
	
    void on_steady_timer(const boost::system::error_code& ec);

private:
	boost::asio::io_context       & m_io_context;
	connect_t						m_settings;
	const response_callback_t       m_response_callback;

	using sessions_map = std::unordered_map<std::string, std::shared_ptr<tt_tests::http_session>>;
	sessions_map m_sessions;

	boost::asio::steady_timer m_steady_timer;

	const std::string_view m_http_label = "http";
	const std::string_view m_https_label = "https";
};

http_client::connect_t make_connect(std::string host, std::uint16_t port, std::uint64_t secret, std::string target, std::uint16_t version, http_client::http_method method);

} // namespace tt_tests

#endif // TT_HTTP_CLIENT_HPP
