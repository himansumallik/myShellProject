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

char initial_cwd[MAX_PATH_SIZE];  // To store the initial working directory
void welcomeAnimation();


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
        args[i - 1] = NULL;  // Remove '&' from args
        return 1;  // Indicate background process
    }
    return 0;  // Foreground process
}

// Function to handle I/O redirection
void handleRedirection(char **args) {
    int i = 0;
    int fd;

    // Output redirection
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;  // Terminate arguments list
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

    // Input redirection (if needed, can be implemented similarly)
}

//function to get the relative directory
// Function to compute relative path
void getRelativePath(char *relative_path, const char *cwd) {
    if (strncmp(cwd, initial_cwd, strlen(initial_cwd)) == 0) {
        snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd + strlen(initial_cwd));
    } else {
        snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd);  // Use absolute path if not a subdirectory
    }
}

// Function to check and handle built-in commands
int isBuiltInCommand(char **args) {
    if (args[0] == NULL) {
        return 0;  // No command entered
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "my-shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            } 
            char cwd[MAX_PATH_SIZE];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                char relative_path[MAX_PATH_SIZE];
                getRelativePath(relative_path, cwd);
                printf("my-shell%s> ", relative_path);
            } else {
                perror("getcwd failed");
            }
        }
        return 1;  // Built-in command executed
    }
    return 0;  // Not a built-in command
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    
    while (1) {  // Infinite loop for the shell

        // Display the shell animation at startup
        void welcomeAnimation();  


        // Store the initial working directory
        if (getcwd(initial_cwd, sizeof(initial_cwd)) == NULL) {
            perror("getcwd failed");
            exit(1);
        }

        printf("my-shell> ");  // Display prompt

        // Read input from the user
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        // Parse the input into arguments
        parseInput(input, args);

        // Check if it's a built-in command
        if (isBuiltInCommand(args)) {
            continue;  // Built-in command handled, go to the next iteration
        }

        // Check if it's a background process
        int background = isBackgroundProcess(args);

        // Handle I/O redirection
        handleRedirection(args);

        // Fork a child process to execute the command
        pid_t pid = fork();
        if (pid == 0) {
            // Child process: Execute command
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else if (pid > 0) {
            // Parent process: Wait for the child to finish unless it's a background process
            if (!background) {
                wait(NULL);
            }
        } else {
            perror("fork failed");
        }
    }

    return 0;
}
