#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_sigint() {}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    int status = 0;
    int exit_status = -1;

    char prompt[64] = "$ ";

    while (1) {
        char command[128] = {0};
        int size = 0;

        if (exit_status > -1) {
            sprintf(prompt, "%d $ ", exit_status);
        } else {
            sprintf(prompt, "$ ");
        }
        write(STDOUT_FILENO, prompt, 64);

        size = read(STDIN_FILENO, command, 128);

        if (size == -1) {
            putchar('\n');
            continue;
        } else if (size == 0) {
            return 0;
        } else if (size == 1) {
            continue;
        } else if (command[size - 1] == '\n') {
            command[size - 1] = '\0';
        }

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
    }

    return 0;
}
