#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

int search_two(char *str1, char *str2, int n){
    for(int i = 0; i < n; i++){
        if(str1[i] != str2[i]) return 1;
    }
    return 0;
}

int search_many(char *str, int n, ...){
    if (!str) {
        fprintf(stderr, "Error: string is NULL\n");
        return -1;
    }
    va_list cur;
    va_start(cur, n);
    int len_str = strlen(str);

    for(int i = 0; i < n; i++){
        int c;
        int cap = BUFSIZ;
        int char_cnt = 0;
        int str_cnt = 1;
        char *temp = va_arg(cur, char*);
        FILE *file = fopen(temp, "r");
        if(!file){
            fprintf(stderr, "Error: cannot open file %s.\n", temp);
            continue;
        }

        char *line = malloc(sizeof(char) * cap);
        if(!line){
            fprintf(stderr, "Error: memory allocation error.\n");
            fclose(file);
            return -2;
        }

        while(1){
            c = fgetc(file);
            if(c == EOF){
                break;
            }
            if(c == '\n'){
                line[char_cnt] = '\0';
                for(int i = 0; i < char_cnt - len_str + 1; i++){
                    if(search_two(line + i, str, len_str) == 0){
                        printf("File: %s, Line: %d, Found at index: %d\n", temp, str_cnt, i + 1);
                    }
                }
                char_cnt = 0;
                str_cnt++;
                continue;
            }
            if(char_cnt + 1 > cap){
                char *temp = realloc(line, cap*2);
                if(!temp){
                    fprintf(stderr, "Error: memory allocation error.\n");
                    fclose(file);
                    free(line);
                    va_end(cur);
                    return -2;
                }

                line = temp;
                cap *= 2;
            }

            line[char_cnt] = c;
            char_cnt ++;
        }

        if(char_cnt > 0){
            line[char_cnt] = '\0';
            for(int i = 0; i < char_cnt - len_str + 1; i++){
                if(search_two(line + i, str, len_str) == 0){
                    printf("File: %s, Line: %d, Found at index: %d\n", temp, str_cnt, i + 1);
                }
            }
        }
        fclose(file);
        free(line);
    }
    va_end(cur);
    return 0;
}

int main(int argc, char *argv[]){
    int res = 0;
    if (argc >= 3) {
        char *str = argv[1];
        int file_count = argc - 2;
        if (file_count > 5) {
            fprintf(stderr, "Error: too many files (max 5)\n");
            return 1;
        }

        switch (file_count) {
            case 1: res = search_many(str, 1, argv[2]); break;
            case 2: res = search_many(str, 2, argv[2], argv[3]); break;
            case 3: res = search_many(str, 3, argv[2], argv[3], argv[4]); break;
            case 4: res = search_many(str, 4, argv[2], argv[3], argv[4], argv[5]); break;
            case 5: res = search_many(str, 5, argv[2], argv[3], argv[4], argv[5], argv[6]); break;
        }
    } 
    else {
        fprintf(stderr, "Usage: ./%s <string> <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    if(res != 0) return 1;

    return 0;
}
