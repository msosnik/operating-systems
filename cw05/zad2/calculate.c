#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

double func(double x) {
    return 4.0 / (x * x + 1);
}

double integrate(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += func(x);
    }
    return h * sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rectangle_size> <num_processes>\n", argv[0]);
        return 1;
    }
    double h = atof(argv[1]);
    int num_processes = atoi(argv[2]);
    double a = 0.0, b = 1.0;
    double interval_length = b - a;
    int pipes[num_processes][2];

    for (int i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return 1;
        }
        pid_t pid = fork();
        if (pid == 0) {  // child process
            close(pipes[i][0]); // close read end
            double subinterval_length = interval_length / num_processes;
            double subinterval_start = a + i * subinterval_length;
            double subinterval_end = subinterval_start + subinterval_length;
            double result = integrate(subinterval_start, subinterval_end, subinterval_length / h);
            if (write(pipes[i][1], &result, sizeof(result)) < 0) {
                perror("write");
                return 1;
            }
            close(pipes[i][1]); // close write end
            return 0;
        } else if (pid < 0) {  // error
            perror("fork");
            return 1;
        } else {  // parent process
            close(pipes[i][1]); // close write end
        }
    }

    double sum = 0.0;
    for (int i = 0; i < num_processes; i++) {
        double result;
        if (read(pipes[i][0], &result, sizeof(result)) < 0) {
            perror("read");
            return 1;
        }
        sum += result;
        close(pipes[i][0]); // close read end
    }

    printf("Integral value: %.6f\n", sum);

    return 0;
}
