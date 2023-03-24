import base64
import subprocess


def assert_test(actual: str, expected: str):
    if actual != expected:
        print("TEST FAILED")
        print(f"Expected: {expected}")
        print(f"Got: {actual}")


def run_test(input: str):
    actual = subprocess.run(
        ["./proj1"], input=bytes(input, "ASCII"), capture_output=True
    )
    expected = base64.b64encode(bytes(input, "ASCII")) + bytes("\n", "ASCII")
    assert_test(actual.stdout, expected)


run_test("Hi\n")
