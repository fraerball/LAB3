#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include "publication.h"
#include "container.h"
#include "sort.h"
#include "args.h"
#include "io.h"

static comparator_t get_comparator(sort_type_t type, sort_view_t view){
    if (view == SORT_YEAR){
        if (type == SORT_ASC){
            return cmp_year_asc;
        } else {
            return cmp_year_desc;
        }
    } else if (view == SORT_CITATIONS){
        if (type == SORT_ASC){
            return cmp_cit_asc;
        } else {
            return cmp_cit_desc;
        }
    }
    return cmp_year_asc;
}

int main(int argq, char* argp[]){

    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    program_args_t args;
    
    if (analysis_args(argq, argp, &args)== 0){
        puts( "Ошибка разбора аргументов командной строки\n");
        return 1;
    }
    
    if (args.mode == MODE_NONE){
        return 0;
    }
    
    container_t* publications = NULL;
    int result = 0;
    
    switch (args.mode){
        case MODE_GENERATE:
            if (args.surnames_file == NULL){
                puts( "Ошибка: Для режима генерации должен быть указан файл с фамилиями\n");
                puts( "Используйте --surnames=ФАЙЛ или -S ФАЙЛ\n");
                result = 1;
                break;
            }
            
            publications = generate_random_publications(args.generate_quantity, args.surnames_file,args.words_file,args.initials_file);
            
            if (publications == NULL){
                puts( "Ошибка: Не удалось сгенерировать публикации\n");
                result = 1;
                break;
            }
            
            if (write_publications_csv(publications, args.output_file) == 0){
                puts( "Ошибка: Не удалось записать публикации\n");
                result = 1;
            }
            break;
            
        case MODE_SORT:
            publications = read_publications_csv(args.input_file);
            if (publications == NULL){
                puts( "Ошибка: Не удалось прочитать публикации\n");
                result = 1;
                break;
            }
            
            sort(publications, get_comparator(args.sort_type, args.sort_view), args.sort_method);
            
            if (write_publications_csv(publications, args.output_file) == 0){
                puts( "Ошибка: Не удалось записать отсортированные публикации\n");
                result = 1;
            }
            break;
            
        case MODE_PRINT:
            publications = read_publications_csv(args.input_file);
            if (publications == NULL){
                puts( "Ошибка: Не удалось прочитать публикации\n");
                result = 1;
                break;
            }
            
            if (print_publications_table(publications, args.output_file) == 0){
                puts( "Ошибка: Не удалось вывести таблицу публикаций\n");
                result = 1;
            }
            break;
            
        default:
            break;
    }
    
    if (publications != NULL){
        container_destroy(publications);
    }
    free_txt();
    return result;
}