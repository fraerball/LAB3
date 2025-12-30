#include "io.h"
#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static char** publications = NULL;
static int publications_quantity = 0;

static char** journals = NULL;
static int journals_quantity = 0;

static char** initials = NULL;
static int initials_quantity = 0;

static char** surnames = NULL;
static int surnames_quantity = 0;

static char* copy_string(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    char* new_str = malloc(strlen(str) + 1);
    if (new_str != NULL) {
        strcpy(new_str, str);
    }
    return new_str;
}

static char** read_lines_from_file(const char* filename, int* quantity) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        puts("Ошибка: Не удалось открыть файл\n");
        return NULL;
    }
    
    char** lines = NULL;
    int capacity = 10;
    int size = 0;
    char line[256];
    
    lines = malloc(capacity * sizeof(char*));
    if (lines == NULL) {
        fclose(file);
        return NULL;
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        if (size >= capacity) {
            capacity *= 2;
            char** new_lines = realloc(lines, capacity * sizeof(char*));
            if (new_lines == NULL) {
                for (int i = 0; i < size; i++) {
                    free(lines[i]);
                }
                free(lines);
                fclose(file);
                return NULL;
            }
            lines = new_lines;
        }

        lines[size] = copy_string(line);
        if (lines[size] == NULL) {
            for (int i = 0; i < size; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            return NULL;
        }
        
        size++;
    }
    
    fclose(file);
    
    if (size == 0) {
        puts("Ошибка: В файле не найдено строк\n");
        free(lines);
        return NULL;
    }
    
    *quantity = size;
    return lines;
}

void free_txt() {
    if (publications != NULL) {
        for (int i = 0; i < publications_quantity; i++) {
            free(publications[i]);
        }
        free(publications);
        publications = NULL;
        publications_quantity = 0;
    }

    if (journals != NULL) {
        for (int i = 0; i < journals_quantity; i++) {
            free(journals[i]);
        }
        free(journals);
        journals = NULL;
        journals_quantity = 0;
    }

    if (initials != NULL) {
        for (int i = 0; i < initials_quantity; i++) {
            free(initials[i]);
        }
        free(initials);
        initials = NULL;
        initials_quantity = 0;
    }

    if (surnames != NULL) {
        for (int i = 0; i < surnames_quantity; i++) {
            free(surnames[i]);
        }
        free(surnames);
        surnames = NULL;
        surnames_quantity = 0;
    }
}

