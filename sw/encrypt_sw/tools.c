#include <stdio.h>
#include "tools.h"

/**
 * encrypt() - Uses openSSL library to decript cyphertext
 * @*ciphertext: Number of user args
 * @ciphertext_len: Length of ciphertext
 * @*key: AES-128 key
 * @**iv: AES-128 initialization vector
 * @*plaintext: Points to *filename in main
 *
 * Initializes a EVP Cipher process to encrypt the ciphertext
 * with the Key and IV using AES-128-CBC. Encrypts by going
 * block by block.
 *
 * Return: 0
 */
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, 
    unsigned char *iv, unsigned char *ciphertext){
    
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    int ciphertext_len = 0;

    // Create and initialise the context
    if(!(ctx = EVP_CIPHER_CTX_new())){
        print_errs(ERR_BAD_ENCRYPT);
    }

    // Initialise the encryption operation.
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)){
        print_errs(ERR_BAD_ENCRYPT);
    }

    // TODO: loop to dynamically encrypt full text...
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)){
        print_errs(ERR_BAD_ENCRYPT);
    }
    ciphertext_len = len;
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)){
        print_errs(ERR_BAD_ENCRYPT);
    }
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

/**
 * decrypt() - Uses openSSL library to decript cyphertext
 * @*ciphertext: Number of user args
 * @ciphertext_len: Length of ciphertext
 * @*key: AES-128 key
 * @**iv: AES-128 initialization vector
 * @*plaintext: Points to *filename in main
 *
 * Initializes a EVP Cipher process to decrypt the ciphertext
 * with the Key and IV using AES-128-CBC. Decrypts by going
 * block by block.
 *
 * Return: 0
 */
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, 
    unsigned char *iv, unsigned char *plaintext){
    
    EVP_CIPHER_CTX *ctx;

    int len = 0;
    int plaintext_len = 0;

    // Create and initialise the context
    if(!(ctx = EVP_CIPHER_CTX_new())){
        print_errs(ERR_BAD_ENCRYPT);
    }

    // Initialise the decryption operation.
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)){
        print_errs(ERR_BAD_ENCRYPT);
    }

    // TODO: loop to dynamically encrypt full text
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)){
        print_errs(ERR_BAD_ENCRYPT);
    }
    plaintext_len = len;
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)){
        print_errs(ERR_BAD_ENCRYPT);
    }
    plaintext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

/**
 * arg_parse() - Set user input flags
 * @argc: Number of user args
 * @**argv: User args
 * @**filename: Points to *filename in main
 *
 * Uses getopt() to read the user's input file option and 
 * saves it to *filename.
 *
 * Return: 0
 */
int arg_parse_client(int argc, char **argv, char **filename){
    int opt;
    while ((opt = getopt(argc, argv, ":p:f:hv")) != -1){
        switch (opt){
        case 'f':
            *filename = strdup(optarg);
            break;
        case 'h':
            print_errs(HELP_MSSG);
            break;
        case ':':
            // Missing value after option
            print_errs(ERR_BAD_OPT);
            break;
        case '?':
            // Unknown option
            print_errs(ERR_BAD_OPT);
            break;
        default:
            // Unknown option
            print_errs(ERR_BAD_OPT);
            break;
        }
    }

    return 0;
}

/**
 * print_errs() - Print error and help of function
 * @errtype: Flag for specific errors
 *
 * Called when errors occur. Will print
 * applicable error message or help/usage prompt.
 *
 * Return: void
 */
void print_errs(int errtype){
    if(errtype == ERR_BAD_OPT){
        printf("ERROR: Incorrect options.\n");
    }else if (errtype == ERR_FILE_NOT_FOUND){
        printf("ERROR: File not found.\n");
    }else if (errtype == ERR_BAD_ENCRYPT){
        printf("ERROR: Encryption/decryption process failed. Please try again.\n");
    }

    if(errtype == ERR_BAD_OPT || errtype == HELP_MSSG){
        printf(
            "Usage:\n"
            "  ./encrypt [-f FILE]\n\n"
            "Description:\n"
            "  Takes file and uses a random key and iv to encrypt the file in AES-128. Outputs the key, iv, and encrypted data into separate files.\n\n"
            "Arguments:\n"
            "  -h                    show this help message and exit\n"
            "  -f FILE               specify a text file\n\n"
        );
    };

    exit(0);
}