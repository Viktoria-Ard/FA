#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int to_dig(char c){
    if(c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if(c >= 'a' && c <= 'z') return c - 'a' + 10;
    else return c - '0';
}

char *summ_two(int ss, char *num1, char *num2){
    size_t cap = BUFSIZ;
    int len = 0;
    int carry = 0;
    int pos1 = strlen(num1) - 1;
    int pos2 = strlen(num2) - 1;

    char *rev = (char*)malloc(cap);
    if(!rev){
        fprintf(stderr, "Error: memory allocation error\n");
        return NULL;
    }

    while(num1[pos1] && num2[pos2]){
        int dig1 = to_dig(num1[pos1]);
        int dig2 = to_dig(num2[pos2]);

        if(dig1 >= ss || dig2 >= ss){
            fprintf(stderr, "Error: invalid digit for base %d.\n", ss);
            free(rev);
            return NULL;
        }


        int sum = dig1 + dig2;
        sum += carry;
        carry = sum / ss;
        sum = sum % ss;

        if(len + 1 > cap){
            char * temp = realloc(rev, cap * 2);
            if(!temp){
                free(rev);
                fprintf(stderr, "Error: memory allocation error\n");
                return NULL;       
            }

            cap *= 2;
            rev = temp;
        }

        if(sum > 9){
            rev[len] = 'A' + (sum - 10);
        }
        else{
            rev[len] = '0' + sum;
        }
        pos1--;
        pos2--;
        len++;
    }

    while(num1[pos1]){
        int dig = to_dig(num1[pos1]);
        dig += carry;
        carry = dig / ss;
        dig = dig % ss;
         if(len + 1 > cap){
            char * temp = realloc(rev, cap * 2);
            if(!temp){
                free(rev);
                fprintf(stderr, "Error: memory allocation error\n");
                return NULL;       
            }

            cap *= 2;
            rev = temp;
        }

        if(dig > 9){
            rev[len] = 'A' + (dig - 10);
        }
        else{
            rev[len] = '0' + dig;
        }
        pos1--;
        len++;
    }

    while(num2[pos2]){
        int dig = to_dig(num2[pos2]);
        dig += carry;
        carry = dig / ss;
        dig = dig % ss;
         if(len + 1 > cap){
            char * temp = realloc(rev, cap * 2);
            if(!temp){
                free(rev);
                fprintf(stderr, "Error: memory allocation error\n");
                return NULL;       
            }

            cap *= 2;
            rev = temp;
        }

        if(dig > 9){
            rev[len] = 'A' + (dig - 10);
        }
        else{
            rev[len] = '0' + dig;
        }
        pos2--;
        len++;
    }

    while(carry > 0){
        if(len + 1 > cap){
            char * temp = realloc(rev, cap * 2);
            if(!temp){
                free(rev);
                fprintf(stderr, "Error: memory allocation error\n");
                return NULL;       
            }
            cap *= 2;
            rev = temp;
        }
        int sum = carry % ss;
        carry /= ss;
        rev[len++] = (sum > 9) ? ('A' + sum - 10) : ('0' + sum);
    }

    rev[len] = '\0';
    char *res = malloc(sizeof(char) * (len + 1));
    res[len] = '\0';
    for(int i = 0; i < len; i++){
        res[i] = rev[len - i - 1];
    }
    free(rev);
    return res;
}

char *summ(int ss, int n, ...){
    if(ss < 2 || ss > 36){
        fprintf(stderr, "Error: wrong SS.\n");
        return NULL;
    }
    va_list cur;
    va_start(cur, n);
    char *first = va_arg(cur, char*);
    char *res = strdup(first); 
    if (!res) {
        va_end(cur);
        return NULL;
    }
    for(int i = 0; i < n - 1; i++){
        char *two = summ_two(ss, res, va_arg(cur, char*));
        if(!two){
            return NULL;
        }
        free(res);
        res = two;
    }
    va_end(cur);
    return res;
}


int main(){
    char *res = summ(10, 4, "155555555555555555555555555555555634746537457365473547534765374537567354763549735749375497538749735497563054983549534958348958934589345893498", "2", "3", "4");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);

    res = summ(16, 4, "A", "B", "C", "D");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);

    res = summ(10, 3, "100", "200", "300");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);

    res = summ(2, 2, "101", "11");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);

    res = summ(8, 2, "77", "1");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);

    res = summ(16, 2, "FFFF", "1");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);
    
    res = summ(10, 2, "A", "1");
    if (!res) return 1;
    printf("%s\n", res);
    free(res);
}