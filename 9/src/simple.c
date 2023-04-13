#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "No input file specified");
    return -1;
  }
  
  FILE *inputFile = fopen(argv[1], "r");
  if (inputFile == NULL) {
    fprintf(stderr, "Could not open input file");
    return -2;
  }
  
  unsigned char controlSum;
  char symbol;
  while ((symbol = fgetc(inputFile)) != EOF) {
    controlSum += symbol;
  }
  
  fclose(inputFile);
  
  printf("Control sum for the file is: %d\n", controlSum);
  
  return 0;
}