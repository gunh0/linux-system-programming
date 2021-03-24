# SUID Privilege Escalation Demonstration Makefile
# This Makefile demonstrates privilege escalation vulnerabilities through SUID programs

# Variables
TARGET = setuid_exploit
SOURCE = setuid_exploit.c
DOCKER_SERVICE = ubuntu_1804
DOCKER_WORKDIR = /home/Security_Essentials/4_Privilege_Escalation

# Default target
.PHONY: all
all: help

# Help target
.PHONY: help
help:
	@echo "=== SUID Privilege Escalation Demonstration Makefile ==="
	@echo "Available targets:"
	@echo "  build-vuln      - Build vulnerable SUID program"
	@echo "  build-secure    - Build secure version with mitigations"
	@echo "  test-normal     - Test normal program execution"
	@echo "  test-exploit    - Demonstrate SUID privilege escalation"
	@echo "  test-suid       - Test SUID bit functionality"
	@echo "  test-automated  - Automated privilege escalation testing"
	@echo "  suid-demo       - Interactive SUID demonstration"
	@echo "  docker-shell    - Open shell in Docker container"
	@echo "  clean           - Clean built files"
	@echo "  info            - Show security information"
	@echo "Usage examples:"
	@echo "  make build-vuln && make test-exploit"
	@echo "  make suid-demo"
	@echo "  make test-automated"

# Check if binary exists
.PHONY: check-binary
check-binary:
	@if [ ! -f $(TARGET)_vuln ]; then \
		echo "Binary not found. Run 'make build-vuln' first."; \
		exit 1; \
	fi

# Build vulnerable version with SUID capabilities
.PHONY: build-vuln
build-vuln:
	@echo "Building vulnerable SUID program..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc sudo && \
		gcc -fno-stack-protector -no-pie -O0 $(SOURCE) -o $(TARGET)_vuln && \
		echo 'Built: $(TARGET)_vuln' && \
		ls -la $(TARGET)_vuln"

# Build secure version with mitigations
.PHONY: build-secure
build-secure:
	@echo "Building secure version with privilege mitigations..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		apt-get update -qq && apt-get install -y gcc && \
		gcc -fstack-protector-all -pie -O2 -D_FORTIFY_SOURCE=2 $(SOURCE) -o $(TARGET)_secure && \
		echo 'Built: $(TARGET)_secure' && \
		ls -la $(TARGET)_secure"

# Test normal program execution
.PHONY: test-normal
test-normal: check-binary
	@echo "Testing normal program execution..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Normal Execution Test ===' && \
		echo 'Current user and permissions:' && \
		id && \
		ls -la $(TARGET)_vuln && \
		echo '' && \
		echo 'Running program with normal arguments:' && \
		./$(TARGET)_vuln '-ef' && \
		echo 'Normal execution completed.'"

# Demonstrate SUID privilege escalation
.PHONY: test-exploit
test-exploit: check-binary
	@echo "Demonstrating SUID privilege escalation..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== SUID Privilege Escalation Demo ===' && \
		echo 'Setting up SUID binary (simulated - Docker limitation)...' && \
		echo 'Current permissions:' && \
		ls -la $(TARGET)_vuln && \
		echo '' && \
		echo 'Creating test scenario with user input injection...' && \
		echo 'Testing command injection vulnerability:' && \
		./$(TARGET)_vuln ';whoami;id' && \
		echo '' && \
		echo 'Testing with another injection payload:' && \
		./$(TARGET)_vuln ';echo EXPLOITED: Command injection successful' && \
		echo '' && \
		echo 'Note: In real environment with SUID root, this would execute as root'"

