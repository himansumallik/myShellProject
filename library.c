#include <stdio.h>
#include <unistd.h>  // For usleep()
#include <string.h>  // For strlen()

#define GAP 2  // Define a gap between characters

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


