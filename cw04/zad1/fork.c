#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void handle_signal(int sig) {
    printf("Received signal %d\n", sig);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mode>\n", argv[0]);
        exit(1);
    }

    char *mode = argv[1];
    sigset_t sigmask;

    if (strcmp(mode, "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
    } else if (strcmp(mode, "handler") == 0) {
        signal(SIGUSR1, handle_signal);
    } else if (strcmp(mode, "mask") == 0) {
        sigemptyset(&sigmask);
        sigaddset(&sigmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &sigmask, NULL);
//        sigpending(&sigmask);
        if (sigismember(&sigmask, SIGUSR1)) {
            printf("Signal is still visible in mask\n");
        }
        signal(SIGUSR1, handle_signal);
    } else if (strcmp(mode, "pending") == 0) {
        sigemptyset(&sigmask);
        sigaddset(&sigmask, SIGUSR1);
        sigpending(&sigmask);
        if (sigismember(&sigmask, SIGUSR1)) {
            printf("Signal is visible in pending\n");
        }
        signal(SIGUSR1, handle_signal);
    } else {
        fprintf(stderr, "Invalid mode\n");
        exit(1);
    }

    raise(SIGUSR1);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        if (strcmp(mode, "pending") == 0) {
            sigemptyset(&sigmask);
            sigaddset(&sigmask, SIGUSR1);
            sigpending(&sigmask);
            if (sigismember(&sigmask, SIGUSR1)) {
                printf("Signal is still visible in child\n");
            }
        } else {
            raise(SIGUSR1);
        }
        exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}
