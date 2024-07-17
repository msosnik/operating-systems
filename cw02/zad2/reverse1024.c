#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *in_file = fopen(argv[1], "r");
    if (!in_file) {
        printf("Error opening file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    FILE *out_file = fopen(argv[2], "w");
    if (!out_file) {
        printf("Error opening file %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    fseek(in_file, 0, SEEK_END);
    long long size = ftell(in_file);

    char buffer[BLOCK_SIZE];
    for (long long i = size - BLOCK_SIZE; i >= 0; i -= BLOCK_SIZE) {
        fseek(in_file, i, SEEK_SET);
        fread(buffer, BLOCK_SIZE, 1, in_file);
        for (int j = BLOCK_SIZE - 1; j >= 0; j--) {
            fputc(buffer[j], out_file);
        }
    }

    if (size % BLOCK_SIZE != 0) {
        int remaining_bytes = size % BLOCK_SIZE;
        fseek(in_file, 0, SEEK_SET);
        fread(buffer, remaining_bytes, 1, in_file);
        for (int j = remaining_bytes - 1; j >= 0; j--) {
            fputc(buffer[j], out_file);
        }
    }

    fclose(in_file);
    fclose(out_file);
    return EXIT_SUCCESS;
}
