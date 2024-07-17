#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

int main(void) {
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    long long total_size = 0;

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                stat(ent->d_name, &st);
                printf("File name: %s, Size: %ld bytes\n", ent->d_name, st.st_size);
                total_size += st.st_size;
            }
        }
        closedir(dir);
    } else {
        perror("");
        return EXIT_FAILURE;
    }

    printf("Total size of all files in directory: %lld bytes\n", total_size);
    return EXIT_SUCCESS;
}
