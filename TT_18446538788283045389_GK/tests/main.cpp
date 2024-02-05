/**
 *  ᛝ
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ios>
#include <memory>
#include <stdexcept>
#include <sstream>

#include "command_worker.hpp"
#include "error.hpp"
#include "task.hpp"
#include "settings/cli_options_parser.hpp"
#include "settings/cli_options.hpp"
#include "settings/options_config.hpp"
#include "settings/options_validator.hpp"

#include "utils/test_helpers.hpp"
#include "utils/resource_paths.hpp"

#include "utils/program_main_helper.hpp"


TEST_CASE("empty_options_t")
{
	tt_program::settings::options_t options;

	CHECK( false == options.need_show_help_msg );
	CHECK( true == options.filepath.empty() );
	CHECK( true == options.mode.empty() );
	CHECK( true == options.mode_value.empty() );
}


TEST_CASE("parse_cli_options")
{
    using opt_type = tt_program::settings::cli_options_parser::option_type;

	tt_program::settings::cli_options_parser parser;

	SUBCASE("no_options")
	{
	    int argc = 1;
		char * arguments = "test";
		char ** argv = &arguments;

		CHECK_THROWS_WITH_AS(parser.parse(argc, argv), "Too few parameters to run", std::invalid_argument);

		parser.clear();
	}

	SUBCASE("one_unexpected_option")
	{
		int argc = 2;
		char * argv[] = { "test", "-h" };

		CHECK_THROWS_WITH_AS(parser.parse(argc, argv), "Unknown option: -h", std::invalid_argument);

		parser.clear();
	}


	SUBCASE("many_unexpected_options")
	{
		int argc = 8;
		char * argv[] = { "test", "qwerty", "-h", "-f", "./out.txt", "-m", "word", "qwerty1" };

		CHECK_THROWS_WITH_AS(parser.parse(argc, argv), "Unknown option: qwerty", std::invalid_argument);

		parser.clear();
	}

	SUBCASE("one_valid_option")
	{
		int argc = 2;
		char * argv[] = { "test", "-h" };

		bool is_help_flag = false;

		parser.add_option( {"-h"}, &is_help_flag, opt_type::non_parametric );

		parser.parse(argc, argv);

		CHECK(true == is_help_flag);

		parser.clear();

		is_help_flag = false;

		parser.add_option( { "-h" }, &is_help_flag, opt_type::parametric );

	    CHECK(false == is_help_flag);

		parser.clear();
	}    

	SUBCASE("all_default_valid_options")
	{
		tt_program::settings::options_t options;

		tt_tests::initialize_parser_with_options(parser, options);

		{
			int argc = 2;
			char * argv[] = { "test", "-h" };

			parser.parse(argc, argv);

			CHECK(true == options.need_show_help_msg);
			CHECK(true == options.filepath.empty());
			CHECK(true == options.mode.empty());
			CHECK(true == options.mode_value.empty());

			tt_tests::clear_options(options);
		}

		{
			int argc = 3;
			char *argv[] = { "test", "-f", "path-to-file" };

			parser.parse(argc, argv);

			CHECK(false == options.need_show_help_msg);
			CHECK("path-to-file" == options.filepath);
			CHECK(true == options.mode.empty());
			CHECK(true == options.mode_value.empty());

			tt_tests::clear_options(options);
		}

		{
			int argc = 5;
			char *argv[] = { "test", "-f", "path_to_file", "-m", "checksum" };

			parser.parse(argc, argv);

			CHECK(false == options.need_show_help_msg);
			CHECK("path_to_file" == options.filepath);
			CHECK("checksum" == options.mode);
			CHECK(true == options.mode_value.empty());

			tt_tests::clear_options(options);
		}

		{
			int argc = 5;
			char *argv[] = { "test", "-f", "file_path", "-m", "word" };

			parser.parse(argc, argv);

			CHECK(false == options.need_show_help_msg);
			CHECK("file_path" == options.filepath);
			CHECK("word" == options.mode);
			CHECK(true == options.mode_value.empty());

			tt_tests::clear_options(options);
		}

		{
			int argc = 7;
			char *argv[] = { "test", "-f", "file_path_", "-m", "space", "-v", "mother" };

			parser.parse(argc, argv);

			CHECK(false == options.need_show_help_msg);
			CHECK("file_path_" == options.filepath);
			CHECK("space" == options.mode);
			CHECK("mother" == options.mode_value);

			tt_tests::clear_options(options);
		}

		{
			int argc = 7;
			char * argv[] = { "test", "-m", "sentenсe", "-f", "path_to_file", "-v", "father" };

			parser.parse(argc, argv);

			CHECK(false == options.need_show_help_msg);
			CHECK("path_to_file" == options.filepath);
			CHECK("sentenсe" == options.mode);
			CHECK("father" == options.mode_value);

			tt_tests::clear_options(options);
		}

		{
			int argc = 8;
			char * argv[] = { "test", "-v", "brother", "-m", "substr", "-f", "filepath_", "-h" };

			parser.parse(argc, argv);

			CHECK(true == options.need_show_help_msg);
			CHECK("filepath_" == options.filepath);
			CHECK("substr" == options.mode);
			CHECK("brother" == options.mode_value);

			tt_tests::clear_options(options);
		}

		parser.clear();
	}

	SUBCASE("unexpected_option_with_valid")
	{
		tt_program::settings::options_t options;

		tt_tests::initialize_parser_with_options(parser, options);

		{
			int argc = 4;
			char *argv[] = { "test", "-h", "-a", "qwerty" };

			CHECK_THROWS_WITH_AS(parser.parse(argc, argv), "Unknown option: -a", std::invalid_argument);

			tt_tests::clear_options(options);
		}

		{
			int argc = 7;
			char * argv[] = { "test", "-v", "mother", "-b", "ytrewq" };

			CHECK_THROWS_WITH_AS(parser.parse(argc, argv), "Unknown option: -b", std::invalid_argument);

			tt_tests::clear_options(options);
		}

		{
			int argc = 9;
			char * argv[] = { "test", "-v", "c++", "-h", "-m", "regexp", "-f", "path_to-file", "-c" };

			CHECK_THROWS_WITH_AS(parser.parse(argc, argv), "Unknown option: -c", std::invalid_argument);

			tt_tests::clear_options(options);
		}

		parser.clear();
	}
}


TEST_CASE("validate_cli_options")
{
	auto validator = tt_program::settings::make_unique_option_validator();
	
	SUBCASE("no_options")
	{
		tt_program::settings::options_t options;

		CHECK(validator->is_options_valid(options) == false);

		validator->clear();
	}

	SUBCASE("high_priority_option")
	{
		tt_program::settings::options_t options;
		options.need_show_help_msg = true;

		CHECK(validator->is_options_valid(options) == true);	

		validator->clear();
	}

	SUBCASE("valid_unexpected_options")
	{
		tt_program::settings::options_t options;
		options.filepath = "path_to_file";

		CHECK(validator->is_options_valid(options) == false);
		
		options.mode = "checksum";

		CHECK(validator->is_options_valid(options) == false);

		validator->clear();
	}

	SUBCASE("one_expected_option")
	{
		tt_program::settings::options_t options;
		
		{
			validator->add_option("-h", "-h", {} );

			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.filepath = "filepath";
			CHECK(validator->is_options_valid(options) == true);

			options.mode = "checksum";
			CHECK(validator->is_options_valid(options) == true);

			validator->clear();
			tt_tests::clear_options(options);
		}

		{
			validator->add_option( "checksum", "-m", {} );

			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;

			options.filepath = "filepath_";
			options.mode     = "checksum";

			CHECK(validator->is_options_valid(options) == true);


			options.mode_value = "mother";
			CHECK(validator->is_options_valid(options) == false);

			validator->clear();
			tt_tests::clear_options(options);
		}

		{
			validator->add_option( "word", "-m", {} );

			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;

			options.filepath = "filepath";
			options.mode     = "word";

			CHECK(validator->is_options_valid(options) == true);

			options.mode_value = "mother";
			CHECK(validator->is_options_valid(options) == false);

			validator->clear();
			tt_tests::clear_options(options);
		}

		{
			validator->add_option( "word", "-m", { "-v" } );

			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;

			options.filepath = "filepath";
			options.mode     = "word";

			CHECK(validator->is_options_valid(options) == true);

			options.mode_value = "mother";
			CHECK(validator->is_options_valid(options) == true);

			validator->clear();
			tt_tests::clear_options(options);
		}

		{
			validator->add_option( "mother", "-v", { } );

			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;

			options.filepath = "filepath";
			options.mode_value = "mother";

			CHECK(validator->is_options_valid(options) == false);

			options.mode = "word";

			CHECK(validator->is_options_valid(options) == false);

			validator->add_option( "word", "-m", { } );

			CHECK(validator->is_options_valid(options) == false);

			validator->clear();
			tt_tests::clear_options(options);
		}

		{
			validator->add_option( "mother", "-v", { "-m" } );

			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;

			options.filepath = "filepath";
			options.mode_value = "mother";

			CHECK(validator->is_options_valid(options) == false);

			options.mode = "word";

			CHECK(validator->is_options_valid(options) == false);

			validator->add_option( "word", "-m", { "-v" } );

			CHECK(validator->is_options_valid(options) == true);

			validator->clear();
			tt_tests::clear_options(options);
		}

		validator->clear();
	}

	SUBCASE("all_expected_options")
	{
		tt_program::settings::options_t options;

		tt_tests::initialize_validator_with_default_config(validator);
		
		{
			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;


			options.mode = "checksum";

			CHECK(validator->is_options_valid(options) == false);

			options.mode_value = "mother";

			CHECK(validator->is_options_valid(options) == false);

			options.mode = "word";

			CHECK(validator->is_options_valid(options) == false);

			tt_tests::clear_options(options);
		}

		{
			options.need_show_help_msg = true;

			CHECK(validator->is_options_valid(options) == true);

			options.need_show_help_msg = false;

			options.filepath = "filepath";
			options.mode = "checksum";

			CHECK(validator->is_options_valid(options) == true);

			options.mode_value = "qwerty";

			CHECK(validator->is_options_valid(options) == false);

			options.mode = "words";

			CHECK(validator->is_options_valid(options) == true);

			tt_tests::clear_options(options);
		}

		validator->clear();
	}

}


TEST_CASE("task_t")
{
	SUBCASE("dafault_constructor")
	{
		tt_program::task::task_t task;

		CHECK(false == tt_program::utils::is_valid_task(task));
	}

	SUBCASE("empty_options_t")
	{
		tt_program::settings::options_t options;

		auto task = tt_program::task::make_task(options);

		CHECK(false == tt_program::utils::is_valid_task(task));
	}

	SUBCASE("invalid_options")
	{
		tt_program::settings::options_t options;
		options.mode = "jhfajhjfkahlsdjhkjlhsldfk";

		auto task = tt_program::task::make_task(options);

		CHECK(false == tt_program::utils::is_valid_task(task));
	}
}


TEST_CASE("file_process_option")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	SUBCASE("invalid_options")
	{
		{
			tt_program::settings::options_t options;

			worker.process( tt_program::task::make_task(options) );
		
			CHECK(out_stream.str() == def_stream.str());

			worker.process( tt_program::task::make_task(options) );

			out_stream.str("");
			def_stream.str("");
		}
		
		{
			tt_program::settings::options_t options;
			options.mode = "qwerty";

			worker.process( tt_program::task::make_task(options) );
		
			CHECK(out_stream.str() == def_stream.str());

			worker.process( tt_program::task::make_task(options) );

			out_stream.str("");
			def_stream.str("");
		}

	}

	SUBCASE("print_help_test")
	{
		tt_program::settings::options_t options;
		options.need_show_help_msg = true;

		worker.process( tt_program::task::make_task(options) );

		tt_program::utils::print_message(def_stream, tt_program::settings::help_message);
		
		CHECK(out_stream.str() == def_stream.str());

		worker.process( tt_program::task::make_task(options) );

		out_stream.str("");
		def_stream.str("");
	}
}


TEST_CASE("worker_with_empty_filename")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath;

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	std::string error_msg{"Can't open file: "};
	error_msg += filepath;
	error_msg += ": iostream stream error";
	CHECK_THROWS_WITH_AS( worker.process( tt_program::task::make_task(options) ), 
		"Error: empty fileanme: iostream stream error", std::ios_base::failure);
}


TEST_CASE("worker_with_no_exist_file")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::nonexistent_file };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	std::string error_msg{"Can't open file: "};
	error_msg += filepath;
	error_msg += ": iostream stream error";
	CHECK_THROWS_WITH_AS( worker.process( tt_program::task::make_task(options) ), 
		error_msg.data(), std::ios_base::failure);
}


TEST_CASE("worker_empty_file_count_all_words")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::empty_file };
		
	tt_program::settings::options_t options;

	options.filepath = filepath;
	options.mode = "words";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_empty_file_count_word")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::empty_file };
		
	tt_program::settings::options_t options;
	
	// some value will be moved
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = "qwerty";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_empty_file_checksum")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::empty_file };

	tt_program::settings::options_t options;

	options.filepath = filepath;
	options.mode = "checksum";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_empty_file_checksum_with_additional_option")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::empty_file };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "checksum";
	options.mode_value = "qwerty";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}
	

TEST_CASE("worker_file_with_one_lexeme_count_all_words")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::one_value_file };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_file_with_one_lexeme_count_word")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::one_value_file };
	std::string pattern = "q";

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath, pattern);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_file_with_one_lexeme_checksum")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::one_value_file };

	tt_program::settings::options_t options;

	options.filepath = filepath;
	options.mode = "checksum";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_file_with_one_lexeme_checksum_with_additionaL_option")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::one_value_file };

	tt_program::settings::options_t options;

	options.filepath = filepath;
	options.mode = "checksum";
	options.mode_value = "qwerty";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_main_file_count_all_words")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::main_file };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_main_file_count_one_letter_word")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::main_file };
	std::string pattern { "q" };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath, pattern);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_main_file_count_one_word")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::main_file };
	std::string pattern { "mother" };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath, pattern);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_main_file_checksum")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::main_file };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "checksum";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}

TEST_CASE("worker_main_file_checksum_with_additional_option_value")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::main_file };
	std::string pattern { "@:SK+!(#$**" };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "checksum";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_boundary_case_count_all_words")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::boundary_case_file };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_boundary_case_file_count_one_word")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::boundary_case_file };
	std::string pattern { "qwerty" };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath, pattern);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_boundary_case_1_file_count_all_words")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::boundary_case_file_1 };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("worker_boundary_case_file_count_one_word")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	std::string filepath{ tt_tests::boundary_case_file_1 };
	std::string pattern { "qwerty" };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath, pattern);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


#if ENABLE_LARGE_FILE_TESTS

TEST_CASE( "large_file_count_word" )
{
	std::string filepath{ tt_tests::large_file };

	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };
	std::string pattern { "@:SK+!(#$**" };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";
	options.mode_value = pattern;

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath, pattern);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE( "large_file_count_all_words" )
{
	std::string filepath{ tt_tests::large_file };

	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "words";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::cals_words_in_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE( "large_file_checksum" )
{
	std::string filepath{ tt_tests::large_file };

	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "checksum";

	worker.process( tt_program::task::make_task(options) );

	auto test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE( "large_file_checksum_with_additional_option" )
{
	std::string filepath{ tt_tests::large_file };

	std::stringstream out_stream;
	std::stringstream def_stream;

	tt_program::command_worker worker{ out_stream };

	tt_program::settings::options_t options;
	options.filepath = filepath;
	options.mode = "checksum";
	options.mode_value = "@:SK+!(#$**";

	worker.process( tt_program::task::make_task(options) );

	std::uint32_t test_sum = tt_tests::calc_checksum_of_file(filepath);

	tt_program::utils::print_message( def_stream, std::to_string(test_sum) );

	CHECK(out_stream.str() == def_stream.str());
}

#endif // ENABLE_LARGE_FILE_TESTS

TEST_CASE("program_main_without_options")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	int argc = 1;
	char * argv[] = { "test" } ;

	auto error = tt_tests::main_helper(argc, argv, out_stream);

	tt_program::utils::print_message( def_stream, "Too few parameters to run");
	tt_program::utils::print_message( def_stream, tt_program::settings::help_message );

	CHECK(error == error::to_int(error::errc::invalid_argument));
	CHECK(out_stream.str() == def_stream.str());
}


TEST_CASE("program_main_invalid_options_only")
{
	std::stringstream out_stream;
	std::stringstream def_stream;

	int argc = 6;
	char * argv[] = { "test", "-a", "qwe", "-?", "-c" "copy" } ;

	auto error = tt_tests::main_helper(argc, argv, out_stream);
		
	tt_program::utils::print_message( def_stream, "Unknown option: -a");
	tt_program::utils::print_message( def_stream, tt_program::settings::help_message );
	
	CHECK(error == error::to_int(error::errc::invalid_argument));
	CHECK(out_stream.str() == def_stream.str());
}


/* etc */

