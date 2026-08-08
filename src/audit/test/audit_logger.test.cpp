#include "audit_logger.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("audit_logger")
{
    SECTION("valid database")
    {
        audit_logger logger(":memory:");

        SECTION("reading empty database")
        {
            std::vector<AuditEntry> entries;

            auto result = logger.fetch(entries);

            REQUIRE(result == AuditStatus::OK);
            REQUIRE(entries.size() == 0);
        }

        SECTION("write entries and read them back")
        {
            SECTION("single entries")
            {
                SECTION("status")
                {
                    auto write_result = logger.log(Operation::STATUS, AuditResult::SUCCESS, "");

                    REQUIRE(write_result == AuditStatus::OK);

                    std::vector<AuditEntry> entries;
                    auto read_result = logger.fetch(entries);

                    REQUIRE(read_result == AuditStatus::OK);
                    REQUIRE(entries.size() == 1);
                    REQUIRE(entries[0].timestamp != "");
                    REQUIRE(entries[0].operation == "STATUS");
                    REQUIRE(entries[0].auditResult == "SUCCESS");
                    REQUIRE(entries[0].options == "");
                }

                SECTION("erase key")
                {
                    auto write_result =
                        logger.log(Operation::ERASE_KEY, AuditResult::SUCCESS, "slot=2");

                    REQUIRE(write_result == AuditStatus::OK);

                    std::vector<AuditEntry> entries;
                    auto read_result = logger.fetch(entries);

                    REQUIRE(read_result == AuditStatus::OK);
                    REQUIRE(entries.size() == 1);
                    REQUIRE(entries[0].timestamp != "");
                    REQUIRE(entries[0].operation == "ERASE_KEY");
                    REQUIRE(entries[0].auditResult == "SUCCESS");
                    REQUIRE(entries[0].options == "slot=2");
                }

                SECTION("generate key")
                {
                    auto write_result = logger.log(Operation::GENERATE_KEY, AuditResult::FAILED,
                                                   "slot=2 curve=p256");

                    REQUIRE(write_result == AuditStatus::OK);

                    std::vector<AuditEntry> entries;
                    auto read_result = logger.fetch(entries);

                    REQUIRE(read_result == AuditStatus::OK);
                    REQUIRE(entries.size() == 1);
                    REQUIRE(entries[0].timestamp != "");
                    REQUIRE(entries[0].operation == "GENERATE_KEY");
                    REQUIRE(entries[0].auditResult == "FAILED");
                    REQUIRE(entries[0].options == "slot=2 curve=p256");
                }
            }

            SECTION("multiple entries preserve order")
            {
                AuditStatus write_result;
                int n_entries = 3;

                write_result = logger.log(Operation::STATUS, AuditResult::SUCCESS, "");
                REQUIRE(write_result == AuditStatus::OK);
                write_result = logger.log(Operation::ERASE_KEY, AuditResult::SUCCESS, "slot=6");
                REQUIRE(write_result == AuditStatus::OK);
                write_result = logger.log(Operation::GENERATE_KEY, AuditResult::SUCCESS,
                                          "slot=15 curve=ed25519");
                REQUIRE(write_result == AuditStatus::OK);

                std::vector<AuditEntry> entries;
                auto read_result = logger.fetch(entries);

                REQUIRE(read_result == AuditStatus::OK);
                REQUIRE(entries.size() == n_entries);

                REQUIRE(entries[0].timestamp != "");
                REQUIRE(entries[0].operation == "STATUS");
                REQUIRE(entries[0].auditResult == "SUCCESS");
                REQUIRE(entries[0].options == "");

                REQUIRE(entries[1].timestamp != "");
                REQUIRE(entries[1].operation == "ERASE_KEY");
                REQUIRE(entries[1].auditResult == "SUCCESS");
                REQUIRE(entries[1].options == "slot=6");

                REQUIRE(entries[2].timestamp != "");
                REQUIRE(entries[2].operation == "GENERATE_KEY");
                REQUIRE(entries[2].auditResult == "SUCCESS");
                REQUIRE(entries[2].options == "slot=15 curve=ed25519");
            }
        }
    }

    SECTION("invalid database")
    {
        audit_logger logger("/invalid/path/hsmctl.db");

        SECTION("reading from database fails")
        {
            std::vector<AuditEntry> entries;

            auto result = logger.fetch(entries);

            REQUIRE(result == AuditStatus::ERROR_DB);
        }

        SECTION("writing to database fails")
        {
            auto result = logger.log(Operation::ERASE_KEY, AuditResult::SUCCESS, "");

            REQUIRE(result == AuditStatus::ERROR_DB);
        }
    }
}