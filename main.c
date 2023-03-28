#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

const char alphabet[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890_";

char *encode(char *input, int length) {
	char *result = malloc(BASE64_ENCODE_OUT_SIZE(length));
	base64_encode(input, length, result);
	return result;
}

int main() {

	char inputPassword[5];
	char *encodedPassword;

	char currentPassword[5];
	for (int i = 0; i < 5; i++)
		currentPassword[i] = 0;

	int alphabetLength = strlen(alphabet);
	int currentSymbol = 0;

	puts("Input your beloved password here:");
	scanf("%s", inputPassword);
	printf("%i %i %i %i\n", inputPassword[0], inputPassword[1], inputPassword[2], inputPassword[3]);
	encodedPassword = encode(inputPassword, strlen(inputPassword));
	
	puts("Your beloved password now looks like that:");
	puts(encodedPassword);

	while (!strcmp(encode(currentPassword, strlen(currentPassword)), encodedPassword));

	for (int fourthPos = -1; fourthPos < alphabetLength; fourthPos++) {
		for (int thirdPos = -1; thirdPos < alphabetLength; thirdPos++) {
			for (int secondPos = -1; secondPos < alphabetLength; secondPos++) {
				for (int firstPos = -1; firstPos < alphabetLength; firstPos++) {
					currentPassword[0] = firstPos != -1 ? alphabet[firstPos] : 0;
					currentPassword[1] = secondPos != -1 ? alphabet[secondPos] : 0;
					currentPassword[2] = thirdPos != -1 ? alphabet[thirdPos] : 0;
					currentPassword[3] = fourthPos != -1 ? alphabet[fourthPos] : 0;

					printf("\rCurrent password: %s", currentPassword);

					if (!strcmp(encode(currentPassword, strlen(currentPassword)), encodedPassword)) {
						puts("\rGotcha! The password is");
						puts(currentPassword);

						return 0;
					}
				}
			}
		}
	}
					

	return 0;
}
