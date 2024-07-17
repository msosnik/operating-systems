#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

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

    double results[num_processes];
    struct timeval start_time[num_processes];
    struct timeval end_time[num_processes];


    for (int i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return 1;
        }
        pid_t pid = fork();
        if (pid == 0) {  // child process
            gettimeofday(&start_time[i], NULL);

            close(pipes[i][0]); // close read end
            double subinterval_length = interval_length / num_processes;
            double subinterval_start = a + i * subinterval_length;
            double subinterval_end = subinterval_start + subinterval_length;
            double result = integrate(subinterval_start, subinterval_end, subinterval_length / h);
            if (write(pipes[i][1], &result, sizeof(result)) < 0) {
                perror("write");
                return 1;
            }

            gettimeofday(&end_time[i], NULL);
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
        results[i] = result;
        close(pipes[i][0]); // close read end
    }

    printf("Integral value: %.6f\n", sum);
    FILE *report_file = fopen("report.txt", "w");
    if (report_file == NULL) {
        perror("fopen");
        return 1;
    }

    fprintf(report_file, "Execution times, results:\n");
    for (int i = 0; i < num_processes; i++) {
        double elapsed_time = (end_time[i].tv_sec - start_time[i].tv_sec) +
                              (end_time[i].tv_usec - start_time[i].tv_usec) / 1000000.0;
        fprintf(report_file, "Process %d: %f seconds\t%f\n", i, elapsed_time, results[i]);
    }

    fclose(report_file);
    printf("Results and execution times saved to report.txt\n");

    return 0;
}
