#include <stdio.h>
#include "tools.h"


int main(int argc, char *argv[]){
    // Check for user args
    if (argc == 1){
        print_errs(ERR_BAD_OPT);
    }

    // Parse user args using getopt
    unsigned char key[16];
    unsigned char iv[16];
    char *filename = NULL;
    arg_parse_client(argc, argv, &filename);

    unsigned char *plaintext = NULL;

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL){
        print_errs(ERR_FILE_NOT_FOUND);
    }

    // Read through file and collect patterns
    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    plaintext = malloc(file_size + 1);
    fread(plaintext, 1, file_size, fptr);
    plaintext[file_size] = '\0';


    // TODO: DEBUG: hardcoded testing key, remove
    uint32_t REG0_1 = 0x1234;
    uint32_t REG1_1 = 0x5678;
    uint32_t REG2_1 = 0x9abc;
    uint32_t REG3_1 = 0xdef0;
    uint32_t REG0_2 = 0x1234;
    uint32_t REG1_2 = 0x5678;
    uint32_t REG2_2 = 0x9abc;
    uint32_t REG3_2 = 0x9def0;

    memcpy(key, &REG0_1, 4);     
    memcpy(key + 4, &REG1_1, 4);
    memcpy(key + 8, &REG2_1, 4);     
    memcpy(key + 12, &REG3_1, 4);
    memcpy(iv, &REG0_2, 4);
    memcpy(iv + 4, &REG1_2, 4);
    memcpy(iv + 8, &REG2_2, 4);
    memcpy(iv + 12, &REG3_2, 4);


    // TODO: remove above debug and uncomment this (changing read_trng_register_from_driver() thing)

    /* 
    uint32_t read[8];

    for (int i=0; i<8; i++){
        read[i] = read_trng_register_from_driver();
        wait(1 ms);
    } 
    
    memcpy(key, &read[7], 4);     
    memcpy(key + 4, &read[6], 4);
    memcpy(key + 8, &read[5], 4);     
    memcpy(key + 12, &read[4], 4);
    memcpy(iv, &read[3], 4);
    memcpy(iv + 4, &read[2], 4);
    memcpy(iv + 8, &read[1], 4);
    memcpy(iv + 12, &read[0], 4);
    */

    // Allocate ciphertext and decypted text, accounting for padding
    unsigned char* ciphertext = malloc(file_size + 16);
    unsigned char* decryptedtext = malloc(file_size + 16);
    int ciphertext_len, decryptedtext_len;

    // Encrypt the plaintext
    ciphertext_len = encrypt(plaintext, strlen((char*) plaintext), 
        (unsigned char*) key, (unsigned char*) iv, ciphertext);

    // TODO: Save IV, KEY, and Cyphertext to output files.
    

    // Show the ecrypted text
    printf("Encrypted text is:\n");
    printf("%s\n", ciphertext);

    // Decrypt the ciphertext
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, 
        (unsigned char*) key, (unsigned char*) iv, decryptedtext);
 
    // Display decrypted text
    decryptedtext[decryptedtext_len] = '\0';

    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);

    // Free memory
    free(plaintext);
    free(ciphertext);
    free(decryptedtext);
    free(filename);

    return 0;

}
