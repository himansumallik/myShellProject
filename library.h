#ifndef LIBRARY_H
#define LIBRARY_H

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define MAX_PATH_SIZE 1024
extern char initial_cwd[MAX_PATH_SIZE]; 

void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);

#endif // LIBRARY_H
