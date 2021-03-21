/*
 * Buffer Overflow Vulnerability Example - strcpy() exploitation
 *
 * This demonstrates a classic stack-based buffer overflow vulnerability
 * that can lead to arbitrary code execution.
 *
 * Security Learning Objectives:
 * 1. Understanding stack layout and buffer boundaries
 * 2. How unchecked string operations can overwrite return addresses
 * 3. Mitigation techniques (stack canaries, ASLR, NX bit)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vulnerable_function(char *user_input)
{
    char buffer[64]; // Fixed-size buffer on stack

    printf("Buffer address: %p\n", buffer);
    printf("Input length: %zu\n", strlen(user_input));

    // VULNERABILITY: No bounds checking!
    // If user_input > 64 bytes, it will overflow the buffer
    strcpy(buffer, user_input);

    printf("Buffer contents: %s\n", buffer);
    printf("Function completed successfully\n");
}

void secret_function()
{
    printf("🚨 SECURITY BREACH: You've successfully exploited the buffer overflow!\n");
    printf("This function should never be called in normal execution.\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    printf("=== Buffer Overflow Demonstration ===\n");
    printf("Secret function address: %p\n", secret_function);

    if (argc != 2)
    {
        printf("Usage: %s <input_string>\n", argv[0]);
        printf("\nTry different input lengths:\n");
        printf("  Safe:   %s \"Hello World\"\n", argv[0]);
        printf("  Unsafe: %s \"$(python -c 'print(\"A\" * 80)')\"\n", argv[0]);
        return 1;
    }

    vulnerable_function(argv[1]);

    printf("Program finished normally\n");
    return 0;
}

/*
 * EXPLOITATION NOTES:
 *
 * 1. Compile without protections:
 *    gcc -fno-stack-protector -z execstack -no-pie vulnerable_strcpy.c -o vuln
 *
 * 2. Find the exact offset to overwrite return address:
 *    - Use GDB or create pattern with tools like pattern_create.rb
 *    - Typically around 72-76 bytes for this example
 *
 * 3. Craft exploit payload:
 *    python -c "print('A' * 72 + '\x<secret_function_address>')"
 *
 * 4. Modern mitigations to understand:
 *    - Stack Canaries: gcc -fstack-protector-all
 *    - ASLR: echo 2 > /proc/sys/kernel/randomize_va_space
 *    - NX Bit: -z noexecstack (default)
 *    - PIE: -pie (default in modern gcc)
 */
