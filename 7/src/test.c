#include <stdio.h>

int main() {
    FILE *outputFile = fopen("output.txt", "wb");
    if (outputFile == NULL) {
        fprintf(stderr, "Could not open output file.\n");
        return 1;
    }

    fprintf(outputFile, "\x", 45);

    fclose(outputFile);
    return 0;
}