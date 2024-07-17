#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include "time.h"
#include "pthread.h"
#include "unistd.h"

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void ignore_handler(int signo, siginfo_t* info, void* context) {}

typedef struct {
    char* src;
    char* dst;
    int start;
} UpdateGridArgs;

void* update_grid_region(void* _args) {
    UpdateGridArgs* args = (UpdateGridArgs*) _args;

    while (1) {
        int i = args->start;
        args->dst[i] = is_alive(i / grid_width, i % grid_width, args->src);

        pause();

        char* tmp = args->src;
        args->src = args->dst;
        args->dst = tmp;
    }

    return NULL;
}

void update_grid_thread(char* src, char* dst) {
    static pthread_t* _threads = NULL;
    static int _n_threads = -1;

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = ignore_handler;
    sigaction(SIGUSR1, &action, NULL);

    if (!_threads) {

        int grid_size = grid_width*grid_height;
        // use one thread for each tile

        _n_threads = grid_size;
        _threads = malloc(sizeof(pthread_t) * _n_threads);


//        int block = n2/n_threads + (n2 % n_threads ? 1 : 0);

        for (int i = 0; i < grid_size; i += 1//block
                ) {

            // ending the program will release this memory, it's executed only once so it's not leaking
            UpdateGridArgs* args = malloc(sizeof(UpdateGridArgs));
            args->src = src;
            args->dst = dst;
            args->start = i;

//                    (i+block+block > n2) ? n2 : i+block;

            pthread_create(
                    _threads+(i),
                    NULL,
                    update_grid_region,
                    (void*) args
            );
        }
    }

    for (int i=0; i < _n_threads; i++) {
        pthread_kill(_threads[i], SIGUSR1);
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}
