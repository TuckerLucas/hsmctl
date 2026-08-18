import subprocess

BINARY = "../build/src/cli/hsmctl"

def run_command(command):
    return subprocess.run(command, capture_output=True, text=True)

def run_test(description, command, expected_exit_code=0, setup=None, cleanup=None):
    if setup:
        if isinstance(setup[0], list):
            for s in setup:
                run_command(s)
        else:
            run_command(setup)

    result = run_command(command)

    if cleanup:
        if isinstance(cleanup[0], list):
            for c in cleanup:
                run_command(c)
        else:
            run_command(cleanup)

    if result.returncode == expected_exit_code:
        print(f"\033[92mPASS\033[0m: {description}")
    else:
        print(f"\033[91mFAIL\033[0m: {description} (expected {expected_exit_code}, got {result.returncode})")



print(f"\033[93m{'=' * 50}\033[0m")
print(f"\033[93mWARNING: System tests will erase ALL keys from\033[0m")
print(f"\033[93mthe HSM before running.\033[0m")
print()
print(f"\033[93mDo NOT run on a production device.\033[0m")
print(f"\033[93m{'=' * 50}\033[0m")
print()
response = input("Continue? (yes/no): ")
if response.lower() != "yes":
    print("Aborted.")
    exit(0)



print("\n" + "="*50)
print("Running System Tests")
print("="*50 + "\n")

print("Preparing HSM...")
for slot in range(32):
    run_command([BINARY, "erase-key", "--slot", str(slot)])
print("Ready.\n")



# Help
run_test("help menu", [BINARY, "--help"])

# Status
run_test("status:", [BINARY, "status", "--help"])
run_test("status", [BINARY, "status"])

# Logs
run_test("logs: help", [BINARY, "logs", "--help"])
run_test("logs", [BINARY, "logs"])

# Erase key
run_test("erase key: help", [BINARY, "erase-key", "--help"])
run_test("erase key", [BINARY, "erase-key", "--slot", "1"])

# Generate key
run_test("generate key: help", [BINARY, "generate-key", "--help"])

run_test("generate key: unspecified curve",
         [BINARY, "generate-key", "--slot", "1"],
         cleanup=[BINARY, "erase-key", "--slot", "1"])

run_test("generate key: Ed25519 curve",
         [BINARY, "generate-key", "--slot", "2", "--curve", "ed25519"],
         cleanup=[BINARY, "erase-key", "--slot", "2"])

run_test("generate key: NIST P-256 curve",
         [BINARY, "generate-key", "--slot", "3", "--curve", "p256"],
         cleanup=[BINARY, "erase-key", "--slot", "3"])

run_test("generate key: curve specified first",
         [BINARY, "generate-key", "--curve", "p256", "--slot", "3"],
         cleanup=[BINARY, "erase-key", "--slot", "3"])   

run_test("generate key: occupied slot",
         [BINARY, "generate-key", "--slot", "10"],
         setup=[BINARY, "generate-key", "--slot", "10"],
         cleanup=[BINARY, "erase-key", "--slot", "10"],
         expected_exit_code=1)

# Read key
run_test("read key: help", [BINARY, "read-key", "--help"])

run_test("read key: Ed25519",
         [BINARY, "read-key", "--slot", "17"],
         setup=[BINARY, "generate-key", "--slot", "17"],
         cleanup=[BINARY, "erase-key", "--slot", "17"])

run_test("read key: NIST P-256",
         [BINARY, "read-key", "--slot", "25"],
         setup=[BINARY, "generate-key", "--slot", "25", "--curve", "p256"],
         cleanup=[BINARY, "erase-key", "--slot", "25"])

run_test("read key: empty slot",
         [BINARY, "read-key", "--slot", "10"],
         expected_exit_code=1)

# List all keys
run_test("list all keys: help", [BINARY, "list-keys", "--help"])

run_test("list all keys: no keys", [BINARY, "list-keys"])

run_test("list all keys: one key",
         [BINARY, "list-keys"],
         setup=[BINARY, "generate-key", "--slot", "5"],
         cleanup=[BINARY, "erase-key", "--slot", "5"])

run_test("list all keys: multiple keys",
         [BINARY, "list-keys"],
         setup=[
             [BINARY, "generate-key", "--slot", "5"],
             [BINARY, "generate-key", "--slot", "6"],
             [BINARY, "generate-key", "--slot", "7"],
         ],
         cleanup=[
             [BINARY, "erase-key", "--slot", "5"],
             [BINARY, "erase-key", "--slot", "6"],
             [BINARY, "erase-key", "--slot", "7"],
         ])

run_test("list all keys: verbose",
         [BINARY, "list-keys", "--verbose"],
                  setup=[
             [BINARY, "generate-key", "--slot", "6"],
             [BINARY, "generate-key", "--slot", "7"],
         ],
         cleanup=[
             [BINARY, "erase-key", "--slot", "6"],
             [BINARY, "erase-key", "--slot", "7"],
         ])

print("")    