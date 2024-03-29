#!/usr/bin/env python3

import glob
import subprocess

testdir = "testcases"

exts = {"hexdump": ".hex", "enc-base64": ".b64", "dec-base64": ".dat"}

input_files = {"hexdump": "", "enc-base64": "", "dec-base64": ".b64"}

test_against = {"hexdump": ".hex", "enc-base64": ".b64", "dec-base64": ""}


def find_different_bit_index(first: bytes, second: bytes) -> int:
    iterations = min(len(first), len(second))
    for index in range(iterations):
        if first[index] != second[index]:
            return index
    return iterations


def diff_file(file_number: int, subcommand: str, read_from_stdin: bool = False) -> None:
    print(f"Testing file {file_number}...")

    ext = exts[subcommand]
    with open(f"f{file_number}{ext}", "wb") as output_file:
        if read_from_stdin:
            with open(
                f"{testdir}/f{file_number}{input_files[subcommand]}", "rb"
            ) as stdin_file:
                command = subprocess.run(
                    ["./proj1", subcommand],
                    capture_output=True,
                    stdin=stdin_file,
                )
        else:
            command = subprocess.run(
                [
                    "./proj1",
                    subcommand,
                    f"{testdir}/f{file_number}{input_files[subcommand]}",
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
        differ_bit = find_different_bit_index(actual, expected)
        # if differ_bit == min(len(actual), len(expected)):
        print("Actual:")
        print(actual[: differ_bit + 2])
        print("Expected:")
        print(expected[: differ_bit + 2])
        print(f"Strings differ on the {differ_bit} bit out of {len(expected)}")
        # raise Exception(f"Test {file_number} for {subcommand} failed")
        print(f"Test {file_number} for {subcommand} failed")


for file_number in [0, 1, 2, 3, 4, 5, 6, 7, 8]:
    diff_file(file_number, "hexdump")

for file_number in [9, 10, 11, 12, 13, 14]:
    diff_file(file_number, "hexdump", read_from_stdin=True)

for file_number in [5, 6, 7, 8, 9]:
    diff_file(file_number, "enc-base64")

for file_number in [0, 1, 3, 4, 9]:
    diff_file(file_number, "enc-base64", read_from_stdin=True)

for file_number in [5, 6, 7, 9]:
    diff_file(file_number, "dec-base64")

for file_number in [0, 1, 2, 3, 9]:
    diff_file(file_number, "dec-base64", read_from_stdin=True)

for file_number in [100, 101]:
    diff_file(file_number, "dec-base64")

for file_number in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]:
    pass
