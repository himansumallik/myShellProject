#ifdef _WIN32
    #include <windows.h>  // For Sleep() and CreateProcess()
    #include <direct.h>   // For _getcwd and _chdir on Windows
    #include <io.h>       // For _open() and _dup2()
    #include <shlobj.h> // For SHGetFolderPath and CSIDL constants

#else
    #include <unistd.h>   // For usleep(), getcwd(), chdir()
    #include <fcntl.h>    // For open(), dup2()
    #include <sys/types.h>
    #include <sys/wait.h> // For fork(), wait()
    #include <errno.h>    // For errno
#endif

#ifndef PATH_MAX
    #define PATH_MAX MAX_PATH
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "library.h"

#define MAX_PATH_SIZE 1024
#define GAP 2  // Define a gap between characters

extern char initial_cwd[MAX_PATH_SIZE];  // Declared in shell.c

// List of built-in commands
const char *built_in_commands[] = {
    "cd", "exit", "help", "pwd", "echo", "history", "clear", NULL
};

// Function to set the initial directory to the user's home directory
void setInitialDirectory() {
#ifdef _WIN32
    char homeDir[MAX_PATH_SIZE];
    if (SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, homeDir) == S_OK) {
        if (SetCurrentDirectory(homeDir)) {
            printf("Starting in home directory: %s\n", homeDir);
        } else {
            fprintf(stderr, "Error setting initial directory: %lu\n", GetLastError());
        }
    } else {
        fprintf(stderr, "Error retrieving home directory\n");
    }
#else
    // For Unix/Linux, you can use getenv to get HOME
    char *homeDir = getenv("HOME");
    if (homeDir != NULL) {
        if (chdir(homeDir) != 0) {
            perror("Error setting initial directory");
        } else {
            printf("Starting in home directory: %s\n", homeDir);
        }
    } else {
        fprintf(stderr, "HOME environment variable not set\n");
    }
#endif
}

// Initialize shell and set initial directory
void initializeShell() {
    setInitialDirectory();
    // Additional initialization can go here...
}



// Check if the given command is built-in and execute it
int isBuiltInCommand(char **args) {
    if (args[0] == NULL) {
        return 0;  // Empty command
    }

    // Handle "exit" command
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // Handle "cd" command
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "my-shell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("my-shell: cd failed");
            }
        }
        return 1;
    }

    // Handle "pwd" command
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_PATH_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("my-shell: pwd failed");
        }
        return 1;
    }

    // Handle "clear" command with cross-platform compatibility
    if (strcmp(args[0], "clear") == 0) {
        #ifdef _WIN32
            system("cls");  // Windows clear command
        #else
            printf("\033[H\033[J");  // ANSI escape code for Unix/Linux
        #endif
        return 1;
    }

    // Handle "help" command
    if (strcmp(args[0], "help") == 0) {
        printf("my-shell supports the following built-in commands:\n");
        for (int i = 0; built_in_commands[i] != NULL; i++) {
            printf("  %s\n", built_in_commands[i]);
        }
        return 1;
    }

    return 0;
}


// Function to display the "Welcome to the Shell" animation at the current cursor position
void welcomeAnimation() {
    const char* message = "Welcome to the Shell";
    int length = strlen(message);

    for (int i = 0; i < length; i++) {
        printf("%c", message[i]);
        fflush(stdout);

        #ifdef _WIN32
            Sleep(200);  // Sleep for 200 milliseconds on Windows
        #else
            usleep(200000);  // Sleep for 200 milliseconds on Linux
        #endif

        for (int j = 0; j < GAP; j++) {
            printf(" ");
        }
    }
    printf("\n");
}

// Function to execute a command (cross-platform)
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

