#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

// Define constants for shortcut codes
#define SHORTCUT_PE 3
#define SHORTCUT_CAP 14
#define SHORTCUT_RAM 40

// Structure to represent the state of the keyboard
struct KeyboardState {
    int keys[KEY_CNT];
};

// Function to handle specific keyboard shortcuts
void handleShortcuts(const struct KeyboardState *state);

// Function to get the string representation of an event type
const char *getEventType(int value);

// Function to monitor the keyboard input
void monitorKeyboard(const char *device_path, const char *output_file);


int main() {
    const char *device_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    const char *output_file = "ex1.txt";
    monitorKeyboard(device_path, output_file);
    return 0;
}

// Function to handle specific keyboard shortcuts based on the keyboard state
void handleShortcuts(const struct KeyboardState *state) {
    if (state->keys[KEY_P] && state->keys[KEY_E]) {
        printf("Shortcut: P+E -> \"I passed the Exam!\"\n");
    } else if (state->keys[KEY_C] && state->keys[KEY_A] && state->keys[KEY_P]) {
        printf("Shortcut: C+A+P -> \"Get some cappuccino!\"\n");
    } else if (state->keys[KEY_R] && state->keys[KEY_A] && state->keys[KEY_M]) {
        printf("Custom Shortcut: R+A+M -> \"RAM is refreshed\"\n");
    }
}

// Function to get the string representation of an event type
const char *getEventType(int value) {
    switch (value) {
    case 0:
        return "RELEASED";
    case 1:
        return "PRESSED";
    case 2:
        return "REPEATED";
    default:
        return "UNKNOWN";
    }
}

// Function to monitor the keyboard input and perform corresponding actions
void monitorKeyboard(const char *device_path, const char *output_file) {
    // Open the keyboard device
    int fd = open(device_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }

    // Open the output file
    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Initialize the keyboard state with all keys in the released state
    struct input_event ev;
    struct KeyboardState keyboardState = {0};

    // Continuously read input events from the keyboard
    while (1) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == sizeof(ev)) {
            if (ev.type == EV_KEY && (ev.value == 0 || ev.value == 1 || ev.value == 2)) {
                // Get the string representation of the event type
                const char *event_type = getEventType(ev.value);

                // Update the keyboard state based on the event
                keyboardState.keys[ev.code] = (ev.value == 1);

                // Handle specific keyboard shortcuts
                handleShortcuts(&keyboardState);

                // Print event information to stdout and the output file
                fprintf(stdout, " %s 0x%04X (%d)\n", event_type, ev.code, ev.code);
                fprintf(output, " %s 0x%04X (%d)\n", event_type, ev.code, ev.code);

                // Exit the program if the E and X keys are pressed simultaneously
                if (keyboardState.keys[KEY_E] && keyboardState.keys[KEY_X]) {
                    exit(0);
                }
            }
        } else {
            perror("Error reading from device");
            break;
        }
    }

    // Close the output file and the keyboard device
    fclose(output);
    close(fd);
}

