# Makefile for TOCTOU Race Condition Demonstration
# 
# This Makefile builds and runs TOCTOU race condition examples in Docker environment
# It demonstrates timing-based security vulnerabilities and their mitigations

# Variables
CC = gcc
TARGET = toctou_file_race
SOURCE = toctou_file_race.c
DOCKER_SERVICE = ubuntu_1804
DOCKER_WORKDIR = /home/Security_Essentials/3_Race_Conditions

# Compiler flags for demonstration
VULN_FLAGS = -fno-stack-protector -no-pie -O0
SECURE_FLAGS = -fstack-protector-all -pie -O2 -D_FORTIFY_SOURCE=2

# Default target
.PHONY: all
all: help

# Help target
.PHONY: help
help:
	@echo "=== TOCTOU Race Condition Demonstration Makefile ==="
	@echo ""
	@echo "Available targets:"
	@echo "  build-vuln      - Build vulnerable version (optimizations disabled)"
	@echo "  build-secure    - Build secure version (with optimizations)"
	@echo "  test-normal     - Run with normal file operations"
	@echo "  test-exploit    - Demonstrate TOCTOU exploitation"
	@echo "  test-symlink    - Test symlink attack scenarios"
	@echo "  test-automated  - Automated race condition testing"
	@echo "  race-demo       - Interactive race condition demonstration"
	@echo "  docker-shell    - Open shell in Docker container"
	@echo "  clean           - Clean built files"
	@echo "  info            - Show security information"
	@echo ""
	@echo "Usage examples:"
	@echo "  make build-vuln && make test-exploit"
	@echo "  make race-demo"
	@echo "  make test-automated"

# Build vulnerable version (without optimizations)
.PHONY: build-vuln
build-vuln:
	@echo "Building vulnerable version with race condition window..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		$(CC) $(VULN_FLAGS) $(SOURCE) -o $(TARGET)_vuln && \
		echo 'Built: $(TARGET)_vuln' && \
		ls -la $(TARGET)_vuln"

# Build secure version (with optimizations and protections)
.PHONY: build-secure
build-secure:
	@echo "Building secure version with race condition mitigations..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		$(CC) $(SECURE_FLAGS) $(SOURCE) -o $(TARGET)_secure && \
		echo 'Built: $(TARGET)_secure' && \
		ls -la $(TARGET)_secure"

# Test with normal file operations
.PHONY: test-normal
test-normal: check-binary
	@echo "Testing normal file operations..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Normal File Operations Test ===' && \
		echo 'Creating test file...' && \
		echo 'Original content' > /tmp/test_file.txt && \
		echo 'Running program...' && \
		./$(TARGET)_vuln /tmp/test_file.txt && \
		echo '' && \
		echo 'Final file contents:' && \
		cat /tmp/test_file.txt && \
		rm -f /tmp/test_file.txt"

# Demonstrate TOCTOU exploitation with timing
.PHONY: test-exploit
test-exploit: check-binary
	@echo "Demonstrating TOCTOU race condition exploitation..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== TOCTOU Exploitation Demo ===' && \
		echo 'Setting up exploitation scenario...' && \
		echo 'original data' > /tmp/race_target.txt && \
		echo 'Creating sensitive target file...' && \
		echo 'SENSITIVE SYSTEM FILE' > /tmp/sensitive_target.txt && \
		chmod 600 /tmp/sensitive_target.txt && \
		echo '' && \
		echo 'Running program in background...' && \
		./$(TARGET)_vuln /tmp/race_target.txt & \
		echo 'PID: '\$$! && \
		sleep 1 && \
		echo 'Exploiting race condition...' && \
		rm -f /tmp/race_target.txt && \
		ln -s /tmp/sensitive_target.txt /tmp/race_target.txt && \
		wait && \
		echo '' && \
		echo 'Checking if exploitation succeeded:' && \
		ls -la /tmp/race_target.txt && \
		echo 'Contents of target file:' && \
		cat /tmp/sensitive_target.txt && \
		rm -f /tmp/race_target.txt /tmp/sensitive_target.txt"

# Test symlink attack scenarios
.PHONY: test-symlink
test-symlink: check-binary
	@echo "Testing symlink attack scenarios..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Symlink Attack Test ===' && \
		echo 'Creating symlink to /etc/passwd...' && \
		ln -sf /etc/passwd /tmp/symlink_test && \
		echo 'Testing vulnerable version:' && \
		./$(TARGET)_vuln /tmp/symlink_test || echo 'Vulnerable version completed' && \
		echo '' && \
		echo 'Testing secure version:' && \
		./$(TARGET)_secure /tmp/symlink_test || echo 'Secure version completed' && \
		rm -f /tmp/symlink_test"

# Automated race condition testing
.PHONY: test-automated
test-automated: check-binary
	@echo "Running automated race condition tests..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Automated Race Condition Testing ===' && \
		for i in \$$(seq 1 5); do \
			echo \"Test run \$$i:\" && \
			echo 'test data' > /tmp/race_file_\$$i.txt && \
			timeout 10s bash -c \"\
				./$(TARGET)_vuln /tmp/race_file_\$$i.txt & \
				sleep 0.5 && \
				rm -f /tmp/race_file_\$$i.txt && \
				ln -s /etc/hostname /tmp/race_file_\$$i.txt && \
				wait\" || echo 'Test completed' && \
			rm -f /tmp/race_file_\$$i.txt && \
			echo '---' ; \
		done"

