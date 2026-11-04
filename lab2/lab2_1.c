#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>

int is_alpha(char c) {
	return((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int is_digit(char c){
	return (c >= '0' && c<= '9');
}

char upper(char c){
    if(is_alpha(c) && c >= 'a') return 'A' + c - 'a';
    else return c;
}

long len_str(char * str){
    long len = 0;
    while(str[len]){
        if(len >= LONG_MAX){
            fprintf(stderr, "Error: too long string/\n");
            return -1;
        }
        len++;
    }
    return len;
}

char * reverse(char *str, long len){
    char *rev = malloc(len + 1);
    if(!rev){
        fprintf(stderr, "Error: memory allocation failed\n");
        return NULL;
    }

    long p = len;
    rev[p] = '\0';
    p--;
    for(long i = 0; i < len; i++){
        rev[p] = str[i];
        p--;
    }
    return rev;
}

char * up(char * str, long len){
    char *res = malloc(len + 1);
    if(!res){
        fprintf(stderr, "Error: memory allocation failed\n");
        return NULL;
    }

    res[len] = '\0';
    for(long i = 0; i< len; i++){
        if(i % 2 == 1) res[i] = upper(str[i]);
        else res[i] = str[i];
    }
    return res;
}

char * sort(char * str, long len){
    char * ans = malloc(len + 1);
    if(!ans){
        fprintf(stderr, "Error: memory allocation failed\n");
        return NULL;
    }

    long count_dig = 0;
    long count_alph = 0;

    for(long i = 0; i < len; i++){
        if(is_digit(str[i])) count_dig += 1;
        else if(is_alpha(str[i])) count_alph += 1;
    }

    long pos_dig = 0;
    long pos_alph = count_dig; //запись после цифр
    long pos_symb = count_alph + count_dig; //запись после цифр и букв

    for(long i = 0; i < len; i++){
        if(is_digit(str[i])){
            ans[pos_dig] = str[i];
            pos_dig++;
        }
        else if(is_alpha(str[i])){
            ans[pos_alph] = str[i];
            pos_alph++;
        }
        else{
            ans[pos_symb] = str[i];
            pos_symb++;
        }
    }

    ans[len] = '\0';
    return ans;
}


void permut(int * digits, int n){
    for(int i = 0; i < n; i++){ 
        digits[i] = i;
    }
    
    for(int i = n-1; i > 0; i--){ //переставляем элементы с конца
        int j = rand() % (i + 1); //j - элемент от 0 до i
        int t = digits[i];
        digits[i] = digits[j];
        digits[j] = t;
    }
}

void concat(char * str1, char * str2, int size_1){
    for(int i = 0; str2[i]; i++){
        str1[size_1 + i] = str2[i];
    }
}

char * rand_str(int *rand_order,char ** strings, long size_of_rand){
    long len = 1;
    char *res = malloc(len);
    if(!res){
        fprintf(stderr, "Error: memory allocation failed\n");
        return NULL;
    }
    res[0] = '\0';
    for(long i = 0; i < size_of_rand; i++){
        long cur_index = rand_order[i]; //текущий номер строки в произвольном порядке
        long cur_size = len_str(strings[cur_index]);

        if(len > LONG_MAX - cur_size){
            fprintf(stderr, "Error: result too long\n");
            free(res);
            return NULL;
        }

        char * temp = realloc(res, len + cur_size); //увеличиваем строку для конкатенации

        if(!temp){
            fprintf(stderr, "Error: memory allocation failed\n");
            free(res);
            return NULL;
        }

        res = temp;
        concat(res, strings[cur_index], len - 1); //"приклеиваем" к результирующей строке текущую
        len += cur_size;
        res[len - 1] = '\0';
    }
    return res;
}

int main(int argc, char * argv[]){
    if(argc < 3){
        fprintf(stderr, "Error: invalid number of arguments\n");
        return 1;
    }

    if(strcmp(argv[1], "-l") == 0){
        if(argc != 3){
            fprintf(stderr, "Usage: %s -l <string>\n", argv[0]);
            return 1;
        }
        long len = len_str(argv[2]);
        if(len != -1){
            printf("%ld\n", len);
        }
        else return 1;
    }

    else if (strcmp(argv[1], "-r") == 0){
        if(argc != 3){
            fprintf(stderr, "Usage: %s -r <string>\n", argv[0]);
            return 1;
        }

        long len = len_str(argv[2]);
        if(len == -1) return 1;

        char * revers = reverse(argv[2], len);
        if(!revers){
            return 1;
        }

        printf("%s\n", revers);
        free(revers);
    }

    else if(strcmp(argv[1], "-u") == 0){
        if(argc != 3){
            fprintf(stderr, "Usage: %s -u <string>\n", argv[0]);
            return 1;
        }

        long len = len_str(argv[2]);
        if(len == -1) return 1;

        char *res = up(argv[2], len);
        if(!res) return 1;

        printf("%s\n", res);
        free(res);
    }

    else if(strcmp(argv[1], "-n") == 0){
        if(argc != 3){
            fprintf(stderr, "Usage: %s -n <string>\n", argv[0]);
            return 1;
        }

        long len = len_str(argv[2]);
        if(len == -1) return 1;

        char *res = sort(argv[2], len);
        if(!res) return 1;

        printf("%s\n", res);
        free(res);
    }

    else if(strcmp(argv[1], "-c") == 0){
        if(argc < 4){
            fprintf(stderr, "Usage: %s -c <number> <string 1> <string 2> ... <string n>\n", argv[0]);
            return 1;
        }

        unsigned long big_seed; //берем тип больше, чтобы проверить на переполнение меньшего типа
        char c = '\0';
        if(sscanf(argv[2], "%lu%c", &big_seed, &c) != 1 || c != '\0' || argv[2][0] == '-'){
            fprintf(stderr, "Error: the 3rd argument must be unsigned int\n");
            return 1;
        }

        if(big_seed > UINT_MAX){
            fprintf(stderr, "Error: the 3rd argument is too big\n");
            return 1;
        }

        unsigned int seed = (unsigned int)big_seed;

        srand(seed);
        int rand_order[argc - 3];
        permut(rand_order, argc - 3);

        char *strings[argc - 3];
        for(int i = 0; i < (argc - 3); i++){
            strings[i] = argv[3 + i];
        }

        char * res = rand_str(rand_order, strings, argc - 3);

        if(!res){
            return 1;
        }
        printf("%s\n", res);
        free(res);
    }
    
    else{
        fprintf(stderr, "Error: unknown flag.\n");
        return 1;
    }
    return 0;

}