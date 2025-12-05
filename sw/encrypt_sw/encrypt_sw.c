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
    int v = arg_parse_client(argc, argv, &filename);

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

    // Get values from memory register for encryption
    uint32_t read[8];
    for (int i=0; i<8; i++){
        //TODO: REPLACE WITH DRIVER READ
        read[i]=0x12345678;
        //read[i] = read_trng_register_from_driver();
        usleep(100);
    } 

    // Build key
    for (int i = 0; i < 4; i++) {
        key[i*4 + 0] = (read[i] >> 24) & 0xFF;
        key[i*4 + 1] = (read[i] >> 16) & 0xFF;
        key[i*4 + 2] = (read[i] >> 8) & 0xFF;
        key[i*4 + 3] = read[i] & 0xFF;
    }

    // Build IV
    for (int i = 4; i < 8; i++) {
        iv[(i - 4)*4 + 0] = (read[i] >> 24) & 0xFF;
        iv[(i - 4)*4  + 1] = (read[i] >> 16) & 0xFF;
        iv[(i - 4)*4 + 2] = (read[i] >> 8) & 0xFF;
        iv[(i - 4)*4 +  3] = read[i] & 0xFF;
    }

    if(v){
        printf("Key is:\n");
        for (int i = 0; i < 16; i++){
            printf("%02x", key[i]);
        }
        printf("\nIV is:\n");
        for (int i = 0; i < 16; i++){
            printf("%02x", iv[i]);
        }
        printf("\n");
    }


    // Allocate ciphertext and decypted text, accounting for padding
    unsigned char* ciphertext = malloc(file_size + 16);
    unsigned char* decryptedtext = malloc(file_size + 16);
    int ciphertext_len, decryptedtext_len;

    // Encrypt the plaintext
    ciphertext_len = encrypt(plaintext, strlen((char*) plaintext), 
        (unsigned char*) key, (unsigned char*) iv, ciphertext);

    
    // Save ciphertext,iv, and key to output bin
    fptr = fopen("output/key.hex", "wb");
    if(fptr==NULL){
        print_errs(ERR_FILE_NOT_FOUND);
    }
    for(int i = 0; i < 16; i++){
        fprintf(fptr, "%02x", key[i]);
    }
    fclose(fptr);  
    fptr = fopen("output/iv.hex", "wb");
    if(fptr==NULL){
        print_errs(ERR_FILE_NOT_FOUND);
    }
    for(int i = 0; i < 16; i++){
        fprintf(fptr, "%02x", iv[i]);
    }
    fclose(fptr);
    fptr = fopen("output/ciphertext.bin", "wb");
    if(fptr==NULL){
        print_errs(ERR_FILE_NOT_FOUND);
    }
    fwrite(ciphertext, 1, ciphertext_len, fptr);
    fclose(fptr); 

    if(v){
    // Show the ecrypted text
    ciphertext[ciphertext_len] = '\0';
    printf("Encrypted text is:\n");
    printf("%s\n", ciphertext);
    }

    // Decrypt the ciphertext
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, 
        (unsigned char*) key, (unsigned char*) iv, decryptedtext);
 
    if(v){
        // Display decrypted text
        decryptedtext[decryptedtext_len] = '\0';

        printf("Decrypted text is:\n");
        printf("%s\n", decryptedtext);
    }

    // Free memory
    free(plaintext);
    free(ciphertext);
    free(decryptedtext);
    free(filename);

    return 0;

}
