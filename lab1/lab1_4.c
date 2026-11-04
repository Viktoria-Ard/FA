#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int is_alpha(char c) {
	return((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int is_digit(char c){
	return (c >= '0' && c<= '9');
}

int is_symb(char c){
	return (!is_alpha(c) && !is_digit(c) && c != ' ' && c!= '\n');
}

void delete_digits(FILE*fin, FILE*fout){
	int c;
		while ((c = fgetc(fin)) != EOF) {
			if (!is_digit(c)) {
				fputc(c, fout);
			}
		}
}

void count_alph(FILE*fin, FILE*fout){
	char line[1024];
	while (fgets(line, sizeof(line), fin) != NULL) {
		int count = 0;
		for (int i = 0; line[i]; i++) {
			if (is_alpha(line[i])) count++;
		}
		fprintf(fout,"%d\n",count);
	}
}

void count_symb(FILE*fin, FILE*fout){
	char line[1024];
		while (fgets(line, sizeof(line), fin) != NULL) {
			int c = 0;
			for (int i = 0; line[i]; i++) {
				if (is_symb(line[i])) c++;
			}
			fprintf(fout, "%d\n", c);
		}
}

void replace_hex(FILE*fin, FILE*fout){
	int c;
		while ((c = getc(fin)) != EOF) {
			if (is_digit(c)) fprintf(fout, "%d", c);
			else fprintf(fout, "%X", c);
		}
}

FILE* open_input(char *name){
	FILE * fin = fopen(name, "r");
	if(!fin){
		fprintf(stderr, "Cannot open input file '%s'\n", name);
    }
    return fin;
}

FILE* open_output(char *name){
	FILE * fout = fopen(name, "w");
	if(!fout){
	    fprintf(stderr, "Cannot open output file '%s'\n", name);
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

int make_out_name(char * out_name, size_t size, char *input){
	int n = snprintf(out_name, size, "out_%s", input);
	return (n >= 0 && n < size);
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
        fprintf(stderr, "Usage: %s <-d|-i|-s|-a> <input> [output]\n", argv[0]);
        return 1;
    }
	if (argv[1][0] == '/') argv[1][0] = '-';
	char *input_name = argv[2];
	char output_name[512];
	if (argv[1][1] == 'n') {
		if (argc != 4) {
			fprintf(stderr, "Usage: <name> <-d|-i|-s|-a> <input> <output>\n");
			return 1;
		}
		snprintf(output_name, sizeof(output_name), "%s", argv[3]);
	}
	else {
		if (argc != 3) {
			fprintf(stderr, "Usage: <name> <-d|-i|-s|-a> <input>\n");
			return 1;
		}
		if (!make_out_name(output_name, sizeof(output_name), input_name)) {
            fprintf(stderr, "Error: output name too long\n");
            return 1;
        }
	}
	FILE *fin = open_input(input_name);
	if (!fin) return 1;

	int same = sameFile(fin, output_name);
	
	if(same){
		fprintf(stderr, "Error: <inputFile> and <outputFile> must be different\n");
		fclose(fin);
		return 1;
	}
	
	FILE *fout = open_output(output_name);
	if (!fout) {
		fclose(fin);
		return 1;
	}
	
	if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-nd") == 0) {
		delete_digits(fin, fout);
	}
	else if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "-ni") == 0) {
		count_alph(fin, fout);
	}
	else if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-ns") == 0) {
		count_symb(fin, fout);
	}
	else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "-na") == 0) {
		replace_hex(fin, fout);
	}
	else {
		fprintf(stderr, "Usage: <name> <-d|-i|-s|-a> <input>\nor\n<name> <-d|-i|-s|-a> <input> <output>");
		fclose(fin);
		fclose(fout);
		return 1;
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
