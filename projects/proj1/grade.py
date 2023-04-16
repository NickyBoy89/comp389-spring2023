#!/usr/bin/env python3

import glob
import subprocess

testdir = "testcases"

exts = {"hexdump": ".hex", "enc-base64": ".b64", "dec-base64": ".dat"}

test_against = {"hexdump": ".hex", "enc-base64": ".b64", "dec-base64": ""}


def diff_file(file_number: int, subcommand: str) -> None:
    print(f"Testing file {file_number}...")

    ext = exts[subcommand]
    with open(f"f{file_number}{ext}", "wb") as output_file:
        command = subprocess.run(
            [
                "./proj1",
                subcommand,
                f"{testdir}/f{file_number}",
            ],
            capture_output=True,
        )
        output_file.write(command.stdout)

    actual = ""
    with open(f"f{file_number}{ext}", "rb") as actual_file:
        actual = actual_file.read()

    expected = ""
    with open(f"{testdir}/f{file_number}{test_against[subcommand]}", "rb") as test_file:
        expected = test_file.read()

    if actual != expected:
        print("Actual:")
        print(actual)
        print("Expected:")
        print(expected)
        raise Exception(f"Test {file_number} for {subcommand} failed")


for file_number in [0, 1, 2, 3, 4, 5, 6, 7, 8]:
    diff_file(file_number, "hexdump")

# FIXME: Test STDIN instead of file input
for file_number in [9, 10, 11, 12, 13, 14]:
    diff_file(file_number, "hexdump")

for file_number in [5, 6, 7, 8, 9]:
    diff_file(file_number, "enc-base64")

# FIXME: Test STDIN instead of file input
for file_number in [0, 1, 3, 4, 9]:
    diff_file(file_number, "enc-base64")

for file_number in [5, 6, 7, 9]:
    diff_file(file_number, "dec-base64")

for file_number in [0, 1, 2, 3, 9]:
    diff_file(file_number, "dec-base64")

for file_number in [100, 101]:
    diff_file(file_number, "dec-base64")

for file_number in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]:
    pass
