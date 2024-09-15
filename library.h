// library.h
#ifndef LIBRARY_H
#define LIBRARY_H
#define MAX_PATH_SIZE 1024

extern char initial_cwd[MAX_PATH_SIZE];  // Declaration of initial_cwd


// imported functions from library.c
void welcomeAnimation();
void parseInput(char *input, char **args);
int isBackgroundProcess(char **args);
void handleRedirection(char **args);
void getRelativePath(char *relative_path, const char *cwd);
int isBuiltInCommand(char **args);


#endif
