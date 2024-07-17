#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE 8192

static char alternate_stack[STACK_SIZE];

void handler(int sig) {
    printf("Signal %d received\n", sig);
    printf("Signal handler completed\n");
}

int main() {
    struct sigaction sa;

    // set up the alternate signal stack
    stack_t ss;
    ss.ss_sp = alternate_stack;
    ss.ss_size = sizeof(alternate_stack);
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) == -1) {
        perror("sigaltstack");
        exit(EXIT_FAILURE);
    }

    // set up the signal handler with SA_ONSTACK and SA_NODEFER
    sa.sa_handler = handler;
    sa.sa_flags = SA_ONSTACK | SA_NODEFER;
    if (sigemptyset(&sa.sa_mask) == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // wait for signals in a loop
    while (1) {
        printf("%d Waiting for signal...\n", getpid());
        sleep(1);
    }

    return 0;
}
