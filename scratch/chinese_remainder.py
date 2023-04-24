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


def remainder(number: int, power: int, modulus: KnownFactorableInt) -> int:
    # Split the number into two mods
    xm = number % modulus.first
    ym = number % modulus.second
    print("Split the number into two mods:")
    print(f"[{xm}^{{{power}}} mod {modulus.first}]")
    print(f"[{ym}^{{{power}}} mod {modulus.second}]")

    # Next, reduce the power
    px = power % (modulus.first - 1)
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


print(remainder(3, 1000, KnownFactorableInt.factor_from(100)))
print(naive(3, 1000, KnownFactorableInt.factor_from(100)))

print(remainder(101, 4_800_000_023, KnownFactorableInt.factor_from(35)))
print(naive(101, 4_800_000_023, KnownFactorableInt.factor_from(35)))
