#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void pwd() {
    char *cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd);
}

static void cd(char *filename) {
    int ret = chdir(filename);
    if (ret == -1) {
        printf("Error: %s\n", strerror(errno));
    }
}

int execute_builtin(char **argv) {
    if (strcmp(argv[0], "pwd") == 0) {
        pwd();
    } else if (strcmp(argv[0], "cd") == 0) {
        cd(argv[1]);
    } else {
        return -1;
    }
    return 0;
}
