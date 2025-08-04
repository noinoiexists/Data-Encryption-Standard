#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "des.h"

uint64_t str_to_uint64(const char* str) {
    uint64_t result = 0;
    for (int i = 0; i < 8 && i < (int)strlen(str); i++) {
        result = (result << 8) | (uint8_t)str[i];
    }
    return result;
}

int main() {
    char key_str[9];   
    char block_str[9]; 
    printf("Key:\n");
    scanf("%8s", key_str);
    printf("Block:\n");
    scanf("%8s", block_str);
    
    uint64_t key = str_to_uint64(key_str);
    uint64_t block = str_to_uint64(block_str);
    
    uint64_t ciphertext = DESEncryption(block, key);
    printf("Ciphertext (hex):\n%016llX\n", (unsigned long long)ciphertext);
    
    return 0;
}