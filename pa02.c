/* I Andrew Huddleston (an235608) affirm that
 * this program is entirely my own work and that I have neither developed my code with any
 * another person, nor copied any code from any other person, nor permitted my code to be copied
 * or otherwise used by any other person, nor have I copied, modified, or otherwise used programs
 * created by others. I acknowledge that any violation of the above terms will be treated as
 * academic dishonesty.”
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv) {
	char file[] = "in10A.txt";
	int blockSize = 32;
	if (blockSize != 8 && blockSize != 16 && blockSize != 32) {
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
		exit(1);
	}
	
	int ptLen, b, numBlocks, padSize;
	
	FILE* infile = fopen(file, "r");
	char plainText[1000];
	
	fgets(plainText, 1000, infile);

	ptLen = 0;
	while (plainText[ptLen] != '\n') ptLen++;
	ptLen++; // Get length of plaintext + newline
	
	padSize = ptLen % (blockSize / 8); // Compute amount of padding
	if (padSize > 0) {
		padSize = blockSize / 8 - padSize;
		for (int i = 0; i < padSize; i++) {
			plainText[ptLen + i] = 'X'; // Pad with X's
		}
	}
	
	b = blockSize / 8;
	numBlocks = (ptLen + b - 1) / b;
	ptLen += padSize;
	
	char hexStr[ptLen][2];
	char hexdig[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	
	for (int i = 0; i < ptLen; i++) { // Convert ascii to hex and copy to hexStr
		sprintf(hexStr[i], "%02x", plainText[i]);
	}

	
	int ctr = -1;
	unsigned long long sum = 0;
	for (int i = 0; i < numBlocks; i++) { // Compute sum
		for (int j = 0; j < b * 2; j++) {
			if (j % 2 == 0) ctr++;
			for (int k = 0; k < 16; k++) { 
				if (hexStr[ctr][j % 2] == hexdig[k]) { // Hex -> dec, add to sum
					int a = 1;
					for (int l = 0; l < b * 2 - j - 1; l++) a *= 16;
					sum += k * a;
					break;
				}
			}
		}
	}

	unsigned long long bits = 1;
	for (int i = 0; i < blockSize; i++) bits *= 2;
	unsigned long int val = sum % bits; // Convert sum back to hex
	for (int i = 0 ; i < ptLen; i++) {
		if (i % 80 == 0) printf("\n");
		printf("%c", plainText[i]);
	}
	printf("\n%2d bit checksum is %8lx for all %4d chars\n", blockSize, val, ptLen);

	return 0;
}
