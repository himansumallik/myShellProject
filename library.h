#ifndef LIBRARY_H
#define LIBRARY_H

#ifdef _WIN32
    #include <direct.h>  // For _getcwd, _chdir on Windows
    #include <io.h>      // For _open, _dup2
    #include <fcntl.h>   // For file operations
    #define usleep(x) Sleep((x) / 1000)  // Convert usleep to Sleep for Windows
#else
    #include <unistd.h>  // For usleep(), getcwd(), chdir() on Linux
    #include <fcntl.h>   // For file operations
#endif

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_PATH_SIZE 1024

extern char initial_cwd[MAX_PATH_SIZE];  // Declare initial_cwd as external


void initializeShell();                   // Initializes the shell and sets the initial directory
void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);
char *command_generator(const char *text, int state);
char **command_completion(const char *text, int start, int end);
void displayPrompt();

#endif // LIBRARY_H
