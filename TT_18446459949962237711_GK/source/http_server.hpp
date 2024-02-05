#ifndef TT_HTTP_SERVER_HPP
#define TT_HTTP_SERVER_HPP

#include <memory>
#include <optional>
#include <thread>
#include <utility>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

// boost def certs
#include "libs/beast/example/common/server_certificate.hpp"

#include "auth_gater.hpp"
#include "listener.hpp"
#include "settings.hpp"

namespace tt_program
{

class http_server
{
public:

    void run(connect_t settings);

private:
    using asio_work_guard_t = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

    connect_t m_settings;
    std::shared_ptr<boost::asio::io_context> m_io_context;
    std::shared_ptr<boost::asio::ssl::context> m_ssl_context;
    std::shared_ptr<tt_program::listener> m_listener;
    std::optional<asio_work_guard_t> m_io_context_work;
    std::vector<std::thread> m_thread_pool;
    tt_program::auth_gater m_auth_gate;
};

}  // namespace tt_program

#endif // TT_HTTP_SERVER_HPP
