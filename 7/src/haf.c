#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Union {
    char *data;
    int dataLength;
    int rate;
    char *code;
    int codeLength;
};

char *getUnionCode(struct Union *u, int codesLength, char symbol) {
    char *result;
    for (int i = 0; i < codesLength; i++) {
        if (symbol == u[i].data[0]) {
            result = (char *) malloc(sizeof(char) * (u[i].codeLength));
            result = u[i].code;
            break;
        }
    }
    return result;
}

void removeElement(struct Union *codes, int codesLength, struct Union *code) {
    for (int i = 0; i < codesLength; i++) {
        if (strcmp(codes[i].data, code->data) == 0) {
            for (int j = i; j < codesLength - 1; j++) {
                codes[j] = codes[j + 1];
            }
            codesLength--;
            break;
        }
    }
}

void addElement(struct Union *codes, int codesLength, struct Union *code) {
    memcpy(codes + codesLength, code, sizeof(struct Union));
}

long getFileSize(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "failed to open input file");
        return 0;
    }

    long result = 0;
    char symbol;
    while (fscanf(file, "%c", &symbol) != EOF)
        result += 1;
    
    return result;
}

void checkCompression(char *firstFileName, char *secondFileName) {
    long firstSize = getFileSize(firstFileName);
    long secondSize = getFileSize(secondFileName);

    printf("First file size: %ld\nSecond file size: %ld\nCompression K = %d",
        firstSize, secondSize, firstSize / secondSize);

    return;
}

void encode(struct Union *codes, int codesLength) {
    if (codesLength > 1) {
        struct Union *minimum = (struct Union *) malloc(sizeof(struct Union));
        struct Union *preminimum = (struct Union *) malloc(sizeof(struct Union));
        memcpy(minimum, codes + 0, sizeof(struct Union));
        memcpy(preminimum, codes + 1, sizeof(struct Union));
        for (int i = 0; i < codesLength; i++) {
            if (codes[i].rate < minimum->rate) {
                memcpy(preminimum, minimum, sizeof(struct Union));
                memcpy(minimum, codes + i, sizeof(struct Union));
            } else if (codes[i].rate > minimum->rate && codes[i].rate < preminimum->rate) {
                memcpy(preminimum, codes + i, sizeof(struct Union));
            }
        }

        struct Union *newUnion = (struct Union *)malloc(sizeof(struct Union));
        newUnion->data = (char *)malloc(minimum->dataLength + preminimum->dataLength);
        newUnion->dataLength = minimum->dataLength + preminimum->dataLength;
        newUnion->rate = minimum->rate + preminimum->rate;
        newUnion->code = NULL;
        newUnion->codeLength = 0;

        memcpy(newUnion->data, minimum->data, minimum->dataLength);
        memcpy(newUnion->data + minimum->dataLength, preminimum->data, preminimum->dataLength);

        removeElement(codes, codesLength, minimum);
        codesLength -= 1;
        removeElement(codes, codesLength, preminimum);
        codesLength -= 1;

        addElement(codes, codesLength, newUnion);
        codesLength += 1;

        encode(codes, codesLength);

        for (int i = 0; i < codesLength; i++) {
            if (strcmp(codes[i].data, newUnion->data) == 0) {
                memcpy(newUnion, codes + i, sizeof(struct Union));
                break;
            }
        }

        char *newCode = (char *)malloc(minimum->codeLength + preminimum->codeLength);
        memcpy(newCode, newUnion->code, newUnion->codeLength);
        int codeLength = newUnion->codeLength;

        removeElement(codes, codesLength, newUnion);
        free(newUnion);
        codesLength -= 1;

        minimum->code = (char *)malloc(codeLength + 1);
        preminimum->code = (char *)malloc(codeLength + 1);

        minimum->codeLength = codeLength + 1;
        preminimum->codeLength = codeLength + 1;

        if (codeLength != 0) {
            memcpy(minimum->code, newCode, codeLength);
            memcpy(preminimum->code, newCode, codeLength);
        }

        minimum->code[codeLength] = '0';
        preminimum->code[codeLength] = '1';

        addElement(codes, codesLength, minimum);
        codesLength += 1;
        addElement(codes, codesLength, preminimum);
        codesLength += 1;

        free(minimum);
        free(preminimum);

        return;
    }  
}

void haffman(char *inputFileName, char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "failed to open input file\n");
        return;
    }

    int dictionary[256];
    for (int i = 0; i < 256; i++)
        dictionary[i] = 0;
    
    char symbol;
    while (fscanf(inputFile, "%c", &symbol)!= EOF)
        dictionary[symbol] += 1;

    fclose(inputFile);

    int notNull = 0;
    for (int i = 0; i < 256; i++)
        if (dictionary[i]!= 0)
            notNull += 1;
    
    if (notNull == 0) {
        fprintf(stderr, "input file is empty");
        return;
    }

    int count = 0;
    struct Union *codes = (struct Union *)malloc(sizeof(struct Union) * notNull);
    for (int i = 0; i < 256; i++) {
        if (dictionary[i]!= 0) {
            codes[count].data = (char *)malloc(sizeof(char));
            codes[count].data[0] = (char)i;
            codes[count].dataLength = 1;
            codes[count].rate = dictionary[i];
            codes[count].code = NULL;
            codes[count++].codeLength = 0;
        }
    }

    encode(codes, count);

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < codes[i].codeLength; j++) {
            printf("%c", codes[i].code[j]);
        }
        printf("\n");
    }
    
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments\n");
        return 0;
    }

    haffman(argv[1], argv[2]);
}