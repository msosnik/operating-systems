//
// Created by michal on 16.03.23.
//
#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s [ASCI code to replace] [ASCI code to replace it with] [input file path] [output file path]\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) != 1) {
        return 1;
    }
    if (strlen(argv[2]) != 1) {
        return 1;
    }
    char old_ASCII = argv[1][0];
    char new_ASCII = argv[2][0];

    int old_ascii = old_ASCII;
    int new_ascii = new_ASCII;
    char *input_path = argv[3];
    char *output_path = argv[4];
//    creat(output_path, "O");

    printf("Znak: %c\to kodzie ASCI: %d\n",old_ascii, old_ascii);
    printf("Znak: %c\to kodzie ASCI: %d\n",new_ascii, new_ascii);
    int input_file = open(input_path, O_RDONLY | S_IRUSR);
    if (input_file<0) {
        printf("Failed to open input file %s\n", input_path);
        return 1;
    }

    int output_file = open(output_path,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    if (output_file<0) {
        printf("Failed to open output file %s\n", output_path);
        return 1;
    }

    char c[1];
    while ((read(input_file, c, 1)) >0) {
        if (c[0] == old_ascii) {
            c[0] = new_ascii;
        }
        write(output_file, c , 1);
    }

    close(input_file);
    close(output_file);

    printf("Replacement complete!\n");
    return 0;
}
