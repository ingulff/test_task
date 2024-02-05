#ifndef TT_HTTP_SERVER_SESSION_HPP
#define TT_HTTP_SERVER_SESSION_HPP

#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/read.hpp>

#include "request_handler.hpp"
#include "utils/fail_report.hpp"

namespace tt_program
{
    // Handles an HTTP server connection
class http_session : public std::enable_shared_from_this<http_session>
{
public:
    // Take ownership of the socket
    explicit http_session(
        boost::asio::ip::tcp::socket&& socket,
        boost::asio::ssl::context& ctx,
        std::shared_ptr<std::string const> const& doc_root,
        tt_program::auth_gater & auth_gate);

    // Start the asynchronous operation
    void
    run();

private:
    void
    on_run();

    void on_handshake(boost::beast::error_code ec);

    void do_read();

    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

    void send_response(boost::beast::http::message_generator&& msg);

    void on_write(bool keep_alive, boost::beast::error_code ec, std::size_t bytes_transferred);

    void do_close();

    void on_shutdown(boost::beast::error_code ec);

private:
    boost::beast::ssl_stream<boost::beast::tcp_stream> m_stream;
    boost::beast::flat_buffer m_buffer;
    std::shared_ptr<std::string const> m_uploads_path;
    boost::beast::http::request<boost::beast::http::string_body> m_request;
    tt_program::auth_gater & m_auth_gate;
};

} // namespace tt_program

#endif // 