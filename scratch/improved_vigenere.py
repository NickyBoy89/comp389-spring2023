from typing import List

from alphabet import letter_to_n, n_to_letter

from vigenere import decrypt_known_plaintext


def encrypt_improved_vigenere(alphabet_keys: List[str], message: str) -> str:

    key_length = len(alphabet_keys)

    ciphertext = ["a" for i in range(len(message))]

    for index, char in enumerate(message):
        # char_index is the position of the character, relative to the alphabet
        char_index = letter_to_n(char)

        ciphertext[index] = alphabet_keys[index % key_length][char_index]

    return "".join(ciphertext)


def decrypt_improved_vignere(
    message: str, ciphertext: str, alphabet_size: int = 26
) -> str:

    # At worst, there will the same number of substitutions as the length of
    # the message
    alphabets = ["" for _ in range(len(message))]

    for index, char in enumerate(message):
        alphabets[index] = 0


if __name__ == "__main__":
    shift_key = "cafe"
    shifts = [letter_to_n(letter) for letter in shift_key]

    alphabets = [shifted_alphabet(shift) for shift in shifts]

    m = "tellhimaboutme"
    c = encrypt_improved_vigenere(alphabets, m)
    print(c)

    print(decrypt_known_plaintext(m, c))
