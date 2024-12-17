#include <stdio.h>
#include "parallelepiped.h"

// Функция для инициализации параллелепипеда
void inic_parallelepiped(pr_par *p) {
    printf("Введите сторону a: ");
    scanf("%f", &p->a);
    printf("Введите сторону b: ");
    scanf("%f", &p->b);
    printf("Введите высоту h: ");
    scanf("%f", &p->h);
}

// Функция для вычисления объема параллелепипеда
float find_ob(pr_par p) {
    return p.a * p.b * p.h;//обращение к элементам структуры
}

// Функция для отображения структуры в виде ASCII-графики
void print_parallelepiped(pr_par p) {
    printf("Прямоугольный параллелепипед:\n");
    printf("Размеры основания: %.2f x %.2f\n", p.a, p.b);//обращение к элементам структуры
    printf("Высота: %.2f\n", p.h);
    
    // Отображение верхней части
    for (int i = 0; i < p.b; i++) {
        printf("-");
    }
    printf("\n");

    // Отображение боков
    for (int i = 0; i < p.h; i++) {
        printf("|");
        for (int j = 0; j < p.b - 1; j++) {
            printf(" ");
        }
        printf("|\n");
    }

    // Отображение нижней части
    for (int i = 0; i < p.b; i++) {
        printf("-");
    }
    printf("\n");
}

// Функция для нахождения параллелепипеда с наименьшим объемом
int find_parallelepiped(pr_par *parallelepiped, int N) {
    int minIndex = 0;
    float minVolume = find_ob(parallelepiped[0]);

    for (int i = 1; i < N; i++) {
        float volume = find_ob(parallelepiped[i]);//переменнaя которая принимает структуру
        if (volume < minVolume) {
            minVolume = volume;
            minIndex = i;
        }
    }
    return minIndex;
}


