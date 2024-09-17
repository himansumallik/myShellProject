#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For system calls like fork, exec
#include <sys/wait.h> // For wait
#include <fcntl.h>   // For open()
#include <errno.h>   // For errno
#include "library.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_PATH_SIZE 1024
#define GAP 2  // Define a gap between characters



// Function to display the "Welcome to the Shell" animation at the current cursor position
void welcomeAnimation() {
    const char* message = "Welcome to the Shell";
    int length = strlen(message);

    for (int i = 0; i < length; i++) {
        printf("%c", message[i]);
        fflush(stdout);
        usleep(200000);
        for (int j = 0; j < GAP; j++) {
            printf(" ");
        }
    }
    printf("\n");
}




// Function to parse input into arguments
void parseInput(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}


// Function to check if the command is a background process
int isBackgroundProcess(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        i++;
    }
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        args[i - 1] = NULL;
        return 1;
    }
    return 0;
}


// Function to handle I/O redirection
void handleRedirection(char **args) {
    int i = 0;
    int fd;

    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (fd < 0) {
                perror("open failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            return;
        }
        i++;
    }

    i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;
            fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("open failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            return;
        }
        i++;
    }
}





// Function to get the relative path from initial_cwd
void getRelativePath(char *relative_path, const char *cwd) {
    // Check if cwd is a subdirectory of initial_cwd
    if (strncmp(cwd, initial_cwd, strlen(initial_cwd)) == 0) {
        // If inside the initial directory, calculate the relative path
        if (cwd[strlen(initial_cwd)] == '\0') {
            // Special case: if cwd == initial_cwd, set relative_path to "."
            snprintf(relative_path, MAX_PATH_SIZE, ".");
        } else {
            // Use the part of cwd after initial_cwd to get the relative path
            snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd + strlen(initial_cwd));
        }
    } else {
        // If cwd is outside the initial directory, use absolute path
        snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd);
    }
}




// Function to check and handle built-in commands
int isBuiltInCommand(char **args) {
    if (args[0] == NULL) {
        return 0;
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "my-shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }
    return 0;
}