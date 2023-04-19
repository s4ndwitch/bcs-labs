#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Pair {
  char symbol;
  char *code;
};

struct List {
  char symbol;
  struct List *nextElement;
};

char seek(char symbol, struct List *symbolsList) {
  if (symbolsList == NULL)
    return 0;
  while (symbolsList->nextElement != NULL) {
    struct List *nextElement = symbolsList->nextElement;
    if (symbolsList->symbol == symbol)
      return 1;
    symbolsList = nextElement;
  }
  return 0;
};

void add(char symbol, struct List *symbolsList) {
  struct List *newElement = (struct List *)malloc(sizeof(struct List) * 1);
  newElement->nextElement = NULL;
  newElement->symbol = symbol;
  
  if (symbolsList != NULL) {
    while (symbolsList->nextElement != NULL)
      symbolsList = symbolsList->nextElement;
    symbolsList->nextElement = newElement;
  } else {
    symbolsList = newElement;
  }
};

int getSymbolsAmount(char *fileName, char *symbols) {
  FILE *inputFile = fopen(fileName, "r");
  if (inputFile == NULL) {
    fprintf(stderr, "Cannot open input file");
    return -2;
  }
  
  struct List *symbolsList = (struct List *)malloc(sizeof(struct List) * 1);
  char symbol;
  int counter = 0;
  while ((symbol = getc(inputFile)) != EOF) {
    if (!seek(symbol, symbolsList)) {
      add(symbol, symbolsList);
      counter += 1;
    }
  }
  
  symbols = (char *)malloc(sizeof(char) * counter);
  counter = 0;
  while (symbolsList->nextElement != NULL) {
    struct List *nextElement = symbolsList->nextElement;
    symbols[counter++] = symbolsList->symbol;
    free(symbolsList);
    symbolsList = nextElement;
  }
  
  fclose(inputFile);
  return counter;
}

//float getOdd(char symbol, char *fileName) {
//  FILE *inputFile = fopen(fileName, "r");
//  if (inputFile == NULL) {
//    fprintf(stderr, "Cannot open input file");
//    return -1;
//  }
//  
//  
//  
//  fclose(fileName);
//}
//
//float *getOdds(char *fileName, char *symbols, int symbolsAmount) {
//  float *odds = (int *)malloc(sizeof(float) * symbolsAmount);
//  
//  for (int i = 0; i < symbolsAmount; i++) {
//    odds[i] = getOdd(symbols[i], fileName);
//  }
//  
//  sortOdds(odds);
//  return odds;
//}

int encode(char *fileName, struct Pair *pairs) {
    int length = 0;
    
    FILE *inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
      fprintf(stderr, "Cannot open input file");
      return -2;
    }
    
    char *symbols;
    int symbolsAmount = getSymbolsAmount(fileName, symbols);
    
    pairs = (struct Pair *)malloc(sizeof(struct Pair) * symbolsAmount);
    int *sortedOdds = getOdds(fileName, symbols, symbolsAmount);
    
    char **sortedCodes = getCodes(sortedOdds);
    
    int counter = 0;
    for (int i = 0; i < symbolsAmount; i++) {
      struct Pair newPair;
      newPair.symbol = symbols[i];
      newPair.code = sortedCodes[i];
      pairs[counter++] = newPair;
    }
    
    fclose(inputFile);
    
    return length;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "No input file specified");
    return -1;
  }
  if (argc < 3) {
    fprintf(stderr, "No mode specified");
    return -2;
  }
  
  if (!strcmp(argv[2], "-e")) {
    struct Pair *pairs;
    int length = encode(argv[1], pairs);
    if (length < 0) {
      return -10 + length;
    }
  }
}