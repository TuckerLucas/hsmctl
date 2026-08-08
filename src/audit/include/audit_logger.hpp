#pragma once

#include <sqlite3.h>

#include <cstdint>
#include <string>
#include <vector>

#include "IAuditLogger.hpp"

class audit_logger : public IAuditLogger
{
public:
    audit_logger(const std::string& connection_string);
    ~audit_logger();

    AuditStatus fetch(std::vector<AuditEntry>& entries) override;
    AuditStatus log(Operation operation, AuditResult auditResult, std::string options) override;

private:
    sqlite3* m_db = nullptr;
};