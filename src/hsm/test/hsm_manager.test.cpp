#include "hsm_manager.hpp"
#include "MockSecureElement.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("hsm_manager status")
{
    MockSecureElement mock;
    hsm_manager hsm(mock);

    SECTION("init fails")
    {
        mock.initResult = SecureElementStatus::ERROR_INIT;

        auto result = hsm.status();

        REQUIRE(result == SecureElementStatus::ERROR_INIT);
    }

    SECTION("ping fails")
    {
        mock.pingResult = SecureElementStatus::ERROR_PING;

        auto result = hsm.status();

        REQUIRE(result == SecureElementStatus::ERROR_PING);
    }

    SECTION("deinit fails")
    {
        mock.deinitResult = SecureElementStatus::ERROR_DEINIT;

        auto result = hsm.status();

        REQUIRE(result == SecureElementStatus::ERROR_DEINIT);
    }

    SECTION("success")
    {
        auto result = hsm.status();

        REQUIRE(result == SecureElementStatus::OK);
    }
}