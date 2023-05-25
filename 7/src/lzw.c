#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct List {
	void *data;
	struct List *next;
};

struct Code {
	int line;
	char *string;
};

struct Code *create_code(int line, char *string) {
	struct Code *newCode = (struct Code *)malloc(sizeof(struct Code));
	newCode->line = line;
	newCode->string = string;
	return newCode;
}

void add_code(struct List *list, void *data) {
	if (list->data == NULL) {
		list->data = data;
		return;
	}
	while (list->next != NULL)
		list = list->next;
	struct List *newElement = (struct List *)malloc(sizeof(struct List));
	newElement->data = data;
	newElement->next = NULL;
	list->next = newElement;
}

int find_code(struct List *list, char *string) {

	while (list->next != NULL) {
		if (!strncmp(((struct Code *)list->data)->string,
				string, strlen(string)))
			return ((struct Code *)list->data)->line;
		list = list->next;
	}

	return 0;
	
}

int intlen(int number) {
	int result = 0;
	while (number > 0) {
		result += 1;
		number /= 10;
	}
	return result;
}

int decode(int argc, char *argv[]) {
	return 0;
}

int encode(int argc, char *argv[]) {

	if (argc < 4) {
		fprintf(stderr, "Not enough arguments\n");
		return 1;
	}

	FILE *inputFile = fopen(argv[2], "r");
	if (inputFile == NULL) {
		fprintf(stderr, "Could not open file for reading: %s\n", argv[2]);
		return 1;
	}

	FILE *outputFile = fopen(argv[3], "w");
	if (outputFile == NULL) {
		fprintf(stderr, "Could not open file for writing: %s\n", argv[3]);
		return 1;
	}

	struct List *codeList = (struct List *)
			malloc(sizeof(struct List));

	for (int i = 0; i < 256; i++) {

		char *newString = (char *)malloc(sizeof(char) * 2);
		newString[0] = i;
		newString[1] = 0;
		add_code(codeList, create_code(i, newString));
		
	}

	char *prefix = (char *)malloc(sizeof(char)), symbol;
	int currentCode = 256;
	while (!feof(inputFile)) {
		symbol = fgetc(inputFile);
		printf("symbol is %c, its code is %d\n", symbol, symbol);
		char *newLine = (char *)malloc(sizeof(char) * (strlen(prefix) + 2));
		strncpy(newLine, prefix, strlen(prefix));
		newLine[strlen(prefix)] = symbol;
		newLine[strlen(prefix) + 1] = 0;
		printf("current new line is %s\n", newLine);

		int foundLine = find_code(codeList, newLine);
		printf("it has already met: %d\n", foundLine);
		if (foundLine == 0) {
			printf("registering and printing prefix\n");
			add_code(codeList, create_code(
					currentCode++, newLine
			));

			// Мне было лень что-то придумывать для обхода
			// ограничений по таблице ASCII. ־\(-ω-)/־
			int prefixCode = find_code(codeList, prefix);
			char command[20 + strlen(argv[1]) + intlen(prefixCode)];
			sprintf(command, "./src/printing.py %s %d", argv[3], prefixCode);
			system(command);
			free(prefix);
			prefix = (char *)malloc(sizeof(char) * 2);
			prefix[0] = symbol;

		} else {
			printf("reading further\n");
			free(prefix);
			prefix = newLine;
		}
	}

	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "Not enough arguments\n");
		return 1;
	}

	if (!strncmp(argv[1], "-d", 2) || !strncmp(argv[1], "--decode", 8)) {
		return decode(argc, argv);
	} else if (!strncmp(argv[1], "-e", 2) ||
			!strncmp(argv[1], "--encode", 8)) {
		return encode(argc, argv);
	} else {
		fprintf(stderr, "Unknown flag\n");
		return 1;
	}

	return 0;
	
}
