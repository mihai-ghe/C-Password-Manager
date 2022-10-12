/*
 * commons.h
 *
 *  Created on: Apr 18, 2022
 *      Author: mishu
 */

#ifndef COMMONS_H_
#define COMMONS_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


	#include <stdio.h>
	#include <openssl/sha.h>
	#include <string.h>
	#include <openssl/rand.h>
	#include <openssl/aes.h>
	#include <openssl/evp.h>
	#include <windows.h>
	#include <GL/glut.h>
	#include <sys/stat.h>

	void echo_off() {

		HANDLE h;
		DWORD mode;

		h = GetStdHandle(STD_INPUT_HANDLE);

		if(GetConsoleMode(h, &mode)) {

			mode &= ~ENABLE_ECHO_INPUT;
			SetConsoleMode(h, mode);
		}

	}

	void echo_on() {

		HANDLE h;
		DWORD mode;

		h = GetStdHandle(STD_INPUT_HANDLE);

		if(GetConsoleMode(h, &mode)) {

			mode |= ENABLE_ECHO_INPUT;
			SetConsoleMode(h, mode);
		}

	}

	void sha256_string(char *string, char outputBuffer[65], char salt[33]) {

		unsigned char hash[SHA256_DIGEST_LENGTH];
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, string, strlen(string));
		SHA256_Update(&sha256, salt, 32);
		SHA256_Final(hash, &sha256);
		int i = 0;

		for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
			sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
		outputBuffer[64] = 0;

	}


	void salt_generator(char string[33]) {

		unsigned char salt[16];
		RAND_poll();
		RAND_bytes(salt, 16);
		for(int i = 0; i < 16; i++)
				sprintf(string + (i*2),"%02x", salt[i]);
	}

	int File_Copy(char Source[], char Destination[]) {

		char c[4096]; // BUFFER SIZE
		FILE *source = fopen(Source, "r");
		FILE *dest = fopen(Destination, "w");

		if(!source || !dest)
			return 1;

		while(!feof(source)) {

			size_t bytes = fread(c, 1, sizeof(c), source);
			if(bytes) {

				fwrite(c, 1, bytes, dest);
			}
		}

		fclose(source);
		fclose(dest);

		return 0;
	}

	void en_de_crypt(int should_encrypt, FILE *ifp, FILE *ofp, unsigned char *ckey, unsigned char *ivec) {

	    const unsigned BUFSIZE=4096;
	    unsigned char *read_buf = malloc(BUFSIZE);
	    unsigned char *cipher_buf;
	    unsigned blocksize;
	    int out_len;
	    EVP_CIPHER_CTX *ctx;
	    ctx = EVP_CIPHER_CTX_new();

	    EVP_CipherInit(ctx, EVP_aes_256_cbc(), ckey, ivec, should_encrypt);
	    blocksize = EVP_CIPHER_CTX_block_size(ctx);
	    cipher_buf = malloc(BUFSIZE + blocksize);

	    while (1) {

	        // Read data in blocks until EOF. Update the ciphering with each read.

	        int numRead = fread(read_buf, sizeof(unsigned char), BUFSIZE, ifp);
	        EVP_CipherUpdate(ctx, cipher_buf, &out_len, read_buf, numRead);
	        fwrite(cipher_buf, sizeof(unsigned char), out_len, ofp);
	        if (numRead < BUFSIZE) { // EOF
	            break;
	        }
	    }

	    // Now cipher the final block and write it out.

	    EVP_CipherFinal(ctx, cipher_buf, &out_len);
	    fwrite(cipher_buf, sizeof(unsigned char), out_len, ofp);

	    // Free memory

	    free(cipher_buf);
	    free(read_buf);
	}

	int Dir_Check(const char *path) {

	struct stat stats;

	stat(path, &stats);

	if (S_ISDIR(stats.st_mode))
		return 1;


	return 0;
}


#endif /* COMMONS_H_ */
