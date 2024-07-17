#include <stdio.h>
#include <string.h>

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

    printf("Znak: %c\to kodzie ASCI: %d\n",old_ascii, old_ascii);
    printf("Znak: %c\to kodzie ASCI: %d\n",new_ascii, new_ascii);
    FILE *input_file = fopen(input_path, "rb");
    if (!input_file) {
        printf("Failed to open input file %s\n", input_path);
        return 1;
    }

    FILE *output_file = fopen(output_path, "wb");
    if (!output_file) {
        printf("Failed to open output file %s\n", output_path);
        return 1;
    }

    int c;
    while ((c = fgetc(input_file)) != EOF) {
        if (c == old_ascii) {
            c = new_ascii;
        }
        fputc(c, output_file);
    }

    fclose(input_file);
    fclose(output_file);

    printf("Replacement complete!\n");
    return 0;
}
