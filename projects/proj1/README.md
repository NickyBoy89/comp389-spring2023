# Project 1

This project is a command-line program that has three separate subcommands that help to debug binary files.

## Requirements

1. The `gcc` C compiler

## Running the project

1. `make` to compile the binary
2. `./proj1 <command name> [file name]` to run each of the subcommands
    1. If no file is specified, the command will run with input from `STDIN`

And `<command name>` refers to one of the three commands:

1. `hexdump`, which prints out a standard hexdump of a binary file
2. `enc-base64`, which encodes the specified file in a slightly modified version of base64
    1. The format only differs in that it word-wraps the encoded output every 64 characters, and puts a new line on the end.
3. `dec-base64`, which decodes the slightly modified base64 files back to their original states