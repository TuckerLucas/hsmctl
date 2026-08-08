#pragma once

enum class AuditStatus
{
    OK,

    ERROR_DB,
    ERROR_DB_READ,
    ERROR_DB_WRITE,
};

struct AuditEntry
{
    std::string timestamp;
    std::string operation;
    std::string auditResult;
    std::string options;
};

enum class AuditResult
{
    SUCCESS,
    FAILED
};