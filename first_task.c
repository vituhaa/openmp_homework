#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

// 1. сгенерировать точку
// 2. понять, попадает ли она в окружность +
// 3. если да, то прибавить к локальному счётчику попаданий в окружность +1
// 4. локальный счётчик общего числа точек
// 5. вероятность +

float probability(int all_points, int hit_points){
    return hit_points / all_points;
}

bool hit_rate(float x, float y){
    if ((x - 1) * (x - 1) + (y - 1) * (y - 1) <= 1){
        return true;
    }
    return false;
}

int routine(int my_rank, int block, int tail)
{
    int points = block + (my_rank < tail ? 1 : 0);
    int circle_points_counter = 0;
    for (int i = 0; i < points; i++)
    {
        float x = (float)rand() / RAND_MAX * 2.0f;
        float y = (float)rand() / RAND_MAX * 2.0f;

        if (hit_rate(x, y))
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
    float all_circle_points = 0;
    #pragma omp parallel num_threads(thread_count)
        int my_rank = omp_get_thread_num();
        int thread_circle_points = routine(my_rank, blocks, tail);
        #pragma omp atomic
        all_circle_points += thread_circle_points;
        
    float pi = 4 * all_circle_points / n;
    printf("%f", pi);
    return 0;
}
