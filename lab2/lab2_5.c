#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "func.h"
#define MAX_FLAG_SIZE 3


int overfprintf(FILE *file, char *str, ...){
    if(!file || !str){
        print_err("Error: invalid arguments\n");
        return -1;
    }

    va_list arg;
    va_start(arg, str);
    int i = 0;
    int cnt = 0;
    int has_error = 0;

    while(str[i] != '\0' && !has_error){
        if(str[i] != '%'){
            fputc(str[i], file);
            cnt++;
            i++;
            continue;
        }

        i++;
        if(str[i] == '\0') break;

        char curr = str[i];
        int flag_len = 1;

        if(curr == 'R' || curr == 'Z' || curr == 'C'||
        curr == 't' || curr == 'm' || curr == 'T'){

            char next = str[i + 1];
            if(next == 'o' || next == 'r' || next == 'v' || next == 'V' || 
                next == 'O' || next == 'i' || next == 'u' ||
                    next == 'd' || next == 'f'){
                flag_len = 2;
            }
        }

        if(flag_len == 1){

            switch(curr){
                case 's':{
                    char *val = va_arg(arg, char*);
                    int m = str_to_file(file, val);
                    if(m < 0){
                        print_err("Invalid arguments\n");
                        has_error = 1;
                        break;
                    }
                    cnt += m;
                    break;
                }

                case 'd':{
                    int val = va_arg(arg, int);
                    char *s = int_to_str(val);
                    if(!s){
                        print_err("Memory allocated failed\n");
                        has_error = 1;
                        break;
                    }
                    int m = str_to_file(file, s);
                    if(m < 0){
                        print_err("Invalid arguments\n");
                        free(s);
                        has_error = 1;
                        break;
                    }
                    cnt += m;
                    free(s);
                    break;
                }
                case 'u':{
                    unsigned int val = va_arg(arg, unsigned int);
                    char *s = unit_to_str(val);
                    if(!s){
                        print_err("Memory allocated failed\n");
                        has_error = 1;
                        break;
                    }

                    int m = str_to_file(file, s);
                    if(m < 0){
                        print_err("Invalid arguments\n");
                        free(s);
                        has_error =1;
                        break;
                    }
                    cnt += m;
                    free(s);
                    break;
                }

                case 'c':{
                    int temp = va_arg(arg, int);
                    char val = (char)temp;
                    fputc(val, file);
                    cnt++;
                    break;
                }
                case 'f':{
                    double val = va_arg(arg, double);

                    char *s = double_to_str(val);
                    if(!s){
                        print_err("Memory allocation error\n");
                        has_error = 1;
                        break;
                    }

                    int m = str_to_file(file, s);
                    if(m < 0){
                        print_err("Error in %f\n");
                        has_error = 1;
                        break;
                    }
                    free(s);
                    break;
                }

                case '%':{
                    fputc('%', file);
                    cnt++;
                    break;
                }
                default: {
                fputc('%', file);
                fputc(curr, file);
                cnt += 2; 
                break;
                }

            }

            i++;
        }

        else if(flag_len == 2){

            char next = str[i + 1];

            if (curr == 'R' && next == 'o') {
                int val = va_arg(arg, int);
                char *s = roman(val);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error = 1;
                    break;
                }
                int m = str_to_file(file, s);
                if (m < 0){
                    print_err("Error in %Ro\n");
                    has_error = 1;
                }
                else cnt += m;
                free(s);
            }

            else if(curr == 'Z' && next == 'r'){
                unsigned int val = va_arg(arg, unsigned int);
                char *s = zeckendorf(val);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                int m = str_to_file(file, s);
                if(m < 0){
                    print_err("Error in %Zr\n");
                    has_error = 1;
                }
                else cnt+= m;
                free(s);
            }

            else if(curr == 'C' && next == 'v'){
                int val = va_arg(arg, int);
                int ss = va_arg(arg, int);
                char *s = to_ss(ss, val, true);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }

                int m = str_to_file(file, s);
                if(m < 0){
                    print_err("Error in %Cv\n");
                    has_error = 1;
                }
                else cnt+= m;
                free(s);
            }

            else if(curr == 'C' && next == 'V'){
                int val = va_arg(arg, int);
                int ss = va_arg(arg, int);
                char *s = to_ss(ss, val, false);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                int m = str_to_file(file, s);
                if(m < 0){
                    print_err("Error in %CV\n");
                    has_error = 1;
                }
                else cnt+= m;
                free(s);
            }

            else if(curr == 't' && next == 'o'){
                char *str = va_arg(arg, char*);
                int ss = va_arg(arg, int);
                int num = to_mini_des(ss, str);
                if(num == -1){
                    print_err("Error: invalid number format for %to\n");
                    has_error = 1;
                }
                else{
                    char *s = int_to_str(num);
                    if(!s){
                        print_err("Invalid arguments\n");
                        has_error =1;
                        break;
                    }
                    int m = str_to_file(file, s);
                    if(m < 0){
                        print_err("Error in %to\n");
                        has_error = 1;
                    }
                    else cnt+= m;
                    free(s);
                }
            }

            else if(curr == 'T' && next == 'O'){
                char *str = va_arg(arg, char*);
                int ss = va_arg(arg, int);
                int num = to_big_des(ss, str);
                if(num == -1){
                    print_err("Error: invalid number format for %TO\n");
                    has_error = 1;
                }
                else{
                    char *s = int_to_str(num);
                    if(!s){
                        print_err("Invalid arguments\n");
                        has_error =1;
                        break;
                    }
                    int m = str_to_file(file, s);
                    if(m < 0){
                        print_err("Error in %TO\n");
                        has_error = 1;
                    }
                    else cnt+= m;
                    free(s);
                }
            }
            else if(curr == 'm' && next == 'i'){
                int dig = va_arg(arg, int);
                char *s = print_mi(dig);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                int m = str_to_file(file, s);
                if (m < 0){
                    print_err("Error in %mi\n");
                    has_error = 1;
                }
                else cnt += m;
                free(s);
            }

            else if(curr == 'm' && next == 'u'){
                unsigned int dig = va_arg(arg, unsigned int);
                char *s = print_mu(dig);

                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                int m = str_to_file(file, s);
                if(m < 0){
                    print_err("Error in %mu\n");
                    has_error = 1;
                }
                else cnt += m;
                free(s);
            }

            else if(curr == 'm' && next == 'd'){
                double dig = va_arg(arg, double);
                char *s = print_md(dig);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                int m = str_to_file(file, s);
                if(m < 0){
                    print_err("Error in %md\n");
                    has_error = 1;
                }
                else cnt += m;
                free(s);
            }

            else if(curr == 'm' && next == 'f'){
                double dig = va_arg(arg, double);
                float f = (float)dig;
                char *s = print_mf(f);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                int m = str_to_file(file, s);
                if(m < 0){
                    print_err("Error in %mf\n");
                    has_error = 1;
                }
                else cnt += m;
                free(s);
            }
            else {
                fputc('%', file);
                fputc(curr, file);
                fputc(next, file);
                cnt += 3;
            }
            i += 2;
        }
    }
    va_end(arg);

    if (has_error) {
        return -1;
    }
    return cnt;
}

