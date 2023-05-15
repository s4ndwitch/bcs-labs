#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pairsCounter;
char ***pairs;

struct Node {
    char *data;
    int rate;
    struct Node *fp;
    struct Node *sp;
};

struct Node *create_node() {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = NULL;
    newNode->rate = 0;
    newNode->fp = NULL;
    newNode->sp = NULL;

    return newNode;
}

void descend(struct Node *node, char *code) {

    if (strlen(node->data) == 1) {
        pairs[pairsCounter][0] = node->data;
        char *newCode = (char *)malloc(sizeof(char) * (strlen(code) + 1));
        strcpy(newCode, code);
        pairs[pairsCounter++][1] = newCode;

        return;
    } {
        char *newCode = (char *)malloc(sizeof(char) * (strlen(code) + 2));
        memcpy(newCode, code, sizeof(char) * strlen(code));
        newCode[strlen(code)] = '0';
        newCode[strlen(code) + 1] = '\0';
        
        descend(node->fp, newCode);
        newCode[strlen(code)] = '1';

        descend(node->sp, newCode);

        free(newCode);

        return; 
    }

}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Could not open the input file\n");
        return 1;
    }

    int counter = 0;
    char symbol;
    char dictionary[128];
    for (int i = 0; i < 128; i++)
        dictionary[i] = 0;
    
    while ((fscanf(inputFile, "%c", &symbol) != EOF)) {
        dictionary[symbol] += 1;
        if (dictionary[symbol] == 1)
            counter += 1;
    }

    fclose(inputFile);

    struct Node *nodes = (struct Node *)malloc(sizeof(struct Node) * counter);
    int nodesLength = counter;

    pairs = (char ***)malloc(sizeof(char **) * counter);
    for (int i = 0; i < counter; i++)
        pairs[i] = (char **)malloc(sizeof(char *) * 2);
    pairsCounter = 0;

    for (int i = 0; i < 128; i++) {
        if (dictionary[i] != 0) {
            char *newData = (char *)malloc(sizeof(char) * 2);
            newData[0] = i;
            newData[1] = '\0';
            nodes[--counter].data = newData;
            nodes[counter].rate = dictionary[i];
            nodes[counter].fp = NULL;
            nodes[counter].sp = NULL;
        }
    }

    while (nodesLength > 1) {

        struct Node *min = (struct Node *)malloc(sizeof(struct Node));
        struct Node *premin = (struct Node *)malloc(sizeof(struct Node));

        memcpy(min, nodes + 0, sizeof(struct Node));
        memcpy(premin, nodes + 1, sizeof(struct Node));

        for (int i = 0; i < nodesLength; i++) {
            if (nodes[i].rate < min->rate) {
                memcpy(premin, min, sizeof(struct Node));
                memcpy(min, nodes + i, sizeof(struct Node));
            }
        }

        for (int i = 0; i < nodesLength; i++) {
            if (!strcmp(nodes[i].data, min->data) ||
                    !strcmp(nodes[i].data, premin->data)) {
                for (int j = i; j < nodesLength - 1; j++) {
                    memcpy(nodes + j, nodes + j + 1, sizeof(struct Node));
                }
                nodesLength -= 1;
                i -= 1;
            }
        }

        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        char *newData = (char *)malloc(sizeof(char) * 
            (strlen(min->data) + strlen(premin->data) + 1));
        strcat(newData, min->data);
        strcat(newData, premin->data);

        newNode->data = newData;
        newNode->rate = min->rate + premin->rate;

        newNode->fp = min;
        newNode->sp = premin;

        memcpy(nodes + nodesLength, newNode, sizeof(struct Node));
        nodesLength += 1;
        free(newNode);

    }

    char *code = (char *)malloc(sizeof(char) * 1);
    code[0] = '\0';
    descend(nodes, code);

    for (int i = 0; i < pairsCounter; i++) {
        printf("%s = %s\n", pairs[i][0], pairs[i][1]);
    }    

    return 0;
}