void executeCommand(char **args) {
#ifdef _WIN32
    // Map Linux commands to their Windows equivalents
    if (strcmp(args[0], "ls") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "dir";
        args[3] = NULL;
    } else if (strcmp(args[0], "mkdir") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "mkdir";
    } else if (strcmp(args[0], "rm") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "del";
    } else if (strcmp(args[0], "touch") == 0) {
        char *filename = args[1];
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "type nul >";
        args[3] = filename;
        args[4] = NULL;
    } else if (strcmp(args[0], "cp") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "copy";
    } else if (strcmp(args[0], "mv") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "move";
    } else if (strcmp(args[0], "cat") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "type";
    } else if (strcmp(args[0], "clear") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "cls";
        args[3] = NULL;
    } else if (strcmp(args[0], "pwd") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "cd";
        args[3] = NULL;
    } else if (strcmp(args[0], "rmdir") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "rmdir";
    } else if (strcmp(args[0], "echo") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "echo";
    } else if (strcmp(args[0], "whoami") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "whoami";
    } else if (strcmp(args[0], "date") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "date /T";
        args[3] = NULL;
    } else if (strcmp(args[0], "time") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "time /T";
        args[3] = NULL;
    } else if (strcmp(args[0], "find") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "findstr";
    } else if (strcmp(args[0], "grep") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "findstr";
    } else if (strcmp(args[0], "ps") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "tasklist";
    } else if (strcmp(args[0], "kill") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "taskkill /PID";
    } else if (strcmp(args[0], "ifconfig") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "ipconfig";
    } else if (strcmp(args[0], "shutdown") == 0) {
        args[0] = "cmd.exe";
        args[1] = "/C";
        args[2] = "shutdown /s";
    } else {
        // Prepare command string for other unmapped commands
        char command[MAX_INPUT_SIZE] = "cmd.exe /C ";
        for (int i = 0; args[i] != NULL; i++) {
            strcat(command, args[i]);
            strcat(command, " ");
        }

        // Create pipes for output redirection
        HANDLE hReadPipe, hWritePipe;
        SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

        if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
            fprintf(stderr, "CreatePipe failed: %lu\n", GetLastError());
            return;
        }

        // Set the read end of the pipe to be inherited by the child process
        SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdOutput = hWritePipe;  // Redirect standard output
        si.hStdError = hWritePipe;    // Redirect standard error
        si.dwFlags |= STARTF_USESTDHANDLES;

        // Create the process
        char commandCopy[MAX_INPUT_SIZE];
        strcpy(commandCopy, command);

        if (!CreateProcess(NULL, commandCopy, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "CreateProcess failed: %lu\n", GetLastError());
        } else {
            CloseHandle(hWritePipe);  // Close the write end of the pipe

            // Read output from the read end of the pipe
            char buffer[4096];
            DWORD bytesRead;
            while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';  // Null-terminate the string
                printf("%s", buffer);  // Print the output
            }

            CloseHandle(hReadPipe);  // Close the read end of the pipe
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
#else
    // Linux/Unix: Use fork and execvp
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        handleRedirection(args);
        if (execvp(args[0], args) == -1) {
            perror("exec failed");
        }
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
#endif
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

// Function to handle I/O redirection (cross-platform)
void handleRedirection(char **args) {
    int i = 0;
    int fd;

    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;
            #ifdef _WIN32
                fd = _open(args[i + 1], _O_WRONLY | _O_CREAT | _O_TRUNC, 0);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                _dup2(fd, _fileno(stdout));
                _close(fd);
            #else
                fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            #endif
            return;
        }
        i++;
    }

    i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;
            #ifdef _WIN32
                fd = _open(args[i + 1], _O_RDONLY);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                _dup2(fd, _fileno(stdin));
                _close(fd);
            #else
                fd = open(args[i + 1], O_RDONLY);
                if (fd < 0) {
                    perror("open failed");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            #endif
            return;
        }
        i++;
    }
}

// Function to get the relative path from initial_cwd
void getRelativePath(char *relative_path, const char *cwd) {
    if (strncmp(cwd, initial_cwd, strlen(initial_cwd)) == 0) {
        if (cwd[strlen(initial_cwd)] == '\0') {
            snprintf(relative_path, MAX_PATH_SIZE, ".");
        } else {
            snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd + strlen(initial_cwd));
        }
    } else {
        snprintf(relative_path, MAX_PATH_SIZE, "%s", cwd);
    }
}

// Command generator for command completion
char *command_generator(const char *text, int state) {
    static int list_index, len;
    const char *command;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((command = built_in_commands[list_index++])) {
        if (strncmp(command, text, len) == 0) {
            return strdup(command);
        }
    }

    return NULL;
}

// Command completion function for readline
char **command_completion(const char *text, int start, int end) {
    rl_attempted_completion_over = 1;
    (void)end;

    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    } else {
        return rl_completion_matches(text, rl_filename_completion_function);
    }
}

// Function to check if a command is a background process
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


void displayPrompt() {
    char cwd[MAX_PATH_SIZE];
    char relative_path[MAX_PATH_SIZE];

    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        return;
    }

    // Get the relative path from initial_cwd
    getRelativePath(relative_path, cwd);

    // Display the prompt
    printf("%s> ", relative_path);
    fflush(stdout);
}