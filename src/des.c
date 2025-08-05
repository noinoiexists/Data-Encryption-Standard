#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "des.h"

#define ROUNDS 16

//Operations on ith bit from msb side on len length binary 
#define GET_BIT(val, i, len) (((val)>>((len)-(i)))&0x1)
#define SET_BIT(val, i, len) ((val)|=(1ULL<<((len)-(i))))
#define UNSET_BIT(val, i, len) ((val)&=~(1ULL<<((len)-(i))))

const uint8_t ip_table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7 };

const uint8_t ip_inverse_table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25 };

const uint8_t E_table[48] =	{
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1 };

const uint8_t P_table[32]  = {
	16,  7, 20, 21,
	29, 12, 28, 17,
	 1, 15, 23, 26,
	 5, 18, 31, 10,
	 2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25 };

const uint8_t Sbox[8][64] = {	
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},

	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},

	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},

	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},

	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},

	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},

	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},

	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

const uint8_t PC1_table[56] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4 };

const uint8_t PC2_table[48] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32 };

const uint8_t LS_table[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


void InitialPermutation(uint64_t * block){
	uint64_t permuted_block = 0;
	for(uint8_t i=1; i<=64; ++i){
		if(GET_BIT(*block, ip_table[i-1], 64))
			SET_BIT(permuted_block, i, 64);
	}
	*block = permuted_block;
}

void InitialPermutationInverse(uint64_t * block){
	uint64_t permuted_block = 0;
	for(uint8_t i=1; i<=64; ++i){
		if(GET_BIT(*block, ip_inverse_table[i-1], 64))
			SET_BIT(permuted_block, i, 64);
	}
	*block = permuted_block;
}

void ExpansionPermutation(const uint32_t halfblock, uint64_t * expanded_halfblock){
	*expanded_halfblock = 0;
	for(uint8_t i=1; i<=48; ++i){
		if(GET_BIT(halfblock, E_table[i-1], 32))
			SET_BIT(*expanded_halfblock, i, 48);
	}
}

void P_Permutation(uint32_t * fValue){
	uint32_t modified_fValue = 0;
	for(uint8_t i=1; i<=32; ++i){
		if(GET_BIT(*fValue, P_table[i-1], 32))
			SET_BIT(modified_fValue, i, 32);
	}
	*fValue = modified_fValue;
}

void LeftRotate(uint32_t * halfkey, uint8_t current_round){
	for(uint8_t i=1; i<=LS_table[current_round-1]; ++i){
		*halfkey = GET_BIT(*halfkey, 1, 28) | (*halfkey << 1);
		UNSET_BIT(*halfkey, 1, 29);
	}
}

void GenerateSubkeys(const uint64_t key, uint64_t subkey[]){
	uint64_t PC1_key = 0;
	for(uint8_t i=1; i<=56; ++i){
		if(GET_BIT(key, PC1_table[i-1], 64))
			SET_BIT(PC1_key, i, 56);
	}
	uint32_t C=0, D=0;
	for(uint8_t i=1; i<=28; ++i){
		if(GET_BIT(PC1_key, i, 56))
			SET_BIT(C, i, 28);
		if(GET_BIT(PC1_key, i+28, 56))
			SET_BIT(D, i, 28);
	}

	for(uint8_t current_round=1; current_round<=ROUNDS; ++current_round){
		LeftRotate(&C, current_round); LeftRotate(&D, current_round);
		uint64_t combined_CD = 0;
		for(uint8_t i=1; i<=28; ++i){
			if(GET_BIT(C, i, 28))
				SET_BIT(combined_CD, i, 56);
			if(GET_BIT(D, i, 28))
				SET_BIT(combined_CD, i+28, 56);
		}

		subkey[current_round-1] = 0;
		for(uint8_t i=1; i<=48; ++i){
			if(GET_BIT(combined_CD, PC2_table[i-1], 56))
				SET_BIT(subkey[current_round-1], i, 48);
		}
	}
}

uint32_t fCompute(const uint32_t halfblock, const uint64_t subkey){
	uint64_t expanded_halfblock;
	ExpansionPermutation(halfblock, &expanded_halfblock);

	expanded_halfblock ^= subkey;

	uint32_t fValue=0;
	for(uint8_t i=0; i< 8; ++i){
		uint8_t row=0, column=0;
		for(uint8_t j=1; j<=6; ++j){
			if(GET_BIT(expanded_halfblock, (i*6+j), 48)){
				if(j==1)
					SET_BIT(row, 1, 2);
				if(j==6)
					SET_BIT(row, 2, 2);
				if(j!=1 && j!=6)
					SET_BIT(column, j-1, 4);
			}
		}
		fValue = (fValue << 4) | Sbox[i][row*16 + column];
	}
	P_Permutation(&fValue);
	return fValue;
}

uint64_t DESEncryption(const uint64_t input_block, const uint64_t key){
	uint64_t block = input_block;

	uint64_t subkey[ROUNDS];
	GenerateSubkeys(key, subkey);

	InitialPermutation(&block);

	uint32_t L=0, R=0;
	for(uint8_t i=1; i<=32; ++i){
		if(GET_BIT(block, i, 64))
			SET_BIT(L, i, 32);
		if(GET_BIT(block, i+32, 64))
			SET_BIT(R, i, 32);
	}

	for(uint8_t current_round=1; current_round<=ROUNDS; ++current_round){
		uint32_t XORmask = fCompute(R, subkey[current_round-1]);
		uint32_t temp = L ^ XORmask;
		L = R;
		R = temp;
	}
	block=0;
	for(uint8_t i=1; i<=32; ++i){
		if(GET_BIT(L, i, 32))
			SET_BIT(block, i+32, 64);
		if(GET_BIT(R, i, 32))
			SET_BIT(block, i, 64);
	}

	InitialPermutationInverse(&block);

	return block;
}

// For ascii based systems only:

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