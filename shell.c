#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    while (1) {
        char command[128] = {0};
        int size = 0;
        write(STDOUT_FILENO, "$ ", 2);

        size = read(STDIN_FILENO, command, 128);
        if (command[size - 1] == '\n') {
            command[size - 1] = '\0';
        }

        if (size == 1) {
            continue;
        }

        pid_t pid = fork();
        int ret = 0;
        if (pid == 0) {
            if ((ret = execve(command, NULL, NULL)) == -1) {
                printf("Error: %s\n", strerror(errno));
            }
        } else {
            waitpid(0, &ret, 0);
            printf("returned: %d\n", WEXITSTATUS(ret));
        }
    }

    return 0;
}
