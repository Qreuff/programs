#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int loadstroka(char ***stroka, int maxdlina) { //Загружает строки из файла
    FILE *file = fopen("for1.txt", "r");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        return 0;
    }

    *stroka = (char **)malloc(maxdlina * sizeof(char *));
    if (*stroka == NULL) {
        printf("Ошибка выделения памяти\n");
        fclose(file);
        return 0;
    }

    int count = 0;
    while (count < maxdlina && !feof(file)) {//feof возвращает ненулевое значение, если операция чтения пытается продолжить чтение файла после того, как он закончился; в противном случае возвращается значение 0
        (*stroka)[count] = (char *)malloc(64 * sizeof(char));
        if (fgets((*stroka)[count], 64, file)) {
            (*stroka)[count][strcspn((*stroka)[count], "\n")] = '\0';
            count++;
        } else {
            free((*stroka)[count]); //Освобождаем память если чтение не удалось
        }
    }

    fclose(file);
    return count;
}

void reverse(char *line) { //Переворачивает слова в строке
    char *words[333]; //Массив для хранения слов
    int count = 0;
    char *word = strtok(line, " ");//Разбиваем строку на слова
    while (word != NULL && count < 333) {
        words[count] = word;
        count++;
        word = strtok(NULL, " ");
    }
    for (int i = count - 1; i >= 0; i--) {//Выводим слова в обратном порядке
        printf("%s ", words[i]);
    }
    printf("\n");
}
int main() {
    char **stroka = NULL;
    int maxcount = 100; //Максимальное количество строк
    int countstrok = loadstroka(&stroka, maxcount);

    for (int i = 0; i < countstrok; i++) {
        reverse(stroka[i]);
        free(stroka[i]);
    }
    free(stroka);

    return 0;
}