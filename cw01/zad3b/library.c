#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

blocks_table create_table(int no_blocks) {
    blocks_table table = {
            calloc(no_blocks, sizeof(char*)),
            no_blocks,
            0
    };

    return table;
}

int get_free_index(blocks_table* table) {
    return table->current_size;
}

long get_file_size(FILE* fp) {
    int no_chars = 0;
    while(fgetc(fp) != EOF) {
        no_chars++;
    }
    rewind(fp);
    return no_chars;
}

void wc_file(blocks_table* table, char* path) {

    int index = get_free_index(table);
    if (index>= table->max_size) {
        printf("maximum size exceeded");
        return;
    }

//    char* outputFileName = malloc(L_tmpnam);
//    tmpnam(outputFileName);
    char outputFileName[] = "/tmp/wc_file_XXXXXX";
    int outputfile = mkstemp(outputFileName);
    if (outputfile == 0) {
        printf("failed to create a temporary file");
        return;
    }

    char* command = malloc(50+ strlen(outputFileName)+ strlen(path));
    strcpy(command, "wc ");
//    strcpy(command+4, outputFileName);
//    strcpy(command+4+ strlen(outputFileName), " ");
//    strcpy(command+5+ strlen(outputFileName), path);

    strcat(command, path);
    strcat(command, " >");
    strcat(command, outputFileName);

    system(command);
    free(command);


    FILE *outputFileP  = fopen(outputFileName, "r");
    long size = get_file_size(outputFileP);

    char* block = calloc(size, sizeof(char));
    fread(block, sizeof(char), size, outputFileP);
    table->blocks[index] = block;

    table->current_size++;

    fclose(outputFileP);
    remove(outputFileName);

}

char* get_block(blocks_table* table, int index) {
    if (index < 0 || index >= table->current_size) {
        printf("wrong index! not 0 <= %d < %d\n", index, table->current_size);
        return "";
    }
    char* block = table->blocks[index];
    return block;
}

void remove_block(blocks_table* table, int index) {
    if (index < 0 || index >= table->current_size) {
        printf("wrong index! not 0 <= %d < %d\n", index, table->current_size); return;
    }
    // free block
    free(table->blocks[index]);
    // shift next blocks by one
    for (int i = index + 1; i < table->current_size; i++) {
        table->blocks[i-1] = table->blocks[i];
    }

    table->current_size--;
}

void free_table(blocks_table* table) {
    for(int i =0; i<table->current_size; i++) {
        free(table->blocks[i]);
    }
    table->current_size = 0;
    table->max_size = 0;
    free(table->blocks);

}

