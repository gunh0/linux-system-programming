# Memory Corruption: Use-After-Free Vulnerability Demonstration Makefile
# This Makefile demonstrates use-after-free vulnerabilities and memory safety

# Variables
TARGET = use_after_free
SOURCE = use_after_free.c
DOCKER_SERVICE = ubuntu_1804
DOCKER_WORKDIR = /home/Security_Essentials/5_Memory_Corruption

# Default target
.PHONY: all
all: help

# Help target
.PHONY: help
help:
	@echo "=== Use-After-Free Memory Corruption Demonstration Makefile ==="
	@echo "Available targets:"
	@echo "  build-vuln      - Build vulnerable version for exploitation"
	@echo "  build-secure    - Build with memory safety protections"
	@echo "  build-asan      - Build with AddressSanitizer for debugging"
	@echo "  test-normal     - Test normal program execution"
	@echo "  test-exploit    - Demonstrate use-after-free exploitation"
	@echo "  test-secure     - Test secure memory management"
	@echo "  test-asan       - Test with AddressSanitizer detection"
	@echo "  test-automated  - Automated use-after-free testing"
	@echo "  memory-demo     - Interactive memory corruption demonstration"
	@echo "  docker-shell    - Open shell in Docker container"
	@echo "  clean           - Clean built files"
	@echo "  info            - Show security information"
	@echo "Usage examples:"
	@echo "  make build-vuln && make test-exploit"
	@echo "  make build-asan && make test-asan"
	@echo "  make memory-demo"

# Check if binary exists
.PHONY: check-binary
check-binary:
	@if [ ! -f $(TARGET)_vuln ]; then \
		echo "Binary not found. Run 'make build-vuln' first."; \
		exit 1; \
	fi

# Build vulnerable version optimized for exploitation
.PHONY: build-vuln
build-vuln:
	@echo "Building vulnerable use-after-free program..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		gcc -fno-stack-protector -no-pie -O0 -g $(SOURCE) -o $(TARGET)_vuln && \
		echo 'Built: $(TARGET)_vuln' && \
		ls -la $(TARGET)_vuln"

# Build secure version with protections
.PHONY: build-secure
build-secure:
	@echo "Building secure version with memory protections..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		gcc -fstack-protector-all -pie -O2 -D_FORTIFY_SOURCE=2 -Wformat=2 -Wformat-security $(SOURCE) -o $(TARGET)_secure && \
		echo 'Built: $(TARGET)_secure' && \
		ls -la $(TARGET)_secure"

# Build with AddressSanitizer for vulnerability detection
.PHONY: build-asan
build-asan:
	@echo "Building with AddressSanitizer for memory error detection..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		gcc -fsanitize=address -fno-omit-frame-pointer -g -O1 $(SOURCE) -o $(TARGET)_asan && \
		echo 'Built: $(TARGET)_asan' && \
		ls -la $(TARGET)_asan"

# Test normal program execution
.PHONY: test-normal
test-normal: check-binary
	@echo "Testing normal program execution..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Normal Execution Test ===' && \
		echo 'Running vulnerable version:' && \
		./$(TARGET)_vuln && \
		echo '' && \
		echo 'Testing secure mode:' && \
		./$(TARGET)_vuln secure"

# Demonstrate use-after-free exploitation
.PHONY: test-exploit
test-exploit: check-binary
	@echo "Demonstrating use-after-free exploitation..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Use-After-Free Exploitation Demo ===' && \
		echo 'Running multiple exploitation attempts to increase success rate:' && \
		for i in \$$(seq 1 3); do \
			echo \"--- Attempt \$$i ---\" && \
			./$(TARGET)_vuln && \
			echo '' ; \
		done"

# Test secure memory management
.PHONY: test-secure
test-secure:
	@echo "Testing secure memory management..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Secure Memory Management Test ===' && \
		if [ -f $(TARGET)_secure ]; then \
			echo 'Running secure version:' && \
			./$(TARGET)_secure && \
			echo 'Testing secure mode specifically:' && \
			./$(TARGET)_secure secure ; \
		else \
			echo 'Secure binary not found. Run make build-secure first.' ; \
		fi"

# Test with AddressSanitizer detection
.PHONY: test-asan
test-asan:
	@echo "Testing with AddressSanitizer for memory error detection..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== AddressSanitizer Detection Test ===' && \
		if [ -f $(TARGET)_asan ]; then \
			echo 'Running with AddressSanitizer (should detect use-after-free):' && \
			./$(TARGET)_asan || echo 'AddressSanitizer detected memory corruption!' && \
			echo '' && \
			echo 'Testing secure mode with ASan:' && \
			./$(TARGET)_asan secure || echo 'Execution completed with ASan protection' ; \
		else \
			echo 'ASan binary not found. Run make build-asan first.' ; \
		fi"

