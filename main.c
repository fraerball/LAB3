#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include "container.h"
#include "args.h"
#include "sort.h"

static comparator_t choose_comparator(sort_type_t type, sort_view_t view){
    if (view == SORT_YEAR){
        if (type == SORT_ASC){
            return cmp_year_asc;
        } else{
            return cmp_year_desc;
        }
    } 
    else if (view == SORT_CITATIONS){
        if (type == SORT_ASC){
            return cmp_cit_asc;
        } else{
            return cmp_cit_desc;
        }
    }
    else{
        return cmp_year_asc;
    }
}


int main(int argc, char* argp[]){

        SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

        program_args_t args;
    
    if (!analysis_args(argc, argp, &args)) {
        fprintf(stderr, "Ошибка разбора аргументов командной строки\n");
        return 1;
    }
    
    if (args.mode == MODE_NONE) {
        return 0;
    }
    
    container_t* publications = NULL;
    int result = 0;
    
    switch (args.mode) {
        case MODE_GENERATE:
            if (!args.surnames_file) {
                fprintf(stderr, "Ошибка: Для режима генерации должен быть указан файл с фамилиями\n");
                fprintf(stderr, "Используйте --surnames=ФАЙЛ или -S ФАЙЛ\n");
                result = 1;
                break;
            }
            
            publications = generate_random_publications(args.generate_quantity, args.surnames_file,args.words_file,args.initials_file);
            
            if (!publications) {
                fprintf(stderr, "Ошибка: Не удалось сгенерировать публикации\n");
                result = 1;
                break;
            }
            
            if (!write_publications_csv(publications, args.output_file)) {
                fprintf(stderr, "Ошибка: Не удалось записать публикации\n");
                result = 1;
            }
            break;
            
        case MODE_SORT:
            publications = read_publications_csv(args.input_file);
            if (!publications) {
                fprintf(stderr, "Ошибка: Не удалось прочитать публикации\n");
                result = 1;
                break;
            }
            
            sort(publications, choose_comparator(args.sort_type, args.sort_view));
            
            if (!write_publications_csv(publications, args.output_file)) {
                fprintf(stderr, "Ошибка: Не удалось записать отсортированные публикации\n");
                result = 1;
            }
            break;
            
        case MODE_PRINT:
            publications = read_publications_csv(args.input_file);
            if (!publications) {
                fprintf(stderr, "Ошибка: Не удалось прочитать публикации\n");
                result = 1;
                break;
            }
            
            if (!print_publications_table(publications, args.output_file)) {
                fprintf(stderr, "Ошибка: Не удалось вывести таблицу публикаций\n");
                result = 1;
            }
            break;
            
        default:
            break;
    }
    
    if (publications) {
        container_destroy(publications);
    }
    
    return result;
}