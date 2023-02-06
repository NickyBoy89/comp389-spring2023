from alphabet import letter_to_n, n_to_letter


def encrypt(message: str, shift: int, alphabet_size: int = 26) -> str:
    return "".join(
        [n_to_letter((letter_to_n(char) + shift) % alphabet_size) for char in message]
    )


def decrypt_known_plaintext(message: str, ciphertext: str) -> int:
    return letter_to_n(ciphertext[0]) - letter_to_n(message[0])


if __name__ == "__main__":
    m = "helloworldz"
    shift = 2
    c = encrypt(m, shift)
    print(c)
    print(decrypt_known_plaintext(m, c))
