# Data Encryption Standard

The **Data Encryption Standard ([DES](https://en.wikipedia.org/wiki/Data_Encryption_Standard))** is a symmetric-key cryptographic algorithm that was widely adopted in the late 20th century. Developed from IBM’s [Lucifer](https://en.wikipedia.org/wiki/Lucifer_%28cipher%29) cipher, DES was standardized by the U.S. NBS in 1977, with input from the NSA. The official specification is available [here](https://csrc.nist.gov/files/pubs/fips/46/final/docs/nbs.fips.46.pdf).

Due to advances in cryptanalysis, DES is now considered insecure. It has largely been superseded by the Advanced Encryption Standard ([AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)).

This repository contains an implementation of DES that strictly follows the official specification.

## Usage
This repository contains the core DES algorithm in the file `src/des.c` and also includes a minimal cli-wrapper `src/cli-wrapper.c` for testing.
Follow these steps to try it:
- Clone the repository and move in:
  ```sh
  git clone https://github.com/noinoiexists/Data-Encryption-Standard.git && cd Data-Encryption-Standard
  ```
- Compile the code:
  ```sh
  gcc src/des.c src/cli-wrapper.c src/operation-modes.c -o des-minimal -Wall -Wextra
  ```
The tool can be run using `./des-minimal`.

#### Example
```
$ ./des-minimal
Plaintext:
This is DES
Key (8 bytes):
icebergs
Mode (1=CBC, 2=ECB):
1
IV (8 bytes, optional):
vacation
Ciphertext (hex):
d7fb39281da198897db700ff60be9ebd
```


## To-do

- [x] Core Data Encryption Standard Algorithm
- [x] PKCS#5/7 Padding
- [x] Multi-block Encryption
  - [x] Electronic Code Book (ECB) mode 
  - [x] Cipher Block Chain (CBC) mode
- [ ] Decryption
- [ ] Enhanced command-line interface

## Licence
This implementation was created by me, [Nithik R](https://github.com/noinoiexists) and is protected under the MIT Licence. You can read more about it in the [LICENCE](https://github.com/noinoiexists/Data-Encryption-Standard/blob/main/LICENSE) file.
