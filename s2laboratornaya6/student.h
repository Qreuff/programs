#ifndef STUDENT_H
#define STUDENT_H

#define N 100
#define MAX_SCORE 300

typedef struct {
    char *name;      // Указатель на имя студента
    int math;        // Балл по математике
    int phy;         // Балл по физике
    int inf;         // Балл по информатике
    int obshiybal;   // Общий балл
} STUDENT;

// Функция создания студента
STUDENT addSTUDENT(const char *name, int math, int phy, int inf);

// Функция вывода информации о студенте
void infabtstdt(STUDENT student);

// Функция загрузки имен из файла
int loadNAME(char ***names, int maxNames);

// Функция сортировки выбором
void selectionSort(STUDENT arr[], int n);

// Функция сортировки подсчетом
void countingSort(STUDENT arr[], int n);

#endif /* STUDENT_H */