int oversprintf(char *buf, char *str, ...){
    if(!buf || !str){
        print_err("Error: invalid arguments\n");
        return -1;
    }

    va_list arg;
    va_start(arg, str);
    int i = 0;
    int cnt = 0;
    int has_error = 0;

    while(str[i] != '\0' && !has_error){
        if(str[i] != '%'){
            buf[cnt] = str[i];
            cnt++;
            i++;
            continue;
        }

        i++;
        if(str[i] == '\0') break;

        char curr = str[i];
        int flag_len = 1;

        if(curr == 'R' || curr == 'Z' || curr == 'C'||
        curr == 't' || curr == 'm' || curr == 'T'){

            char next = str[i + 1];
            if(next == 'o' || next == 'r' || next == 'v' || next == 'V' || 
                next == 'O' || next == 'i' || next == 'u' ||
                    next == 'd' || next == 'f'){
                flag_len = 2;
            }
        }

        if(flag_len == 1){

            switch(curr){
                case 's':{
                    char *val = va_arg(arg, char*);
                    for(int j = 0; val[j]; j++){
                        buf[cnt] = val[j];
                        cnt++;
                    }
                    break;
                }

                case 'd':{
                    int val = va_arg(arg, int);
                    char *s = int_to_str(val);
                    if(!s){
                        print_err("Memory allocated failed\n");
                        has_error = 1;
                        break;
                    }
                    for(int j = 0; s[j]; j++){
                        buf[cnt] = s[j];
                        cnt++;
                    }
                    free(s);
                    break;
                }
                case 'u':{
                    unsigned int val = va_arg(arg, unsigned int);
                    char *s = unit_to_str(val);
                    if(!s){
                        print_err("Memory allocated failed\n");
                        has_error = 1;
                        break;
                    }

                    for(int j = 0; s[j]; j++){
                        buf[cnt] = s[j];
                        cnt++;
                    }
                    free(s);
                    break;
                }
                case 'f':{
                    double val = va_arg(arg, double);
    
                    char *s = double_to_str(val);
                    if (!s) {
                        print_err("Memory allocation failed\n");
                        has_error = 1;
                        break;
                    }
                    
                    for(int j = 0; s[j] != '\0'; j++){
                        buf[cnt] = s[j];
                        cnt++;
                    }
                    
                    free(s);
                    break;
                }

                case 'c':{
                    int temp = va_arg(arg, int);
                    char val = (char)temp;
                    buf[cnt] = val;
                    cnt++;
                    break;
                }

                case '%':{
                    buf[cnt] = '%';
                    cnt++;
                    break;
                }
                default: {
                    buf[cnt++] = '%';
                    buf[cnt++] = curr;
                    break;
                }

            }

            i++;
        }

        else if(flag_len == 2){

            char next = str[i + 1];

            if (curr == 'R' && next == 'o') {
                int val = va_arg(arg, int);
                char *s = roman(val);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error = 1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 'Z' && next == 'r'){
                unsigned int val = va_arg(arg, unsigned int);
                char *s = zeckendorf(val);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 'C' && next == 'v'){
                int val = va_arg(arg, int);
                int ss = va_arg(arg, int);
                char *s = to_ss(ss, val, true);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }

                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 'C' && next == 'V'){
                int val = va_arg(arg, int);
                int ss = va_arg(arg, int);
                char *s = to_ss(ss, val, false);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 't' && next == 'o'){
                char *str = va_arg(arg, char*);
                int ss = va_arg(arg, int);
                int num = to_mini_des(ss, str);
                if(num == -1){
                    print_err("Error: invalid number format for %to\n");
                    has_error = 1;
                }
                else{
                    char *s = int_to_str(num);
                    if(!s){
                        print_err("Invalid arguments\n");
                        has_error =1;
                        break;
                    }
                    for(int j = 0; s[j]; j++){
                        buf[cnt] = s[j];
                        cnt++;
                    }
                    free(s);
                }
            }

            else if(curr == 'T' && next == 'O'){
                char *str = va_arg(arg, char*);
                int ss = va_arg(arg, int);
                int num = to_big_des(ss, str);
                if(num == -1){
                    print_err("Error: invalid number format for %TO\n");
                    has_error = 1;
                }
                else{
                    char *s = int_to_str(num);
                    if(!s){
                        print_err("Invalid arguments\n");
                        has_error =1;
                        break;
                    }
                    for(int j = 0; s[j]; j++){
                        buf[cnt] = s[j];
                        cnt++;
                    }
                    free(s);
                }
            }
            else if(curr == 'm' && next == 'i'){
                int dig = va_arg(arg, int);
                char *s = print_mi(dig);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 'm' && next == 'u'){
                unsigned int dig = va_arg(arg, unsigned int);
                char *s = print_mu(dig);

                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 'm' && next == 'd'){
                double dig = va_arg(arg, double);
                char *s = print_md(dig);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }

            else if(curr == 'm' && next == 'f'){
                double dig = va_arg(arg, double);
                float f = (float)dig;
                char *s = print_mf(f);
                if(!s){
                    print_err("Invalid arguments\n");
                    has_error =1;
                    break;
                }
                for(int j = 0; s[j]; j++){
                    buf[cnt] = s[j];
                    cnt++;
                }
                free(s);
            }
            else {
                buf[cnt++] = '%';
                buf[cnt++] = curr;
                buf[cnt++] = next;
            }
            i += 2;
        }
    }
    va_end(arg);

    if (has_error) {
        return -1;
    }
    buf[cnt] = '\0';
    return cnt;
}

