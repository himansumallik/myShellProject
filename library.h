#ifndef LIBRARY_H
#define LIBRARY_H

#define MAX_PATH_SIZE 1024

// Declaration of initial_cwd
extern char initial_cwd[MAX_PATH_SIZE];

// Function prototypes
void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);


// Declaration for tab completion
char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);

#endif // LIBRARY_H
