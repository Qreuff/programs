#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

// Структура для описания прямоугольного параллелепипеда
typedef struct {//объявление структуры struct - структура переменных
    float a;  // Сторона a
    float b;  // Сторона b
    float h;  // Высота h
} pr_parallelepiped;

// Прототипы функций
void inic_parallelepiped(pr_parallelepiped *p);//переменные которые принимают указатель
float find_ob(pr_parallelepiped p);//переменные которые принимают структуру
void print_parallelepiped(pr_parallelepiped p);//переменные которые принимают структуру
int find_parallelepipedob(pr_parallelepiped *parallelepiped, int N);//переменные которые принимают указатель

#endif