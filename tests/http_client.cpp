/**
 *  ᛝ
 */


#include <chrono>
#include <memory>
#include <functional>
#include <type_traits>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>  

#include "http_client.hpp"
#include "http_session.hpp"

namespace tt_tests
{

http_client::http_client(boost::asio::io_context & io_context, response_callback_t response_callback )
	: m_io_context(io_context)
	, m_settings()
	, m_response_callback(std::move(response_callback))
	, m_steady_timer( m_io_context, tt_tests::default_inactive_timeout )
{ 
	using namespace std::chrono_literals;
	m_steady_timer.async_wait(std::bind(&http_client::on_steady_timer, this, std::placeholders::_1));
}

http_client::~http_client() = default;


void http_client::send_http(connect_t settings)
{
	auto session = find_http_session(ssl::disabled_ssl{});

    if (!session)
    {
        session = create_http_session(ssl::disabled_ssl{});
    }

	//@NOTE: client - is member of http_connection, and it not share live-time with io_context.
    //    so pass `this` pointer is safe.
    auto callback = std::bind(&http_client::on_receive, this, std::placeholders::_1, std::placeholders::_2);

	session->send(std::move(settings), callback);
}

void http_client::send_https(connect_t settings)
{
	auto session = find_http_session(ssl::enabled_ssl{});
	
    if (!session)
    {
        session = create_http_session(ssl::enabled_ssl{});
    }

	//@NOTE: session - is member of http_client, and it not share live-time with io_context.
    //    so pass `this` pointer is safe.
    auto callback = std::bind(&http_client::on_receive, this, std::placeholders::_1, std::placeholders::_2);

	session->send(std::move(settings), callback);
}


void http_client::on_receive(std::string response, std::uint32_t error_code)
{
	boost::asio::post(m_io_context, std::bind(m_response_callback, response, error_code));
}

std::shared_ptr<tt_tests::http_session> http_client::find_http_session(ssl::disabled_ssl) const
{
	std::shared_ptr<tt_tests::http_session> result;
	std::chrono::steady_clock::time_point max_point = std::chrono::steady_clock::time_point ::min();

	for(auto && [session_id, session] : m_sessions)
    {
        if(session->get_status() == status::wait_next_request)
        {
			bool is_https_session = session_id.substr(session_id.size() - m_https_label.size() ) == m_https_label;
            if (is_https_session == false &&  max_point < session->last_activity())
            {
                max_point = session->last_activity();
                result = session;
            }
        }
    }

	if (result != nullptr)
        return result;
	
	max_point = std::chrono::steady_clock::time_point ::min();
    //2. IF not found, error status-ed clients also possible

	for (auto &&[session_id, session] : m_sessions )
    {
        if (session->get_status() == status::error_occurred)
        {
			bool is_https_session = session_id.substr(session_id.size() - m_https_label.size() ) == m_https_label;
            if ( is_https_session == false && max_point < session->last_activity())
            {
                max_point = session->last_activity();
                result = session;
            }
        }
    }

	//There may be not found, returns empty shared_ptr.
    return result;
}

std::shared_ptr<tt_tests::http_session> http_client::find_http_session(ssl::enabled_ssl) const
{
    std::shared_ptr<tt_tests::http_session> result;
	std::chrono::steady_clock::time_point max_point = std::chrono::steady_clock::time_point ::min();

	for (auto && [session_id, session] : m_sessions)
    {
        if (session->get_status() == status::wait_next_request)
        {
			bool use_https = is_https_session(session_id);
            if (use_https == true &&  max_point < session->last_activity())
            {
                max_point = session->last_activity();
                result = session;
            }
        }
    }

	if (result != nullptr)
        return result;
	
	max_point = std::chrono::steady_clock::time_point ::min();
    //2. IF not found, error status-ed clients also possible

	for (auto &&[session_id, session] : m_sessions )
    {
        if (session->get_status() == status::error_occurred)
        {
			bool use_https = is_https_session(session_id);
            if ( use_https == true && max_point < session->last_activity())
            {
                max_point = session->last_activity();
                result = session;
            }
        }
    }

	//There may be not found, returns empty shared_ptr.
    return result;
}


std::shared_ptr<tt_tests::http_session> http_client::create_http_session(ssl::disabled_ssl)
{
	std::shared_ptr<tt_tests::http_session> session;

	session = std::make_shared<tt_tests::http_session>(m_io_context, ssl::disabled_ssl{});
	
	map_session(session);

	return session;
}


std::shared_ptr<tt_tests::http_session> http_client::create_http_session(ssl::enabled_ssl)
{
	std::shared_ptr<tt_tests::http_session> session;

	session = std::make_shared<tt_tests::http_session>(m_io_context, ssl::enabled_ssl{});
	
	map_session(session);

	return session;
}


void http_client::map_session(std::shared_ptr<tt_tests::http_session> session)
{
	std::string session_id;
	
	do
	{
		session_id = boost::uuids::to_string(boost::uuids::random_generator()());;
	}
	while ( m_sessions.count(session_id) );

	m_sessions[session_id] = session;
}


bool http_client::is_https_session(const std::string_view & session_id) const 
{
	return session_id.substr(session_id.size() - m_https_label.size() ) == m_https_label;
}


void http_client::on_steady_timer(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		return;
	}

	if (m_steady_timer.expiry() <= boost::asio::steady_timer::clock_type::now())
    {
        
        std::chrono::milliseconds timeout = std::max(std::chrono::milliseconds(5000),  tt_tests::default_inactive_timeout /3);
        if (m_sessions.empty())
            timeout = tt_tests::default_inactive_timeout;

        m_steady_timer.expires_after(timeout);

        std::chrono::steady_clock::time_point limit_time = std::chrono::steady_clock::now() - tt_tests::default_inactive_timeout;

        for (auto iter = m_sessions.cbegin(); iter != m_sessions.cend(); )
        {
            auto [session_id, session] = *iter;
            auto activity = session->last_activity();
            status status = session->get_status();

            bool can_delete = (activity < limit_time) && (status == status::error_occurred || status == status::wait_next_request);
            if (can_delete)
            {
                iter = m_sessions.erase(iter);
                session->set_status(status::null);
            } else {
                ++iter;
            }
        }
    } // timer.expiry() <= now
}

http_client::connect_t make_connect(std::string host, std::uint16_t port, std::uint64_t secret, std::string target, std::uint16_t version, http_client::http_method method)
{
	return { std::move(host), port, secret, std::move(target), version, method };
}

} // namespace tt_tests
