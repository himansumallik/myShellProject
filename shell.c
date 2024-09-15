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



//function declared and imported from library.h file
void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);


int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];

    // Display the shell animation at startup
    welcomeAnimation(); 
    
    // Store the initial working directory
    if (getcwd(initial_cwd, sizeof(initial_cwd)) == NULL) {
        perror("getcwd failed");
        exit(1);
    }

    while (1) {  // Infinite loop for the shell 

        // Get the current working directory
        char cwd[MAX_PATH_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            char relative_path[MAX_PATH_SIZE];
            getRelativePath(relative_path, cwd);  // Compute relative path
            printf("my-shell%s> ", relative_path);  // Print prompt with relative path
        } else {
            perror("getcwd failed");
            continue;  // Skip to the next iteration if getcwd fails
        }

        // Read input from the user
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            perror("fgets failed");
            continue;  // Skip to the next iteration if fgets fails
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
