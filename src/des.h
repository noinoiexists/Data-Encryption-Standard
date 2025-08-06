#define ROUNDS 16

//Operations on ith bit from msb side on len length binary 
#define GET_BIT(val, i, len) (((val)>>((len)-(i)))&0x1)
#define SET_BIT(val, i, len) ((val)|=(1ULL<<((len)-(i))))
#define UNSET_BIT(val, i, len) ((val)&=~(1ULL<<((len)-(i))))

uint64_t DESEncryption(const uint64_t input_block, const uint64_t key);
unsigned char * ECB_DESEncryption(const unsigned char * plaintext, const unsigned char * key, const uint32_t plaintext_len);
unsigned char * CBC_DESEncryption(const unsigned char * plaintext, const unsigned char * key, const uint32_t plaintext_len, const unsigned char * IV);
