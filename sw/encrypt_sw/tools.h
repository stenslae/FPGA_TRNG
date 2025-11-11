#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>

#define ERR_BAD_OPT 0
#define ERR_FILE_NOT_FOUND 1
#define HELP_MSSG 2
#define ERR_BAD_ENCRYPT 3

int arg_parse_client(int argc, char **argv, char **filename);
int encrypt(unsigned char *plaintext, int plaintext_len, 
    unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, 
    unsigned char *key, unsigned char *iv, unsigned char *plaintext);
void print_errs(int errtype);

#endif