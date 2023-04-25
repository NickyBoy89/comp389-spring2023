import math


class KnownFactorableInt:
    first: int
    second: int

    def num(self) -> int:
        return self.first * self.second

    def __init__(self, first: int, second: int):
        # Make sure that the numbers are coprime
        assert math.gcd(first, second) == 1
        self.first = first
        self.second = second

    def factor_from(number: int):
        for i in range(2, number // 2):
            if number % i == 0 and math.gcd(number // i, i) == 1:
                return KnownFactorableInt(i, number // i)
        raise Exception(f"Number {number} has no known factors")


def is_prime(num: int) -> bool:
    prime_flag = 0

    if num > 1:
        for i in range(2, int(math.sqrt(num)) + 1):
            if num % i == 0:
                prime_flag = 1
                break
        if prime_flag == 0:
            return True
        else:
            return False
    else:
        return False


def remainder(number: int, power: int, modulus: KnownFactorableInt) -> int:
    # Split the number into two mods
    xm = number % modulus.first
    ym = number % modulus.second
    print("Split the number into two mods:")
    print(f"[{xm}^{{{power}}} mod {modulus.first}]")
    print(f"[{ym}^{{{power}}} mod {modulus.second}]")

    px = power
    py = power
    # Next, reduce the power
    if is_prime(modulus.first):
        px = power % (modulus.first - 1)
    if is_prime(modulus.second):
        py = power % (modulus.second - 1)
    print(f"Reduce the power by the order (p - 1) or (q - 1)")
    print(f"[{xm}^{{{px}}} mod {modulus.first}]")
    print(f"[{ym}^{{{py}}} mod {modulus.second}]")

    # Calculate the powers
    nx = (xm**px) % modulus.first
    ny = (ym**py) % modulus.second
    print("Do the calculations on the smaller numbers")
    print(f"[{nx} mod {modulus.first}]")
    print(f"[{ny} mod {modulus.second}]")

    # Finally, find a set of equations that will satisfy the requirements
    print("Finding a modulus that will satisfy both moduluses")
    start = nx
    while start % modulus.second != ny:
        start += modulus.first
        print(
            f"Testing {start}: mod first {start % modulus.first}, mod second: {start % modulus.second}"
        )

    print(f"[{start} mod {modulus.num()}]")

    return start


def naive(number: int, power: int, modulus: KnownFactorableInt) -> int:
    return (number**power) % modulus.num()


if __name__ == "__main__":
    print(remainder(3, 1000, KnownFactorableInt.factor_from(100)))
    print(naive(3, 1000, KnownFactorableInt.factor_from(100)))

    print(remainder(101, 4_800_000_023, KnownFactorableInt.factor_from(35)))
    print(naive(101, 4_800_000_023, KnownFactorableInt.factor_from(35)))
