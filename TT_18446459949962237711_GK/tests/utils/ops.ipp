void check_pregenerated_files()
{
    REQUIRE_FALSE(tt_tests::server_host.empty());
    REQUIRE_FALSE(tt_tests::server_port == 0);
    REQUIRE_FALSE(tt_tests::upload_target_filepaths.empty());
    REQUIRE_FALSE(tt_tests::upload_filepaths.empty());
    REQUIRE_FALSE(tt_tests::upload_files_hashs.empty());
    //REQUIRE_FALSE(tt_tests::download_filepaths.empty());
    REQUIRE_FALSE(tt_tests::auth_tokens.empty());
    REQUIRE_FALSE(tt_tests::fake_auth_tokens.empty());
}
