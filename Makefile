# =============================================
# OurShell Makefile - Optimized Version
# Combines: 
# 1. Compiler warnings (-Wall -Wextra)
# 2. Clean rule
# 3. Explicit variables
# 4. Easy to extend
# =============================================

# Compiler and flags (customize these)
CC      = gcc
CFLAGS  = -Wall -Wextra
TARGET  = OurShell
SOURCES = OurShell.c

# Default build rule
all: $(TARGET)

# Link the executable
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Helper to show variables (run: make vars)
vars:
	@echo "Compiler: $(CC)"
	@echo "Flags:    $(CFLAGS)"
	@echo "Target:   $(TARGET)"
	@echo "Sources:  $(SOURCES)"

.PHONY: all clean vars