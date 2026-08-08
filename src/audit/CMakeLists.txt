# ==== Audit Logger library ====
add_library(audit
    src/audit_logger.cpp
)

target_include_directories(audit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(audit
                      PUBLIC sqlite3
                      PRIVATE common)

# ==== Tests ====
add_executable(test_audit
    test/audit_logger.test.cpp
)

target_include_directories(test_audit PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

target_link_libraries(test_audit PRIVATE
    audit
    common
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(test_audit)