/*
 * Time-of-Check-Time-of-Use (TOCTOU) Race Condition Vulnerability
 * 
 * This demonstrates a classic race condition where the state of a resource
 * changes between the time it's checked and the time it's used.
 * 
 * Security Learning Objectives:
 * 1. Understanding TOCTOU vulnerabilities in file operations
 * 2. Symlink attacks and their exploitation
 * 3. Secure file handling with atomic operations
 * 4. Proper use of file descriptors to avoid race conditions
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

void print_file_info(const char *prefix, struct stat *st) {
    struct passwd *pw = getpwuid(st->st_uid);
    printf("%s File info - Owner: %s(%d), Size: %ld bytes, Mode: %o\n", 
           prefix, pw ? pw->pw_name : "unknown", st->st_uid, st->st_size, st->st_mode & 0777);
}

void vulnerable_file_operation(const char *filename) {
    struct stat file_stat;
    FILE *fp;
    
    printf("=== Vulnerable File Operation ===\n");
    printf("Target file: %s\n", filename);
    
    // TIME OF CHECK: Verify file properties
    if (stat(filename, &file_stat) == -1) {
        perror("stat() failed");
        return;
    }
    
    print_file_info("[CHECK]", &file_stat);
    
    // Check if it's a regular file
    if (!S_ISREG(file_stat.st_mode)) {
        printf("Not a regular file, skipping...\n");
        return;
    }
    
    // Additional security check: file size limit
    if (file_stat.st_size > 1024) {
        printf("File too large, security policy violation!\n");
        return;
    }
    
    printf("Security checks passed, proceeding...\n");
    
    // RACE CONDITION WINDOW: File could change here!
    printf("Processing... (simulating delay that creates race window)\n");
    printf("⚠️  VULNERABILITY: Between stat() and fopen(), file can be replaced!\n");
    sleep(3);  // Increased delay to make exploitation easier
    
    // TIME OF USE: Actually open and modify the file
    printf("Opening file for writing...\n");
    fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("fopen() failed");
        return;
    }
    
    // Check what we actually opened (post-exploitation verification)
    struct stat actual_stat;
    if (fstat(fileno(fp), &actual_stat) == 0) {
        print_file_info("[ACTUAL]", &actual_stat);
        if (actual_stat.st_ino != file_stat.st_ino) {
            printf("🚨 WARNING: File inode changed! Possible exploitation detected!\n");
        }
    }
    
    fprintf(fp, "Sensitive data written by program (PID: %d)\n", getpid());
    printf("Data written successfully!\n");
    
    fclose(fp);
}

void secure_file_operation(const char *filename) {
    int fd;
    struct stat file_stat;
    FILE *fp;
    
    printf("=== Secure File Operation ===\n");
    printf("Target file: %s\n", filename);
    
    // SECURE: Open with O_NOFOLLOW to prevent symlink attacks
    // O_EXCL with O_CREAT ensures we create a new file atomically
    fd = open(filename, O_WRONLY | O_APPEND | O_CREAT | O_NOFOLLOW, 0644);
    if (fd == -1) {
        if (errno == ELOOP) {
            printf("✅ Security: Symlink attack blocked by O_NOFOLLOW!\n");
        } else if (errno == EEXIST) {
            printf("✅ Security: File already exists, using existing file safely\n");
            // Try again without O_EXCL for demonstration
            fd = open(filename, O_WRONLY | O_APPEND | O_NOFOLLOW, 0644);
            if (fd == -1) {
                perror("open() failed on retry");
                return;
            }
        } else {
            perror("open() failed");
        }
        
        if (fd == -1) return;
    }
    
    // Check properties of the OPEN file descriptor (atomic check)
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat() failed");
        close(fd);
        return;
    }
    
    print_file_info("[SECURE]", &file_stat);
    
    // Additional security checks on the open file descriptor
    if (!S_ISREG(file_stat.st_mode)) {
        printf("✅ Security: Not a regular file, operation blocked!\n");
        close(fd);
        return;
    }
    
    fp = fdopen(fd, "a");
    if (fp == NULL) {
        perror("fdopen() failed");
        close(fd);
        return;
    }
    
    fprintf(fp, "Secure write operation completed (PID: %d)\n", getpid());
    printf("✅ Data written securely using file descriptor!\n");
    
    fclose(fp);  // This also closes the underlying fd
}

int main(int argc, char *argv[]) {
    printf("=== TOCTOU Race Condition Demonstration ===\n\n");
    
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        printf("Example: %s /tmp/test_file.txt\n", argv[0]);
        printf("\n🎯 To exploit the vulnerability:\n");
        printf("Terminal 1: Create file and run program\n");
        printf("  echo 'original data' > /tmp/test_file.txt\n");
        printf("  %s /tmp/test_file.txt\n", argv[0]);
        printf("\nTerminal 2: During the 3-second delay, replace with symlink\n");
        printf("  rm /tmp/test_file.txt && ln -s /etc/passwd /tmp/test_file.txt\n");
        printf("  # Or try: ln -s /tmp/sensitive_target /tmp/test_file.txt\n");
        printf("\n📝 Expected results:\n");
        printf("  - Vulnerable function: Writes to symlink target\n");
        printf("  - Secure function: Blocks symlink attack\n");
        printf("\n⚠️  Note: Run in Docker container for safe testing!\n");
        return 1;
    }
    
    printf("Process ID: %d\n", getpid());
    printf("Target file: %s\n\n", argv[1]);
    
    vulnerable_file_operation(argv[1]);
    printf("\n==================================================\n");
    secure_file_operation(argv[1]);
    
    return 0;
}
