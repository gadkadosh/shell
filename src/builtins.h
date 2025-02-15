typedef int(builtin_func_t)(char *args[]);

typedef struct Builtin {
    char *name;
    builtin_func_t *func;
} builtin_t;

builtin_func_t *find_builtin(char *command);
