#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <sys/stat.h>

long long total_size = 0;

int print_file_info(const char *path, const struct stat *stat_buf, int type) {
    if (type == FTW_F) {
        printf("File name: %s, Size: %lld bytes\n", path, (long long) stat_buf->st_size);
        total_size += stat_buf->st_size;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s directory\n", argv[0]);
        return EXIT_FAILURE;
    }

//    int flags = FTW_PHYS;
    if (ftw(argv[1], print_file_info, 20) == -1) {
        perror("Error traversing directory tree");
        return EXIT_FAILURE;
    }

    printf("Total size of all files: %lld bytes\n", total_size);
    return EXIT_SUCCESS;
}
