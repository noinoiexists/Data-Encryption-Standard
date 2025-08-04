# Data Encryption Standard

The **Data Encryption Standard ([DES](https://en.wikipedia.org/wiki/Data_Encryption_Standard))** is a symmetric-key cryptographic algorithm that was widely adopted in the late 20th century. Developed from IBM’s [Lucifer](https://en.wikipedia.org/wiki/Lucifer_%28cipher%29) cipher, DES was standardized by the U.S. NBS in 1977, with input from the NSA. The official specification is available [here](https://csrc.nist.gov/files/pubs/fips/46/final/docs/nbs.fips.46.pdf).

Due to advances in cryptanalysis, DES is now considered insecure. It has largely been superseded by the Advanced Encryption Standard ([AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)).

This repository contains an implementation of DES that strictly follows the official specification.

## Usage
This repository contains the core DES algorithm in the file `src/des.c` and also includes a minimal cli-wrapper for testing that supports single-block encryption.  
Follow these steps to try it:
- Clone the repository and move in:
  ```sh
  git clone https://github.com/noinoiexists/Data-Encryption-Standard.git && cd Data-Encryption-Standard
  ```
- Compile the code:
  ```sh
  gcc src/cli-wrapper.c src/des.c -o des-minimal -Wall -Wextra
  ```
The tool can be run using `./des-minimal`.

#### Example
```
$ ./des-minimal
Key:
icebergs
Plaintext:
vacation
Ciphertext (hex):
F7BDB5339002C42B
```

## To-do

- [x] Core Data Encryption Standard Algorithm
- [ ] Multi-block Encryption/Decryption using ECB, CBC modes
- [ ] PKCS#5/7 Padding
- [ ] Enhanced command-line interface

## Licence
This implementation was created by me, [Nithik R](https://github.com/noinoiexists) and is protected under the MIT Licence. You can read more about it in the [LICENCE](https://github.com/noinoiexists/Data-Encryption-Standard/blob/main/LICENSE) file.
