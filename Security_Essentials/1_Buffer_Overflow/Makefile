# Makefile for Buffer Overflow Demonstration
# 
# This Makefile builds and runs the vulnerable_strcpy program in Docker environment
# It demonstrates buffer overflow vulnerability with different security settings

# Variables
CC = gcc
TARGET = vulnerable_strcpy
SOURCE = vulnerable_strcpy.c
DOCKER_SERVICE = ubuntu_1804
DOCKER_WORKDIR = /home/Security_Essentials/1_Buffer_Overflow

# Compiler flags for vulnerability demonstration
VULN_FLAGS = -fno-stack-protector -z execstack -no-pie
SECURE_FLAGS = -fstack-protector-all -z noexecstack -pie

# Default target
.PHONY: all
all: help

# Help target
.PHONY: help
help:
	@echo "=== Buffer Overflow Demonstration Makefile ==="
	@echo ""
	@echo "Available targets:"
	@echo "  build-vuln    - Build vulnerable version (no protections)"
	@echo "  build-secure  - Build secure version (with protections)"
	@echo "  test-safe     - Run with safe input"
	@echo "  test-overflow - Run with buffer overflow input"
	@echo "  test-exploit  - Run exploitation attempt"
	@echo "  docker-shell  - Open shell in Docker container"
	@echo "  clean         - Clean built files"
	@echo "  info          - Show security information"
	@echo ""
	@echo "Usage examples:"
	@echo "  make build-vuln && make test-overflow"
	@echo "  make build-secure && make test-overflow"

# Build vulnerable version (without security protections)
.PHONY: build-vuln
build-vuln:
	@echo "Building vulnerable version without security protections..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		$(CC) $(VULN_FLAGS) $(SOURCE) -o $(TARGET)_vuln && \
		echo 'Built: $(TARGET)_vuln' && \
		ls -la $(TARGET)_vuln"

# Build secure version (with security protections)
.PHONY: build-secure
build-secure:
	@echo "Building secure version with security protections..."
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
		./$(TARGET)_vuln 'Hello World' || ./$(TARGET)_secure 'Hello World'"

# Test with buffer overflow input (non-exploitative)
.PHONY: test-overflow
test-overflow: check-binary
	@echo "Testing with buffer overflow input..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Buffer Overflow Test ===' && \
		echo 'Generating 80 character input...' && \
		perl -E 'say \"A\" x 80' | xargs ./$(TARGET)_vuln || \
		perl -E 'say \"A\" x 80' | xargs ./$(TARGET)_secure || \
		echo 'Program crashed or terminated - this demonstrates the vulnerability'"

# Test exploitation attempt
.PHONY: test-exploit
test-exploit: check-binary
	@echo "Attempting exploitation (educational purposes)..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Exploitation Attempt ===' && \
		echo 'Note: Actual exploitation requires precise address calculation' && \
		echo 'This demonstrates the concept with pattern input' && \
		perl -E 'say \"A\" x 72 . \"BBBB\" . \"CCCC\"' | xargs ./$(TARGET)_vuln || \
		echo 'Exploitation attempt completed (may have crashed)'"

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

# Advanced: GDB debugging session
.PHONY: debug
debug: build-vuln
	@echo "Starting GDB debugging session..."
	@echo "Use 'run <input>' to test, 'disas main' to see assembly, etc."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gdb && \
		gdb ./$(TARGET)_vuln"

# Generate pattern for precise offset finding
.PHONY: pattern
pattern:
	@echo "Generating pattern for offset calculation..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		perl -E 'say join(\"\", map { \$$_ x 4 } \"A\"..\"T\")' | head -c 100 && \
		echo '' && \
		echo 'Use this pattern as input to find exact offset where return address is overwritten'"

# Show compilation differences
.PHONY: compare
compare: build-vuln build-secure
	@echo "Comparing vulnerable vs secure compilation..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo 'Vulnerable binary:' && \
		file $(TARGET)_vuln && \
		echo '' && \
		echo 'Secure binary:' && \
		file $(TARGET)_secure && \
		echo '' && \
		echo 'Size comparison:' && \
		ls -la $(TARGET)_*"
