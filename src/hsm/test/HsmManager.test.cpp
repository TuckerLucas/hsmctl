#include "HsmManager.hpp"

#include <catch2/catch_test_macros.hpp>

#include "MockAuditLogger.hpp"
#include "MockSecureElement.hpp"

TEST_CASE("hardware status")
{
    MockSecureElement mock;
    MockAuditLogger mock_logger;
    HsmManager hsm(mock, mock_logger);

    SECTION("success")
    {
        auto result = hsm.status();

        REQUIRE(result == SystemStatus::OK);
    }

    SECTION("initialization fails")
    {
        mock.initResult = SystemStatus::HSM_ERROR_INIT;

        auto result = hsm.status();

        REQUIRE(result == SystemStatus::HSM_ERROR_INIT);
    }

    SECTION("ping fails")
    {
        mock.statusResult = SystemStatus::HSM_ERROR_STATUS;

        auto result = hsm.status();

        REQUIRE(result == SystemStatus::HSM_ERROR_STATUS);
    }

    SECTION("deinitialization fails")
    {
        mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

        auto result = hsm.status();

        REQUIRE(result == SystemStatus::HSM_ERROR_DEINIT);
    }

    SECTION("logging")
    {
        SECTION("success")
        {
            hsm.status();

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::STATUS);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::OK);
            REQUIRE(mock_logger.lastOptions == "");
        }

        SECTION("initialization fails")
        {
            mock.initResult = SystemStatus::HSM_ERROR_INIT;

            hsm.status();

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::STATUS);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_INIT);
            REQUIRE(mock_logger.lastOptions == "");
        }

        SECTION("ping fails")
        {
            mock.statusResult = SystemStatus::HSM_ERROR_STATUS;

            hsm.status();

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::STATUS);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_STATUS);
            REQUIRE(mock_logger.lastOptions == "");
        }

        SECTION("deinitialization fails")
        {
            mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

            hsm.status();

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::STATUS);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_DEINIT);
            REQUIRE(mock_logger.lastOptions == "");
        }
    }
}

TEST_CASE("erase key")
{
    MockSecureElement mock;
    MockAuditLogger mock_logger;
    HsmManager hsm(mock, mock_logger);
    uint8_t slot;

    SECTION("success")
    {
        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SystemStatus::OK);
    }

    SECTION("initialization fails")
    {
        mock.initResult = SystemStatus::HSM_ERROR_INIT;

        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SystemStatus::HSM_ERROR_INIT);
    }

    SECTION("erasing key fails")
    {
        mock.eraseKeyResult = SystemStatus::HSM_ERROR_ERASE_KEY;

        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SystemStatus::HSM_ERROR_ERASE_KEY);
    }

    SECTION("deinitialization fails")
    {
        mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

        auto result = hsm.eraseKey(slot);

        REQUIRE(result == SystemStatus::HSM_ERROR_DEINIT);
    }

    SECTION("logging")
    {
        SECTION("success")
        {
            hsm.eraseKey(slot);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::ERASE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::OK);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }

        SECTION("initialization fails")
        {
            mock.initResult = SystemStatus::HSM_ERROR_INIT;

            hsm.eraseKey(slot);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::ERASE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_INIT);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }

        SECTION("erasing key fails")
        {
            mock.eraseKeyResult = SystemStatus::HSM_ERROR_ERASE_KEY;

            hsm.eraseKey(slot);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::ERASE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_ERASE_KEY);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }

        SECTION("deinitialization fails")
        {
            mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

            hsm.eraseKey(slot);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::ERASE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_DEINIT);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }
    }
}

