#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void search_dir(char* path, char* str) {
    DIR* dir = opendir(path);

    if (dir == NULL) {
        fprintf(stderr, "Error opening directory %s\n", path);
        exit(1);
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char filepath[PATH_MAX];
        snprintf(filepath, PATH_MAX, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR && strncmp(entry->d_name, ".", 1) != 0) {
            pid_t pid = fork();

            if (pid == -1) {
                fprintf(stderr, "Error forking process\n");
                exit(1);
            } else if (pid == 0) {
                search_dir(filepath, str);
                exit(0);
            } else {
                wait(NULL);
            }
        } else if (entry->d_type == DT_REG && access(filepath, R_OK) == 0) {
            if (strncmp(entry->d_name, str, strlen(str)) == 0) {
                printf("%s (pid=%d)\n", filepath, getpid());
            }
        }
    }

    closedir(dir);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <directory> <string>\n", argv[0]);
        exit(1);
    }

    char* dirpath = argv[1];
    char* str = argv[2];

    search_dir(dirpath, str);

    return 0;
}
