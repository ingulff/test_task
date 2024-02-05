/**
 *  ᛝ
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>
#include <string>
#include <system_error>
#include <type_traits>

#include <cstdint>

#include "error/http_error.hpp"
#include "handle_response.hpp"
#include "http_client.hpp"
#include "utils/file_generators.hpp"
#include "utils/filesystem_helpers.hpp"

#include <memory>
#include "http_session.hpp"

namespace tt_tests
{

#ifdef HOST_IP
constexpr std::string_view server_host = SERVER_HOST_IP;
#else
constexpr std::string_view server_host = "127.0.0.1";
#endif // SERVER_HOST_IP

#ifdef PORT_IP
constexpr std::uint16_t server_port = SERVER_PORT_IP;
#else
constexpr std::uint16_t server_port = 1234;
#endif // SERVER_PORT_IP


} // namspace tt_tests

#include "utils/ops.ipp"

TEST_CASE("prepare_files")
{
	REQUIRE_FALSE(tt_tests::upload_resource_dir.empty());
	REQUIRE_FALSE(tt_tests::download_resourse_dir.empty());
	REQUIRE_FALSE(tt_tests::config_filepath.empty());

	REQUIRE_NOTHROW(tt_program::utils::throw_if_no_exist(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_program::utils::throw_if_no_directory(tt_tests::upload_resource_dir));
	
	REQUIRE_NOTHROW(tt_tests::generate_upload_filepaths(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_tests::generate_upload_file_hashs(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_tests::generate_uploads_targets_filepaths(tt_tests::upload_resource_dir));
	//REQUIRE_NOTHROW(tt_tests::generate_download_filepaths(tt_tests::download_resourse_dir));
	REQUIRE_NOTHROW(tt_tests::generate_config_gates(tt_tests::config_filepath));
	REQUIRE_NOTHROW(tt_tests::generate_tests_tokens(tt_tests::config_filepath));

	check_pregenerated_files();
}


TEST_CASE("single_client_session_with_single_request")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

#if 0  // есть некоторый конфликт в сертификатах? boost-дефолтные принимает только boost::server
	SUBCASE("google.com_without_hhtps")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::moved_permanently;
		auto settings = tt_tests::make_connect("/index.html", "google.com", 443, 11, tt_tests::http_client::http_method::GET);
		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("google.com_invalid_filepath")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::not_found;
		auto settings = tt_tests::make_connect("/test.txt", "google.com", 443, 11, tt_tests::http_client::http_method::GET);
		client.send_http( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}
#endif

	SUBCASE("single_cllient_nonexist_file_without_secret")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::unautorized;
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			0, 
			"/qwerty", 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("single_cllient_nonexist_file_with_fake_secret")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::unautorized;
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*(std::begin(tt_tests::fake_auth_tokens)), 
			"/qwerty", 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("single_cllient_nonexist_file_with_secret")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::not_found;
		auto settings = tt_tests::make_connect( 
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*(std::begin(tt_tests::auth_tokens)), 
			"/qwerty", 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("single_cllient_exist_file_without_secret")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::unautorized;
		auto settings = tt_tests::make_connect( 
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			0, 
			{ tt_tests::upload_target_filepaths[0] }, 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("single_cllient_exist_file_with_fake_secret")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::unautorized;
		auto settings = tt_tests::make_connect( 
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*(std::begin(tt_tests::fake_auth_tokens)), 
			{ tt_tests::upload_target_filepaths[0] }, 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("single_cllient_exist_file_with_secret")
	{
		tt_tests::http_client client( io_context, { handle_response_body } );
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = true;
		tt_tests::utils::expected_errc = error::http_errc::successs;
		auto settings = tt_tests::make_connect( 
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*(std::begin(tt_tests::auth_tokens)), 
			{ tt_tests::upload_target_filepaths[0] }, 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}
}


TEST_CASE("single_client_multiple_nonexist_files_without_secret")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );

	tt_tests::utils::is_reseived = false;
	tt_tests::utils::is_valid_file = false;
	tt_tests::utils::expected_errc = error::http_errc::unautorized;
	for(std::uint32_t i = 0; i < tt_tests::fake_auth_tokens.size(); ++i)  // fake_auth_tokens.size() = token_test_count in file_generators.cpp (def 5000)
	{
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			0, 
			"/qwerty", 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );
		io_context.run(); // ?
	}
}


TEST_CASE("single_client_multiple_nonexist_files_with_fake_secret")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );

	tt_tests::utils::is_reseived = false;
	tt_tests::utils::is_valid_file = false;
	tt_tests::utils::expected_errc = error::http_errc::unautorized;
	auto key_pos = std::begin(tt_tests::fake_auth_tokens);
	for(std::uint32_t i = 0; i < tt_tests::fake_auth_tokens.size(); ++i)
	{
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*key_pos, 
			"/qwerty", 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );
		io_context.run(); // ?
	}
}


TEST_CASE("single_client_multiple_nonexist_files_with_secret")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );

	tt_tests::utils::is_reseived = false;
	tt_tests::utils::is_valid_file = false;
	tt_tests::utils::expected_errc = error::http_errc::not_found;
	auto key_pos = std::begin(tt_tests::auth_tokens);
	for(std::uint32_t i = 0; i < tt_tests::auth_tokens.size(); ++i)
	{
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*key_pos, 
			"/qwerty", 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );
		io_context.run(); // ?
	}
}


TEST_CASE("single_client_multiple_exist_files_without_secret")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );

	tt_tests::utils::is_reseived = false;
	tt_tests::utils::is_valid_file = false;
	tt_tests::utils::expected_errc = error::http_errc::unautorized;
	for(std::uint32_t i = 0, targets_size = tt_tests::upload_target_filepaths.size(); 
		i < tt_tests::fake_auth_tokens.size(); ++i)  // fake_auth_tokens.size() = token_test_count in file_generators.cpp (def 5000)
	{
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			0, 
			{ tt_tests::upload_target_filepaths[i % targets_size] }, 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );
		io_context.run(); // ?
	}
}


TEST_CASE("single_client_multiple_exist_files_with_fake_secret")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );

	tt_tests::utils::is_reseived = false;
	tt_tests::utils::is_valid_file = false;
	tt_tests::utils::expected_errc = error::http_errc::unautorized;
	auto key_pos = std::begin(tt_tests::fake_auth_tokens);
	for(std::uint32_t i = 0, targets_size = tt_tests::upload_target_filepaths.size(); 
		i < tt_tests::fake_auth_tokens.size(); ++i)
	{
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*key_pos, 
			{ tt_tests::upload_target_filepaths[i % targets_size] }, 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );
		io_context.run(); // ?
	}
}


TEST_CASE("single_client_multiple_exist_files_with_secret")
{
	check_pregenerated_files();

	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );

	tt_tests::utils::is_reseived = false;
	tt_tests::utils::is_valid_file = true;
	tt_tests::utils::expected_errc = error::http_errc::successs;
	auto key_pos = std::begin(tt_tests::auth_tokens);
	for(std::uint32_t i = 0, targets_size = tt_tests::upload_target_filepaths.size(); 
		i < tt_tests::auth_tokens.size(); ++i)
	{
		auto settings = tt_tests::make_connect(
			std::string{ tt_tests::server_host }, 
			tt_tests::server_port, 
			*key_pos, 
			{ tt_tests::upload_target_filepaths[i % targets_size] }, 
			11, 
			tt_tests::http_client::http_method::GET);

		client.send_https( settings );
		io_context.run(); // ?
	}
}


#if 0
TEST_CASE("clear_section")
{
	REQUIRE_FALSE(tt_tests::download_resourse_dir.empty());

	REQUIRE_NOTHROW(tt_program::utils::throw_if_no_exist(tt_tests::download_resourse_dir));

	REQUIRE_NOTHROW(tt_tests::generate_download_filepaths(tt_tests::download_resourse_dir));

	REQUIRE(tt_tests::clear_downloading_directory() == std::error_code{});
}
#endif
