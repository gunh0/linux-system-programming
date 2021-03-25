/*
 * Memory Corruption: Use-After-Free Vulnerability Example
 *
 * This program demonstrates a classic use-after-free vulnerability, where a program
 * continues to use a pointer after the memory it points to has been deallocated.
 *
 * Security Learning Objectives:
 * 1. Understanding dynamic memory allocation (malloc, free).
 * 2. Recognizing the dangers of dangling pointers.
 * 3. How use-after-free can lead to arbitrary code execution.
 * 4. Mitigation strategies like setting freed pointers to NULL.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// A function pointer for a callback
typedef void (*callback_func)();

// A structure that contains data and a function pointer
struct user_data
{
    char name[32];
    int user_id;
    callback_func action;
    char padding[8]; // Extra padding to make exploitation more reliable
};

void say_hello()
{
    printf("Hello from the original function!\n");
}

void secret_shell()
{
    printf("💀 !!! ROOT SHELL SPAWNED !!! 💀\n");
    printf("⚠️  VULNERABILITY EXPLOITED: Function pointer hijacked!\n");
    // In a real exploit, this would be something like: system("/bin/sh");
}

void safe_say_hello()
{
    printf("✅ Safe function executed successfully!\n");
}

// Vulnerable implementation
void vulnerable_use_after_free()
{
    printf("=== 🔴 VULNERABLE Use-After-Free Demonstration ===\n");

    // 1. Allocate memory for a user_data struct and initialize it.
    struct user_data *user = (struct user_data *)malloc(sizeof(struct user_data));
    if (!user) {
        printf("❌ Memory allocation failed!\n");
        return;
    }
    
    strncpy(user->name, "Alice", sizeof(user->name) - 1);
    user->name[sizeof(user->name) - 1] = '\0';
    user->user_id = 1001;
    user->action = say_hello;
    
    printf("📝 User '%s' (ID: %d) created at address: %p\n", user->name, user->user_id, (void*)user);
    printf("🔗 Function pointer address: %p\n", (void*)user->action);

    // 2. Call the function pointer to show normal operation.
    printf("🎯 Executing original function: ");
    user->action();

    // 3. VULNERABILITY: The memory for 'user' is freed, but the 'user' pointer is not set to NULL.
    // This makes 'user' a "dangling pointer".
    printf("\n⚠️  VULNERABILITY: Freeing memory but keeping dangling pointer...\n");
    free(user);
    printf("💔 Memory freed. 'user' is now a dangling pointer pointing to: %p\n", (void*)user);

    // 4. Force memory reuse by creating fragmentation and then allocating
    printf("🎪 Creating memory fragmentation to force reuse...\n");
    
    // Allocate multiple blocks first
    struct user_data *temp_allocs[3];
    for (int i = 0; i < 3; i++) {
        temp_allocs[i] = malloc(sizeof(struct user_data));
        if (temp_allocs[i]) {
            printf("   Temp allocation %d at: %p\n", i, (void*)temp_allocs[i]);
        }
    }
    
    // Free them to create holes
    for (int i = 0; i < 3; i++) {
        free(temp_allocs[i]);
    }

    // 5. EXPLOIT: Attacker allocates new memory of the same size
    // This is more likely to reuse one of the freed blocks
    struct user_data *attacker_data = (struct user_data *)malloc(sizeof(struct user_data));
    if (!attacker_data) {
        printf("❌ Attacker allocation failed!\n");
        return;
    }
    
    strncpy(attacker_data->name, "💀 Mallory (ATTACKER)", sizeof(attacker_data->name) - 1);
    attacker_data->name[sizeof(attacker_data->name) - 1] = '\0';
    attacker_data->user_id = 666;
    attacker_data->action = secret_shell; // 🎯 Malicious function pointer!
    
    printf("🏴‍☠️ Attacker data allocated at: %p\n", (void*)attacker_data);
    printf("🎭 Attacker name: '%s' (ID: %d)\n", attacker_data->name, attacker_data->user_id);
    printf("💣 Malicious function pointer: %p\n", (void*)attacker_data->action);

    // Check if memory was reused (highly dependent on allocator behavior)
    if (user == attacker_data) {
        printf("🎯 SUCCESS: Memory was reused! Dangling pointer now points to attacker data!\n");
    } else {
        printf("⚠️  Memory addresses differ, but vulnerability demonstration continues...\n");
        printf("   Original pointer: %p, Attacker data: %p\n", (void*)user, (void*)attacker_data);
        printf("🎲 Simulating memory reuse by manually overwriting freed memory...\n");
        
        // For demonstration purposes, we'll copy the attacker data to the original location
        // This simulates what could happen in a real-world scenario with memory reuse
        memcpy(user, attacker_data, sizeof(struct user_data));
        printf("✅ Simulated memory reuse: copied attacker data to freed location\n");
    }

    // 6. The program later uses the dangling pointer, unaware that it may point to malicious data.
    printf("\n💥 EXPLOITING: Using dangling pointer to call function...\n");
    printf("🎲 Attempting to execute through dangling pointer: ");
    
    // This is dangerous - we're calling through a freed pointer!
    user->action(); // This might call secret_shell() instead of say_hello()

    // Cleanup
    free(attacker_data);

    printf("🔚 End of vulnerable demonstration\n\n");
}

// Secure implementation
void secure_memory_management()
{
    printf("=== ✅ SECURE Memory Management Demonstration ===\n");

    // 1. Allocate memory with error checking
    struct user_data *user = (struct user_data *)malloc(sizeof(struct user_data));
    if (!user) {
        printf("❌ Memory allocation failed!\n");
        return;
    }
    
    strncpy(user->name, "Bob", sizeof(user->name) - 1);
    user->name[sizeof(user->name) - 1] = '\0';
    user->user_id = 1002;
    user->action = safe_say_hello;
    
    printf("📝 Secure user '%s' (ID: %d) created\n", user->name, user->user_id);

    // 2. Normal operation
    printf("🎯 Executing function safely: ");
    if (user && user->action) {
        user->action();
    }

    // 3. SECURE: Free memory and immediately nullify pointer
    printf("🔒 SECURE: Freeing memory and nullifying pointer...\n");
    free(user);
    user = NULL; // 🔑 KEY MITIGATION: Set pointer to NULL after free!

    // 4. Attempt to use the now-NULL pointer
    printf("✅ Testing null pointer protection: ");
    if (user != NULL && user->action != NULL) {
        user->action();
    } else {
        printf("🛡️  Null pointer detected - safely prevented use-after-free!\n");
    }

    printf("🔚 End of secure demonstration\n\n");
}

int main(int argc, char *argv[])
{
    printf("=== 🧠 Use-After-Free Memory Corruption Demonstration ===\n");
    printf("Process ID: %d\n\n", getpid());

    if (argc > 1 && strcmp(argv[1], "secure") == 0) {
        secure_memory_management();
    } else {
        vulnerable_use_after_free();
        printf("🔄 Now demonstrating secure approach...\n\n");
        secure_memory_management();
    }

    return 0;
}

/*
 * DETAILED EXPLANATION:
 *
 * VULNERABILITY MECHANICS:
 * The use-after-free vulnerability occurs when:
 * 1. Memory is allocated and a pointer references it
 * 2. The memory is freed, but the pointer retains the old address (dangling pointer)
 * 3. New memory allocation may reuse the same memory location
 * 4. An attacker can control the contents of the reused memory
 * 5. The dangling pointer is used, executing attacker-controlled code
 *
 * EXPLOITATION TECHNIQUES:
 * - Function pointer hijacking (demonstrated here)
 * - Virtual table (vtable) corruption in C++
 * - Data structure corruption leading to logic bypasses
 * - Information disclosure through freed memory contents
 *
 * REAL-WORLD IMPACT:
 * - CVE-2020-1472 (Zerologon): Use-after-free in Windows Netlogon
 * - CVE-2021-3156 (Baron Samedit): sudo heap overflow leading to UAF
 * - Browser vulnerabilities: DOM use-after-free leading to RCE
 *
 * DETECTION METHODS:
 * 1. Static Analysis: Tools like Clang Static Analyzer, Coverity
 * 2. Dynamic Analysis: AddressSanitizer (ASan), Valgrind
 * 3. Runtime Protection: Hardened malloc implementations
 * 4. Fuzzing: Automated testing with tools like AFL, libFuzzer
 *
 * MITIGATION STRATEGIES:
 *
 * 1. Immediate Pointer Nullification:
 *    free(ptr); ptr = NULL;
 *    This converts dangerous use-after-free into safer null dereference crashes.
 *
 * 2. Memory Safety Languages:
 *    Rust: Ownership system prevents use-after-free at compile time
 *    Go: Garbage collection eliminates manual memory management
 *    Modern C++: Smart pointers (unique_ptr, shared_ptr) with RAII
 *
 * 3. Compiler Protections:
 *    -fsanitize=address: AddressSanitizer for debugging
 *    -D_FORTIFY_SOURCE=2: Enhanced buffer overflow protection
 *    -fstack-protector-all: Stack canary protection
 *
 * 4. Runtime Mitigations:
 *    ASLR: Address Space Layout Randomization
 *    DEP/NX: Data Execution Prevention
 *    CFI: Control Flow Integrity
 *    Hardened allocators: jemalloc, tcmalloc with security features
 *
 * 5. Secure Coding Practices:
 *    - Use static analysis tools in CI/CD pipeline
 *    - Implement custom allocator wrappers with debugging
 *    - Regular security code reviews
 *    - Comprehensive unit testing with memory debugging tools
 *
 * ADVANCED EXPLOITATION:
 * - Heap Feng Shui: Manipulating heap layout for reliable exploitation
 * - Type confusion: Exploiting freed objects allocated as different types
 * - Temporal safety violations: Racing conditions in multi-threaded programs
 */
