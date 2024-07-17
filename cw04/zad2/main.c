#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void handler(int signum, siginfo_t *siginfo, void *context) {
    time_t current_time;
    time(&current_time);
    printf("Signal %d received from process %d at %s", signum, siginfo->si_pid, ctime(&current_time));
}

int main() {
    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    while (1) {
        printf("%d Waiting for signal...\n", getpid());
        sleep(1);
    }

    return 0;
}
