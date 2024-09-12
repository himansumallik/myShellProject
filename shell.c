#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For system calls like fork, exec
#include <sys/wait.h> // For wait()

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    
    while (1) {  // Infinite loop for the shell
        printf("my-shell> ");  // Display prompt
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        
        // Remove newline character
        input[strcspn(input, "\n")] = 0;
        
        // Split input into arguments
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Exit if command is "exit"
        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            break;
        }
        

        //Handling for Built-in Commands
        if (args[0] == NULL) {
            continue;  // Empty command
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL || chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }

        // Fork a child process to execute the command
        pid_t pid = fork();
        if (pid == 0) {
            // Child process: Execute command
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else if (pid > 0) {
            // Parent process: Wait for the child to finish
            wait(NULL);
        } else {
            perror("fork failed");
        }
    }

    return 0;
}
