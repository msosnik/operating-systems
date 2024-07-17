#ifndef LAB1_LIBRARY_H
#define LAB1_LIBRARY_H

#include <bits/types/FILE.h>


typedef struct
{
    char** blocks;
    int max_size;
    int current_size;
} blocks_table;


blocks_table create_table(int no_blocks);
void wc_file(blocks_table* table, char* paths);
char* get_block(blocks_table* table, int index);
void remove_block(blocks_table* table, int index);
void free_table(blocks_table* table);



#endif //LAB1_LIBRARY_H

