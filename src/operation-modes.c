#include <stdint.h>
#include <stdlib.h>
#include "des.h"

// For ascii based systems:

uint64_t str_to_uint64(const unsigned char * str){
	uint64_t strbin=0;
	for(uint8_t i=0; i<8 && str[i] != '\0'; ++i){
		strbin = (strbin << 8) | str[i];
	}
	return strbin;
}


void PKCS5Padding(uint64_t * block, const uint8_t block_bytes_len){
	if (block_bytes_len < 8){
		for(uint8_t i=0; i< (8-block_bytes_len); ++i){
			*block = (*block << 8) | (8-block_bytes_len);
		}
	}
}

unsigned char * ECB_DESEncryption(const unsigned char * plaintext, const unsigned char * key, const uint32_t plaintext_len){
	uint32_t blocks_count = (plaintext_len+7)/8;
	unsigned char * ciphertext = malloc(blocks_count*8);
	uint64_t key_bits = str_to_uint64(key);

	for(uint32_t i=0; i<blocks_count; ++i){
		uint64_t plaintext_block=0, ciphertext_block=0;
		unsigned char plaintext_block_ascii[8] = {0};

		uint8_t j=0;
		for(; j<8 && plaintext[i*8 +j] != '\0' && (i*8 +j) < plaintext_len; ++j )
			plaintext_block_ascii[j] = plaintext[i*8 +j];

		plaintext_block = str_to_uint64(plaintext_block_ascii);
		PKCS5Padding(&plaintext_block, j);
		ciphertext_block = DESEncryption(plaintext_block, key_bits);

		for(uint8_t k=0; k<8; ++k){	
			uint8_t byte=0;
			for(uint8_t m=1; m<=8; ++m){
				if(GET_BIT(ciphertext_block, k*8 +m, 64))
					SET_BIT(byte, m, 8);
			}
			ciphertext[i*8 +k] = byte;
		}
		
	}
	return ciphertext;
}

unsigned char * CBC_DESEncryption(const unsigned char * plaintext, const unsigned char * key, const uint32_t plaintext_len, const unsigned char * IV){
	uint32_t blocks_count = (plaintext_len+7)/8;
	unsigned char * ciphertext = malloc(blocks_count*8);
	uint64_t key_bits = str_to_uint64(key);
	uint64_t mask_bits = str_to_uint64(IV);

	for(uint32_t i=0; i<blocks_count; ++i){
		uint64_t plaintext_block=0, ciphertext_block=0;
		unsigned char plaintext_block_ascii[8] = {0};

		uint8_t j=0;
		for(; j<8 && plaintext[i*8 +j] != '\0' && (i*8 +j) < plaintext_len; ++j )
			plaintext_block_ascii[j] = plaintext[i*8 +j];

		plaintext_block = str_to_uint64(plaintext_block_ascii);
		PKCS5Padding(&plaintext_block, j);
		plaintext_block ^= mask_bits;
		ciphertext_block = DESEncryption(plaintext_block, key_bits);
		mask_bits = ciphertext_block;

		for(uint8_t k=0; k<8; ++k){	
			uint8_t byte=0;
			for(uint8_t m=1; m<=8; ++m){
				if(GET_BIT(ciphertext_block, k*8 +m, 64))
					SET_BIT(byte, m, 8);
			}
			ciphertext[i*8 +k] = byte;
		}
		
	}
	return ciphertext;
}