# Test SUID bit functionality
.PHONY: test-suid
test-suid: check-binary
	@echo "Testing SUID command injection vulnerability..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Command Injection Vulnerability Test ===' && \
		echo 'Current binary permissions:' && \
		ls -la $(TARGET)_vuln && \
		echo '' && \
		echo 'Testing various injection payloads:' && \
		echo 'Payload 1: Command chaining with semicolon' && \
		./$(TARGET)_vuln ';echo INJECTED_COMMAND_1' && \
		echo '' && \
		echo 'Payload 2: Command substitution' && \
		./$(TARGET)_vuln ';\$$(echo INJECTED_COMMAND_2)' && \
		echo '' && \
		echo 'Payload 3: Multiple commands' && \
		./$(TARGET)_vuln ';echo START;whoami;echo END' && \
		echo 'All injection tests completed.'"

# Automated privilege escalation testing
.PHONY: test-automated
test-automated: check-binary
	@echo "Running automated command injection tests..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Automated Command Injection Testing ===' && \
		payloads=( \
			';echo TEST_1_PASSED' \
			';cat /etc/hostname' \
			';whoami' \
			';\$$(echo TEST_4_PASSED)' \
			';echo START && echo END' \
		) && \
		for i in \$${!payloads[@]}; do \
			echo \"Test \$$((i+1)): \$${payloads[i]}\" && \
			./$(TARGET)_vuln \"\$${payloads[i]}\" && \
			echo '---' ; \
		done && \
		echo 'All automated tests completed.'"

# Interactive SUID demonstration
.PHONY: suid-demo
suid-demo: build-vuln
	@echo "Starting interactive command injection demonstration..."
	docker compose run --rm $(DOCKER_SERVICE) bash -c "cd $(DOCKER_WORKDIR) && \
		echo '=== Interactive Command Injection Demo ===' && \
		echo 'This demo shows command injection vulnerabilities' && \
		echo '' && \
		echo 'Step 1: Normal program execution' && \
		./$(TARGET)_vuln '-u' && \
		echo '' && \
		echo 'Step 2: Command injection demonstration' && \
		echo 'Injecting: ;echo COMMAND_INJECTED' && \
		./$(TARGET)_vuln ';echo COMMAND_INJECTED' && \
		echo '' && \
		echo 'Step 3: Multiple command injection' && \
		echo 'Injecting: ;echo START;whoami;echo END' && \
		./$(TARGET)_vuln ';echo START;whoami;echo END' && \
		echo '' && \
		echo 'Step 4: File system access' && \
		echo 'Injecting: ;cat /etc/hostname' && \
		./$(TARGET)_vuln ';cat /etc/hostname' && \
		echo '' && \
		echo 'Demo completed. In real SUID scenario, these would run as root!'"

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
		rm -f $(TARGET)_vuln $(TARGET)_secure && \
		echo 'Cleaned: $(TARGET)_vuln $(TARGET)_secure'"

# Show security information
.PHONY: info
info:
	@echo "=== SUID Privilege Escalation Security Information ==="
	@echo ""
	@echo "VULNERABILITY: SUID Binary with system() Call"
	@echo "- Programs with SUID bit run with owner's privileges"
	@echo "- Using system() with user input allows command injection"
	@echo "- Attackers can execute arbitrary commands as root"
	@echo ""
	@echo "ATTACK VECTORS:"
	@echo "- Command injection through user arguments"
	@echo "- Shell metacharacter exploitation (;, &&, ||)"
	@echo "- Environment variable manipulation"
	@echo ""
	@echo "MITIGATIONS:"
	@echo "- Avoid SUID unless absolutely necessary"
	@echo "- Never use system() in privileged programs"
	@echo "- Use exec-family functions with sanitized input"
	@echo "- Drop privileges immediately after use"
	@echo "- Validate and sanitize all user input"
	@echo "- Use modern compiler security flags"
	@echo ""
	@echo "FILES:"
	@echo "- $(SOURCE): Source code with vulnerability"
	@echo "- $(TARGET)_vuln: Vulnerable binary"
	@echo "- $(TARGET)_secure: Secure version"
	@echo ""
	@echo "TESTING:"
	@echo "- Use 'make test-exploit' for exploitation demo"
	@echo "- Use 'make suid-demo' for interactive demonstration"
	@echo "- Requires --privileged Docker mode for SUID testing"
