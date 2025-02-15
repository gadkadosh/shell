#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"

volatile sig_atomic_t received_sigint = 0;
void handle_sigint() { received_sigint = 1; }

static void read_command(char **command, size_t capacity) {
    size_t size = 0;
    int c;
    do {
        c = getc(stdin);
        if (size >= capacity) {
            capacity *= 2;
            char *new_command = realloc(*command, sizeof(char) * capacity);
            if (new_command == NULL) {
                printf("Memory reallocation failed\n");
                exit(1);
            }
            *command = new_command;
        }
        if (c != '\n' && !(c == ' ' && (*command)[size - 1] == ' ')) {
            (*command)[size++] = c;
        }
    } while (c != EOF && c != '\n');
    (*command)[size++] = '\0';
}

static char **create_argv(const char *command) {
    char **argv = malloc(sizeof(*argv) * 10);
    if (argv == NULL) {
        return NULL;
    }

    char *command_copy = strdup(command);
    if (command_copy == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }

    char *token = strtok(command_copy, " ");
    argv[0] = strdup(token);
    int i = 1;
    while ((token = strtok(NULL, " ")) != NULL) {
        argv[i++] = strdup(token);
    }
    argv[i] = NULL;
    return argv;
    free(command_copy);
}

static void free_argv(char **argv) {
    char **tmp = argv;
    while (*tmp != NULL) {
        free(*tmp++);
    }
    free(argv);
}

static int execute_file(char *command, char *argv[], char *envp[]) {
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        if ((status = execve(command, argv, envp)) == -1) {
            printf("Error: %s\n", strerror(errno));
            exit(1);
        }
    } else {
        waitpid(0, &status, 0);
        return WEXITSTATUS(status);
    }
    return 0;
}

static char *search_path(const char *command) {
    const char *path = getenv("PATH");

    char *p = strdup(path), *token;
    if (p == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    DIR *dir;
    struct dirent *entry;

    token = strtok(p, ":");
    while (token != NULL) {
        dir = opendir(token);
        if (dir == NULL) {
            return NULL;
        }
        (void)command;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, command) == 0) {
                size_t length = strlen(token) + entry->d_namlen + 1;
                char *full_command = malloc(sizeof(char) * length);
                sprintf(full_command, "%s/%s", token, entry->d_name);
                return full_command;
            }
        }
        token = strtok(NULL, ":");
    }

    closedir(dir);
    free(p);

    return NULL;
}

static void print_prompt(int exit_status) { printf("[%d] $ ", exit_status); }

int main(int argc, char *argv[], char *envp[]) {
    (void)argc;
    (void)argv;

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    int exit_status = 0;

    while (1) {
        print_prompt(exit_status);

        const size_t capacity = 64;
        char *command_string = malloc(sizeof(char) * capacity);
        if (command_string == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
        read_command(&command_string, capacity);

        if (received_sigint == 1) {
            received_sigint = 0;
            free(command_string);
            putchar('\n');
            continue;
        } else if (command_string[0] == EOF) {
            free(command_string);
            putchar('\n');
            return 0;
        }

        char **argv = create_argv(command_string);
        if (argv == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }

        if (strchr(argv[0], '/') == NULL) {
            builtin_func_t *builtin_func;
            builtin_func = find_builtin(argv[0]);
            if (builtin_func != NULL) {
                exit_status = builtin_func(argv);
            } else {
                char *command = search_path(argv[0]);
                if (command != NULL) {
                    exit_status = execute_file(command, argv, envp);
                    free(command);
                } else {
                    exit_status = execute_file(argv[0], argv, envp);
                }
            }
        } else {
            exit_status = execute_file(argv[0], argv, envp);
        }

        free(command_string);
        free_argv(argv);
    }

    return 0;
}