TEST_CASE("generate key")
{
    MockSecureElement mock;
    MockAuditLogger mock_logger;
    HsmManager hsm(mock, mock_logger);
    uint8_t slot;
    Curve curve = Curve::P256;

    SECTION("success")
    {
        SECTION("Ed25519 curve")
        {
            auto result = hsm.generateKey(slot, Curve::Ed25519);

            REQUIRE(mock.lastCurve == Curve::Ed25519);
            REQUIRE(result == SystemStatus::OK);
        }

        SECTION("NIST P-256 curve")
        {
            auto result = hsm.generateKey(slot, Curve::P256);

            REQUIRE(mock.lastCurve == Curve::P256);
            REQUIRE(result == SystemStatus::OK);
        }
    }

    SECTION("initialization fails")
    {
        mock.initResult = SystemStatus::HSM_ERROR_INIT;

        auto result = hsm.generateKey(slot, curve);

        REQUIRE(result == SystemStatus::HSM_ERROR_INIT);
    }

    SECTION("generating key fails")
    {
        mock.generateKeyResult = SystemStatus::HSM_ERROR_GENERATE_KEY;

        auto result = hsm.generateKey(slot, curve);

        REQUIRE(result == SystemStatus::HSM_ERROR_GENERATE_KEY);
    }

    SECTION("deinitialization fails")
    {
        mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

        auto result = hsm.generateKey(slot, curve);

        REQUIRE(result == SystemStatus::HSM_ERROR_DEINIT);
    }

    SECTION("logging")
    {
        SECTION("success")
        {
            hsm.generateKey(slot, curve);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::GENERATE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::OK);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot) +
                                                   (slot < 10 ? "  " : " ") +
                                                   "curve=" + curveToString(curve));
        }

        SECTION("initialization fails")
        {
            mock.initResult = SystemStatus::HSM_ERROR_INIT;

            hsm.generateKey(slot, curve);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::GENERATE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_INIT);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot) +
                                                   (slot < 10 ? "  " : " ") +
                                                   "curve=" + curveToString(curve));
        }

        SECTION("generating key fails")
        {
            mock.generateKeyResult = SystemStatus::HSM_ERROR_GENERATE_KEY;

            hsm.generateKey(slot, curve);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::GENERATE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_GENERATE_KEY);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot) +
                                                   (slot < 10 ? "  " : " ") +
                                                   "curve=" + curveToString(curve));
        }

        SECTION("deinitialization fails")
        {
            mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

            hsm.generateKey(slot, curve);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::GENERATE_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_DEINIT);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot) +
                                                   (slot < 10 ? "  " : " ") +
                                                   "curve=" + curveToString(curve));
        }
    }
}

TEST_CASE("read key")
{
    MockSecureElement mock;
    MockAuditLogger mock_logger;
    HsmManager hsm(mock, mock_logger);
    uint8_t slot;
    std::vector<uint8_t> pubKey;

    SECTION("success")
    {
        auto result = hsm.readKey(slot, pubKey);

        REQUIRE(pubKey == mock_public_key);
        REQUIRE(result == SystemStatus::OK);
    }

    SECTION("initialization fails")
    {
        mock.initResult = SystemStatus::HSM_ERROR_INIT;

        auto result = hsm.readKey(slot, pubKey);

        REQUIRE(result == SystemStatus::HSM_ERROR_INIT);
    }

    SECTION("reading key fails")
    {
        mock.readKeyResult = SystemStatus::HSM_ERROR_READ_KEY;

        auto result = hsm.readKey(slot, pubKey);

        REQUIRE(result == SystemStatus::HSM_ERROR_READ_KEY);
    }

    SECTION("deinitialization fails")
    {
        mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

        auto result = hsm.readKey(slot, pubKey);

        REQUIRE(result == SystemStatus::HSM_ERROR_DEINIT);
    }

    SECTION("logging")
    {
        SECTION("success")
        {
            hsm.readKey(slot, pubKey);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::READ_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::OK);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }

        SECTION("initialization fails")
        {
            mock.initResult = SystemStatus::HSM_ERROR_INIT;

            hsm.readKey(slot, pubKey);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::READ_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_INIT);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }

        SECTION("read key fails")
        {
            mock.readKeyResult = SystemStatus::HSM_ERROR_READ_KEY;

            hsm.readKey(slot, pubKey);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::READ_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_READ_KEY);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }

        SECTION("deinitialization fails")
        {
            mock.deinitResult = SystemStatus::HSM_ERROR_DEINIT;

            hsm.readKey(slot, pubKey);

            REQUIRE(mock_logger.logCalled == true);
            REQUIRE(mock_logger.lastOperation == Operation::READ_KEY);
            REQUIRE(mock_logger.lastSystemStatus == SystemStatus::HSM_ERROR_DEINIT);
            REQUIRE(mock_logger.lastOptions == "slot=" + std::to_string(slot));
        }
    }
}