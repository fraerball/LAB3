#include "io.h"
#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static char** publications = NULL;
static int publications_quantity = 0;
static char** journals = NULL; 
static int journals_quantity = 0;
static char** initials = NULL;
static int initials_quantity = 0;

static char* copy_string(const char* str) {
    if (!str) return NULL;
    char* new_str = malloc(strlen(str) + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

static char** read_lines_from_file(const char* filename, int* quantity) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл %s\n", filename);
        return NULL;
    }
    
    char** lines = NULL;
    int capacity = 10;
    int size = 0;
    char line[256];
    
    lines = malloc(capacity * sizeof(char*));
    if (!lines) {
        fclose(file);
        return NULL;
    }
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, "\r")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (size >= capacity) {
            capacity *= 2;
            char** new_lines = realloc(lines, capacity * sizeof(char*));
            if (!new_lines) {
                for (int i = 0; i < size; i++) free(lines[i]);
                free(lines);
                fclose(file);
                return NULL;
            }
            lines = new_lines;
        }
        
        lines[size] = copy_string(line);
        if (!lines[size]) {
            for (int i = 0; i < size; i++) free(lines[i]);
            free(lines);
            fclose(file);
            return NULL;
        }
        
        size++;
    }
    
    fclose(file);
    
    if (size == 0) {
        fprintf(stderr, "Ошибка: В файле %s не найдено строк\n", filename);
        free(lines);
        return NULL;
    }
    
    *quantity = size;
    return lines;
}

static int init_data_from_files(const char* words_file, const char* initials_file) {
    const char* w_file = words_file ? words_file : "words.txt";
    const char* i_file = initials_file ? initials_file : "initials.txt";
    
    int total_words = 0;
    char** all_words = read_lines_from_file(w_file, &total_words);
    if (!all_words || total_words == 0) {
        fprintf(stderr, "Ошибка: Не удалось загрузить слова из %s\n", w_file);
        return 0;
    }
    
    int half_quantity = total_words / 2;
    
    publications = malloc(half_quantity * sizeof(char*));
    if (!publications) {
        for (int i = 0; i < total_words; i++) free(all_words[i]);
        free(all_words);
        return 0;
    }
    
    for (int i = 0; i < half_quantity; i++) {
        publications[i] = all_words[i];
        all_words[i] = NULL;
    }
    publications_quantity = half_quantity;
    
    journals_quantity = total_words - half_quantity;
    if (journals_quantity > 0) {
        journals = malloc(journals_quantity * sizeof(char*));
        if (!journals) {
            for (int i = 0; i < publications_quantity; i++) free(publications[i]);
            free(publications);
            for (int i = half_quantity; i < total_words; i++) free(all_words[i]);
            free(all_words);
            return 0;
        }
        
        for (int i = 0; i < journals_quantity; i++) {
            journals[i] = all_words[half_quantity + i];
            all_words[half_quantity + i] = NULL;
        }
    }
    
    free(all_words);
    
    initials = read_lines_from_file(i_file, &initials_quantity);
    if (!initials || initials_quantity == 0) {
        fprintf(stderr, "Ошибка: Не удалось загрузить инициалы из %s\n", i_file);
        return 0;
    }
    
    return 1;
}

char** read_surnames_from_file(const char* filename, int* quantity) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл фамилий %s\n", filename);
        return NULL;
    }
    
    char** surnames = NULL;
    int capacity = 10;
    int size = 0;
    char line[256];
    
    surnames = malloc(capacity * sizeof(char*));
    if (!surnames) {
        fclose(file);
        return NULL;
    }
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, "\r")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (size >= capacity) {
            capacity *= 2;
            char** new_surnames = realloc(surnames, capacity * sizeof(char*));
            if (!new_surnames) {
                free_surnames_array(surnames, size);
                fclose(file);
                return NULL;
            }
            surnames = new_surnames;
        }
        
        surnames[size] = copy_string(line);
        if (!surnames[size]) {
            free_surnames_array(surnames, size);
            fclose(file);
            return NULL;
        }
        
        size++;
    }
    
    fclose(file);
    
    if (size == 0) {
        fprintf(stderr, "Ошибка: В файле %s не найдено фамилий\n", filename);
        free_surnames_array(surnames, size);
        return NULL;
    }
    
    *quantity = size;
    return surnames;
}

