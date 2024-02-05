/**
 *  ᛝ
 */

#include <boost/beast/ssl.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/read.hpp>

#include "auth_gater.hpp"
#include "http_session.hpp"
#include "request_handler.hpp"
#include "utils/fail_report.hpp"
#include "utils/files_prepare.hpp"


namespace tt_program
{

http_session::http_session(
    boost::asio::ip::tcp::socket&& socket,
    boost::asio::ssl::context& ctx,
    std::shared_ptr<std::string const> const& doc_root,
    tt_program::auth_gater & auth_gate)
        : m_stream(std::move(socket), ctx)
        , m_uploads_path(doc_root)
        , m_auth_gate(auth_gate)
{}

// Start the asynchronous operation
void http_session::run()
{
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this http_session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    boost::asio::dispatch(
        m_stream.get_executor(),
        boost::beast::bind_front_handler(
            &http_session::on_run,
            shared_from_this()));
}

void http_session::on_run()
{
    // Set the timeout.
    boost::beast::get_lowest_layer(m_stream).expires_after(
        std::chrono::seconds(30));

    // Perform the SSL handshake
    m_stream.async_handshake(
        boost::asio::ssl::stream_base::server,
        boost::beast::bind_front_handler(
            &http_session::on_handshake,
            shared_from_this()));
}

void http_session::on_handshake(boost::beast::error_code ec)
{
    if(ec)
        return tt_program::utils::fail(ec, "handshake");

    do_read();
}

void http_session::do_read()
{
    // Make the request empty before reading,
    // otherwise the operation behavior is undefined.
    m_request = {};

    // Set the timeout.
    boost::beast::get_lowest_layer(m_stream).expires_after(std::chrono::seconds(30));

    // Read a request
    boost::beast::http::async_read(m_stream, m_buffer, m_request,
        boost::beast::bind_front_handler(
            &http_session::on_read,
            shared_from_this()));
}

void http_session::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == boost::beast::http::error::end_of_stream)
        return do_close();

    if(ec)
        return tt_program::utils::fail(ec, "read");

    // Send the response
    send_response(
        tt_program::handle_request(*m_uploads_path, std::move(m_request), m_auth_gate));
}

void http_session::send_response(boost::beast::http::message_generator&& msg)
{
    bool keep_alive = msg.keep_alive();

    // Write the response
    boost::beast::async_write(
        m_stream,
        std::move(msg),
        boost::beast::bind_front_handler(
            &http_session::on_write,
            this->shared_from_this(),
            keep_alive));
}

void http_session::on_write( bool keep_alive, boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return tt_program::utils::fail(ec, "write");

    if(! keep_alive)
    {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return do_close();
    }

    // Read another request
    do_read();
}

void http_session::do_close()
{
    // Set the timeout.
    boost::beast::get_lowest_layer(m_stream).expires_after(std::chrono::seconds(30));

    // Perform the SSL shutdown
    m_stream.async_shutdown(
        boost::beast::bind_front_handler(
            &http_session::on_shutdown,
            shared_from_this()));
}

void http_session::on_shutdown(boost::beast::error_code ec)
{
    if(ec)
        return tt_program::utils::fail(ec, "shutdown");

    // At this point the connection is closed gracefully
}


} // namespace tt_program

