#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char checkForZero(char *buffer, int bufferCapacity) {
  for (int i = 0; i < bufferCapacity; i++)
    if (buffer[i] != -1)
      return 0;
  return 1;
}

int power(int a, int b) {
  int result = 1;
  while (b--)
    result *= a;
  return result;
}

int btoi(char *binary, int binaryLength) {
  int count = 0;
  int result = 0;
  while (count < binaryLength) {
    if (binary[binaryLength - 1 - count++])
      result += power(2, count - 1);
  }
  return result;
}

char *ctob(unsigned char symbol) {

  char *result = (char *)malloc(sizeof(char) * 8);
  for (int i = 0; i < 8; i++) {
    result[i] = 0;
  }
  int count = 0;
  while (symbol > 0) {
    result[8 - 1 - count++] = symbol % 2;
    symbol /= 2;
  }
  return result;
}

void loadSymbol(char symbol, char *buffer, int bufferCapacity) {
  int i = -1;
  while (i++ < bufferCapacity && buffer[i] != -1);
  if (i >= bufferCapacity)
    return;
  char *binary = ctob(symbol);
  memcpy(buffer + i, binary, 8);
  free(binary);
}

void updateBinary(char *buffer, int bufferCapacity) {
  int i = -1;
  while (i++ < bufferCapacity && buffer[i] == 0);
  for (int count = i; count < bufferCapacity; count++)
    buffer[count - i] = buffer[count];
  for (int count = bufferCapacity - i; count < bufferCapacity; count++)
    buffer[count] = -1;
}

char *itob(int number) {
  int grade = 1, border = 2;
  while (border < number && grade++)
    border *= 2;
  
  char *result = (char *)malloc(sizeof(char) * (grade - 1));
  for (int i = 0; i < grade; i++) {
    result[i] = 0;
  }
  int count = 0;
  while (number > 0) {
    result[grade - 1 - count++] = number % 2;
    number /= 2;
  }
  return result;
}

char *xor(char *firstNumber, char *secondNumber, int length) {
  char *result = (char *)malloc(sizeof(char) * length);
  while (length--) {
    result[length] = secondNumber[length] ^ firstNumber[length];
  }
  return result;
}

char *moveCRC(char *fileName, int polynom) {
  char *polynomBin = itob(polynom);

  FILE *inputFile = fopen(fileName, "r");
  if (inputFile == NULL) {
    fprintf(stderr, "Could not open input file");
    return NULL;
  }
  char *buffer = (char *)malloc(sizeof(char) * 16);
  for (int i = 0; i < 16; i++)
    buffer[i] = -1;
  int counter = 0;

  unsigned char symbol = 'a', *binary;
  char isLoaded = 0;
  while ("This shit gets too long") {
    char last = buffer[0];
    memcpy(buffer, buffer + 1, 15);
    buffer[15] = -1;
    if (buffer[8] == -1) {
      if ((symbol = fgetc(inputFile)) == 255 && feof(inputFile)) {
        if (isLoaded) {
          break;
        }
        isLoaded += 1;
        loadSymbol(0, buffer, 16);
        continue;
      }
      loadSymbol(symbol, buffer, 16);
      continue;
    }
    if (last) {
      char *binary = xor(buffer, polynomBin, 9);
      memcpy(buffer, binary, 9);
      free(binary);
    }
  }

  free(polynomBin);
  fclose(inputFile);

  char *result = (char *)malloc(sizeof(char *) * 8);
  for (int i = 0; i < 8; i++)
    result[i] = 0;
  int i = -1;
  counter = 7;
  while (buffer[++i] != -1);
  for (i -= 1;i > -1; i--) {
    result[counter--] = buffer[i];
  }

  free(buffer);
  return result;
}
    
char *divisionCRC(char *fileName, int polynom) {
  char *polynomBin = itob(polynom);
  
  FILE *inputFile = fopen(fileName, "r");
  if (inputFile == NULL) {
    fprintf(stderr, "Could not open input file");
    return NULL;
  }
  char *buffer = (char *)malloc(sizeof(char) * 16);
  for (int i = 0; i < 16; i++)
    buffer[i] = -1;
  int counter = 0;

  unsigned char symbol = 'a', *binary;
  char isLoaded = 0;
  while ("This shit gets too long") {
    if (buffer[8] == -1) {
      if ((symbol = fgetc(inputFile)) == 255 && feof(inputFile)) {
        if (isLoaded) {
          break;
        }
        isLoaded += 1;
        loadSymbol(0, buffer, 16);
        continue;
      }
      loadSymbol(symbol, buffer, 16);
      continue;
    }
    char *binary = xor(buffer, polynomBin, 9);
    memcpy(buffer, binary, 9);
    free(binary);
    updateBinary(buffer, 16);
  }
  
  free(polynomBin);
  fclose(inputFile);
  
  char *result = (char *)malloc(sizeof(char *) * 8);
  for (int i = 0; i < 8; i++)
    result[i] = 0;
  int i = -1;
  counter = 7;
  while (buffer[++i] != -1);
  for (i -= 1;i > -1; i--) {
    result[counter--] = buffer[i];
  }
  
  free(buffer);
  return result;
}

int main(int argc, char *argv[]) {
  
  if (argc < 2) {
    fprintf(stderr, "No input file specified");
    return 1;
  }
  
//  FILE *inputFile = fopen(argv[1], "r");
//  if (inputFile == NULL) {
//    fprintf(stderr, "Could not open input file");
//    return -1;
//  }
//  
//  char data[101];
//  fgets(data, 101, inputFile);
//  data[100] = 0;
//  
//  fclose(inputFile);
//  
//  for (int i = 0b100000001; i < 0b1000000000; i++) {
//    char *binary = divisionCRC(argv[1], i);
//    unsigned char decimal = btoi(binary, 8);
//    free(binary);
//    
//    FILE *outputFile = fopen("output.txt", "w");
//    fprintf(outputFile, "%s", data);
//    fprintf(outputFile, "%c", decimal);
//    fclose(outputFile);
//    
//    binary = divisionCRC("output.txt", i);
//    decimal = btoi(binary, 8);
//    free(binary);
//    
//    if (decimal != 0)
//      printf("Found unzeroed polynom %d\n", i);
//  }
  
  char *binary = moveCRC(argv[1], 0b100011101);
  int decimal = btoi(binary, 8);
  
  printf("Results\n---------------\nsymbolic\t%c\t\nbinary\t\t", decimal > 127 ? decimal - 127 : (char)decimal);
  for (int i = 0; i < 8; i++)
    printf("%d", binary[i]);
  printf("\ndecimal\t\t%d\nhexadecimal\t%02x\n", decimal, decimal);
  
  free(binary);
  
  return 0;
}