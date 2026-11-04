#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MINCAP 10

typedef struct{
    char *name;
    int size;
    int importance;
}File;

typedef struct{
    int total_importance;
    int total_size;
    int count;
    int *inds;
}Result;

Result choose_files(File *files, int n, int capacity){
    Result res;
    res.total_size = 0;
    res.count = 0;
    res.inds = (int*)malloc(n * sizeof(int));
    res.total_importance = 0;

    if(n == 0 || capacity <= 0) return res;

    int **table = (int**)malloc((n + 1) * sizeof(int*));

    for(int i = 0; i <= n; i++){
        table[i] = (int*)calloc((capacity + 1), sizeof(int)); //заполнение нулями для 0 строки и столбца
    }

    for(int i = 1; i <= n; i++){
        int curr_size = files[i-1].size;
        int curr_imp = files[i-1].importance;

        for(int w = 0; w <= capacity; w++){
            table[i][w] = table[i - 1][w]; //по умолчанию
            if(curr_size <= w){
                int take = table[i - 1][w - curr_size] + curr_imp;
                if(take > table[i][w]){
                    table[i][w] = take;
                }
            }
        }
    }

    res.total_importance = table[n][capacity];

    int w = capacity;
    for(int i = n; i > 0 && w > 0; i--){
        if(table[i][w] != table[i-1][w]){ //если изменилось - значит взяли
            res.inds[res.count++] = i - 1;
            res.total_size += files[i-1].size;
            w -= files[i - 1].size;
        }
    }
    for(int i = 0; i <= n; i++) free(table[i]);
    free(table);

    return res;
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

int main(){
    int n;
    int cap;
    if(scanf("%d %d", &n, &cap) != 2) return 1;

    File *files = (File*)malloc(n * sizeof(File));
    for(int i = 0; i < n; i++){
        files[i].name = read_word(stdin);
        if(scanf("%d %d", &files[i].size, &files[i].importance) != 2){
            free(files);
            fprintf(stderr, "Error: wrong format\n");
            return 1;
        }
    }
    Result res = choose_files(files, n, cap);
    printf("TOTAL_SIZE %d\n", res.total_size);
    printf("TOTAL_IMPORTANCE %d\n", res.total_importance);
    printf("FILES %d\n", res.count);

    for(int i = 0; i < n; i++){
        int is_chosen = 0;
        for(int j = 0; j < res.count; j++){
            if(res.inds[j] == i){
                is_chosen = 1;
                break;
            }
        }
        if(is_chosen) printf("%s\n", files[i].name);
    }
    return 0;
}