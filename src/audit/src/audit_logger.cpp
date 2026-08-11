#include "audit_logger.hpp"

#include <cassert>

audit_logger::audit_logger(const std::string& connection_string)
{
    if (sqlite3_open(connection_string.c_str(), &m_db) != SQLITE_OK)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
        return;
    }

    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS audit_log ("
        "timestamp DATETIME DEFAULT (datetime('now', 'localtime')),"
        "operation TEXT NOT NULL,"
        "auditResult TEXT NOT NULL,"
        "options TEXT NOT NULL"
        ");";

    if (sqlite3_exec(m_db, create_table_sql, nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

audit_logger::~audit_logger()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

AuditStatus audit_logger::fetch(std::vector<AuditEntry>& entries)
{
    if (!m_db)
    {
        return AuditStatus::ERROR_DB;
    }

    const char* sql = "SELECT timestamp, operation, auditResult, options FROM audit_log;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        // TODO: Implement database mock to test this line
        return AuditStatus::ERROR_DB_READ;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        AuditEntry entry;
        entry.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        entry.operation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        entry.auditResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        entry.options = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        entries.push_back(entry);
    }

    sqlite3_finalize(stmt);

    return AuditStatus::OK;
}

AuditStatus audit_logger::log(Operation operation, AuditResult auditResult, std::string options)
{
    if (!m_db)
    {
        return AuditStatus::ERROR_DB;
    }

    std::string op_str;

    switch (operation)
    {
        case Operation::STATUS:
            op_str = "STATUS";
            break;
        case Operation::ERASE_KEY:
            op_str = "ERASE_KEY";
            break;
        case Operation::GENERATE_KEY:
            op_str = "GENERATE_KEY";
            break;
        case Operation::READ_KEY:
            op_str = "READ_KEY";
            break;
        default:
            // unreachable
            // TODO: decide if this is worth testing
            assert(false && "Unhandled operation value");
            op_str = "UNKNOWN";
            break;
    }

    std::string audit_res_str;

    switch (auditResult)
    {
        case AuditResult::SUCCESS:
            audit_res_str = "SUCCESS";
            break;
        case AuditResult::FAILED:
            audit_res_str = "FAILED";
            break;
        default:
            // unreachable
            // TODO: decide if this is worth testing
            assert(false && "Unhandled audit result value");
            audit_res_str = "UNKNOWN";
            break;
    }

    std::string sql = "INSERT INTO audit_log (operation, auditResult, options) VALUES ('" + op_str +
                      "', '" + audit_res_str + "', '" + options + "');";

    if (sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        // TODO: Implement database mock to test this line
        return AuditStatus::ERROR_DB_WRITE;
    }

    return AuditStatus::OK;
}