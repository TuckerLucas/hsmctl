#pragma once

#include <vector>

#include "IAuditLogger.hpp"

class MockAuditLogger : public IAuditLogger
{
public:
    AuditStatus fetchResult = AuditStatus::OK;
    AuditStatus logResult = AuditStatus::OK;

    bool logCalled = false;
    Operation lastOperation = Operation::NONE;
    AuditResult lastAuditResult = AuditResult::FAILED;
    std::string lastOptions = "";

    AuditStatus fetch(std::vector<AuditEntry>& entries) override
    {
        return fetchResult;
    }

    AuditStatus log(Operation operation, AuditResult auditResult, std::string options) override
    {
        logCalled = true;
        lastOperation = operation;
        lastAuditResult = auditResult;
        lastOptions = options;

        return logResult;
    }
};