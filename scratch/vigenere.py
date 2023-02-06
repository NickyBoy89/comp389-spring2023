from alphabet import letter_to_n, n_to_letter


def encrypt(message: str, key: str, alphabet_size: int = 26) -> str:
    ciphertext = list(message)
    for index in range(len(ciphertext)):
        ciphertext[index] = n_to_letter(
            (letter_to_n(ciphertext[index]) + letter_to_n(key[index % len(key)]))
            % alphabet_size
        )
    return "".join(ciphertext)


def shift_letter(letter: str, shift: int) -> str:
    return n_to_letter(letter_to_n(letter) + shift)


def decrypt_known_plaintext(
    message: str, ciphertext: str, alphabet_size: int = 26
) -> str:
    key = ["a" for i in range(len(message))]

    for index, char in enumerate(message):
        for shift in range(alphabet_size):
            shifted = shift_letter(char, shift)
            if shifted == ciphertext[index]:
                key[index] = n_to_letter(shift)

    return "".join(key)


if __name__ == "__main__":
    m = "tellhimaboutme"
    k = "cafe"
    c = encrypt(m, k)
    print(c)
    print(decrypt_known_plaintext(m, c))
