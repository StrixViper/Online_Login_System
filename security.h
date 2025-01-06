#ifndef SECURITY_H_INCLUDED
#define SECURITY_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_VALUE_PASS_LEN 50
#define SEC_FILE_SIZE 10000

// SHA-256 Constants
#define SHA256_BLOCK_SIZE 64  // SHA256 operates on 512-bit blocks (64 bytes)
#define SHA256_HASH_SIZE 32   // SHA256 produces a 256-bit hash (32 bytes)

static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0x49b40821, 0x6a1e6c5e, 0x8bbf63f6, 0x1f83d9ab, 0x5be0cd19, 0xc1401e7d, 0x9c2b3f0a, 0x71a5d39e,
    0x3c6ef372, 0x5d4c8021, 0x6d703ef3, 0xa0b67443, 0x5f1e4428, 0x34f6ab5b, 0x3f52e6e1, 0x70641ab0,
    0x8c2a7d62, 0x1f8c71f5, 0x925bcb74, 0xa6b411cb, 0x1ad9cf00, 0x88c29f43, 0xe09b5db0, 0x445ed39e,
    0x2139e220, 0x28588d38, 0xd53b3a7e, 0xded1e67b, 0x5b9e8ef2, 0x32f46444, 0x28fe739f, 0xf741a268
};

void sha256_transform(uint32_t state[8], const uint8_t data[SHA256_BLOCK_SIZE]); // Function to process a single 512-bit block
void sha256(const uint8_t *input, size_t length, uint8_t *output); // Function to pad the message and compute the hash
void printHash(uint8_t *hash, size_t length); // Function to print the SHA-256 hash as a hexadecimal string
int compareHashes(const uint8_t *hash1, const char *hash2); // Function to compare two hashes byte-by-byte
void xorCipher(uint8_t *data, size_t length, uint8_t *key, size_t keyLength); // XOR Cipher function declaration
void hexStringToBytes(const char* hexString, uint8_t* byteArray);  // Helper function to convert hex string to byte array
void encrypt(char *plaintext); // Function to encrypt the text with a fixed shift of 12
void decrypt(char *ciphertext); // Function to decrypt the text with a fixed shift of 12
void base64Encode(const char* input, char* output, size_t maxLength);
uint32_t hash_username(const char* username);


char* encrypt_password(char* password);


// XOR Cipher key definition
static const uint8_t xorKey[] = {0x12, 0x34, 0x56, 0x78};  // Example XOR key (can be any byte sequence)
static const size_t xorKeyLength = sizeof(xorKey);  // Length of the XOR key



#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define SHR(x, n)  ((x) >> (n))
#define CH(x, y, z) ((x) & (y) ^ (~(x) & (z)))
#define MAJ(x, y, z) ((x) & (y) ^ ((x) & (z)) ^ ((y) & (z)))
#define SIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sigma0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define sigma1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))



