#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>

// hardcoded testing key.
// TODO: need a bit of a delay so you get diffy trngs on read
#define uint128_t HARDCODED_KEY = 0x123456789abcdef0;
#define uint128_t HARDCODED_IV  = 0x123456789abcdef0;


int arg_parse_client(int argc, char **argv, char **filename);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

int main(int argc, char *argv[]){
    // Check for user args
    if (argc == 1){
        print_errs(ERR_BAD_OPTIONS);
    }

    // Parse user args using getopt
    char *filename = NULL;

    // Message to be encrypted
    char *plaintext = NULL;

    //TODO: retrieve iv and convert to char*
    char *iv;
    sprintf(iv, "%u", HARDCODED_IV);

    FILE *fptr = fopen(*filename, "r");
    if (fptr == NULL){
        print_errs(ERR_FILE_NOT_FOUND);
    }

    // Read through file and collect patterns
    char temp[LINE_BUF_SIZE];
    while (fgets(temp, sizeof(temp), fptr) != NULL){
        
    }
    fclose(fptr);

    //TODO: retrieve key and convert to char*
    char *key;
    sprintf(key, "%u", HARDCODED_KEY);

    // Buffer for ciphertext. TODO: dynamically
    unsigned char ciphertext[128];
    unsigned char decyrptedtext[128];

    // 
    int ciphertext_len, decyptedtext_len;

    // Encrypt the plaintext
    ciphertext_len = encrypt(plaintext, strlen(plaintext), HARDCODED_KEY, HARDCODED_IV, ciphertext);

    // TODO: Save IV, KEY, and Cyphertext to output files.
    
    // DEBUG: Show the ecrypted text
    printf("Encrypted text is:\n");
    printf("%s\n", ciphertext);

    // Decrypt the ciphertext
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, HARDCODED_KEY, HARDCODED_IV, decryptedtext);

    // Add a NULL terminator.
    decryptedtext[decryptedtext_len] = '\0';

    // Show the decrypted text
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);

    return 0;

}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext){
    
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    int ciphertext_len = 0;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // Initialise the encryption operation.
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    // TODO: loop to dynamically encrypt full text...
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

/**
 * decrypt() - Uses openSSL library to decript cyphertext
 * @*ciphertext: Number of user args
 * @ciphertext_len: Length of ciphertext
 * @*key: AES-12
 * @**argv: User args
 * @**filename: Points to *filename in main
 *
 * Uses getopt() to read the user's input file option and 
 * saves it to *filename.
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
        handleErrors();
    }

    // Initialise the decryption operation.
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)){
        handleErrors();
    }

    // TODO: loop to dynamically encrypt full text
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)){
        handleErrors();
    }
    plaintext_len = len;
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)){
        handleErrors();
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
