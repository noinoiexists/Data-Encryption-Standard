#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "des.h"

int main() {
    unsigned char * plaintext = NULL; size_t plaintext_buff_len = 0;
    printf("Plaintext:\n");
    ssize_t plaintext_len = getline((char **)&plaintext, &plaintext_buff_len, stdin);
    if (plaintext_len != -1){
        if(plaintext[plaintext_len-1] == '\n')
            plaintext[--plaintext_len] = '\0';

        unsigned char key[8] = {0};
        printf("Key (8 bytes):\n");
        scanf("%8s", key);

        uint8_t mode=0;
        printf("Mode (1=CBC, 2=ECB):\n");
        scanf("%"SCNu8, &mode); getchar();

        if(mode == 1) {
            unsigned char IV[8]={0};
            printf("IV (8 bytes, optional):\n");
            for(uint8_t i=0; i<8; ++i){
                if ((IV[i]=getchar()) == '\n'){
                    for(uint8_t j=0; j<8; ++j)
                        IV[j] = '\0';
                    break;
                }
            }
            unsigned char * ciphertext = CBC_DESEncryption(plaintext, key, (uint32_t)plaintext_len, IV);
            printf("Ciphertext (hex):\n");
            for(uint32_t i=0; i<(((uint32_t)plaintext_len+7)/8)*8; ++i)
                printf("%02x", ciphertext[i]);
            printf("\n");
            free(ciphertext);
        } 

        else if (mode == 2) {
            unsigned char * ciphertext = ECB_DESEncryption(plaintext, key, (uint32_t)plaintext_len);
            printf("Ciphertext (hex):\n");
            for(uint32_t i=0; i<(((uint32_t)plaintext_len+7)/8)*8; ++i)
                printf("%02x", ciphertext[i]);
            printf("\n");
            free(ciphertext);
        } 

        else {
            fprintf(stderr, "Invalid mode\n");
            return 1;
        }

        free(plaintext);
        return 0;
    }
    else {
        fprintf(stderr, "Unable to get input\n");
        return 1;
    }
}