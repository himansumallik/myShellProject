#include <stdio.h>
#include <unistd.h>  // For usleep()
#include <string.h>  // For strlen()
#include "library.h"

#define WIDTH 40
#define HEIGHT 10

// Function to display the "Welcome to the Shell" animation
void welcomeAnimation()
{
    const char* message = "Welcome to the Shell";
    int length = strlen(message);  // Using strlen to find the length of the message
    
    int startRow = HEIGHT / 2;  // Row where text will appear
    int startCol = (WIDTH - length) / 2;  // Center text horizontally

    // Function to clear the screen using ANSI escape codes
    void clearScreen() {
        printf("\033[H\033[J");  // Move cursor to the top left and clear the screen
    }

    // Function to position the cursor using ANSI escape codes
    void setCursorPosition(int row, int col) {
        printf("\033[%d;%dH", row, col);
    }

    // Loop through each character and display it one by one
    for (int i = 0; i < length; i++) {
        clearScreen();  // Clear the screen each time
        setCursorPosition(startRow, startCol + i);  // Position for the next character
        printf("%c", message[i]);  // Print each character
        fflush(stdout);  // Flush the output to make sure it appears immediately
        usleep(200000);  // 200 ms delay for animation effect
    }
}


