/**
 *  ᛝ
 */

#include <memory>
#include <optional>
#include <thread>
#include <utility>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

// boost def certs
#include "libs/beast/example/common/server_certificate.hpp"

#include "auth_gater.hpp"
#include "http_server.hpp"
#include "listener.hpp"
#include "request_handler.hpp"
#include "settings.hpp"
#include "utils/run_io_context.hpp"

namespace tt_program
{

void http_server::run(connect_t settings)
{
    m_settings = std::move(settings);
    m_io_context = std::make_shared<boost::asio::io_context>(m_settings.threads_count);
    m_ssl_context = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    // This holds the self-signed certificate used by the server
    ::load_server_certificate(*m_ssl_context);
    m_ssl_context->set_options( /* boost::asio::ssl::context::default_workarounds | */
                        boost::asio::ssl::context::verify_peer );
    
    m_listener = std::make_shared<listener>(*m_io_context, 
        *m_ssl_context,
        boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address(m_settings.host), m_settings.port},
        std::make_shared<std::string>(m_settings.upload_path),
        m_auth_gate);

    m_listener->run();

    m_auth_gate.run(m_settings.auth_tokens_path);

    m_io_context_work.emplace( boost::asio::make_work_guard(*m_io_context) );

    m_thread_pool.reserve(m_settings.threads_count - 1);
    for(auto i = m_settings.threads_count - 1; i > 0; --i)
    {
        m_thread_pool.emplace_back( std::thread( tt_program::utils::run_io_context(*m_io_context) ) );
    }

    m_io_context->run();
}

} //tt_program

