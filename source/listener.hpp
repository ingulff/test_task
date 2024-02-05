#ifndef TT_HTTP_SERVER_LISSTENER_HPP
#define TT_HTTP_SERVER_LISSTENER_HPP

#include <memory>
#include <string>

#include <boost/asio/io_context.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl.hpp>

#include "auth_gater.hpp"

namespace tt_program
{

// Accepts incoming connections and launches the tt_program::http_sessions
class listener : public std::enable_shared_from_this<listener>
{
public:
    listener(boost::asio::io_context& ioc,
        boost::asio::ssl::context& ctx,
        boost::asio::ip::tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root,
        tt_program::auth_gater & auth_gate);

    // Start accepting incoming connections
    void run();

private:

void do_accept();

void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

private:
    boost::asio::io_context& m_io_context;
    boost::asio::ssl::context& m_ssl_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::shared_ptr<std::string const> m_uploads_path;
    tt_program::auth_gater & m_auth_gate;

};

} // namespace tt_program

#endif // TT_HTTP_SERVER_LISSTENER_HPP
