#include "AuditLogger.hpp"

#include <cassert>

AuditLogger::AuditLogger(const std::string& connection_string)
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

AuditLogger::~AuditLogger()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

SystemStatus AuditLogger::fetch(std::vector<AuditEntry>& entries)
{
    if (!m_db)
    {
        return SystemStatus::AUDIT_ERROR_DB_OPEN;
    }

    const char* sql = "SELECT timestamp, operation, auditResult, options FROM audit_log;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        // TODO: Implement database mock to test this line
        return SystemStatus::AUDIT_ERROR_DB_READ;
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

    return SystemStatus::OK;
}

SystemStatus AuditLogger::log(Operation operation, SystemStatus auditResult, std::string options)
{
    if (!m_db)
    {
        return SystemStatus::AUDIT_ERROR_DB_OPEN;
    }

    std::string op_str = operationToString(operation);
    std::string audit_res_str = (auditResult == SystemStatus::OK) ? "SUCCESS" : "FAILED";

    std::string sql = "INSERT INTO audit_log (operation, auditResult, options) VALUES ('" + op_str +
                      "', '" + audit_res_str + "', '" + options + "');";

    if (sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        // TODO: Implement database mock to test this line
        return SystemStatus::AUDIT_ERROR_DB_WRITE;
    }

    return SystemStatus::OK;
}