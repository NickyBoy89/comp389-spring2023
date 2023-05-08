/*
 * Project 2 of COMP389: Cryptography
 * Function: Header File of fns.c
 * File Name: fns.h
 */

#ifndef FNS_H
#define FNS_H

/*
 * A simple stream cipher utilizing MD5 Message-Digest Algorithm
 * Parameters:
 * p: pointer to locate the pass phrase
 * l: number of bytes to output
 * fp: file pointer pointing to the specified temporary file or stdout
 *
 * Return value:
 * on success, return 0; else return -1
 */
int stream(char *p, int l, FILE *fp);

/*
 * Encrypts an input PBM file with the simple stream cipher based on 4x data expension visual cryptography by Naor and Shamir
 * Parameters:
 * p: pointer to locate the pass phrase
 * out: pointer to locate the output filename string
 * fp: file pointer pointing to the specified input file or stdin
 *
 * Return value:
 * on success, return 0; else return -1
 */
int encrypt(char *p, char *out, FILE *fp);

/*
 * Merge two input encrypted PBM files
 * Parameters:
 * fp1: file pointer pointing to the specified input file #1
 * fp2: file pointer pointing to the specified input file #2
 *
 * Return value:
 * on success, return 0; else return -1
 */
int merge(FILE *fp1, FILE *fp2);

/*
 * Decrypt either stdin or a specified merged PBM input file
 * Parameters:
 * fp: file pointer pointing to either the specified input file or stdin
 *
 * Return value:
 * on success, return 0; else return -1
 */
int decrypt(FILE *fp);

#endif
