#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32
    #include <windows.h>  // Windows-specific headers
    #include <direct.h>   // For _getcwd, _chdir on Windows
    #define clearScreen() system("cls")
    #define getcwd _getcwd  // For Windows, map getcwd to _getcwd
    #include <shlobj.h> // For SHGetFolderPath and CSIDL constants

#else
    #include <unistd.h>   // Unix/Linux-specific headers
    #include <sys/wait.h> // For wait()
    #include <fcntl.h>    // For file control
    #define clearScreen() printf("\033[H\033[J")
#endif

#ifndef PATH_MAX
    #define PATH_MAX MAX_PATH
#endif


#include <errno.h>        // For errno
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_PATH_SIZE 1024

char initial_cwd[MAX_PATH_SIZE];

void initializeShell();
void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);
char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);
void displayPrompt();
void executeCommand(char **args);


// Main function
int main() {
    char *args[MAX_ARGS];
    char relative_path[MAX_PATH_SIZE];

    // Initialize the shell
    initializeShell();

    // Get the initial working directory
    if (getcwd(initial_cwd, sizeof(initial_cwd)) == NULL) {
        perror("my-shell: getcwd failed");
        return 1;
    }

    // Welcome message
    welcomeAnimation();

    // Main shell loop
    while (1) {
        // Get the current working directory (relative to the initial directory)
        char cwd[MAX_PATH_SIZE];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("my-shell: getcwd failed");
            continue;
        }
        getRelativePath(relative_path, cwd);

        // Display prompt (relative path)
        printf("%s> ", relative_path);

        // Read input using readline
        char *input_line = readline("");
        if (input_line == NULL) {
            continue;
        }

        // Add to history
        add_history(input_line);

        // Parse the input into arguments
        parseInput(input_line, args);

        // Check for built-in commands
        if (isBuiltInCommand(args)) {
            free(input_line);
            continue;  // If it's a built-in command, execute and continue
        }

        // Execute external commands (cross-platform)
        executeCommand(args);

        // Free the input line memory
        free(input_line);
    }

    return 0;
}

// Define your helper functions as needed...
