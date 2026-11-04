#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#define MAX_FLAG_LEN 3

int is_sep(int c){
    return c == ' ' || c == '\n' || c == '\t';
}

int int_from_file(FILE *file){
    int c = fgetc(file);
    int neg = 1;

    while (c != EOF && is_sep(c))
    {
        c = fgetc(file);
    }

    if(c == EOF) return 0;
    
    int res = 0;
    if(c == '-'){
        neg = -1;
        c = fgetc(file);
    }
    else if( c == '+') c = fgetc(file);

    while (!is_sep(c) && c != EOF )
    {
        res = res * 10 + (c - '0');
        c = fgetc(file);
    }

    if(c != EOF) ungetc(c, file);
    return res * neg;
}

int int_from_str(char *str, int *position){
    int neg = 1;
    int i = *position;

    while (str[i] != '\0' && is_sep(str[i]))
    {
        i++;
    }

    if(str[i] == '-'){
        neg = -1;
        i++;
    }else if(str[i] == '+'){
        i++;
    }

    if(str[i] == '\0') return 0;
    int res = 0;

    while (str[i] != '\0' && !is_sep(str[i]))
    {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    *position = i;
    return res * neg;
}

unsigned int uint_from_file(FILE *file){
    int c = fgetc(file);

    while (c != EOF && is_sep(c))
    {
        c = fgetc(file);
    }
    if(c == EOF) return 0;
    
    unsigned int res = 0;
    while (!is_sep(c) && c != EOF )
    {
        res = res * 10 + (c - '0');
        c = fgetc(file);
    }

    if(c != EOF) ungetc(c, file);
    return res;
}

unsigned int uint_from_str(char *str, int *position){
    int i = *position;

    while (str[i] != '\0' && is_sep(str[i]))
    {
        i++;
    }

    if(str[i] == '\0') return 0;

    unsigned int res = 0;
    while(!is_sep(str[i]) && str[i] != '\0'){
        res = res * 10 + (str[i] - '0');
        i++;
    }
    *position = i;
    return res;
}

void str_from_file(FILE *file, char *buf){
    int c = fgetc(file);

    while (c != EOF && is_sep(c))
    {
        c = fgetc(file);
    }
    
    int ind = 0;
    while (!is_sep(c) && c != EOF)
    {
        buf[ind++] = c;
        c = fgetc(file);
    }
    buf[ind] = '\0';
    if(c != EOF) ungetc(c, file);
}

void str_from_str(char *str, char *buf, int *position){
    int i = *position;
    while (is_sep(str[i]) && str[i] != '\0')
    {
        i++;
    }

    int ind = 0;
    while (!is_sep(str[i]) && str[i] != '\0')
    {
        buf[ind++] = str[i++];
    }

    buf[ind] = '\0';
    *position = i;
}

int roman_value(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

int roman_from_file(FILE *file){
    int c = fgetc(file);
    int res = 0;

    while (c != EOF && is_sep(c))
    {
        c = fgetc(file); //пропуск разделителей в файле
    }

    if(c == EOF) return 0;
    int curr_val = roman_value((char)c);

    while (c != EOF && !is_sep(c))
    {
        int next = fgetc(file);
        if(next == EOF || is_sep(next)){
            res += curr_val;
            if(next != EOF) ungetc(next, file);
            break;
        }

        int next_val = roman_value((char)next);
        if(curr_val < next_val){ //для чисел типа IV, когда первая цифра меньше второй, вычитаем первую и прибавляем вторую
            res -= curr_val;
        }
        else res += curr_val;

        c = next;
        curr_val = next_val;
    }
    return res;
}

int roman_from_str(char *str, int *position){
    int i = *position;
    int res = 0;

    while (str[i] != '\0' && is_sep(str[i])) i++;
    if(str[i] == '\0') return 0;
    int curr_val = roman_value(str[i]);

    while(str[i] != '\0' && !is_sep(str[i])){
        int next = str[i + 1];
        if(next == '\0' || is_sep(next)){
            res += curr_val;
            i++;
            break;
        }

        int next_val = roman_value(next);
        if(curr_val < next_val){
            res -= curr_val;
        }
        else res += curr_val;

        i++;
        curr_val = next_val;
    }

    *position = i;
    return res;
}

unsigned int fib(int c){
    if (c < 0) return 0;
    if(c == 0) return 1;
    if(c == 1) return 2;
    unsigned int temp = 2;
    unsigned int prev = 1;
    for(int i = 2; i <= c; i++){
        unsigned int n = temp;
        temp = temp + prev;
        prev = n;
    }
    return temp;
}

unsigned int zeckendorf_from_file(FILE *file){
    int c = fgetc(file);
    unsigned int res = 0;
    int ind = 0;

    while (c != EOF && is_sep(c))
    {
        c = fgetc(file); //пропуск разделителей в файле
    }
    if(c == EOF) return 0;

    while(1){
        int next = fgetc(file);
        if (next == EOF || is_sep(next)){
            if(next != EOF) ungetc(next, file);
            break;
        }
        if(c == '1'){
            res += fib(ind);
        }
        ind++;
        c = next;
    }
    return res;
}

unsigned int zeckendorf_from_str(char *str, int *position){
    int i = *position;
    unsigned int res = 0;
    int ind = 0;

    while(str[i] != '\0' && is_sep(str[i])) i++;
    if(str[i] == '\0') return 0;

    while(1){
        int next = str[i + 1];
        if(next == '\0' || is_sep(next)) break;

        if(str[i] == '1') res += fib(ind);

        ind++;
        i++;
    }
    *position = i;
    return res;
}

int mini_to_dig(char c){
    if(c >= '0' && c <= '9') return c - '0';
    else return c - 'a' + 10;
}

int big_to_dig(char c){
    if(c >= '0' && c <= '9') return c - '0';
    else return c - 'A' + 10;
}

int cv(FILE *file, int ss, bool is_small){
    int c = fgetc(file);
    int res = 0;

    while (c != EOF && is_sep(c))
    {
        c = fgetc(file); //пропуск разделителей в файле
    }
    if(c == EOF) return 0;

    while (c != EOF && !is_sep(c))
    {
        if(is_small == 1){
            res = res * ss + mini_to_dig(c);
        }
        else{
            res = res * ss + big_to_dig(c);
        }
        c = fgetc(file);
    }
    if (c != EOF) ungetc(c, file);
    return res;
}

int cv_str(char *str, int ss, bool is_small, int *position){
    int i = *position;
    int res = 0;
    while(str[i] != '\0' && is_sep(str[i])) i++;
    if(str[i] == '\0') return 0;

    while (str[i] != '\0' && !is_sep(str[i]))
    {
        if(is_small == 1){
            res = res * ss + mini_to_dig(str[i]);
        }else{
            res = res * ss + big_to_dig(str[i]);
        }
        i++;
    }
    *position = i;
    return res;
}

float float_from_file(FILE *file){
    int c = fgetc(file);
    double res = 0.0;
    int sign = 1;

    while (c != EOF && is_sep(c)) c = fgetc(file);
    if (c == EOF) return 0.0;

    if(c == '-'){
        sign = -1;
        c = fgetc(file);
    }else if(c == '+'){
        c = fgetc(file);
    }
    
    while (c != EOF && c >= '0' && c <= '9')
    {
        res = res * 10.0 + (c - '0');
        c = fgetc(file);
    }

    if(c == '.' || c == ','){
        c = fgetc(file);
        double frac = 0.1;
        while(c != EOF && c >= '0' && c <= '9'){
            res += (c - '0') * frac;
            frac *= 0.1;
            c = fgetc(file);
        }
    }
    if(c != EOF) ungetc(c, file);
    return res * sign;
}

float float_from_str(char *str, int *position){
    int i = *position;
    float res = 0.0;
    int sign = 1;

    while(str[i] != '\0' && is_sep(str[i])) i++;
    if(str[i] == '\0') return 0.0;

    if(str[i] == '-'){
        sign = -1;
        i++;
    }else if(str[i] == '+'){
        i++;
    }

    while(str[i] != '\0' && str[i] >= '0' && str[i] <= '9'){
        res = res * 10.0 + (str[i] - '0');
        i++;
    }

    if(str[i] == '.' || str[i] == ','){
        i++;
        double frac = 0.1;
        while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
        {
            res += (str[i] - '0') * frac;
            frac *= 0.1;
            i++;
        }
    }
    *position = i;
    return res * sign;
}

int overfscanf(FILE *file, char *str, ...){
    if(!file || !str){
        fprintf(stderr, "Error: Invalid  arguments.\n");
        return -1;
    }

    int i = 0;
    va_list arg;
    va_start(arg, str);
    int cnt = 0;
    int c;

    while (str[i]){
        if (str[i] == ' ') { //пропускаем пробелы в файле
            while ((c = fgetc(file)) != EOF && is_sep(c)) { }
            if (c != EOF) ungetc(c, file);
            i++;
            continue;
        }
        if(str[i] != '%') { //сравнение строк
            c = fgetc(file);
            if(c == EOF) break;
            
            if(c != str[i]){
                ungetc(c, file);
                return cnt;
            }
            i++;
            continue;
        }

        i++;
        if(str[i] == '\0') break; 

        char curr = str[i];
        char next = str[i+1];
        int flag_size = 1;

        if((curr == 'R' && next == 'o') ||
        (curr == 'Z' && next == 'r') ||
        (curr == 'C' && next == 'v') ||
        (curr == 'C' && next == 'V')){
            flag_size = 2;
        }

        if(flag_size == 1){
            switch (curr)
            {
                case 'd':{
                    int *ptr = va_arg(arg, int*);
                    *ptr = int_from_file(file);
                    cnt++;
                    break;
                }
                case 'c':{
                    char *ptr = va_arg(arg, char *);
                    *ptr = fgetc(file);
                    cnt++;
                    break;
                }
                case 's':{
                    char *buf = va_arg(arg, char*);
                    str_from_file(file, buf);
                    cnt++;
                    break;
                }
                case 'u':{
                    unsigned int *ptr = va_arg(arg, unsigned int*);
                    *ptr = uint_from_file(file);
                    cnt++;
                    break;
                }
                case 'f':{
                    float *ptr = va_arg(arg, float*);
                    *ptr = float_from_file(file);
                    cnt++;
                    break;
                }
                case '%':{
                    int c = fgetc(file);
                    if(c != '%'){
                        if(c != EOF) ungetc(c, file);
                        return cnt;
                    }
                    break;
                }
                default:{
                    return cnt;
                }
            }
            i++;
        }

        else if (flag_size == 2){
            if(curr == 'R' && next == 'o'){
                int *ptr = va_arg(arg, int*);
                *ptr = roman_from_file(file);
                cnt++;
            }
            else if(curr == 'Z' && next == 'r'){
                unsigned int *ptr = va_arg(arg, unsigned int*);
                *ptr = zeckendorf_from_file(file);
                cnt++;
            }
            else if(curr == 'C' && next == 'v'){
                int *ptr = va_arg(arg, int*);
                int ss = va_arg(arg, int);
                if(ss < 2 || ss > 36) ss = 10;
                *ptr = cv(file, ss, 1);
                cnt++;
            }
            else if(curr == 'C' && next == 'V'){
                int *ptr = va_arg(arg, int*);
                int ss = va_arg(arg, int);
                if(ss < 2 || ss > 36) ss = 10;
                *ptr = cv(file, ss, 0);
                cnt++;
            }
            else{
                return cnt;
            }

            i += 2;
        }

    }
    va_end(arg);
    return cnt;
}

int oversscanf(char *from, char *str, ...){
    if(!from || !str){
        fprintf(stderr, "Error: Invalid  arguments.\n");
        return -1;
    }

    int str_ind = 0;
    int from_ind = 0;
    va_list arg;
    va_start(arg, str);
    int cnt = 0;

    while (str[str_ind])
    {
        if (str[str_ind] == ' ') {
            while (from[from_ind] != '\0' && is_sep(from[from_ind])) {
                from_ind++;
            } //пропускаем пробелы
            str_ind++;
            continue;
        }
        if(str[str_ind] != '%') { //сравнение строк
            if(from[from_ind] == '\0') break;
            
            if(from[from_ind] != str[str_ind]){
                from_ind--;
                return cnt;
            }
            str_ind++;
            continue;
        }

        str_ind++;
        if(str[str_ind] == '\0') break; 

        char curr = str[str_ind];
        char next = str[str_ind + 1];
        int flag_size = 1;

        if((curr == 'R' && next == 'o') ||
        (curr == 'Z' && next == 'r') ||
        (curr == 'C' && next == 'v') ||
        (curr == 'C' && next == 'V')){
            flag_size = 2;
        }

        if(flag_size == 1){
            switch (curr)
            {
                case 'd':{
                    int old = from_ind;
                    int *ptr = va_arg(arg, int*);
                    *ptr = int_from_str(from, &from_ind);
                    if(from_ind != old)cnt++;
                    break;
                }
                case 'u':{
                    int old = from_ind;
                    unsigned int *ptr = va_arg(arg, unsigned int*);
                    *ptr = uint_from_str(from, &from_ind);
                    if(from_ind != old)cnt++;
                    break;
                }

                case 'c':{
                    int old = from_ind;
                    char *ptr = va_arg(arg, char*);
                    *ptr = from[from_ind++];
                    if(from_ind != old)cnt++;
                    break;
                }

                case 's':{
                    int old = from_ind;
                    char *ptr = va_arg(arg, char*);
                    str_from_str(from, ptr, &from_ind);
                    if(from_ind != old)cnt++;
                    break;
                }

                case 'f':{
                    int old = from_ind;
                    float *ptr = va_arg(arg, float*);
                    *ptr = float_from_str(from, &from_ind);
                    if(from_ind != old)cnt++;
                    break;
                }

                case '%':{
                    if(from[from_ind] != '%') return cnt;
                    from_ind++;
                    break;
                }
                default:{
                    return cnt;
                }
            }
            str_ind++;
        }
        else if(flag_size == 2){
            if(curr == 'R' && next == 'o'){
                int *ptr = va_arg(arg, int*);
                *ptr = roman_from_str(from ,&from_ind);
                cnt++;
            }
            else if(curr == 'Z' && next == 'r'){
                unsigned int *ptr = va_arg(arg, unsigned int*);
                *ptr = zeckendorf_from_str(from, &from_ind);
                cnt++;
            }
            else if(curr == 'C' && next == 'v'){
                int *ptr = va_arg(arg, int*);
                int ss = va_arg(arg, int);
                if(ss < 2 || ss > 36) ss = 10;
                *ptr = cv_str(from, ss, 1, &from_ind);
                cnt++;
            }
            else if(curr == 'C' && next == 'V'){
                int *ptr = va_arg(arg, int*);
                int ss = va_arg(arg, int);
                if(ss < 2 || ss > 36) ss = 10;
                *ptr = cv_str(from, ss, 0, &from_ind);
                cnt++;
            }
            else return cnt;
            str_ind += 2;
        }
        
    }
    va_end(arg);
    return cnt;
}

int main() {
    printf("Тесты overfscanf\n");
    FILE *file = fopen("lab6_input.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open fin.txt\n");
        return 1;
    }

    //Стандартные флаги
    int id;
    char name[50], surname[50];
    float sal;
    int cnt = overfscanf(file, "%d %s %s %f", &id, name, surname, &sal);
    printf("Standard: cnt = %d (%d, %s, %s, %.2f)\n", cnt, id, name, surname, sal);

    // 2. Римские цифры
    int roman_val;
    cnt = overfscanf(file, "%Ro", &roman_val);
    printf("Roman: cnt = %d (roman = %d)\n", cnt, roman_val);

    // Цекендорф 
    unsigned int zeck_val;
    cnt = overfscanf(file, "%Zr", &zeck_val);
    printf("Zeckendorf: cnt = %d (zeckendorf = %u)\n", cnt, zeck_val);

    // Система счисления
    int hex_val1;
    cnt = overfscanf(file, "%Cv", &hex_val1, 16);
    printf("Base small: cnt = %d (hex = %d)\n", cnt, hex_val1);

    int hex_val2;
    cnt = overfscanf(file, "%CV", &hex_val2, 16);
    printf("Base big: cnt = %d (hex = %d)\n", cnt, hex_val2);

    fclose(file);
    printf("\n");


    printf("Тесты oversscanf\n");

    //Стандартные флаги
    int a, b;
    char s1[50], s2[50];
    float f1, f2;
    cnt = oversscanf("123 Hello 3.14", "%d %s %f", &a, s1, &f1);
    printf("Standard: cnt = %d (%d, %s, %.2f)\n", cnt, a, s1, f1);

    //Римские цифры
    int roman_val2;
    cnt = oversscanf("MCMXCIV", "%Ro", &roman_val2);
    printf("Roman: cnt = %d (roman = %d)\n", cnt, roman_val2);

    //Цекендорф
    unsigned int zeck_val2;
    cnt = oversscanf("1010011", "%Zr", &zeck_val2);
    printf("Zeckendorf: cnt = %d (zeckendorf = %u)\n", cnt, zeck_val2);

    //Система счисления 
    int hex_val3;
    cnt = oversscanf("ff", "%Cv", &hex_val3, 16);
    printf("Base small: cnt = %d (hex = %d)\n", cnt, hex_val3);
    int hex_val4;
    cnt = oversscanf("FF", "%CV", &hex_val4, 16);
    printf("Base big: cnt = %d (hex = %d)\n", cnt, hex_val4);
    int x, y, z;
    cnt = oversscanf("10 20 30", "%d %d %d", &x, &y, &z);
    printf("Multiple: cnt = %d (%d, %d, %d)\n", cnt, x, y, z);

    // Смешанный формат
    char mixed_str[50];
    int mixed_num1, mixed_num2;
    cnt = oversscanf("42 Answer 100", "%d %s %d", &mixed_num1, mixed_str, &mixed_num2);
    printf("Mixed: cnt = %d (%d, %s, %d)\n", cnt, mixed_num1, mixed_str, mixed_num2);

    return 0;
}