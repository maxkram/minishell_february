#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Reuse the setup function to redirect stdout
void setup(void) {
    cr_redirect_stdout();
}

// Function to capture command output
char* execute_command(const char* command) {
    FILE* pipe = popen(command, "r");
    if (!pipe) return NULL;

    char* result = malloc(1024);
    fgets(result, 1024, pipe);
    pclose(pipe);

    // Remove the trailing newline character
    result[strcspn(result, "\n")] = 0;

    return result;
}

Test(minishell_tests, pwd_command) {
    char* expected_output = execute_command("pwd");
    char* actual_output = execute_command("./tests/builtins.sh");

    cr_expect_str_eq(actual_output, expected_output, "The output of 'pwd' in minishell does not match the expected output.");

    free(expected_output);
    free(actual_output);
}


// Test function for the echo command
Test(minishell_tests, echo_command, .init = setup) {
    // Assuming you have a generic function to execute commands and capture output
    char* expected_output = "Hello, world!\n"; // Expected output of the echo command
    system("./tests/builtins.sh"); // Execute your shell script

    // Capture the actual output
    char actual_output[1024];
    fflush(stdout); // Ensure all output has been written to the buffer
    fgets(actual_output, sizeof(actual_output), stdout); // Read the output

    // Compare the actual output to the expected output
    cr_expect_str_eq(actual_output, expected_output, "The output of 'echo Hello, world!' was not as expected.");
}

// // Adjust the echo_command test to print actual output before asserting
// Test(minishell_tests, echo_command, .init = setup) {
//     char* expected_output = "Hello, world!\n"; // Ensure this matches expected echo behavior
//     system("./tests/builtins.sh echo"); // Assuming builtins.sh takes the command as an argument

//     char actual_output[1024];
//     fflush(stdout);
//     fgets(actual_output, sizeof(actual_output), stdout);
//     actual_output[strcspn(actual_output, "\n")] = 0; // Remove newline for consistent comparison

//     printf("Actual output: '%s'\n", actual_output); // Debug print
//     cr_expect_str_eq(actual_output, expected_output, "The output of 'echo Hello, world!' was not as expected.");
// }
