#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
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

        unsigned char * ciphertext = ECB_DESEncryption(plaintext, key, (uint32_t)plaintext_len);
        printf("Ciphertext (hex):\n");
        for(uint32_t i=0; i<(((uint32_t)plaintext_len+7)/8)*8; ++i)
            printf("%02x", ciphertext[i]);
        printf("\n");

        free(ciphertext); free(plaintext);
        return 0;
    }
    else {
        fprintf(stderr, "Unable to get input\n");
        return 1;
    }
}