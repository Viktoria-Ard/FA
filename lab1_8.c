#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include "lab1_8_struct.h"

int is_alpha(int x) {
    return ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'));
}
int is_separator(int x){
    return x == ' ' || x == '\t' || x == '\n';
}

int tolower(int x){
    if(x >= 'A' && x <= 'Z') x = x + 'a' - 'A';
    return x;
}

FILE*open_in(char *name){
    FILE* fin = fopen(name, "r");
    if(!fin){
        fprintf(stderr, "file %s does not exist.\n",name);
        return NULL;
    }
    return fin;
}

FILE*open_out(char *name){
    FILE* fout = fopen(name, "w");
    if(!fout){
        fprintf(stderr, "file %s does not exist.\n",name);
        return NULL;
    }
    return fout;
}

int sameFile(FILE* fin, char * path2){ //проверка, не одинаковы ли файлы
	FILE* file2 = fopen(path2, "r");
	if(file2 == NULL){
		return 0;
	}

	struct stat s1, s2; //объявляем переменные типа struct stat для хранения данных о файлах
	int fd1 = fileno(fin); //получаем файловые дискрипторы
	int fd2 = fileno(file2);

	int result = 0; 

	if(fd1 != -1 && fd2 != -1 && 
		fstat(fd1, &s1) == 0 && fstat(fd2, &s2) == 0){ //проверка дескрипторов и запись данных в s1, s2
		if(s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino){ 
			result = 1;
		}
	}
	fclose(file2);
	return result;

}

int osn(int x){
    if(is_alpha(x)){
        x = tolower(x) - 'a' + 10;
    }
    else x = x - '0';
    return x;
}

long long transform(char * x, int min_ss){
    long long ans = 0;
    for(int i = 0; x[i]; i++){
        int digit = osn(x[i]);
        if(ans > (LLONG_MAX - digit) / min_ss){
            return -1;
        }
        ans = ans * min_ss + osn(x[i]);
    }
    return ans;
}

Number command(FILE* fin, int x){
    int cap = 16;
    int n = 0;
    char * buf = malloc(sizeof(char) * cap);
    if(!buf){
        Number bad = {NULL, 0, 0};
        return bad;
    }
    int min_ss = 2;
    
    while(!is_separator(x) && x!= EOF){
        if(n + 1 >= cap){
            cap *= 2;
            char * temp = realloc(buf, sizeof(char) * cap);
            if(!temp){
                free(buf);
                Number bad = {NULL, 0, 0};
                return bad;
            }
            buf = temp;
        }
        buf[n] = x;
        n++;
        if(osn(x) + 1 > min_ss) min_ss = osn(x) + 1;
        x = getc(fin);
    }
    buf[n] = '\0';

    char *p = buf;
    while(*p == '0' && p[1]) p++;

    Number res;
    res.s = malloc(strlen(p) + 1);

    if(!res.s){
        free(buf);
        Number bad = {NULL, 0, 0};
        return bad;
    }

    strcpy(res.s, p);
    res.min_ss = min_ss;
    res.ans = transform(p, min_ss);

    free(buf);
    return res;
}


int main(int argc, char * argv[]){
    if(argc != 3){
        printf("USAGE: %s <infile> <outfile>\n", argv[0]);
        return 1;
    }

    FILE* fin = open_in(argv[1]);
    if(!fin) return 1;

    int same = sameFile(fin, argv[2]);

    if(same){
        fprintf(stderr, "Error: <inputFile> and <outputFile> must be different\n");
        fclose(fin);
        return 1;
    }

    FILE* fout = open_out(argv[2]);
    if(!fout){
        fclose(fin);
        return 1;
    }

    int x;
    while((x = fgetc(fin)) != EOF){
        while(is_separator(x)) x = fgetc(fin);
        if(x == EOF) break;
        Number ans = command(fin, x);

        if(ans.s == NULL){
            fprintf(stderr, "Error processing input\n");
            fclose(fin);
            fclose(fout);
            return 1; 
        }

        if(ans.ans == -1){   // переполнение
            fprintf(stderr, "Error: overflow for number '%s'\n", ans.s);
            free(ans.s);
            fclose(fin);
            fclose(fout);
            return 1;
        }

        fprintf(fout, "%s %d %lld\n", ans.s, ans.min_ss, ans.ans);
        fputc(' ', fout);
        free(ans.s);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}