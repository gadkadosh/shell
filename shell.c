#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void read_command(char **command, size_t capacity) {
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

    /*printf("command: %s size: %zu capacity: %zu\n", *command, size,
     * capacity);*/
}
volatile sig_atomic_t received_sigint = 0;

void handle_sigint() { received_sigint = 1; }

int main(void) {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    int exit_status = -1;

    char prompt[64];

    while (1) {
        if (exit_status > -1) {
            sprintf(prompt, "[%d] $ ", exit_status);
            exit_status = -1;
        } else {
            sprintf(prompt, "$ ");
        }
        write(STDOUT_FILENO, prompt, strlen(prompt));

        size_t capacity = 64;
        char *command = malloc(sizeof(char) * capacity);
        if (command == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        read_command(&command, capacity);

        if (received_sigint == 1) {
            received_sigint = 0;
            free(command);
            putchar('\n');
            continue;
        } else if (command[0] == EOF) {
            free(command);
            return 0;
        }

        int status;
        pid_t pid = fork();
        if (pid == 0) {
            if ((status = execve(command, NULL, NULL)) == -1) {
                printf("Error: %s\n", strerror(errno));
                return 1;
            }
        } else {
            waitpid(0, &status, 0);
            exit_status = WEXITSTATUS(status);
        }

        free(command);
    }

    return 0;
}
