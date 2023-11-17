CXX = g++
CXXFLAGS = -std=c++17
TARGET = notes
SRCS = main.cpp utils.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: init clean
init: clean $(TARGET) create-pre-commit-hook
	@echo "Initialization complete."

clean:
	rm -f $(TARGET)

.PHONY: install
install:
	sudo cp $(TARGET) /usr/local/bin
	@echo "$(TARGET) installed to /usr/local/bin"

.PHONY: create-pre-commit-hook
create-pre-commit-hook:
	@echo "#!/bin/bash" > .git/hooks/pre-commit
	@echo "" >> .git/hooks/pre-commit
	@echo "make clean" >> .git/hooks/pre-commit
	@echo "" >> .git/hooks/pre-commit
	@echo "# Capture the exit status of the make clean command" >> .git/hooks/pre-commit
	@echo "make_clean_exit_status=\$$?" >> .git/hooks/pre-commit
	@echo "" >> .git/hooks/pre-commit
	@echo "# If make clean fails, prevent the commit" >> .git/hooks/pre-commit
	@echo "if [ \$$make_clean_exit_status -ne 0 ]; then" >> .git/hooks/pre-commit
	@echo "    echo \"Error: 'make clean' failed. Please clean up the code before committing.\"" >> .git/hooks/pre-commit
	@echo "    exit 1" >> .git/hooks/pre-commit
	@echo "fi" >> .git/hooks/pre-commit
	@echo "" >> .git/hooks/pre-commit
	@echo "# If make clean succeeds, allow the commit" >> .git/hooks/pre-commit
	@echo "exit 0" >> .git/hooks/pre-commit
	@chmod +x .git/hooks/pre-commit
