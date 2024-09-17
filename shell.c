#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For system calls like fork, exec
#include <sys/wait.h> // For wait
#include <fcntl.h>   // For open()
#include <errno.h>   // For errno
#include <readline/readline.h>
#include <readline/history.h>
#include "library.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_PATH_SIZE 1024
char initial_cwd[MAX_PATH_SIZE];

void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);

// Main function
int main() {
    char *input;
    char *args[MAX_ARGS];

    if (getcwd(initial_cwd, sizeof(initial_cwd)) == NULL) {
        perror("getcwd failed");
        exit(1);
    }
    printf("Initial directory: %s\n", initial_cwd);

    while (1) {
        char cwd[MAX_PATH_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            char relative_path[MAX_PATH_SIZE];
            getRelativePath(relative_path, cwd);
            printf("Relative path: %s\n", relative_path);

            char prompt[MAX_PATH_SIZE + 20];
            snprintf(prompt, sizeof(prompt), "my-shell%s> ", relative_path);

            input = readline(prompt);
        } else {
            perror("getcwd failed");
            continue;
        }

        if (input == NULL) {
            printf("\n");
            exit(0);
        }

        if (*input) {
            add_history(input);
        }

        input[strcspn(input, "\n")] = 0;
        parseInput(input, args);

        if (isBuiltInCommand(args)) {
            free(input);
            continue;
        }

        int background = isBackgroundProcess(args);
        handleRedirection(args);

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else if (pid > 0) {
            if (!background) {
                wait(NULL);
            }
        } else {
            perror("fork failed");
        }

        free(input);
    }

    return 0;
}