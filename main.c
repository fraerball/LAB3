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


int main(int argc, char* argv[]){

        SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");

}