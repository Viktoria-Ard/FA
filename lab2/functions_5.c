#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "func.h"

void print_err(char *message){
    FILE *err = stderr;
    while(*message != '\0'){
        fputc(*message, err);
        message++;
    }
}

int str_to_file(FILE *file, char *s) {
    if (!file || !s) return -1;

    int cnt = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        fputc(s[i], file);
        cnt++;
    }
    return cnt;
}

char* allocate_string(int size) {
    char *str = (char*)malloc(size);
    if (!str) {
        print_err("Error: memory allocation failed\n");
        return NULL;
    }
    return str;
}

char* int_to_str(int val) {
    if (val == 0) {
        char *str = allocate_string(2);
        if (str) strcpy(str, "0");
        return str;
    }

    int is_neg = 0;
    if(val < 0){
        is_neg = 1;
        val = - val;
    }

    int len_dig = 0; 
    unsigned int temp = val;
    while (temp > 0) {
        temp /= 10;
        len_dig++;
    }

    char *str = allocate_string(len_dig + is_neg + 1);
    if (!str) return NULL;

    str[len_dig + is_neg] = '\0';
    if (is_neg) {
        str[0] = '-';
    }

    for (int i = len_dig + is_neg - 1; i >= is_neg; i--) {
        str[i] = '0' + (val % 10);
        val /= 10;
    }

    return str;
}

char* unit_to_str(unsigned int val) {
    if (val == 0) {
        char *str = allocate_string(2);
        if (str) strcpy(str, "0");
        return str;
    }

    int len_dig = 0; 
    unsigned int temp = val;
    while (temp > 0) {
        temp /= 10;
        len_dig++;
    }

    char *str = allocate_string(len_dig + 1);
    if (!str) return NULL;

    str[len_dig] = '\0';
    for (int i = len_dig - 1; i >= 0; i--) {
        str[i] = '0' + (val % 10);
        val /= 10;
    }

    return str;
}

