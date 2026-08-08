#pragma once

#include <vector>

#include "audit_types.hpp"
#include "operation.hpp"

class IAuditLogger
{
public:
    virtual AuditStatus fetch(std::vector<AuditEntry>& entries) = 0;
    virtual AuditStatus log(Operation operation, AuditResult auditResult, std::string options) = 0;

    virtual ~IAuditLogger() = default;
};