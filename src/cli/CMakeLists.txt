# ==== CLI executable ====
add_executable(hsmctl
    src/main.cpp
    src/cli_parser.cpp
    src/cli_display.cpp
)

target_include_directories(hsmctl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ==== Tests ====
add_executable(test_cli
    test/cli_parser.test.cpp
    src/cli_parser.cpp
)

target_include_directories(test_cli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(test_cli PRIVATE
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(test_cli)