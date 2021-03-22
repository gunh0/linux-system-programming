/*
 * Format String Vulnerability Example
 * 
 * This demonstrates how improper use of printf-family functions
 * can lead to information disclosure and arbitrary memory writes.
 * 
 * Security Learning Objectives:
 * 1. Understanding format string specifiers and stack behavior
 * 2. Information disclosure through stack reading
 * 3. Arbitrary memory write capabilities
 * 4. Mitigation through proper format string usage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable to demonstrate arbitrary write
int secret_value = 0x12345678;
int authentication_bypass = 0;

void vulnerable_log(char *user_input) {
    printf("=== Vulnerable Log Function ===\n");
    printf("Secret value address: %p (value: 0x%x)\n", &secret_value, secret_value);
    printf("Auth bypass address: %p (value: %d)\n", &authentication_bypass, authentication_bypass);
    
    printf("User input: ");
    // VULNERABILITY: User input directly used as format string!
    printf(user_input);
    printf("\n");
    
    printf("After printf - Secret: 0x%x, Auth: %d\n", secret_value, authentication_bypass);
}

void secure_log(char *user_input) {
    printf("=== Secure Log Function ===\n");
    // SECURE: User input treated as data, not format string
    printf("User input: %s\n", user_input);
}

int check_authentication() {
    if (authentication_bypass == 0x41414141) {
        printf("🚨 AUTHENTICATION BYPASSED! Access granted.\n");
        return 1;
    }
    printf("Access denied.\n");
    return 0;
}

int main(int argc, char *argv[]) {
    printf("=== Format String Vulnerability Demonstration ===\n\n");
    
    if (argc != 2) {
        printf("Usage: %s <format_string>\n\n", argv[0]);
        printf("Examples to try:\n");
        printf("  Info leak:     %s \"%%x.%%x.%%x.%%x.%%x.%%x\"\n", argv[0]);
        printf("  Stack dump:    %s \"%%08x.\" (repeat many times)\n", argv[0]);
        printf("  Write attempt: %s \"%%n\"\n", argv[0]);
        printf("  Crash:         %s \"%%s%%s%%s%%s\"\n", argv[0]);
        return 1;
    }
    
    printf("Stack addresses for reference:\n");
    printf("  main() local var: %p\n", &argc);
    printf("  argv: %p\n", argv);
    printf("\n");
    
    // Demonstrate the vulnerability
    vulnerable_log(argv[1]);
    
    printf("\n");
    
    // Check if authentication was bypassed
    check_authentication();
    
    printf("\n=== Comparison with Secure Implementation ===\n");
    secure_log(argv[1]);
    
    return 0;
}

/*
 * EXPLOITATION TECHNIQUES:
 * 
 * 1. Information Disclosure:
 *    ./format_vuln "%x.%x.%x.%x.%x.%x.%x.%x"
 *    - Reads values from stack
 *    - Can leak addresses, defeating ASLR
 * 
 * 2. Arbitrary Memory Read:
 *    ./format_vuln "$(printf '\x78\x56\x34\x12')%s"
 *    - Reads string from arbitrary address 0x12345678
 * 
 * 3. Arbitrary Memory Write:
 *    ./format_vuln "$(printf '\x<target_addr>')%n"
 *    - Writes number of bytes printed so far to target address
 * 
 * 4. Advanced Exploitation:
 *    - Use %hn for 2-byte writes
 *    - Use %hhn for 1-byte writes
 *    - Chain multiple writes to construct arbitrary values
 * 
 * MITIGATION STRATEGIES:
 * 
 * 1. Always use format strings: printf("%s", user_input)
 * 2. Compiler warnings: gcc -Wformat -Wformat-security
 * 3. Static analysis tools
 * 4. FORTIFY_SOURCE: gcc -D_FORTIFY_SOURCE=2
 * 5. Runtime protections: ASLR, stack canaries
 */