const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// SHA-256 Initialization Hash Values
static const uint32_t H0[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

void sha256_transform(uint32_t state[8], const uint8_t data[SHA256_BLOCK_SIZE]) {
    uint32_t W[64], a, b, c, d, e, f, g, h, t1, t2;

    // Prepare the message schedule W[0..63]
    for (int i = 0; i < 16; i++) {
        W[i] = ((uint32_t)data[i * 4] << 24) |
               ((uint32_t)data[i * 4 + 1] << 16) |
               ((uint32_t)data[i * 4 + 2] << 8) |
               ((uint32_t)data[i * 4 + 3]);
    }

    for (int i = 16; i < 64; i++) {
        W[i] = sigma1(W[i - 2]) + W[i - 7] + sigma0(W[i - 15]) + W[i - 16];
    }

    // Initialize working variables
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    // Main loop
    for (int i = 0; i < 64; i++) {
        t1 = h + SIGMA1(e) + CH(e, f, g) + k[i] + W[i];
        t2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // Add the compressed chunk to the current hash value
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

void sha256(const uint8_t *input, size_t length, uint8_t *output) {
    uint32_t state[8];
    uint64_t bit_length = length * 8;
    size_t padded_length = ((length + 8) / SHA256_BLOCK_SIZE + 1) * SHA256_BLOCK_SIZE;
    uint8_t *padded_input = (uint8_t *)malloc(padded_length);

    // Initialize the state with the initial hash values
    memcpy(state, H0, sizeof(state));

    // Copy the input data and append a '1' bit followed by '0's
    memcpy(padded_input, input, length);
    padded_input[length] = 0x80;  // 1 bit followed by 0 bits
    memset(padded_input + length + 1, 0, padded_length - length - 9);
    padded_input[padded_length - 8] = (uint8_t)(bit_length >> 56);
    padded_input[padded_length - 7] = (uint8_t)(bit_length >> 48);
    padded_input[padded_length - 6] = (uint8_t)(bit_length >> 40);
    padded_input[padded_length - 5] = (uint8_t)(bit_length >> 32);
    padded_input[padded_length - 4] = (uint8_t)(bit_length >> 24);
    padded_input[padded_length - 3] = (uint8_t)(bit_length >> 16);
    padded_input[padded_length - 2] = (uint8_t)(bit_length >> 8);
    padded_input[padded_length - 1] = (uint8_t)(bit_length);

    // Process the message in 512-bit chunks
    for (size_t i = 0; i < padded_length; i += SHA256_BLOCK_SIZE) {
        sha256_transform(state, padded_input + i);
    }

    // Output the final hash
    for (int i = 0; i < 8; i++) {
        output[i * 4] = (uint8_t)(state[i] >> 24);
        output[i * 4 + 1] = (uint8_t)(state[i] >> 16);
        output[i * 4 + 2] = (uint8_t)(state[i] >> 8);
        output[i * 4 + 3] = (uint8_t)(state[i]);
    }

    // Free the padded input buffer
    free(padded_input);
}

void printHash(uint8_t *hash, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int compareHashes(const uint8_t *hash1, const char *hash2) {
    size_t len = strlen(hash2) / 2;
    uint8_t *computedHash = (uint8_t *)malloc(len);

    // Convert hex string to byte array
    for (size_t i = 0; i < len; i++) {
        sscanf(&hash2[i * 2], "%2hhx", &computedHash[i]);
    }

    // Compare the two byte arrays
    int result = memcmp(hash1, computedHash, len);

    free(computedHash);  // Free memory allocated for hex-to-byte array
    return result;
}

void xorCipher(uint8_t *data, size_t length, uint8_t *key, size_t keyLength) {
    for (size_t i = 0; i < length; i++) {
        data[i] ^= key[i % keyLength];  // XOR each byte with the corresponding key byte
    }
}

void hexStringToBytes(const char* hexString, uint8_t* byteArray) {
    size_t len = strlen(hexString) / 2; // Each pair of hex chars represents one byte
    for (size_t i = 0; i < len; i++) {
        sscanf(&hexString[i * 2], "%2hhx", &byteArray[i]);
    }
}

void encrypt(char *plaintext) {
    int shift = 20;  // Fixed shift value
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];

        // Encrypt lowercase letters
        if (ch >= 'a' && ch <= 'z') {
            plaintext[i] = (ch - 'a' + shift) % 26 + 'a';
        }
        // Encrypt uppercase letters
        else if (ch >= 'A' && ch <= 'Z') {
            plaintext[i] = (ch - 'A' + shift) % 26 + 'A';
        }
    }
}

void decrypt(char *ciphertext) {
    int shift = 20;  // Fixed shift value
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];

        // Decrypt lowercase letters
        if (ch >= 'a' && ch <= 'z') {
            ciphertext[i] = (ch - 'a' - shift + 26) % 26 + 'a';
        }
        // Decrypt uppercase letters
        else if (ch >= 'A' && ch <= 'Z') {
            ciphertext[i] = (ch - 'A' - shift + 26) % 26 + 'A';
        }
    }
}

char* encrypt_password(char* password) {

    size_t len = strlen(password);
    // Step 1: XOR the password with the XOR key
    uint8_t *xorData = (uint8_t *)malloc(len);
    if (!xorData) {
        fprintf(stderr, "Memory allocation failed for XOR data.\n");
        return NULL;
    }
    memcpy(xorData, password, len);
    xorCipher(xorData, len, (uint8_t *)xorKey, xorKeyLength);

    // Step 2: Hash the XORed password using SHA-256
    uint8_t hash[SHA256_HASH_SIZE];
    sha256(xorData, len, hash);
    free(xorData);  // Free memory for XORed data after hashing
    // Step 3: Convert the hash to a hexadecimal string
    char *hexHash = (char *)malloc(SHA256_HASH_SIZE * 2 + 1);
    if (!hexHash) {
        fprintf(stderr, "Memory allocation failed for hexadecimal hash.\n");
        return NULL;
    }
    for (size_t i = 0; i < SHA256_HASH_SIZE; i++) {
        sprintf(hexHash + (i * 2), "%02x", hash[i]);
    }
    hexHash[SHA256_HASH_SIZE * 2] = '\0';  // Null-terminate the string
    // Step 4: Encrypt the hexadecimal hash using the Caesar cipher
    encrypt(hexHash);

    return hexHash;
}

void base64Encode(const char* input, char* output, size_t maxLength) {
    size_t inputLen = strlen(input);
    size_t j = 0;
    for (size_t i = 0; i < inputLen && j < maxLength; i += 3) {
        unsigned int octet_a = i < inputLen ? input[i] : 0;
        unsigned int octet_b = (i + 1) < inputLen ? input[i + 1] : 0;
        unsigned int octet_c = (i + 2) < inputLen ? input[i + 2] : 0;

        unsigned int triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        if (j < maxLength) output[j++] = base64_chars[(triple >> 18) & 0x3F];
        if (j < maxLength) output[j++] = base64_chars[(triple >> 12) & 0x3F];
        if (j < maxLength) output[j++] = base64_chars[(triple >> 6) & 0x3F];
        if (j < maxLength) output[j++] = base64_chars[triple & 0x3F];
    }
    output[j] = '\0';
}

uint32_t hash_username(const char* username) {
    uint32_t hash = 5381;
    while (*username) {
        hash = ((hash << 5) + hash) + *username++; // hash * 33 + c
    }
    return hash % SEC_FILE_SIZE;
}

#endif // SECURITY_H_INCLUDED
