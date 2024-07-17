#include <stdio.h>
#include <stdlib.h>

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

    for (long long i = size - 1; i >= 0; i--) {
        fseek(in_file, i, SEEK_SET);
        char c = fgetc(in_file);
        fputc(c, out_file);
    }

    fclose(in_file);
    fclose(out_file);
    return EXIT_SUCCESS;
}
