#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 4 // размер строк
#define COLS 4 // размер столбцов

void printMatrix(int matrix[ROWS][COLS]) { // функция печатания матрицы
    for (int i = 0; i < ROWS; i++) { // по строкам
        for (int j = 0; j < COLS; j++) { // по столбцам
            printf("%d ", matrix[i][j]); // печать матрицы
        }
        printf("\n");
    }
}

// Функция для транспонирования квадрата
void transposedSquare(int matrix[ROWS][COLS], int result[ROWS][COLS], int maxRow, int maxCol, int size) {
    for (int i = 0; i < size; i++) { // по строкам
        for (int j = 0; j < size; j++) { // по столбцам
            result[j + maxRow][i + maxCol] = matrix[maxRow + i][maxCol + j]; // транспонирование квадрата
        }
    }
}

int main() { // основная программа
    srand(time(NULL)); // инициализация генератора случайных чисел

    int matrix[ROWS][COLS]; // матрица
    int transposed[ROWS][COLS] = {0}; // матрица для хранения транспонированного квадрата

    for (int i = 0; i < ROWS; i++) { // по строкам
        for (int j = 0; j < COLS; j++) { // по столбцам
            matrix[i][j] = rand() % 10; // генерация рандомного числа от 0-9
        }
    }

    printf("Исходная матрица:\n"); // печать матрицы
    printMatrix(matrix);

    int maxElement = matrix[0][0]; // присваиваем макс элемент на начальное место
    int maxRow = 0, maxCol = 0; // переменные для строк и столбцов максимального элемента

    for (int i = 0; i < ROWS; i++) { // по строкам
        for (int j = 0; j < COLS; j++) { // по столбцам
            if (matrix[i][j] > maxElement) { // если элемент из матрицы больше чем максимальный
                maxElement = matrix[i][j]; // то присваиваем ему максимальный элемент
                maxRow = i; // максимальное значение по строке
                maxCol = j; // максимальное значение по столбцу
            }
        }
    }

    printf("Максимальный элемент: %d на позиции (%d, %d)\n", maxElement, maxRow, maxCol); // печать максимального элемента
    // ? - если условие удовл то выполнится то что сразу после знака
    // : - когда не удовл условию

    int size = (ROWS - maxRow < COLS - maxCol) ? (ROWS - maxRow) : (COLS - maxCol); // определение размера квадрата по местоположению максимального числа
    
    transposedSquare(matrix, transposed, maxRow, maxCol, size); // вызов функции транспонирования

    printf("Транспонированный квадрат:\n"); // печать квадрата
    printMatrix(transposed); // печать транспонированной матрицы

    double meanOriginal = 0.0; // переменная для хранения среднего арифметического матрицы
    double meanTransposed = 0.0; // переменная для хранения среднего арифметического квадрата

    for (int i = 0; i < ROWS; i++) { // по строкам 
        for (int j = 0; j < COLS; j++) { // по столбцам
            meanOriginal += matrix[i][j]; // суммирование всех элементов матрицы
        }
    }
    meanOriginal /= (ROWS * COLS); // общее что получилось делим на кол-во и получаем среднее арифметическое

    for (int i = 0; i < size; i++) { // по строкам транспонированного квадрата
            for (int j = 0; j < size; j++) { // по столбцам транспонированного квадрата
            meanTransposed += transposed[maxRow + i][maxCol + j]; // суммирование всех элементов транспонированного квадрата
        }
    }
    meanTransposed /= (size * size); // вычисление среднего арифметического у транспонированного квадрата

    printf("Среднее арифметическое исходной матрицы: %.2f\n", meanOriginal); // печать среднего арифметического матрицы
    printf("Среднее арифметическое транспонированного квадрата: %.2f\n", meanTransposed); // печать среднего арифметического транспонированного квадрата

    float scalar; // переменная для скаляра который заменит 0 в матрице
    printf("Введите скаляр для замены нулей в матрице: "); // ввод
    scanf("%f", &scalar); // чтение переменной

    for (int i = 0; i < ROWS; i++) { // по строкам
        for (int j = 0; j < COLS; j++) { // по столбцам
            if (matrix[i][j] == 0) { // если элемент == 0
                matrix[i][j] = (int)scalar; // замена 0 на число введенное с клавиатуры
            }
        }
    }

    printf("Матрица после замены нулей:\n"); // печать финальной матрицы
    printMatrix(matrix);

    return 0; // завершение функции
}