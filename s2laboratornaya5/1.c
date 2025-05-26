#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    char start_point[50];    
    char end_point[50];      
    int route_number; 
} Route;
void create_file(const char* filename, int N) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("☠️☠️☠️\n");
        return;
    }
    Route route;
    for (int i = 0; i < N; i++) {
        printf("Введите информацию о маршруте #%d:\n", i + 1);
        printf("Начальный пункт: ");
        scanf("%s", route.start_point);
        printf("Конечный пункт: ");
        scanf("%s", route.end_point);
        printf("Номер маршрута: ");
        scanf("%d", &route.route_number);
        fwrite(&route, sizeof(Route), 1, file);
    }
    fclose(file);
    printf("Файл создан✅\n");
}
void search_route(const char* filename, int target_number) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("☠️☠️☠️\n");
        return;
    }
    Route route;
    int found = 0;
    printf("\nРезультаты поиска:\n");
    while (fread(&route, sizeof(Route), 1, file)) {
        if (route.route_number == target_number) {
            printf("Начальный пункт: %s\n", route.start_point);
            printf("Конечный пункт: %s\n", route.end_point);
            printf("Номер маршрута: %d\n\n", route.route_number);
            found = 1;
        }
    }
    if (!found) {
        printf("Маршрут с номером %d не найден.☠️☠️☠️\n", target_number);
    }
    fclose(file);
}
int main() {
    const char* filename = "Spravka.dat";
    int N, target_number;
    printf("Введите количество маршрутов для записи в файл: ");
    scanf("%d", &N);//кол-во маршрутов
    create_file(filename, N);
    printf("\nВведите номер маршрута для поиска: ");
    scanf("%d", &target_number);
    search_route(filename, target_number);
    return 0;
}