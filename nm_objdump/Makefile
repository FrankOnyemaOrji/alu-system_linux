# Define the C compiler
CC := gcc

# Set compiler flags for strict code standards and debugging
CFLAGS := -Wall -Werror -Wextra -pedantic -g -std=c99

# Extra flags for debug mode
DEBUGFLAGS := -g

# Specify the names of the executables
TARGET_0 := hnm
TARGET_1 := hobjdump

# List source files for each program
SOURCE_0 := hnm-64.c hnm-32.c hnm-main.c hnm.h
SOURCE_1 := hobjdump.c

# Default target to build both programs
all: $(TARGET_0) $(TARGET_1)

# Enable debug mode
debug: CFLAGS += $(DEBUGFLAGS)
debug: all

# Compilation rule for hnm
$(TARGET_0): $(SOURCE_0)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation rule for hobjdump
$(TARGET_1): $(SOURCE_1)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up compiled files and temporary artifacts
clean:
	rm -f $(TARGET_0) $(TARGET_1) *.o *.out *~

# Declare phony targets
.PHONY: all clean debug