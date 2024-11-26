#include <stdio.h>
void fillSpiralMatrix(int n, int matrix[n][n]) {//функция которая будет заполнять матрицу спиральным образом
    int value = 1;
    int top = 0, bot = n - 1, left = 0, right = n - 1;//границы спирали
    while (value <= n * n) { //цикл будет работать пока не заполнится вся матрица
        for (int i = left; i <= right; i++) {
            matrix[top][i] = value++;//заполнение ячейки увеличивая ее значение
        }
        top++;//спускаемся вниз
        for (int i = top; i <= bot; i++) {
            matrix[i][right] = value++;//заполнение ячейки увеличивая ее значение
        }
        right--;//сдвигаем правую границу влево
        for (int i = right; i >= left; i--) {
            matrix[bot][i] = value++;//заполнение ячейки увеличивая ее значение
        }
        bot--;//сдвигаем нижнюю границу вверх
        for (int i = bot; i >= top; i--) {
            matrix[i][left] = value++;//заполнение ячейки увеличивая ее значение
        }
        left++;//сдвигаем левую границу вправо
    }
}
void printMatrix(int n, int matrix[n][n]) {//функция которая будет печатать матрицу на экран 
    for (int i = 0; i < n; i++) {//по строкам матрицы
        for (int j = 0; j < n; j++) {//по столбцам матрицы
            printf("%4d", matrix[i][j]);//печать матрицы
        }
        printf("\n");
    }
}
int main() {
    int n;//размер матрицы
    printf("Введите размер матрицы: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Размер матрицы должен быть положительным.\n");
        return 1;
    }
    int matrix[n][n];
    fillSpiralMatrix(n, matrix);
    printMatrix(n, matrix);
    return 0;
}