# Interactive race condition demonstration
.PHONY: race-demo
race-demo: build-vuln
	@echo "Starting interactive race condition demonstration..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Interactive TOCTOU Demo ===' && \
		echo 'This demo shows the race condition window in real-time' && \
		echo '' && \
		echo 'Step 1: Creating initial file' && \
		echo 'demo content' > /tmp/demo_file.txt && \
		ls -la /tmp/demo_file.txt && \
		echo '' && \
		echo 'Step 2: Running vulnerable program (watch for 3-second window)' && \
		./$(TARGET)_vuln /tmp/demo_file.txt && \
		echo '' && \
		echo 'Step 3: Testing with symlink attack' && \
		echo 'target content' > /tmp/demo_target.txt && \
		ln -sf /tmp/demo_target.txt /tmp/demo_symlink.txt && \
		./$(TARGET)_vuln /tmp/demo_symlink.txt && \
		echo '' && \
		echo 'Step 4: Secure version comparison' && \
		./$(TARGET)_secure /tmp/demo_symlink.txt && \
		echo '' && \
		echo 'Cleanup...' && \
		rm -f /tmp/demo_file.txt /tmp/demo_target.txt /tmp/demo_symlink.txt"

# Show security information
.PHONY: info
info:
	@echo "=== Security Information ==="
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo 'System information:' && \
		uname -a && \
		echo '' && \
		echo 'File system security features:' && \
		mount | grep -E '(nosuid|nodev|noexec)' || echo 'No special mount options detected' && \
		echo '' && \
		echo 'Current user and permissions:' && \
		id && \
		echo '' && \
		echo 'Symlink handling:' && \
		echo 'Testing symlink creation...' && \
		ln -sf /etc/passwd /tmp/test_symlink 2>/dev/null && \
		ls -la /tmp/test_symlink && \
		rm -f /tmp/test_symlink && \
		echo '' && \
		echo 'Binary information:' && \
		ls -la $(TARGET)_* 2>/dev/null || echo 'No binaries found - run build-vuln or build-secure first'"

# Open shell in Docker container for manual testing
.PHONY: docker-shell
docker-shell:
	@echo "Opening shell in Docker container..."
	@echo "Working directory: $(DOCKER_WORKDIR)"
	@echo "Manual testing commands:"
	@echo "  ./$(TARGET)_vuln /tmp/test_file.txt"
	@echo "  ln -s /etc/passwd /tmp/test_symlink"
	@echo "  ./$(TARGET)_secure /tmp/test_symlink"
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
		rm -f $(TARGET)_vuln $(TARGET)_secure && \
		rm -f /tmp/test_*.txt /tmp/race_*.txt /tmp/demo_*.txt /tmp/*_symlink* && \
		echo 'Cleaned: $(TARGET)_vuln $(TARGET)_secure and temporary files'"

# Advanced: GDB debugging session for race condition analysis
.PHONY: debug
debug: build-vuln
	@echo "Starting GDB debugging session for race condition analysis..."
	@echo "Useful GDB commands for race condition debugging:"
	@echo "  run /tmp/test_file.txt"
	@echo "  break vulnerable_file_operation"
	@echo "  break secure_file_operation"
	@echo "  info files"
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gdb && \
		echo 'test data' > /tmp/debug_file.txt && \
		gdb ./$(TARGET)_vuln"

# Performance comparison between vulnerable and secure versions
.PHONY: benchmark
benchmark: build-vuln build-secure
	@echo "Benchmarking vulnerable vs secure implementations..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Performance Benchmark ===' && \
		echo 'Testing vulnerable version (10 iterations):' && \
		time bash -c 'for i in \$$(seq 1 10); do echo \"test\" > /tmp/bench_\$$i.txt && ./$(TARGET)_vuln /tmp/bench_\$$i.txt >/dev/null 2>&1 && rm -f /tmp/bench_\$$i.txt; done' && \
		echo '' && \
		echo 'Testing secure version (10 iterations):' && \
		time bash -c 'for i in \$$(seq 1 10); do echo \"test\" > /tmp/bench_\$$i.txt && ./$(TARGET)_secure /tmp/bench_\$$i.txt >/dev/null 2>&1 && rm -f /tmp/bench_\$$i.txt; done'"

# Educational demonstration with step-by-step explanation
.PHONY: tutorial
tutorial: build-vuln build-secure
	@echo "Running educational tutorial..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== TOCTOU Race Condition Tutorial ===' && \
		echo '' && \
		echo '📚 What is TOCTOU?' && \
		echo 'Time-of-Check-Time-of-Use vulnerabilities occur when:' && \
		echo '1. A program checks a resource (file, permissions, etc.)' && \
		echo '2. Time passes (the \"race window\")' && \
		echo '3. The program uses the resource, assuming it hasnt changed' && \
		echo '' && \
		echo '🎯 Attack Scenario:' && \
		echo 'An attacker can replace a checked file with a symlink during the race window' && \
		echo '' && \
		echo 'Demonstration:' && \
		echo 'Creating legitimate file...' && \
		echo 'legitimate content' > /tmp/tutorial_file.txt && \
		./$(TARGET)_vuln /tmp/tutorial_file.txt && \
		echo '' && \
		echo '🛡️  Mitigation Strategies:' && \
		echo '1. Use file descriptors instead of filenames' && \
		echo '2. Open with O_NOFOLLOW to prevent symlink following' && \
		echo '3. Use fstat() instead of stat() on open files' && \
		echo '' && \
		echo 'Secure implementation demo:' && \
		ln -sf /etc/passwd /tmp/tutorial_symlink && \
		./$(TARGET)_secure /tmp/tutorial_symlink && \
		rm -f /tmp/tutorial_file.txt /tmp/tutorial_symlink"
