#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Node {
    int data;
    struct Node *next;
    struct Node *down;
};

// Функция создания нового узла
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    newNode->down = NULL;
    return newNode;
}

// Функция добавления узла в конец списка
void appendNode(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct Node* temp = *head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Функция создания списка из ввода пользователя
struct Node* createList() {
    struct Node* head = NULL;
    int data;
    printf("Введите числа (0 для завершения): ");
    while (1) {
        scanf("%d", &data);
        if (data == 0) break;
        appendNode(&head, data);
    }
    return head;
}

// Функция вывода списка
void printList(struct Node* head) {
    while (head) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// Функция связывания двух списков (учитывает разную длину)
void linkLists(struct Node* upper, struct Node* lower) {
    while (upper && lower) {
        upper->down = lower;
        upper = upper->next;
        lower = lower->next;
    }
}

// Функция навигации по связанным спискам
void navigate(struct Node* start) {
    struct Node* current = start;
    char command;

    printf("Навигация: W/S - вверх/вниз, A/D - влево/вправо\n");
    while (1) {
        printf("Текущий узел: %d\n", current->data);
        printf("Введите команду: ");
        scanf(" %c", &command);
        command = toupper(command);

        if (command == 'b') break;
        else if (command == 'D') {
            if (current->next) current = current->next;
            else printf("Движение невозможно!\n");
        }
        else if (command == 'A') {
            printf("Нельзя идти влево!\n");
        }
        else if (command == 'S') {
            if (current->down) current = current->down;
            else printf("Движение вниз невозможно!\n");
        }
        else if (command == 'W') {
            printf("Нельзя идти вверх!\n");
        }
        else {
            printf("Неизвестная команда!\n");
        }
    }
}

// Главная функция/
int main() {
    printf("Создание первого списка:\n");
    struct Node* upperList = createList();
    printList(upperList);

    printf("\nСоздание второго списка:\n");
    struct Node* lowerList = createList();
    printList(lowerList);

    // Связываем списки
    linkLists(upperList, lowerList);

    // Навигация по списку
    printf("\nНавигация по списку:\n");
    navigate(upperList);

    return 0;
}