char* roman(int num) {
    if (num <= 0) {
        char *str = allocate_string(1);
        if (str) str[0] = '\0';
        return str;
    }

    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char *romans[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int num_items = sizeof(values) / sizeof(values[0]);

    int capacity = BUFSIZ; 
    int length = 0;   
    
    char *str = allocate_string(capacity);
    if (!str) return NULL;
    str[0] = '\0';

    for (int i = 0; i < num_items; i++) {
        while (num >= values[i]) {
            int sym_len = strlen(romans[i]);
            
            if (length + sym_len >= capacity) {
                capacity *= 2; 
                char *temp = (char*)realloc(str, capacity);
                if (!temp) {
                    free(str); 
                    return NULL;
                }
                str = temp;
            }
            
            strcpy(str + length, romans[i]);
            length += sym_len;
            num -= values[i];
        } 
    }
    return str;
}

char* zeckendorf(unsigned int num) {
    unsigned int fib1 = 1, fib2 = 2;
    int count = 0;

    while (fib1 <= num) {
        count++;
        unsigned int temp = fib1 + fib2;
        fib1 = fib2; 
        fib2 = temp;
    }

    if (count == 0) count = 1;

    unsigned int *fib = (unsigned int *)malloc(count * sizeof(unsigned int));
    if (!fib) return NULL;

    fib[0] = 1;
    if (count > 1) fib[1] = 2;
    for (int i = 2; i < count; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    char *buf = allocate_string(count + 2); //биты Цекендорфа + завершающая '1' + '\0'
    if (!buf) {
        free(fib);
        return NULL;
    }

    int idx = 0;
    int start_zero = 0;
    unsigned int temp = num;

    for (int i = count - 1; i >= 0; i--) {
        if (temp >= fib[i]) {
            buf[idx++] = '1';
            temp -= fib[i];
            start_zero = 1;
        } else if (start_zero) {
            buf[idx++] = '0';
        }
    }

    buf[idx++] = '1';
    buf[idx] = '\0';

    free(fib);
    return buf;
}

char trasform_small(int dig) {
    if (dig < 10) return '0' + dig;
    else return 'a' + dig - 10;
}

char trasform_big(int dig) {
    if (dig < 10) return '0' + dig;
    else return 'A' + dig - 10;
}

char* to_ss(int ss, int num, bool small) {
    if (ss > 36 || ss < 2) ss = 10;

    if (num == 0) {
        char *str = allocate_string(2);
        if (str) strcpy(str, "0");
        return str;
    }

    int neg = 0;
    if(num < 0){
        neg = 1;
        num = -num;
    }

    int max_len = sizeof(int) * 8 + 2; 
    char temp_buf[max_len];
    int i = 0;

    while (num > 0) {
        int ost = num % ss;
        num /= ss;
        if (small) temp_buf[i] = trasform_small(ost);
        else temp_buf[i] = trasform_big(ost);
        i++;
    }

    char *str = allocate_string(i + neg + 1);
    if (!str) return NULL;

    int idx = 0;
    if (neg) {
        str[idx++] = '-';
    }

    while (i > 0) {
        i--;
        str[idx++] = temp_buf[i];
    }
    str[idx] = '\0';

    return str;
}


char* helper_dump_memory(unsigned char *ptr, int size) {
    int total_len = size * 8 + (size - 1) + 1;
    char *str = allocate_string(total_len);
    if (!str) return NULL;

    int idx = 0;
    for (int i = 0; i < size; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            int b = (ptr[i] >> bit) & 1;
            if(b) str[idx++] = '1';
            else str[idx++] = '0';
        }
        if (i < size - 1) {
            str[idx++] = ' ';
        }
    }
    str[idx] = '\0';
    return str;
}

char* print_mi(int val) {
    return helper_dump_memory((unsigned char *)&val, sizeof(int));
}

char* print_mu(unsigned int val) {
    return helper_dump_memory((unsigned char *)&val, sizeof(unsigned int));
}

char* print_md(double val) {
    return helper_dump_memory((unsigned char *)&val, sizeof(double));
}

char* print_mf(float val) {
    return helper_dump_memory((unsigned char *)&val, sizeof(float));
}

// Функции перевода строк в числа оставлены без изменений, так как они не использовали FILE*
int to_mini_des(int osn, char * x) {
    if (osn < 2 || osn > 36) osn = 10;
    int ans = 0;
    int digit;
    for (int i = 0; x[i]; i++) {
        if (x[i] >= 'a' && x[i] <= 'z') digit = x[i] - 'a' + 10;
        else if (x[i] >= '0' && x[i] <= '9') digit = x[i] - '0';
        else return -1;

        if (digit >= osn) return -1;
        ans = ans * osn + digit;
    }
    return ans;
}

int to_big_des(int osn, char * x) {
    if (osn < 2 || osn > 36) osn = 10;
    int ans = 0;
    int digit;
    for (int i = 0; x[i]; i++) {
        if (x[i] >= 'A' && x[i] <= 'Z') digit = x[i] - 'A' + 10;
        else if (x[i] >= '0' && x[i] <= '9') digit = x[i] - '0';
        else return -1;

        if (digit >= osn) return -1;
        ans = ans * osn + digit;
    }
    return ans;
}

char *double_to_str(double val){
    int is_neg = 0;
    if(val < 0){
        is_neg = 1;
        val = -val;
    }

    int number_aft_point = 6; //количество знаков, стандарт
    double round_offset = 0.5;
    for(int i = 0; i < number_aft_point; i++){
        round_offset /= 10.0;
    }

    val += round_offset; //округление

    int int_part = (int)val;
    double frac_part = val - (double)int_part;

    long long ll_frac = 0;
    for(int i = 0; i < number_aft_point; i++){ //запись дробной части в лонг лонг
        frac_part *= 10.0;
        int dig = (int)frac_part;
        ll_frac = ll_frac * 10 + dig;
        frac_part -= dig;
    }

    char *int_str = int_to_str((int)int_part);
    if(!int_str) return NULL;

    int int_len = 0;
    while(int_str[int_len]) int_len++;

    int len = is_neg + int_len + 1 + number_aft_point; //длина результирующей строки
    char *str = allocate_string(len + 1);
    if(!str){
        free(int_str);
        return NULL;
    }

    int pos = 0;
    if(is_neg){
        str[pos++] = '-';
    }

    for(int i = 0; i < int_len; i++){
        str[pos++] = int_str[i];
    }
    free(int_str);

    str[pos++] = '.';

    for(int i = number_aft_point - 1; i >= 0; i--){
        str[pos + i] = '0' + (ll_frac % 10);
        ll_frac /= 10;
    }

    pos += number_aft_point;
    
    str[pos] = '\0';
    return str;
}