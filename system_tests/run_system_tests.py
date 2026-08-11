import subprocess

BINARY = "../build/src/cli/hsmctl"

def run_command(command):
    return subprocess.run(command, capture_output=True, text=True)

def run_test(description, command, expected_exit_code=0, setup=None, cleanup=None):
    if setup:
        run_command(setup)

    result = run_command(command)

    if cleanup:
        run_command(cleanup)

    if result.returncode == expected_exit_code:
        print(f"\033[92mPASS\033[0m: {description}")
    else:
        print(f"\033[91mFAIL\033[0m: {description} (expected {expected_exit_code}, got {result.returncode})")

print("\n" + "="*50)
print("Running System Tests")
print("="*50 + "\n")

# Help
run_test("help", [BINARY, "--help"])

# Status
run_test("status (help)", [BINARY, "status", "--help"])
run_test("status", [BINARY, "status"])

# Logs
run_test("logs (help)", [BINARY, "logs", "--help"])
run_test("logs", [BINARY, "logs"])

# Erase key
run_test("erase key (help)", [BINARY, "erase-key", "--help"])
run_test("erase key", [BINARY, "erase-key", "--slot", "1"])

# Generate key
run_test("generate key (help)", [BINARY, "generate-key", "--help"])

run_test("generate key with unspecified curve",
         [BINARY, "generate-key", "--slot", "1"],
         cleanup=[BINARY, "erase-key", "--slot", "1"])

run_test("generate key with Ed25519 curve",
         [BINARY, "generate-key", "--slot", "2", "--curve", "ed25519"],
         cleanup=[BINARY, "erase-key", "--slot", "2"])

run_test("generate key with NIST P-256 curve",
         [BINARY, "generate-key", "--slot", "3", "--curve", "p256"],
         cleanup=[BINARY, "erase-key", "--slot", "3"])

run_test("generate key with curve specified first",
         [BINARY, "generate-key", "--curve", "p256", "--slot", "3"],
         cleanup=[BINARY, "erase-key", "--slot", "3"])   

run_test("generate key in occupied slot",
         [BINARY, "generate-key", "--slot", "10"],
         setup=[BINARY, "generate-key", "--slot", "10"],
         cleanup=[BINARY, "erase-key", "--slot", "10"],
         expected_exit_code=1)

# Read key
run_test("read key (help)", [BINARY, "read-key", "--help"])

run_test("read key Ed25519",
         [BINARY, "read-key", "--slot", "17"],
         setup=[BINARY, "generate-key", "--slot", "17"],
         cleanup=[BINARY, "erase-key", "--slot", "17"])

run_test("read key P-256",
         [BINARY, "read-key", "--slot", "25"],
         setup=[BINARY, "generate-key", "--slot", "25", "--curve", "p256"],
         cleanup=[BINARY, "erase-key", "--slot", "25"])

run_test("read key from empty slot",
         [BINARY, "read-key", "--slot", "10"],
         expected_exit_code=1)

print("")    