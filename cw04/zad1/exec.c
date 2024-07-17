#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

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

    } else if (strcmp(mode, "mask") == 0) {
        sigemptyset(&sigmask);
        sigaddset(&sigmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &sigmask, NULL);
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

    kill(getpid(), SIGUSR1);
    char *args[] = {"./signal", NULL};
    execvp(args[0], args);

    return 0;
}