# Automated use-after-free testing
.PHONY: test-automated
test-automated: check-binary
	@echo "Running automated use-after-free tests..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Automated Use-After-Free Testing ===' && \
		echo 'Testing vulnerability consistency across multiple runs:' && \
		success_count=0 && \
		total_runs=5 && \
		for i in \$$(seq 1 \$$total_runs); do \
			echo \"Test run \$$i:\" && \
			if ./$(TARGET)_vuln | grep -q 'ROOT SHELL SPAWNED'; then \
				echo \"  ✅ Exploitation successful\" && \
				success_count=\$$((success_count + 1)) ; \
			else \
				echo \"  ❌ Exploitation failed (memory not reused)\" ; \
			fi && \
			echo '---' ; \
		done && \
		echo \"Results: \$$success_count/\$$total_runs successful exploitations\" && \
		echo \"Success rate: \$$((success_count * 100 / total_runs))%\""

# Interactive memory corruption demonstration
.PHONY: memory-demo
memory-demo: build-vuln build-asan
	@echo "Starting interactive memory corruption demonstration..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Interactive Memory Corruption Demo ===' && \
		echo 'Step 1: Demonstrating vulnerable use-after-free' && \
		./$(TARGET)_vuln && \
		echo '' && \
		echo 'Step 2: Showing secure memory management' && \
		./$(TARGET)_vuln secure && \
		echo '' && \
		echo 'Step 3: AddressSanitizer detection (will show error)' && \
		echo 'Note: ASan will detect and report the use-after-free bug' && \
		./$(TARGET)_asan || echo 'ASan successfully detected memory corruption!' && \
		echo '' && \
		echo 'Step 4: ASan with secure mode (should run clean)' && \
		./$(TARGET)_asan secure && \
		echo 'Demo completed!'"

# Open Docker shell for manual testing
.PHONY: docker-shell
docker-shell:
	@echo "Opening shell in Docker container..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && bash"

# Clean built files
.PHONY: clean
clean:
	@echo "Cleaning built files..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		rm -f $(TARGET)_vuln $(TARGET)_secure $(TARGET)_asan && \
		echo 'Cleaned: $(TARGET)_vuln $(TARGET)_secure $(TARGET)_asan'"

# Show security information
.PHONY: info
info:
	@echo "=== Use-After-Free Memory Corruption Security Information ==="
	@echo ""
	@echo "VULNERABILITY: Use-After-Free (UAF)"
	@echo "- Memory is freed but pointer is still used"
	@echo "- Can lead to arbitrary code execution"
	@echo "- Attacker can control freed memory contents"
	@echo ""
	@echo "ATTACK VECTORS:"
	@echo "- Function pointer hijacking"
	@echo "- Virtual table corruption (C++)"
	@echo "- Data structure manipulation"
	@echo "- Information disclosure"
	@echo ""
	@echo "EXPLOITATION TECHNIQUES:"
	@echo "- Heap manipulation for memory reuse"
	@echo "- Timing attacks in multi-threaded programs"
	@echo "- Type confusion attacks"
	@echo ""
	@echo "DETECTION TOOLS:"
	@echo "- AddressSanitizer (ASan): Runtime detection"
	@echo "- Valgrind: Memory error detection"
	@echo "- Static analysis tools"
	@echo "- Fuzzing with memory debugging"
	@echo ""
	@echo "MITIGATIONS:"
	@echo "- Set pointers to NULL after free()"
	@echo "- Use memory-safe languages (Rust, Go)"
	@echo "- Modern C++ smart pointers"
	@echo "- Hardened memory allocators"
	@echo "- Compiler security flags"
	@echo ""
	@echo "FILES:"
	@echo "- $(SOURCE): Source code with vulnerability and fixes"
	@echo "- $(TARGET)_vuln: Vulnerable binary"
	@echo "- $(TARGET)_secure: Secure version"
	@echo "- $(TARGET)_asan: AddressSanitizer build"
	@echo ""
	@echo "TESTING:"
	@echo "- Use 'make test-exploit' for exploitation demo"
	@echo "- Use 'make test-asan' for vulnerability detection"
	@echo "- Use 'make memory-demo' for comprehensive demonstration"
