#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

struct Pair {
    char symbol;
    char code;
};

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

int bin2int(char *bin, int length) {
    int result = 0;
    for (int i = length - 1; i > -1; i--)
        if (bin[i] == '1')
            result += pow(2, length - i - 1);
    return result;
}

int int2bin(int number, char **bin) {
    int length = 1;
    while (pow(2, length) <= number)
        length++;

    *bin = (char *)malloc(sizeof(char) * (length));

    int count = length - 1;
    while (count > -1) {
        (*bin)[count--] = number % 2;
        number /= 2;
    }

    return length;
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
        if (dictionary[i] != 0) {
            codes[count].data = (char *)malloc(sizeof(char));
            codes[count].data[0] = (char)i;
            codes[count].dataLength = 1;
            codes[count].rate = dictionary[i];
            codes[count].code = NULL;
            codes[count++].codeLength = 0;
        }
    }
    if (count == 1) {
        codes[0].code = (char *)malloc(sizeof(char));
        codes[0].code[0] = '0';
        codes[0].codeLength = 1;
    } else
        encode(codes, count);
    
    for (int i = 0; i < count; i++) {
        if (codes[i].data[0] == 'J') {
            printf("%d\n", codes[i].codeLength);
            for (int j = 0; j < codes[i].codeLength; j++)
                printf("%c", codes[i].code[j]);
            printf("\n");
        }
    }

    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "failed to open input file\n");
        return;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "failed to open output file\n");
        return;
    }

    long outSize = 0;
    for (int i = 0; i < count; i++)
        outSize += codes[i].codeLength * codes[i].rate;

    fprintf(outputFile, "%ld", outSize);
    fprintf(outputFile, "%c", 1);
    fprintf(outputFile, "%d", count);
    for (int i = 0; i < count; i++)
        fprintf(outputFile, "%c%c%d", codes[i].data[0], bin2int(codes[i].code, codes[i].codeLength), codes[i].codeLength);
    fprintf(outputFile, "\x2");
    char buffer[16];
    char counter = 0;
    while (fscanf(inputFile, "%c", &symbol)!= EOF) {
        for (int i = 0; i < count; i++) {
            if (codes[i].data[0] == symbol) {
                memcpy(buffer + counter, codes[i].code, codes[i].codeLength);
                counter += codes[i].codeLength;

                if (counter >= 8) {
                    fprintf(outputFile, "%c", bin2int(buffer, 8));
                    memcpy(buffer, buffer + 8, counter - 8);
                    counter -= 8;
                }

                break;
            }
        }
    }
    if (counter > 0) {
        while (counter < 8)
            memcpy(buffer + counter++, "0", 1);
        fprintf(outputFile, "%c", bin2int(buffer, counter));
    }

    fclose(inputFile);
    fclose(outputFile);
}

int c2i(char c) {
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        default:
            return -1;
    }
}

void decode(char *inputFileName, char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r"); // открываем файл к прочению
    if (inputFile == NULL) {
        fprintf(stderr, "failed to open input file\n");
        return;
    }

    unsigned char symbol;
    char step = 0;
    long byteSize = 0;
    int codesLength = 0;
    unsigned char **decodeTable;
    int counter = 0;
    char column = 0;
    char buffer[16];
    char cursor = 0;
    int passedBits = 0;
    while (fscanf(inputFile, "%c", &symbol)!= EOF) // идём посимвольно
    {
        switch (step) { // шаг означает этап считывания
            case 0: // получение количества бит в закодированной части
                if (isdigit(symbol)) {
                    byteSize = byteSize * 10 + c2i(symbol);
                    break;
                }
                step = 1;
                break;
            case 1: // количества кодов
                if (isdigit(symbol)) {
                    codesLength = codesLength * 10 + c2i(symbol);
                    break;
                }
                decodeTable = (unsigned char **)malloc(sizeof(unsigned char *) * codesLength);
                for (int i = 0; i < codesLength; i++) {
                    decodeTable[i] = (unsigned char *)malloc(sizeof(unsigned char) * 2);
                }
                step = 2;
            case 2: // собственно, кодов
                if (counter == codesLength) {
                    counter = 1;
                    step = 3;
                    break;
                }

                // первый столбец - символ, второй - код, третий - размер
                if (column == 0 || column == 1) {
                    decodeTable[counter][column] = symbol;
                } else {
                    decodeTable[counter][column] = c2i(symbol);
                }
                column = (column + 1) % 3;
                if (!column)
                    counter += 1;
                break;
            case 3: // считывание основной части

                // код последнего считанного символа кладётся в буфер
                char **symbolBin = (char **)malloc(sizeof(char *));
                int symbolBinLength = int2bin(symbol, symbolBin);
                memcpy(buffer + cursor + 8 - symbolBinLength, *symbolBin, symbolBinLength);
                free(*symbolBin);
                free(symbolBin);
                for (int i = cursor; i < 8 - symbolBinLength; i++)
                    buffer[i] = 0;
                cursor += 8;

                int result = 0;
                while (result != -1) { // пока не закончится буфер

                    // перебираем все коды в таблице на совпадение
                    for (int i = 0; i < codesLength; i++) {
                        char **gotBin = (char **)malloc(sizeof(char *));
                        int currentBinLength = int2bin(decodeTable[i][1], gotBin);
                        char *currentBin = (char *)malloc(sizeof(char) * decodeTable[i][2]);
                        for (int i = 0; i < currentBinLength; i++)
                            currentBin[i] = 0;
                        counter = currentBinLength - 1;
                        for (int j = decodeTable[i][2] - 1; j > -1; j--) {
                            currentBin[j] = (*gotBin)[counter--];
                        }
                        if (cursor < currentBinLength) {
                            result = -1;
                            printf("current cursor: %d\n", cursor);
                            break;
                        }
                        char equal = 1;
                        printf("Current buffer:\n");
                        for (int i = 0; i < cursor; i++)
                            printf("%d", buffer[i]);
                        printf("\n");

                        printf("Current bin:\n");
                        for (int i = 0; i < decodeTable[i][2]; i++)
                            printf("%d", currentBin[i]);
                        printf("\n");
                        for (int j = 0; j < decodeTable[i][2]; j++)
                            if (currentBin[j] != buffer[j])
                                equal = 0;
                        if (equal) {
                            printf("%c", decodeTable[i][0]);
                            memcpy(buffer, buffer + decodeTable[i][2], cursor - decodeTable[i][2]);
                            cursor -= decodeTable[i][2];
                            passedBits += decodeTable[i][2];
                            if (passedBits >= byteSize)
                                result = -1;
                            free(currentBin);
                            break;
                        }
                        free(currentBin);
                        free(*gotBin);
                        free(gotBin);
                    }
                }
                break;
        }
    }

    fclose(inputFile);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) { // 3 аргумента - мод, входной и выходной файлы.
        fprintf(stderr, "Not enough arguments\n");
        return 0;
    }

    if (!strcmp(argv[1], "-e")) { // -encode
        haffman(argv[2], argv[3]);
    }
    else if (!strcmp(argv[1], "-d")) // -decode
        decode(argv[2], argv[3]);
}