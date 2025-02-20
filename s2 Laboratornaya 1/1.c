#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 100000 //количество студентов
#define MAX_SCORE 300 //максимальный общий балл

struct STUDENT {
    char *name;//указатель на имя
    int math;        
    int phy;         
    int inf;         
    int obshiybal;   
};

//Функция для добавления студента
struct STUDENT addSTUDENT(const char *name, int math, int phy, int inf) {
    struct STUDENT newSTUDENT;
    newSTUDENT.name = (char *)malloc(strlen(name) + 1);//выделение памяти для имени
    if (newSTUDENT.name != NULL) {
        strcpy(newSTUDENT.name, name);
    }
    newSTUDENT.math = math;
    newSTUDENT.phy = phy;
    newSTUDENT.inf = inf;
    newSTUDENT.obshiybal = math + phy + inf;
    return newSTUDENT;
}

// Функция для вывода информации о студенте
void infabtstdt(struct STUDENT student) {
    printf("Имя: %s, Математика: %d, Физика: %d, Информатика: %d, Общий балл: %d\n",
           student.name, student.math, student.phy, student.inf, student.obshiybal);
}

// Функция для загрузки имен из файла
int loadNAME(char ***names, int maxNames) {
    FILE *file = fopen("Имена.txt", "r");//открывает файл с именами
    if (file == NULL) {//если файл не удается открыть
        printf("Ошибка\n");
        return 0;
    }

    *names = (char **)malloc(maxNames * sizeof(char *)); //выделение памяти для указателей на строки
    if (*names == NULL) {
        printf("Ошибка при выделении памяти для имен\n");
        fclose(file);
        return 0;
    }

    int count = 0;
    while (count < maxNames && !feof(file)) {//feof - функция которая проверяет конец файла. Она возвращает ненулевое значение (обычно 1), если достигнут конец файла, и 0 в противном случае.
        (*names)[count] = (char *)malloc(64 * sizeof(char)); //выделение памяти для каждого имени
        if (fgets((*names)[count], 64, file)) {
            (*names)[count][strcspn((*names)[count], "\n")] = 0; //убираем символ новой строки
            count++;
        }
    }
    fclose(file); //закрытие файла
    return count; //возвращаем количество загруженных имен
}

//Функция сортировки выбором по убыванию общего балла
void selectionSort(struct STUDENT arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].obshiybal > arr[maxIdx].obshiybal) { //поиск максимума
                maxIdx = j;
            }
        }
        if (maxIdx != i) { //перестановка значений
            struct STUDENT temp = arr[i];
            arr[i] = arr[maxIdx];
            arr[maxIdx] = temp;
        }
    }
}

// Функция сортировки подсчетом (Counting Sort)
void countingSort(struct STUDENT arr[], int n) {
    struct STUDENT output[N];
    int count[MAX_SCORE + 1] = {0};

    for (int i = 0; i < n; i++) {
        count[arr[i].obshiybal]++; //счет частоты баллов
    }

    for (int i = MAX_SCORE - 1; i >= 0; i--) {
        count[i] += count[i + 1]; //счет позиций в отсортированном массиве
    }

    for (int i = 0; i < n; i++) {
        output[count[arr[i].obshiybal] - 1] = arr[i];
        count[arr[i].obshiybal]--;//уменьшаем кол-во на 1
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];//копирование результата в другую переменную
    }
}

int main() {
    struct STUDENT *students = (struct STUDENT *)malloc(N * sizeof(struct STUDENT)); //динамическая память для студентов
    char **names = NULL;
    int namecount = loadNAME(&names, N); //загрузка имен

    if (students == NULL || names == NULL) {
        printf("Ошибка при выделении памяти\n");
        return 1;
    }

    // Генерация студентов с рандомными баллами
    for (int i = 0; i < N; i++) {
        const char *name = (i < namecount) ? names[i] : "нет имени";
        students[i] = addSTUDENT(name, rand() % 100, rand() % 100, rand() % 100);
    }

    printf("Выберите сортировку:\n1 - SelectionSort\n2 - CountingSort\nВаш выбор: ");
    int choice;
    scanf("%d", &choice);

    printf("\nСписок студентов до сортировки:\n");
    for (int i = 0; i < N; i++) {
        infabtstdt(students[i]);
    }

    clock_t start = clock();//запуск таймера
    
    if (choice == 1) {
        selectionSort(students, N);
    } else if (choice == 2) {
        countingSort(students, N);
    } else {
        printf("Неверный выбор.\n");
        return 1;
    }
    
    clock_t end = clock(); //остановка таймера
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;//перевод разницы в секунды

    printf("\nСписок студентов после сортировки:\n");
    for (int i = 0; i < N; i++) {
        infabtstdt(students[i]);
    }
    
    printf("\nВремя сортировки: %f секунд\n", time_spent);
    printf("CPU: \n");
    system("sysctl -n machdep.cpu.brand_string");
    system("sysctl -n hw.cpufrequency");
    size_t weight = N * sizeof(struct STUDENT); //размер массива студентов
    printf("Размер массива: %zu байт (%f КБ)\n", weight, weight / 1024.0);

    //освобождение памяти
    for (int i = 0; i < namecount; i++) {
        free(names[i]);  //освобождение памяти для каждого имени
    }
    free(names);  //освобождение памяти для массива имен
    for (int i = 0; i < N; i++) {
        free(students[i].name);  //освобождение памяти для имени студента
    }
    free(students);  //освобождение памяти для массива студентов
    
    return 0;
}