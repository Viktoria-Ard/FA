#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024

int is_bigalpha(int x) {
    return (x >= 'A' && x <= 'Z');
}

int is_digit(int x){
    return (x >= '0' && x <= '9');
}

long to_des(int osn, char * x){
    int neg = 0;
    long ans = 0;
    int digit;
    int i = 0;
    if(x[0] == '-'){
        neg = 1;
        i = 1;
    }
    for(; x[i]; i++){
        if(is_bigalpha(x[i])) digit = x[i] - 'A' + 10;
        else digit = x[i] - '0';
        if(ans > (LONG_MAX - digit) / osn){
            return LONG_MAX;   //переполнения
        }
        ans = ans * osn + digit;
    }
    if(neg == 1) return -ans;
    return ans;
}

void des_to_system(int osn, long x){
    int neg = 0;
    char ans[BUFSIZE];
    char * pres = ans;
    pres[BUFSIZE - 1] = 0;
    pres += BUFSIZE - 2;
    int d;
    if (x == 0) {
        *pres = '0';
        printf("%d: 0\n", osn);
        return;
    }
    if(x < 0){
        neg = 1;
        x = -x;
    }
    while(x){
        if((d = x % osn) > 9) *pres = d - 10 + 'A';
        else *pres = d + '0';
        pres --;
        x = x / osn;
    }
    if(neg == 1) printf("%d: -%s\n", osn, pres + 1);
    else printf("%d: %s\n", osn, pres + 1);
}

int validate(char * num, int osn){ //валидация входного числа
    int i = 0;
    if(num[0] == '\0') return 0; //пустая строка
    if(num[0] == '-'){
         i = 1;
         if(!num[i]) return 1; //только минус
    }
    for(; num[i]; i++){
        if(is_bigalpha(num[i]) && (num[i] - 'A' + 10) < osn) continue;
        else if(is_digit(num[i]) && (num[i] - '0') < osn) continue;
        else return 1; //не заглавные буквы и не цифры
    }
    return 0;
}

int main(){
    int osn;
    char number[BUFSIZE]; //текущее число
    long numbers_in10[BUFSIZE]; //все числа в 10 СС для дальнейшего их преобразования
    char * all_numbers[BUFSIZE]; //все числа 
    int count = 0;
    long sum = 0;
    long max = LONG_MIN; //текущий макисмум в 10 сс для сравнения
    char max_number[BUFSIZE];
    char after = '\0';
    printf("Введите основание СС в диапазоне [2;36]\n");
    int n = scanf("%d%c", &osn, &after);
    if(n != 2 || (after != '\0' && after != '\n')){
        fprintf(stderr, "Неправильно введенное основание СС\n");
        return 0;
    }

    if(osn < 2 || osn > 36){
        fprintf(stderr, "Основание СС должно быть в диапазоне [2;36]\n");
        return 0;
    }

    while(fgets(number, sizeof(number), stdin)){
        int len = strlen(number);
        if (len > 0 && number[len - 1] == '\n') {
            number[len - 1] = '\0';
        }

        if(number[0] == '\0') continue;
        
        if(strcmp(number, "Stop") == 0) break;

        if(validate(number, osn) == 1){
            fprintf(stderr, "Неправильно введенное число\n");
            return 0;
        }

        long number_10 = to_des(osn, number);
        if(number_10 == LONG_MAX){
            fprintf(stderr, "Переполнение: число '%s' слишком большое\n", number);
            for(int i = 0; i < count; i++) free(all_numbers[i]);
            return 1;
        }
        if (labs(number_10) > max){
            max = number_10;
            snprintf(max_number, sizeof(max_number), "%s", number);
        }

        sum += number_10;

        numbers_in10[count] = number_10;
        all_numbers[count] = malloc(strlen(number) + 1);
        if (all_numbers[count] == NULL) {
            fprintf(stderr, "Ошибка: не хватило памяти\n");
            return 1;
        }
        strcpy(all_numbers[count], number);

        count ++;
    }

    printf("Максимальное число: %s. Его представление в 10 СС: %ld\n", max_number, max);
    printf("Сумма всех чисел в 10 СС: %ld, сумма всех чисел в заданной СС ", sum);
    des_to_system(osn, sum);

    for(int i = 0; i < count; i++){
        printf("Число %s в 10 СС: %ld\n", all_numbers[i], numbers_in10[i]);
        des_to_system(9, numbers_in10[i]);
        des_to_system(18, numbers_in10[i]);
        des_to_system(27, numbers_in10[i]);
        des_to_system(36, numbers_in10[i]);
    }
    for(int i = 0; i < count; i ++){
        free(all_numbers[i]);
    }
    return 0;
}
