# Project 2: Visual Cryptography

This project includes several command-line tools to interact with the PBM files and visual cryptography

## Requirements

1. `gcc`
2. GNU `make`

## Building and Running

To build, run:
1. `make` to create a binary named `proj2`

To run the programs, there are several subcommands when you run `./proj2`:
* `proj2 stream -p=pphrase -l=len`
* `proj2 encrypt -p=pphrase -out=name [pbmfile]`
* `proj2 merge pbmfile1 pbmfile2`
* `proj2 decrypt [pbmfile]`
