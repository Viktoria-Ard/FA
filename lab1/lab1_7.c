#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int is_alpha(int x) {
    return ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'));
}
int is_separator(int x){
    return x == ' ' || x == '\t' || x == '\n';
}

int is_symb(int x){
    return x != EOF && !is_separator(x);
}

int write_lex(FILE*fin, FILE*fout){ //запсь лексемы в файл
    int x = fgetc(fin);
    while (is_separator(x)) {
        x = getc(fin);
    }
    if (x == EOF) return 0;
    while (is_symb(x)) {
        fputc(x, fout);
        x = fgetc(fin);
    }
    fputc(' ', fout);
    return 1;
}

void flag_r(FILE*file1, FILE*file2, FILE*fout){
    long int pos = 1;
    int x1, x2;
    while (1) {
        if (pos % 2 != 0) {
           x1 = write_lex(file1, fout);
           if(x1 == 0) break;
        }
        else {
            x2 = write_lex(file2, fout);
            if(x2 == 0) break;
        }
        pos++;
    }
    while (1) {
       x1 = write_lex(file1, fout);
       if(x1 == 0) break;
    }
    while (1) {
        x2 = write_lex(file2, fout);
        if(x2 == 0) break;
    }
}

FILE*open_input(char *name){
    FILE* file = fopen(name, "r");
    if (!file) {
        fprintf(stderr, "file %s does not exist.\n",name);
        return NULL;
    }
    return file;
}

FILE*open_output(char*name){
    FILE* file = fopen(name, "w");
    if (!file) {
        fprintf(stderr, "cannot create output file '%s'\n", name);
        return NULL;
    }
    return file;
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

int to4(int x){
        int res = 0;
        int mult = 1;
        while(x > 0){
            res = res + (x % 4) * mult;
            x = x / 4;
            mult *= 10;
        }
        return res;
}

void lex10(FILE*fin, FILE*fout, int x){
    while(is_symb(x)){
        if(is_alpha(x) && (x - 'a') < 0){
            x += 'a' - 'A';
        }
        fprintf(fout, "%d", to4(x));
        x = fgetc(fin);
    }
    ungetc(x, fin);
}

void lex2(FILE*fin, FILE *fout, int x){
    while(is_symb(x)){
        if(is_alpha(x) && (x - 'a') < 0){
            x += 'a' - 'A';
        }
        fputc(x, fout);
        x = fgetc(fin); 
    }
    ungetc(x, fin);
}

int to8(int x){
    int res = 0;
    int mult = 1;
        while(x > 0){
            res = res + mult * (x % 8);
            x = x / 8;
            mult *= 10;
        }
        return res;
}

void lex5(FILE * fin, FILE * fout, int x){
    while(is_symb(x)){
        int res = to8(x);
        fprintf(fout, "%d", res);
        x = fgetc(fin);
    }
    ungetc(x, fin);
}

void flag_a(FILE* fin, FILE* fout){
    int count = 1;
    int x;
    while((x = fgetc(fin)) != EOF){
        if(is_separator(x)){
            while(is_separator(x) && x != EOF) x = fgetc(fin);
            count++;
            if(x == EOF) break;
        }
        if(count % 10 == 0){
            lex10(fin, fout, x);
            fputc(' ', fout);
        }
        else if(count %2 == 0){
            lex2(fin, fout, x);
            fputc(' ', fout);
        }
        else if(count % 5 == 0){
            lex5(fin, fout, x);
            fputc(' ', fout);
        }
        else {
            while (is_symb(x)) {
                fputc(x, fout);
                x = fgetc(fin);
            }
            ungetc(x, fin); 
            fputc(' ', fout);
        }
    }
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s -r <file1> <file2> <output>\n", argv[0]);
		return 1;
	}
	if (argv[1][0] == '/') argv[1][0] = '-';
	if (strcmp(argv[1], "-r") == 0) {
		if (argc != 5) {
			fprintf(stderr, "Usage: %s -r <file1> <file2> <output>\n", argv[0]);
			return 1;
		}
		FILE* file1 = open_input(argv[2]);
		if (!file1) {
			return 1;
		}

		FILE* file2 = open_input(argv[3]);
		if (!file2) {
            fclose(file1);
			return 1;
		}

        int same1 = sameFile(file1, argv[4]);
        int same2 = sameFile(file2, argv[4]);
	
        if(same1 || same2){
            fprintf(stderr, "Error: <inputFile> and <outputFile> must be different\n");
            fclose(file1);
            fclose(file2);
            return 1;
        }
		FILE* fout = open_output(argv[4]);
        if(!fout){
            fclose(file1);
            fclose(file2);
            return 1;
        }
        flag_r(file1, file2, fout);
        fclose(file1);
        fclose(file2);
        fclose(fout);
    }
    else if(strcmp(argv[1], "-a") == 0){
        if(argc != 4){
            fprintf(stderr, "Usage: %s -a <input> <output>\n", argv[0]);
			return 1;
        }
        FILE*fin = open_input(argv[2]);
        if(!fin){
            return 1;
        }

        int same = sameFile(fin, argv[3]);
	
        if(same){
            fprintf(stderr, "Error: <inputFile> and <outputFile> must be different\n");
            fclose(fin);
            return 1;
        }

        FILE*fout = open_output(argv[3]);
        if(!fout){
            fclose(fin);
            return 1;
        }
        flag_a(fin, fout);
        fclose(fin);
        fclose(fout);
    }
    else{
        fprintf(stderr, "Unknown coomand.\ncoomand you can use:\n-r <file1> <file2> <output>\n-a <input> <output>\n");
    }
    return 0;
}
