#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

// 1. сгенерировать точку
// 2. понять, попадает ли она в окружность +
// 3. если да, то прибавить к локальному счётчику попаданий в окружность +1
// 4. локальный счётчик общего числа точек

int hit_rate(double x, double y){
    if ((x - 1) * (x - 1) + (y - 1) * (y - 1) <= 1){
        return 1;
    }
    return 0;
}

int routine(int my_rank, int blocks, int tail)
{
    int points = blocks + (my_rank < tail ? 1 : 0);
    int circle_points_counter = 0;
    for (int i = 0; i < points; i++)
    {
        double x = (double)rand() / RAND_MAX * 2.0;
        double y = (double)rand() / RAND_MAX * 2.0;
	    
        if (hit_rate(x, y) == 1)
        {
            circle_points_counter += 1;
        }
    }
    return circle_points_counter;
}

int main(int argc, char** argv){
    if (argc != 3)
    {
        printf("Error: wrong number of arguments\n");
        return 1;
    }
    int n = strtol(argv[1], NULL, 10);
    int thread_count = strtol(argv[2], NULL, 10);

    int blocks = n / thread_count;
    int tail = n % thread_count;

    srand(time(NULL));

    // прагмы параллельные
    // у каждого потока свой локальный счётчик будет объявлен
    // функция будет знать про этот счётчик
    int all_circle_points = 0;
    
    #pragma omp parallel num_threads(thread_count)
    {
        int my_rank = omp_get_thread_num();

        int thread_circle_points = routine(my_rank, blocks, tail);

        #pragma omp atomic
        all_circle_points += thread_circle_points;
    }
    double pi = 4.0 * (double)all_circle_points / (double)n;
    printf("%f\n", pi);
    return 0;
}