void free_surnames_array(char** surnames, int quantity) {
    if (!surnames) return;
    
    for (int i = 0; i < quantity; i++) {
        free(surnames[i]);
    }
    free(surnames);
}

container_t* read_publications_csv(const char* filename) {
    FILE* file = filename ? fopen(filename, "rb") : stdin;
    if (!file) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл %s\n", filename ? filename : "stdin");
        return NULL;
    }
    
    container_t* container = container_init();
    if (!container) {
        if (file != stdin) fclose(file);
        return NULL;
    }
    
    char line[1024];
    int line_num = 0;
    
    if (fgets(line, sizeof(line), file)) {
        if (strstr(line, "title_publication") && strstr(line, "author_surname")) {
        } else if (strstr(line, "название") || strstr(line, "фамилия")) {
        } else {
            rewind(file);
        }
    }
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';
        
        publication_t publication;
        memset(&publication, 0, sizeof(publication));
        
        char* token = strtok(line, ",");
        int view = 0;
        
        while (token && view < 9) {
            while (*token == ' ') token++;
            
            switch (view) {
                case 0: publication.title_publication = copy_string(token); break;
                case 1: publication.author_surname = copy_string(token); break;
                case 2: publication.author_initials = copy_string(token); break;
                case 3: publication.title_journal = copy_string(token); break;
                case 4: publication.publication_year = atoi(token); break;
                case 5: publication.journal_volume = atoi(token); break;
                case 6: 
                    publication.is_rinz = (atoi(token) == 1);
                    break;
                case 7: publication.page_quantity = atoi(token); break;
                case 8: publication.citation_quantity = atoi(token); break;
            }
            
            token = strtok(NULL, ",");
            view++;
        }
        
        if (view == 9) {
            if (!container_push(container, &publication)) {
                fprintf(stderr, "Предупреждение: Не удалось добавить публикацию из строки %d\n", line_num);
            }
        } else {
            fprintf(stderr, "Предупреждение: Неполная строка на строке %d\n", line_num);
        }
        
        publication_free(&publication);
    }
    
    if (file != stdin) fclose(file);
    return container;
}

int write_publications_csv(const container_t* container, const char* filename) {
    FILE* file = filename ? fopen(filename, "wb") : stdout;
    if (!file) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл %s\n", filename ? filename : "stdout");
        return 0;
    }
    
    fprintf(file, "название,аамилия_автора,инициалы_автора,название_журнала,год_публикации,номер_тома,в_ринц,количество_страниц,количество_цитирований\n");
    
    container_iterator_t* it = container_iterator_create(container);
    if (!it) {
        if (file != stdout) fclose(file);
        return 0;
    }
    
    while (container_iterator_is_valid(it)) {
        publication_t publication;
        if (container_iterator_get(it, &publication)) {
            fprintf(file, "%s,%s,%s,%s,%d,%d,%d,%d,%d\n",
                   publication.title_publication ? publication.title_publication : "",
                   publication.author_surname ? publication.author_surname : "",
                   publication.author_initials ? publication.author_initials : "",
                   publication.title_journal ? publication.title_journal : "",
                   publication.publication_year,
                   publication.journal_volume,
                   publication.is_rinz,
                   publication.page_quantity,
                   publication.citation_quantity);
            
            publication_free(&publication);
        }
        container_iterator_next(it);
    }
    
    container_iterator_destroy(it);
    
    if (file != stdout) fclose(file);
    return 1;
}

