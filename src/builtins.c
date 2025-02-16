#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"

static int shelly_exit(char *argv[]) {
    (void)argv;
    exit(0);
    return 0;
}

static int pwd(char *argv[]) {
    (void)argv;
    char *cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd);
    return 0;
}

static int cd(char *argv[]) {
    char *filename = argv[1];
    if (filename == NULL) {
        filename = getenv("HOME");
    }
    int ret = chdir(filename);
    if (ret == -1) {
        printf("Error(cd): %s\n", strerror(errno));
    }
    return ret;
}

static const builtin_t builtins[] = {
    {.name = "exit", .func = shelly_exit},
    {.name = "cd", .func = cd},
    {.name = "pwd", .func = pwd},
};

builtin_func_t *find_builtin(char *command) {
    (void)command;
    (void)builtins;
    int length = sizeof(builtins) / sizeof(builtin_t);

    for (int i = 0; i < length; i++) {
        if (strcmp(builtins[i].name, command) == 0) {
            return builtins[i].func;
        }
    }

    return NULL;
}
