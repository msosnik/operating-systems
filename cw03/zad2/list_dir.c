#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf( "%s\n", argv[0]);
//    printf("Directory name: %s\n", argv[1]);

    if (execl("/bin/ls", "ls", argv[1], (char*)NULL) < 0) {
        fprintf(stderr, "execl failed\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