int print_publications_table(const container_t* container, const char* filename) {
    FILE* file = filename ? fopen(filename, "w") : stdout;
    if (!file) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл %s\n", filename ? filename : "stdout");
        return 0;
    }

    fprintf(file, "==================================================================================\n");
    fprintf(file, "Название           | Автор          | Иниц | Журнал        | Год     |Том     |РЦ     |Стр    | Цит\n");
    fprintf(file, "-------------------------------------------------------------------------\n");
    
    container_iterator_t* it = container_iterator_create(container);
    if (!it) {
        if (file != stdout) fclose(file);
        return 0;
    }
    
    while (container_iterator_is_valid(it)) {
        publication_t publication;
        if (container_iterator_get(it, &publication)) {
            fprintf(file, "%-18s | %-14s | %-4s | %-15s | %4d |%2d|%2s|%3d|%5d\n",
                   publication.title_publication ? publication.title_publication : "",
                   publication.author_surname ? publication.author_surname : "",
                   publication.author_initials ? publication.author_initials : "",
                   publication.title_journal ? publication.title_journal : "",
                   publication.publication_year,
                   publication.journal_volume,
                   publication.is_rinz ? "да" : "нет",
                   publication.page_quantity,
                   publication.citation_quantity);
            
            publication_free(&publication);
        }
        container_iterator_next(it);
    }
    
    container_iterator_destroy(it);
    
    if (file != stdout) fclose(file);
    return 1;
}

container_t* generate_random_publications(int quantity, 
                                         const char* surnames_file,
                                         const char* words_file,
                                         const char* initials_file) {
    if (quantity <= 0) return NULL;
    
    static int initialized = 0;
    if (!initialized) {
        if (!init_data_from_files(words_file, initials_file)) {
            fprintf(stderr, "Ошибка: Не удалось инициализировать данные из файлов\n");
            return NULL;
        }
        initialized = 1;
        
        if (publications_quantity == 0) {
            fprintf(stderr, "Ошибка: Не загружены названия публикаций\n");
            return NULL;
        }
        if (journals_quantity == 0) {
            fprintf(stderr, "Ошибка: Не загружены названия журналов\n");
            return NULL;
        }
        if (initials_quantity == 0) {
            fprintf(stderr, "Ошибка: Не загружены инициалы\n");
            return NULL;
        }
    }
    
    container_t* container = container_init();
    if (!container) return NULL;
    
    int surnames_quantity = 0;
    char** surnames = NULL;
    
    if (!surnames_file) {
        fprintf(stderr, "Ошибка: Для генерации должен быть указан файл с фамилиями\n");
        container_destroy(container);
        return NULL;
    }
    
    surnames = read_surnames_from_file(surnames_file, &surnames_quantity);
    if (!surnames || surnames_quantity == 0) {
        fprintf(stderr, "Ошибка: Не удалось загрузить фамилии из %s\n", surnames_file);
        container_destroy(container);
        return NULL;
    }
    
    srand(time(NULL));
    
    for (int i = 0; i < quantity; i++) {
        publication_t publication;
        memset(&publication, 0, sizeof(publication));
        
        publication.title_publication = copy_string(publications[rand() % publications_quantity]);
        publication.author_surname = copy_string(surnames[rand() % surnames_quantity]);
        publication.author_initials = copy_string(initials[rand() % initials_quantity]);
        publication.title_journal = copy_string(journals[rand() % journals_quantity]);
        
        publication.publication_year = 1665 + rand() % 360;
        publication.journal_volume = 1 + rand() % 50;
        publication.is_rinz = rand() % 2;
        publication.page_quantity = 510 + rand() % 45;
        publication.citation_quantity = rand() % 1000;
        
        if (!container_push(container, &publication)) {
            fprintf(stderr, "Предупреждение: Не удалось добавить сгенерированную публикацию %d\n", i + 1);
        }
        
        publication_free(&publication);
    }
    
    free_surnames_array(surnames, surnames_quantity);
    
    return container;
}