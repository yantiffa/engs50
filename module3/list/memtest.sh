#!/bin/bash
# Runs Valgrind on all tests and check for leaks or errors.

# Exit if any command fails
set -e

TEST_DIR="./"

# Counter for failed tests
FAIL_COUNT=0

echo "=== Running Valgrind on all tests in $TEST_DIR ==="

# Loop over all test executables
for test in "$TEST_DIR"/*; do
    if [[ -x "$test" ]]; then
        echo -e "\n>>> Testing: $(basename "$test")"

        # Run Valgrind and capture output
        valgrind --leak-check=full --error-exitcode=1 "$test" > valgrind_output.txt 2>&1 || true

        # Check for memory errors or leaks
        if grep -q "definitely lost: 0 bytes in 0 blocks" valgrind_output.txt && \
           grep -q "ERROR SUMMARY: 0 errors" valgrind_output.txt; then
            echo "$(basename "$test"): Passed (no leaks or errors)"
        else
            echo "$(basename "$test"): Memory issues detected!"
            echo "---- Valgrind Summary ----"
            grep -E "definitely lost|ERROR SUMMARY" valgrind_output.txt
            echo "--------------------------"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    fi
done

# Cleanup temporary file
rm -f valgrind_output.txt

# Final summary
echo -e "\n=== Valgrind Test Summary ==="
if [[ $FAIL_COUNT -eq 0 ]]; then
    echo "All tests passed with no leaks or errors!"
    exit 0
else
    echo "$FAIL_COUNT test(s) failed memory checks."
    exit 1
fi