int main(int argc, char *argv[]) {
    FILE *file;
    int is_file_opened = 0;
    if(argc == 1){
        file = stdout;
    }
    else{
        file = fopen(argv[1], "w");
        if(!file){
            overfprintf(stderr, "Error: cannot open the outfile %s", argv[1]);
            return 1;
        }
        is_file_opened = 1;
    }
    overfprintf(file, "Overprintf tests\n");
    int m = overfprintf(file, "Standart flags: Hello %s! You are %d years old. Salary: %u\n", "Ivan", 25, 50000);
    overfprintf(file, "\n[Выведено: %d]\n\n", m);

    m = overfprintf(file, "Roman flag: 1994 = %Ro, 2024 = %Ro, 9 = %Ro\n", 1994, 2024, 9);
    overfprintf(file, "\n[Выведено: %d]\n\n", m);

    m = overfprintf(file, "Zeckendorf flag: 1 = %Zr, 17 = %Zr, 100 = %Zr\n", 1, 17, 100);
    overfprintf(file, "\n[Выведено: %d]\n\n", m);

    m = overfprintf(file, "Base conversion flag: 255 in hex = %Cv, 255 in HEX = %CV\n", 255, 16, 255, 16);
    overfprintf(file, "\n[Выведено: %d]\n\n", m);

    m = overfprintf(file, "To dec flag: ff from hex = %to, FF from HEX = %TO\n", "ff", 16, "FF", 16);
    overfprintf(file, "\n[Выведено: %d]\n\n", m);
    m = overfprintf(file, "Float test: %f\n", 3.14159);
    overfprintf(file, "\n[Выведено: %d]\n\n");

    overfprintf(file, "Memory dumps flags: \n");
    m = overfprintf(file, "int 5: %mi\n", 5);
    m += overfprintf(file, "uint 5: %mu\n", 5);
    m += overfprintf(file, "double 25.0: %md\n", 25.0);
    m += overfprintf(file, "float 1.5: %mf\n", 1.5f);
    overfprintf(file, "\n[Выведено: %d]\n\n", m);

    char buf[256];

    m = oversprintf(buf, "Hello %s! You are %d years old. Salary: %u\n", "Ivan", 25, 50000);
    overfprintf(file, "oversprintf standard: %s\n[Выведено: %d]\n", buf, m);

    m = oversprintf(buf, "ZR : %Zr", 19);
    overfprintf(file, "%s выведено: %d\n", buf, m);
    
    m = oversprintf(buf, "Char: %c, Percent: %%\n", 'A');
    overfprintf(file, "oversprintf char: %s\n[Выведено: %d]\n", buf, m);

    m = oversprintf(buf, "Float in string: %f", 1.234);
    overfprintf(file, "%s\n[Выведено: %d]\n", buf, m);

    if(is_file_opened == 1) fclose(file);

    return 0;
}