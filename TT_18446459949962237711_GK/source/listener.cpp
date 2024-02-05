/**
 *  ᛝ
 */

#include <boost/asio/ip/address.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl.hpp>

#include "http_session.hpp"
#include "utils/fail_report.hpp"

#include "auth_gater.hpp"
#include "listener.hpp"

namespace tt_program
{


listener::listener(
        boost::asio::io_context& io_context,
        boost::asio::ssl::context& ssl_context,
        boost::asio::ip::tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& uploads_path,
        tt_program::auth_gater & auth_gate)
        : m_io_context(io_context)
        , m_ssl_context(ssl_context)
        , m_acceptor(io_context)
        , m_uploads_path(uploads_path)
        , m_auth_gate(auth_gate)
{
    boost::beast::error_code ec;

    // Open the acceptor
    m_acceptor.open(endpoint.protocol(), ec);
    if(ec)
    {
        tt_program::utils::fail(ec, "open");
        return;
    }

    // Allow address reuse
    m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec)
    {
        tt_program::utils::fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    m_acceptor.bind(endpoint, ec);
    if(ec)
    {
        tt_program::utils::fail(ec, "bind");
        return;
    }

    // Start listening for connections
    m_acceptor.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if(ec)
    {
        tt_program::utils::fail(ec, "listen");
        return;
    }
}

void listener::run()
{
    do_accept();
}

void listener::do_accept()
{
    // The new connection gets its own strand
    m_acceptor.async_accept(
        m_io_context,
        boost::beast::bind_front_handler(
            &listener::on_accept,
            shared_from_this()));
}

void listener::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
{
    if(ec)
    {
        tt_program::utils::fail(ec, "accept");
        return; // To avoid infinite loop
    }
    else
    {
        // Create the tt_program::http_session and run it
        std::make_shared<tt_program::http_session>(
            std::move(socket),
            m_ssl_context,
            m_uploads_path,
            m_auth_gate)->run();
    }

    // Accept another connection
    do_accept();
}

}  // namespace tt_program

