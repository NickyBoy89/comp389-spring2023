#!/usr/bin/env python3

import glob
import subprocess

testdir = "testcases"

for file in glob.glob("*"):
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
