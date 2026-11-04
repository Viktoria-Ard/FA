typedef struct Employee{
    long id;
    char *name;
    char *surname;
    double salary;
}Employee;

int len_str(char *s){
    int n = 0;
    while(s[n]) n++;
    return n;
}

char *copy(char *s){
    int n = len_str(s);
    char *strcp = malloc(n + 1);
    if(!strcp) return NULL;
    for(int i = 0; i <= n; i++) strcp[i] = s[i];
    return strcp;
}

Employee create(long id, char *name, char *surname, double salary){
    Employee man;
    man.id = id;
    man.name = copy(name);
    man.surname = copy(surname);
    man.salary = salary;
    return man;
}

long get_id(const Employee *man){
    return man->id;
}

const char *get_name(const Employee *man){
    return man->name;
}

const char *get_surname(const Employee *man){
    return man->surname;
}

double get_salary(const Employee *man){
    return man->salary;
}

