#include <string.h>

void func(char *input) {
  char buffer[100];
  strcpy(buffer, input);
}

int main(int argc, char *argv[]) {
  func(argv[1]);
  return 0;
}