#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For system calls like fork, exec
#include <sys/wait.h> // For wait
#include <fcntl.h>   // For open()
#include <errno.h>   // For errno
#include <dirent.h>  // For reading directories
#include <readline/readline.h>
#include <readline/history.h>
#include "library.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_PATH_SIZE 1024
#define GAP 2  // Define a gap between characters

char initial_cwd[MAX_PATH_SIZE];  // Definition of initial_cwd
// List of built-in commands (you can extend this as needed)
const char *built_in_commands[] = {"cd", "exit", "help", NULL};

// Function to display the "Welcome to the Shell" animation at the current cursor position
void welcomeAnimation() {

    const char* message = "Welcome to the Shell";
    int length = strlen(message);  // Find the length of the message

    // Loop through each character and display it one by one
    for (int i = 0; i < length; i++) {
        printf("%c", message[i]);  // Print each character
        fflush(stdout);  // Flush the output to make sure it appears immediately
        usleep(200000);  // 200 ms delay for animation effect

        // Print the gap between characters if needed
        for (int j = 0; j < GAP; j++) {
            printf(" ");  // Print spaces for the gap
        }
    }
    printf("\n");  // Move to the next line after the animation completes
}





// Function to parse input into arguments
void parseInput(char *input, char **args) {
    //printf("from parseinput..\n");

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
    //printf("from backgroundprocess..\n");
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

    // Input redirection
    i = 0;  // Reset index for input redirection
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;  // Terminate arguments list
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
    // Check if cwd is exactly the initial directory
    if (strcmp(cwd, initial_cwd) == 0) {
        // If in the initial directory, set relative path to "."
        snprintf(relative_path, MAX_PATH_SIZE, ".");
    } else if (strncmp(cwd, initial_cwd, strlen(initial_cwd)) == 0) {
        // If in a subdirectory, show the relative path
        snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd + strlen(initial_cwd));
    } else {
        // If not inside the initial directory, use the absolute path
        snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd);
    }
}




// Function to check and handle built-in commands
int isBuiltInCommand(char **args) {
    //printf("from builtin..\n");
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
        }
        return 1;  // Built-in command executed
    }
    return 0;  // Not a built-in command
}



// Function to match commands and file paths
char **command_completion(const char *text, int start, int end) {
    rl_attempted_completion_over = 1; // Attempt custom completion

    (void)end;  // Mark 'end' as unused to prevent warnings

    if (start == 0) {
        // Complete commands
        return rl_completion_matches(text, command_generator);
    } else {
        // Complete file paths
        return rl_completion_matches(text, rl_filename_completion_function);
    }
}

// Generator for command completion
char *command_generator(const char *text, int state) {
    static int list_index, len;
    const char *command;

    // Initialize if it's the first call
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    // Iterate over built-in commands to find a match
    while ((command = built_in_commands[list_index++])) {
        if (strncmp(command, text, len) == 0) {
            return strdup(command);
        }
    }

    return NULL;  // No more matches
}