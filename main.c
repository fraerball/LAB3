#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--generate") == 0) {
            printf("Генерация\n");
        }
        else if (strcmp(argv[1], "--sort") == 0) {
            printf("Сортировка\n");
        }
        else if (strcmp(argv[1], "--print") == 0) {
            printf("Вывод таблицы\n");
        }
        else {
            printf("Ошибка: %s\n", argv[1]);
        }
    }
    return 0;
}