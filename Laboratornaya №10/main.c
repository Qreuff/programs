#include <stdio.h>
#include "parallelepiped.h"

int main() {
    int N;
    printf("Введите количество параллелепипедов: ");
    scanf("%d", &N);

    // Массив для хранения N параллелепипедов
    pr_par parallelepiped[N];//переменные которые принимают структуру

    // Ввод данных для N параллелепипедов
    for (int i = 0; i < N; i++) {
        printf("Введите данные для параллелепипеда %d:\n", i + 1);
        inic_parallelepiped(&parallelepiped[i]);//переменные которые принимают структуру
    }

    // Поиск параллелепипеда с наименьшим объемом
    int minIndex = find_parallelepiped(parallelepiped, N);//переменные которые принимают структуру
    printf("Параллелепипед с наименьшим объемом: %d\n", minIndex + 1);

    // Вывод структуры
    input_parallelepiped(parallelepiped[minIndex]);//переменные которые принимают структуру
    
    return 0;
}