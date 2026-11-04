#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Employee.c"
#define MINCAP 16
#define FLAG_SIZE 5

int SameFile(FILE*fin, char * path){
    FILE * fout = fopen(path, "r");

    if(fout == NULL){
        return 0;
    }

    struct stat s1, s2;
    int fd1 = fileno(fin);
    int fd2 = fileno(fout);

    int result = 0;

    if(fd1 != -1 && fd2 != -1 && fstat(fd1, &s1) == 0 && fstat(fd2, &s2) ==0){
        if(s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino)
        result = 1;
    }

    fclose(fout);
    return result;
}


char *read_word(FILE *file){
    int cap = MINCAP;
    int len = 0;
    int c;

    char *buf = (char*)malloc(sizeof(char) * cap);
    if(!buf) return NULL;

    while((c = fgetc(file)) != EOF && (c == ' ' || c == '\t' || c == '\n')){

    }

    if(c == EOF){
        free(buf);
        return NULL;
    }

    while(1){ 
        if(c == EOF || c == ' ' || c == '\t' || c == '\n'){
            break;
        }  
        if(len + 1 >= cap){
            char *temp = realloc(buf, cap *2 * sizeof(char));
            if(!temp){
                free(buf);
                return NULL;
            }

            buf = temp;
            cap *= 2;
        }
        buf[len] = c;
        len++;  
        c = fgetc(file);
    }
    buf[len] = '\0';
    return buf;
}

Employee *fill_empl(FILE *file, int *count){
    int id_dublicate = 0;
    long id;
    double salary;
    int cap = BUFSIZ;
    int n = 0;
    int err = 0;

    Employee *arr = (Employee*)malloc(cap * sizeof(Employee));
    if (!arr){
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    int *ids = (int*)malloc(cap * sizeof(int));
    if(!ids){
        fprintf(stderr, "Memory allocation error.\n");
        free(arr);
        return NULL;
    }

    while(1){
        if(feof(file)) break;
        char extra = '\0';
        int s = fscanf(file, "%ld%c", &id, &extra);
        if(s != 2 || (extra != '\0' && extra != ' ')){
            fprintf(stderr, "Error: wrong format for id\n");
            err = 1;
            break;
        }

        for(int i = 0; i < n; i++){
            if(ids[i] == id){
                fprintf(stderr, "Error: ids must be different.\n");
                err = 1;
                id_dublicate = 1;
                break;
            }
        }

        if(id_dublicate == 1)break; 

        char *name = read_word(file);
        if(!name){
            err = 1;
            fprintf(stderr, "Memory allocation error.\n");
            break;
        }
        char *surname = read_word(file);
        if(!surname){
            free(name);
            err = 1;
            fprintf(stderr, "Memory allocation error.\n");
            break;           
        }
        extra = '\0';
        int p = fscanf(file, "%lf%c", &salary, &extra);
        if(p != 2){
            if(s != EOF && !feof(file)){
                fprintf(stderr, "Error: wrong format for salary\n");
                err = 1;
                break;
            }
        }
        if(n + 1 > cap){
            Employee *temp = (Employee*)realloc(arr, cap * 2 * sizeof(Employee));
            if(!temp){
                err = 1;
                free(name);
                free(surname);
                break;  
            }
            int *tempp = (int*)realloc(ids, cap * 2 * sizeof(int));
            if(!tempp){
                free(name);
                free(surname);
                err = 1;
                break;
            }
            arr = temp;
            ids = tempp;
            cap *= 2;
        }
        Employee new = create(id, name, surname, salary);
        arr[n] = new;
        ids[n] = id;
        free(name);
        free(surname);
        n++;
    }
    
    if (err) {
        for (int i = 0; i < n; i++) {
            free(arr[i].name);
            free(arr[i].surname);
        }
        free(arr);
        free(ids);
        return NULL;
    }

    *count = n;
    free(ids);
    return arr;
}

int cmp1(const void *a, const void *b){
    const Employee *x = (const Employee *)a;
    const Employee *y = (const Employee *)b;

    int r;
    if(get_salary(x) > get_salary(y)) return 1;
    if(get_salary(x) < get_salary(y)) return -1;

    r = strcmp(get_surname(x), get_surname(y)); if(r) return r;
    r = strcmp(get_name(x), get_name(y)); if(r) return r;

    if(get_id(x) > get_id(y)) return 1;
    if(get_id(x) < get_id(y)) return -1;

    return 0;
}

int cmp2(const void *a, const void *b){
   return -cmp1(a,b);
}

int main(int argc, char * argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <input_file> <flag> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    if(!fin){
        fprintf(stderr, "Error: could not open input file.\n");
        return 1;
    }

    int is_same = SameFile(fin, argv[3]);
    if(is_same == 1){
        fclose(fin);
        fprintf(stderr, "Error: input file and output file must be different.\n");
        return 1;
    }

    FILE *fout = fopen(argv[3], "w");
    if(!fout){
        fclose(fin);
        fprintf(stderr, "Error: could not open output file.\n");
        return 1;
    }

    int count = 0;
    Employee *employees = fill_empl(fin, &count);
    if(!employees){
        fclose(fin);
        fclose(fout);
        return 1;
    }

    if (count == 0) {
        fprintf(stderr, "Warning: no employees read\n");
    }

    char flag[FLAG_SIZE];
    if(sscanf(argv[2], "%4s", flag) != 1){
        fprintf(stderr, "Error: wrong format for flag\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    if(flag[0] == '/') flag[0] = '-';
    if(strcmp(flag, "-a")==0){
        qsort(employees, count, sizeof(Employee), cmp1);
        for(int i = 0; i < count; i++){
            fprintf(fout, "%ld %s %s %lf\n", 
            get_id(&employees[i]),
            get_name(&employees[i]), 
            get_surname(&employees[i]),
            get_salary(&employees[i]));
        }
    }

    else if(strcmp(flag, "-d")==0){
        qsort(employees, count, sizeof(Employee), cmp2);
        for(int i = 0; i < count; i++){
            fprintf(fout, "%ld %s %s %lf\n", 
                get_id(&employees[i]), 
                get_name(&employees[i]), 
                get_surname(&employees[i]), 
                get_salary(&employees[i]));
        }
    }
    else{
        fprintf(stderr, "Error: unknown flag.\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    fclose(fin);
    fclose(fout);
    for (int i = 0; i < count; i++){
        free(employees[i].name);
        free(employees[i].surname);
    }
    free(employees);
    return 0;
}
