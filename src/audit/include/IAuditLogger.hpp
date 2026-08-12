#pragma once

#include <vector>

#include "SystemStatus.hpp"
#include "audit_types.hpp"
#include "operation.hpp"

class IAuditLogger
{
public:
    virtual SystemStatus fetch(std::vector<AuditEntry>& entries) = 0;
    virtual SystemStatus log(Operation operation, SystemStatus auditResult,
                             std::string options) = 0;

    virtual ~IAuditLogger() = default;
};