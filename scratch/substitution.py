from alphabet import letter_to_n, n_to_letter, shifted_alphabet, AlphabetMap

import random


def encrypt_substitution(message: str, alphabet: str) -> str:
    ciphertext = ""

    for char in message:
        char_index = letter_to_n(char)
        ciphertext += alphabet[char_index]

    return ciphertext


def decrypt_substitution(
    message: str, ciphertext: str, alphabet_length: int = 26
) -> str:

    shifts = [
        (letter_to_n(ciphertext[index]) - letter_to_n(message[index])) % alphabet_length
        for index in range(alphabet_length)
    ]

    alphabet = ""
    for index in range(alphabet_length):
        alphabet += n_to_letter((index + shifts[index]) % alphabet_length)

    return alphabet


def scramble_alphabet(alphabet: str) -> str:
    alpha = list(alphabet)
    random.shuffle(alpha)
    return "".join(alpha)


if __name__ == "__main__":
    m = "thequickbrownfoxjumpedoverthelazydog"
    k = "eomnkcxwitdqshbzpuvljagyfr"
    # k = "defghijklmnopqrstuvwxyzabc"

    c = encrypt_substitution(m, k)

    print(c)

    print(decrypt_substitution(m, c))
