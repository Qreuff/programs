#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
STUDENT addSTUDENT(const char *name, int math, int phy, int inf) {
    STUDENT newSTUDENT;
    newSTUDENT.name = (char *)malloc(strlen(name) + 1);
    if (newSTUDENT.name != NULL) {
        strcpy(newSTUDENT.name, name);
    }
    newSTUDENT.math = math;
    newSTUDENT.phy = phy;
    newSTUDENT.inf = inf;
    newSTUDENT.obshiybal = math + phy + inf;
    return newSTUDENT;
}
void infabtstdt(STUDENT student) {
    printf("Имя: %s, Математика: %d, Физика: %d, Информатика: %d, Общий балл: %d\n",
           student.name, student.math, student.phy, student.inf, student.obshiybal);
}
int loadNAME(char ***names, int maxNames) {
    FILE *file = fopen("/Users/mac/Documents/1/s2laboratornaya6/Имена.txt", "r");
    if (file == NULL) {
        printf("Ошибка\n");
        return 0;
    }
    *names = (char **)malloc(maxNames * sizeof(char *));
    if (*names == NULL) {
        printf("Ошибка при выделении памяти для имен\n");
        fclose(file);
        return 0;
    }
    int count = 0;
    while (count < maxNames && !feof(file)) {
        (*names)[count] = (char *)malloc(64 * sizeof(char));
        if (fgets((*names)[count], 64, file)) {
            (*names)[count][strcspn((*names)[count], "\n")] = 0;
            count++;
        }
    }
    fclose(file);
    return count;
}
void selectionSort(STUDENT arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].obshiybal > arr[maxIdx].obshiybal) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            STUDENT temp = arr[i];
            arr[i] = arr[maxIdx];
            arr[maxIdx] = temp;
        }
    }
}
void countingSort(STUDENT arr[], int n) {
    STUDENT output[N];
    int count[MAX_SCORE + 1] = {0};
    for (int i = 0; i < n; i++) {
        count[arr[i].obshiybal]++;
    }
    for (int i = MAX_SCORE - 1; i >= 0; i--) {
        count[i] += count[i + 1];
    }
    for (int i = 0; i < n; i++) {
        output[count[arr[i].obshiybal] - 1] = arr[i];
        count[arr[i].obshiybal]--;
    }
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}