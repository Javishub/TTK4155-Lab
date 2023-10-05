#include <stdio.h>

// Joystick states
enum JoystickState {
    CENTER,
    DOWN,
    UP,
    LEFT,
    RIGHT
};

// Function to read the joystick state (you should implement this)
// Returns 0 for center, 1 for down, 2 for up, 3 for left, 4 for right
int readJoystick() {
    // Replace this with your actual joystick reading logic
    // This is a placeholder implementation.
    // You should read the joystick input and return the appropriate value.
    return 0; // Assuming the joystick is initially in the center position.
}

int main() {
    enum JoystickState currentState = CENTER;
    enum JoystickState prevState = CENTER;

    while (1) {
        int joystickInput = readJoystick();

        if (joystickInput != prevState) {
            // Joystick state has changed
            prevState = currentState;

            // Handle state transitions
            switch (joystickInput) {
                case DOWN:
                    currentState = DOWN;
                    printf("Joystick is down.\n");
                    break;
                case UP:
                    currentState = UP;
                    printf("Joystick is up.\n");
                    break;
                case LEFT:
                    currentState = LEFT;
                    printf("Joystick is left.\n");
                    break;
                case RIGHT:
                    currentState = RIGHT;
                    printf("Joystick is right.\n");
                    break;
                case CENTER:
                    // Joystick is back to center, but we retain the previous state.
                    printf("Joystick is back to center.\n");
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}
