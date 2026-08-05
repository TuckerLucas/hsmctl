#include "hsm_manager.hpp"

#include <catch2/catch_test_macros.hpp>

#include "MockSecureElement.hpp"

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

TEST_CASE("hsm_manager eraseKey")
{
    MockSecureElement mock;
    hsm_manager hsm(mock);
    uint8_t slot;

    SECTION("init fails")
    {
        mock.initResult = SecureElementStatus::ERROR_INIT;

        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SecureElementStatus::ERROR_INIT);
    }

    SECTION("eraseKey fails")
    {
        mock.eraseKeyResult = SecureElementStatus::ERROR_ERASE_KEY;

        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SecureElementStatus::ERROR_ERASE_KEY);
    }

    SECTION("deinit fails")
    {
        mock.deinitResult = SecureElementStatus::ERROR_DEINIT;

        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SecureElementStatus::ERROR_DEINIT);
    }

    SECTION("success")
    {
        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SecureElementStatus::OK);
    }
}