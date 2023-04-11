#!/usr/bin/env python3

import glob
import subprocess

testdir = "testcases"

for file_number in range(0, 9):
    with open(f"f{file_number}.hex", "wb") as output_file:
        command = subprocess.run(
            [
                "./proj1",
                "hexdump",
                f"{testdir}/f{file_number}",
            ],
            capture_output=True,
        )
        output_file.write(command.stdout)

    actual = ""
    with open(f"f{file_number}.hex") as actual_file:
        actual = actual_file.read()

    expected = ""
    with open(f"{testdir}/f{file_number}.hex") as test_file:
        expected = test_file.read()

    if actual != expected:
        print("Actual:")
        print(actual)
        print("Expected:")
        print(expected)
        raise Exception(f"Test {file_number} failed")
