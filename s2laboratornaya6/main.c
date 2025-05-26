#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    STUDENT *students = (STUDENT *)malloc(N * sizeof(STUDENT));
    char **names = NULL;
    int namecount = loadNAME(&names, N);
    if (students == NULL || names == NULL) {
        printf("Ошибка при выделении памяти\n");
        return 1;
    }
    srand(time(0));
    for (int i = 0; i < N; i++) {
        const char *name = (i < namecount) ? names[i] : "нет имени";
        students[i] = addSTUDENT(name, rand() % 100, rand() % 100, rand() % 100);
    }
    printf("Выберите сортировку:\n1 - SelectionSort\n2 - CountingSort\nВаш выбор: ");
    int choice;
    scanf("%d", &choice);
    clock_t start = clock();
    if (choice == 1) {
        selectionSort(students, N);
    } else if (choice == 2) {
        countingSort(students, N);
    } else {
        printf("Неверный выбор.\n");
        return 1;
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nВремя сортировки: %f секунд\n", time_spent);
    for (int i = 0; i < namecount; i++) {
        free(names[i]);
    }
    free(names);
    for (int i = 0; i < N; i++) {
        free(students[i].name);
    }
    free(students);
    return 0;
}