static int init_data_from_files(const char* words_file,
                                const char* initials_file,
                                const char* surnames_file)
{
    const char* w_file = (words_file != NULL)    ? words_file    : "words.txt";
    const char* i_file = (initials_file != NULL) ? initials_file : "initials.txt";
    const char* s_file = (surnames_file != NULL) ? surnames_file : "surnames.txt";
    
    int total_words = 0;
    char** all_words = read_lines_from_file(w_file, &total_words);
    if (all_words == NULL || total_words == 0) {
        puts("Ошибка: Не удалось загрузить слова из файла\n");
        return 0;
    }
    
    int half_quantity = total_words / 2;
    
    publications = malloc(half_quantity * sizeof(char*));
    if (publications == NULL) {
        for (int i = 0; i < total_words; i++) {
            free(all_words[i]);
        }
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
        if (journals == NULL) {
            for (int i = 0; i < publications_quantity; i++) {
                free(publications[i]);
            }
            free(publications);
            for (int i = half_quantity; i < total_words; i++) {
                free(all_words[i]);
            }
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
    if (initials == NULL || initials_quantity == 0) {
        puts("Ошибка: Не удалось загрузить инициалы из файла\n");
        return 0;
    }

    surnames = read_lines_from_file(s_file, &surnames_quantity);
    if (surnames == NULL || surnames_quantity == 0) {
        puts("Ошибка: Не удалось загрузить фамилии из файла\n");
        return 0;
    }
    
    return 1;
}

container_t* read_publications_csv(const char* filename) {
    FILE* file = (filename != NULL) ? fopen(filename, "r") : stdin;
    if (file == NULL) {
        puts("Ошибка: Не удалось открыть файл\n");
        return NULL;
    }
    
    container_t* container = container_init();
    if (container == NULL) {
        if (file != stdin) {
            fclose(file);
        }
        return NULL;
    }
    
    char line[1024];
    int line_num = 0;
    
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';
        
        publication_t publication;
        memset(&publication, 0, sizeof(publication));
        
        char* token = strtok(line, ",");
        int field = 0;
        
        while (token != NULL && field < 9) {
            while (*token == ' ') {
                token++;
            }
            
            switch (field) {
                case 0: publication.title_publication  = copy_string(token); break;
                case 1: publication.author_surname    = copy_string(token); break;
                case 2: publication.author_initials   = copy_string(token); break;
                case 3: publication.title_journal     = copy_string(token); break;
                case 4: publication.publication_year  = atoi(token); break;
                case 5: publication.journal_volume    = atoi(token); break;
                case 6: publication.is_rinz           = (atoi(token) == 1); break;
                case 7: publication.page_quantity     = atoi(token); break;
                case 8: publication.citation_quantity = atoi(token); break;
            }
            
            token = strtok(NULL, ",");
            field++;
        }
        
        if (field == 9) {
            if (container_push(container, &publication) == 0) {
                puts("Предупреждение: Не удалось добавить публикацию из строки");
            }
        } else {
            puts("Предупреждение: Неполная строка");
        }
        
        publication_free(&publication);
    }
    
    if (file != stdin) {
        fclose(file);
    }
    return container;
}

int write_publications_csv(const container_t* container, const char* filename) {
    FILE* file = (filename != NULL) ? fopen(filename, "w") : stdout;
    if (file == NULL) {
        puts("Ошибка: Не удалось открыть файл\n");
        return 0;
    }

    fprintf(file, "название,фамилия_автора,инициалы_автора,название_журнала,год_публикации,номер_тома,в_ринц,количество_страниц,количество_цитирований\n");

    size_t n = container_size(container);
    for (size_t i = 0; i < n; i++) {
        publication_t publication;
        if (container_get(container, i, &publication) != 0) {
            fprintf(file, "%s,%s,%s,%s,%d,%d,%d,%d,%d\n",
                (publication.title_publication  != NULL) ? publication.title_publication  : "",
                (publication.author_surname    != NULL) ? publication.author_surname    : "",
                (publication.author_initials   != NULL) ? publication.author_initials   : "",
                (publication.title_journal     != NULL) ? publication.title_journal     : "",
                publication.publication_year,
                publication.journal_volume,
                publication.is_rinz,
                publication.page_quantity,
                publication.citation_quantity);

            publication_free(&publication);
        }
    }

    if (file != stdout) {
        fclose(file);
    }
    return 1;
}

int print_publications_table(const container_t* container, const char* filename) {
    FILE* file = (filename != NULL) ? fopen(filename, "w") : stdout;
    if (file == NULL) {
        puts("Ошибка: Не удалось открыть файл\n");
        return 0;
    }

    fprintf(file, "==================================================================================\n");
    fprintf(file, "Название           | Автор          | Иниц | Журнал        | Год     |Том     |РЦ     |Стр    | Цит\n");
    fprintf(file, "-------------------------------------------------------------------------\n");

    size_t n = container_size(container);
    for (size_t i = 0; i < n; i++) {
        publication_t publication;
        if (container_get(container, i, &publication) != 0) {
            fprintf(file, "%-18s | %-14s | %-4s | %-15s | %4d |%2d|%3s|%3d|%5d\n",
                (publication.title_publication  != NULL) ? publication.title_publication  : "",
                (publication.author_surname    != NULL) ? publication.author_surname    : "",
                (publication.author_initials   != NULL) ? publication.author_initials   : "",
                (publication.title_journal     != NULL) ? publication.title_journal     : "",
                publication.publication_year,
                publication.journal_volume,
                (publication.is_rinz != 0) ? "да" : "нет",
                publication.page_quantity,
                publication.citation_quantity);

            publication_free(&publication);
        }
    }

    if (file != stdout) {
        fclose(file);
    }
    return 1;
}

container_t* generate_random_publications(int quantity, 
                                         const char* surnames_file,
                                         const char* words_file,
                                         const char* initials_file)
{
    if (quantity <= 0) {
        return NULL;
    }
    
    static int initialized = 0;
    if (initialized == 0) {
        if (init_data_from_files(words_file, initials_file, surnames_file) == 0) {
            puts("Ошибка: Не удалось инициализировать данные из файлов\n");
            return NULL;
        }
        initialized = 1;
        
        if (publications_quantity == 0) {
            puts("Ошибка: Не загружены названия публикаций\n");
            return NULL;
        }
        if (journals_quantity == 0) {
            puts("Ошибка: Не загружены названия журналов\n");
            return NULL;
        }
        if (initials_quantity == 0) {
            puts("Ошибка: Не загружены инициалы\n");
            return NULL;
        }
        if (surnames_quantity == 0) {
            puts("Ошибка: Не загружены фамилии\n");
            return NULL;
        }
    }
    
    container_t* container = container_init();
    if (container == NULL) {
        return NULL;
    }
    
    srand((unsigned)time(NULL));
    
    for (int i = 0; i < quantity; i++) {
        publication_t publication;
        memset(&publication, 0, sizeof(publication));
        
        publication.title_publication = copy_string(
            publications[rand() % publications_quantity]);
        publication.author_surname = copy_string(
            surnames[rand() % surnames_quantity]);
        publication.author_initials = copy_string(
            initials[rand() % initials_quantity]);
        publication.title_journal = copy_string(
            journals[rand() % journals_quantity]);
        
        publication.publication_year = 1665 + rand() % 360;
        publication.journal_volume = 1 + rand() % 50;
        publication.is_rinz = rand() % 2;
        publication.page_quantity = 510 + rand() % 45;
        publication.citation_quantity = rand() % 1000;
        
        if (container_push(container, &publication) == 0) {
            puts("Не удалось добавить сгенерированную публикацию\n");
        }
        
        publication_free(&publication);
        
    }
    
    return container;
}
