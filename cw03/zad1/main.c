#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {

    if (argc !=2) {
        return EXIT_FAILURE;
    }
    int procesCount = atoi(argv[1]);

    for (int i = 0; i < procesCount; i++) {
        int childProces = fork();


//        if (childProces < 0) {
//            perror("UNABLE TO CREATE A PROCESS");
//            exit(EXIT_FAILURE);
//        } else if (childProces == 0) {
//            printf("pid = %d, ppid = %d\n", getpid(), getppid());
//            exit(0);
//        }
//    }

        switch (childProces) {
            case -1:
                perror("UNABLE TO CREATE A PROCESS");
                exit(EXIT_FAILURE);
            case 0:
                printf("%d %d\n", getpid(), getppid());
                exit(EXIT_SUCCESS);
//            default:
//                wait();
//                printf("Proces główny %d, wywołał %d procesów\n", getpid(), procesCount);
//                exit(EXIT_SUCCESS);
        }
    }
    int status;
    pid_t child_pid;
    int num_children = 0;
    while ((child_pid = wait(&status)) != -1) {
        num_children++;
    }

    printf("Parent process id = %d, %d processes created\n", getpid(), num_children);

    return 0;
}