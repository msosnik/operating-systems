//
// Created by michal on 12.03.23.
//
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include "time.h"
#include "sys/resource.h"


#define BUFFERSIZE 1000

static clock_t ct_start;
static struct rusage ru_start;

static clock_t ct_stop;
static struct rusage ru_stop;

void timing_start(void){
    ct_start = clock();
    getrusage(RUSAGE_SELF, &(ru_start));
}

void timing_stop(void){
    ct_stop = clock();
    getrusage(RUSAGE_SELF, &(ru_stop));
}

void print_time(void){
    double stdtime;
    double cputime_u;
    double cputime_s;

    stdtime = (double) (ct_stop - ct_start) / CLOCKS_PER_SEC;

    cputime_u = (ru_stop.ru_utime.tv_usec - ru_start.ru_utime.tv_usec) / 1.e6;
    cputime_u += (ru_stop.ru_utime.tv_sec - ru_start.ru_utime.tv_sec);

    cputime_s = (ru_stop.ru_stime.tv_usec - ru_start.ru_stime.tv_usec) / 1.e6;
    cputime_s += (ru_stop.ru_stime.tv_sec - ru_start.ru_stime.tv_sec);

    printf(
            "Real time: %.6lf s, User time: %.6lf s, System time: %.6lf s\n",
            stdtime,
            cputime_u,
            cputime_s);
}


int main ()
{
    blocks_table table = create_table(1);
    char buffer[BUFFERSIZE];
    printf("Write 'exit' or '[Ctr+C]' to exit program:\n");
    fgets(buffer, BUFFERSIZE , stdin);
    char* token = strtok(buffer, " \n");
//    printf( "%s\n", token ); //printing the token

    while (strcmp(token, "exit") !=0)
    {
//        struct tms st_cpu;
//        clock_t st_time;
//        st_time = times(&st_cpu);


        if (strcmp(token, "init") == 0) {

            char* token1 = strtok(NULL, " \n");
            int max_size = atoi(token1);
            printf("Initializing block array with size: %d\n", max_size);
//            printf( "%s\n", max_size ); //printing the token

            timing_start();
            table = create_table(max_size);
            timing_stop();
            print_time();


        } else if (strcmp(token, "count") == 0) {
            char* file_name = strtok(NULL, " \n");
            printf("Counting lines and words in file: %s\n", file_name);
//                printf( "%s\n", file_name ); //printing the token

            timing_start();
            wc_file(&table, file_name);
            timing_stop();
            print_time();


        } else if (strcmp(token, "show") == 0) {
            char* token1 = strtok(NULL, " \n");
//            printf( "%s\n", token1 ); //printing the token
            int index = atoi(token1);

            timing_start();
            char* contents;
            contents = get_block(&table, index);

            timing_stop();

            if (contents == NULL)
            {
                fprintf(stderr, "Error: failed to count lines and words\n");
            }
            else
            {
                printf("Block contents at index %d:\n", index);
                printf("%s", contents);

                print_time();
            }


//            printf("%s", get_block(&table, atoi(token1)));
//            char* block_content = malloc(BUFFERSIZE);
//            strcpy(block_content, get_block(&table, atoi(token1)));
//            printf(block_content);

        } else if (strcmp(token, "delete") == 0) {
            char* token1 = strtok(NULL, " \n");
//            printf("%s\n", token1 ); //printing the token

            int index = atoi(token1);
            printf("Deleting block contents at index %d:\n", index);

            timing_start();
            remove_block(&table, atoi(token1));
            timing_stop();
            print_time();
        } else if (strcmp(token, "destroy") == 0) {
            printf("Deleting array\n");

            timing_start();
            free_table(&table);
            timing_stop();
            print_time();
        } else {
            printf("Wrong command\n");
        }
//        clock_t end = clock();
//        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
//        printf("real time %f\n", seconds);
//        struct tms en_cpu;
//        clock_t en_time;
//        en_time = times(&en_cpu);
//
//        char msg[10000];
//        fputs(msg,stdout);
//        printf("Real Time: %f, User Time %f, System Time %f\n",
//               (double)(en_time - st_time),
//               (double)(en_cpu.tms_utime - st_cpu.tms_utime),
//               (double)(en_cpu.tms_stime - st_cpu.tms_stime));

            // get next command
        fgets(buffer, BUFFERSIZE , stdin);
        token = strtok(buffer, " \n");
        printf("\n");
    }
    return 0;
}
