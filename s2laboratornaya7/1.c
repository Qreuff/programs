#include <pthread.h>//для работы с потоками
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// для sleep
#include <string.h>
#include <signal.h>//сигналы для ос
#include <stdbool.h>
#include <time.h>

#define MAX_THREADS 4
#define MAX_SLEEPSORT 50

pthread_mutex_t sync_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0;

typedef struct {
    char** lines;
    int count;
} ThreadData;

void cleanup_handler(void* arg) {
    printf("Поток %lu завершён\n", (unsigned long)pthread_self());
}

void* print_lines(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    pthread_cleanup_push(cleanup_handler, NULL);

    for (int i = 0; i < data->count; ++i) {
        sleep(1);
        printf("Поток %lu: %s\n", (unsigned long)pthread_self(), data->lines[i]);
        
        pthread_testcancel();
    }

    pthread_cleanup_pop(1);
    return NULL;
}

void* sleepsort_func(void* arg) {
    int value = *(int*)arg;
    struct timespec ts = {
        .tv_sec = value / 1000,
        .tv_nsec = (value % 1000) * 1000000
    };
    nanosleep(&ts, NULL);
    printf("%d ", value);
    free(arg);
    return NULL;
}

void* synchronized_output_child(void* arg) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&sync_mutex);
        while (turn != 1) {
            pthread_cond_wait(&cond, &sync_mutex);
        }

        printf("Дочерний поток: строка %d\n", i + 1);
        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&sync_mutex);
    }
    return NULL;
}

void* task1_child(void* arg) {
    // Ждем завершения родительского потока
    sleep(1);
    for (int i = 0; i < 5; ++i) {
        printf("Дочерний поток: строка %d\n", i + 1);
    }
    return NULL;
}

int main() {
    pthread_t thread1;

    printf("Создание потока\n");

    // Задача 1: Простое создание потока
    pthread_create(&thread1, NULL, task1_child, NULL);
    for (int i = 0; i < 5; ++i) {
        printf("Родительский поток: строка %d\n", i + 1);
    }
    pthread_join(thread1, NULL);

    printf("\nОжидание потока\n");
    pthread_create(&thread1, NULL, task1_child, NULL);
    for (int i = 0; i < 5; ++i) {
        printf("Родитель: строка %d\n", i + 1);
    }
    pthread_join(thread1, NULL);

    printf("\nПараметры потока\n");

    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];

    char* texts[4][3] = {
        {"A1", "A2", "A3"},
        {"B1", "B2", "B3"},
        {"C1", "C2", "C3"},
        {"D1", "D2", "D3"},
    };

    for (int i = 0; i < MAX_THREADS; ++i) {
        threadData[i].lines = texts[i];
        threadData[i].count = 3;
        if (pthread_create(&threads[i], NULL, print_lines, &threadData[i]) != 0) {
            perror("Ошибка создания потока");
            exit(EXIT_FAILURE);
        }
    }

    // Сначала выводим родительский поток
    printf("Родительский поток: ожидание дочерних потоков\n");
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("\nЗавершение нити без ожидания\n");

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, print_lines, &threadData[i]) != 0) {
            perror("Ошибка создания потока");
            exit(EXIT_FAILURE);
        }
    }

    printf("Родительский поток: отмена дочерних потоков\n");
    sleep(2);
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("\nSleepsort\n");

    int arr[] = {30, 10, 50, 20, 40, 15, 25, 35, 45, 5, 0};
    int n = sizeof(arr) / sizeof(arr[0]);
    pthread_t sleep_threads[n];

    for (int i = 0; i < n; ++i) {
        int* val = malloc(sizeof(int));
        if (val == NULL) {
            perror("Ошибка выделения памяти");
            exit(EXIT_FAILURE);
        }
        *val = arr[i];
        if (pthread_create(&sleep_threads[i], NULL, sleepsort_func, val) != 0) {
            free(val);
            perror("Ошибка создания потока");
            exit(EXIT_FAILURE);
        }
    }

    printf("Родительский поток: ожидание завершения sleepsort\n");
    for (int i = 0; i < n; ++i) {
        pthread_join(sleep_threads[i], NULL);
    }
    printf("\n");

    printf("\nСинхронизированный вывод\n");

    pthread_t sync_thread;
    if (pthread_create(&sync_thread, NULL, synchronized_output_child, NULL) != 0) {
        perror("Ошибка создания потока");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&sync_mutex);
        printf("Родительский поток: строка %d\n", i + 1);
        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&sync_mutex);

        // Даем время дочернему потоку выполниться
        usleep(10000);
    }

    pthread_join(sync_thread, NULL);

    pthread_mutex_destroy(&sync_mutex);
    pthread_cond_destroy(&cond);

    return 0;
}