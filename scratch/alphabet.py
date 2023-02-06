from typing import Dict


def letter_to_n(l: str, basis: str = "a") -> int:
    return ord(l) - ord(basis)


def n_to_letter(n: int, basis: str = "a") -> str:
    return chr(n + ord(basis))


def shifted_alphabet(shift: int, alphabet_length: int = 26) -> str:
    a = ""

    for index in range(alphabet_length):
        a += n_to_letter((index + shift) % alphabet_length)

    return a


def find_shift(original: str, modified: str) -> int:
    return letter_to_n(modified) - letter_to_n(original)


class AlphabetMap:
    mapping: Dict[str, str]

    def __init__(self, alphabet: str):
        self.mapping = dict([(letter, letter) for letter in alphabet])

    def initial():
        return AlphabetMap("".join([n_to_letter(i) for i in range(26)]))

    def __str__(self):
        return self.mapping.__str__()
