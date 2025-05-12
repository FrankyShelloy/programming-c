#include "huffman.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {

    FILE *inputFile = openFile("in.txt", 0);
    int mode = fgetc(inputFile);
    if ((char) mode == 'c') {
        FILE *outputFile = openFile("out.txt", 1);
        encode(inputFile, true, outputFile);
        fclose(inputFile);
    } else if ((char) mode == 'd') {
        FILE *outputFile = openFile("out.txt", 1);
        decode(inputFile, outputFile);
        fclose(outputFile);

    }
    return 0;
}