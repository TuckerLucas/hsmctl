#include "hsm_manager.hpp"

#include <catch2/catch_test_macros.hpp>

#include "MockSecureElement.hpp"

TEST_CASE("status")
{
    MockSecureElement mock;
    hsm_manager hsm(mock);

    SECTION("success")
    {
        auto result = hsm.status();

        REQUIRE(result == SecureElementStatus::OK);
    }

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
}

TEST_CASE("eraseKey")
{
    MockSecureElement mock;
    hsm_manager hsm(mock);
    uint8_t slot;

    SECTION("success")
    {
        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SecureElementStatus::OK);
    }

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
}

TEST_CASE("generateKey")
{
    MockSecureElement mock;
    hsm_manager hsm(mock);
    uint8_t slot;
    std::string curve = "p256";

    SECTION("success")
    {
        SECTION("Ed25519 curve")
        {
            auto result = hsm.generateKey(slot, "ed25519");

            REQUIRE(result == SecureElementStatus::OK);
        }

        SECTION("NIST P-256 curve")
        {
            auto result = hsm.generateKey(slot, "ed25519");

            REQUIRE(result == SecureElementStatus::OK);
        }
    }

    SECTION("Invalid curve")
    {
        auto result = hsm.generateKey(slot, "invaid-curve");

        REQUIRE(result == SecureElementStatus::ERROR_INVALID_CURVE);
    }

    SECTION("init fails")
    {
        mock.initResult = SecureElementStatus::ERROR_INIT;

        auto result = hsm.generateKey(slot, curve);

        REQUIRE(result == SecureElementStatus::ERROR_INIT);
    }

    SECTION("generateKey fails")
    {
        mock.generateKeyResult = SecureElementStatus::ERROR_GENERATE_KEY;

        auto result = hsm.generateKey(slot, curve);

        REQUIRE(result == SecureElementStatus::ERROR_GENERATE_KEY);
    }

    SECTION("deinit fails")
    {
        mock.deinitResult = SecureElementStatus::ERROR_DEINIT;

        auto result = hsm.generateKey(slot, curve);

        REQUIRE(result == SecureElementStatus::ERROR_DEINIT);
    }
}