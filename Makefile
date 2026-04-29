CC = gcc
# Adding -Itests/include so test_suite.c can find test_util.h
CFLAGS = -Wall -g -Iinclude -Itests/include

# .c files, necessary for compiling (Logica ADT)
COMMON_SRCS = src/date.c src/report.c src/list.c src/p_queue.c

# --- Main program ---
MAIN_SRCS = src/main.c
TARGET = gestione_report

# --- Test program ---
TEST_SRCS = tests/src/test_suite.c tests/src/test_util.c
TEST_TARGET = run_tests

# default rule: compile both test and main program
all: $(TARGET) $(TEST_TARGET)

# Compiling main program
$(TARGET): $(COMMON_SRCS) $(MAIN_SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(COMMON_SRCS) $(MAIN_SRCS)

# Compiling the test suite
$(TEST_TARGET): $(COMMON_SRCS) $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(COMMON_SRCS) $(TEST_SRCS)

# Clean
clean:
	rm -f $(TARGET).exe $(TEST_TARGET).exe tests/*_output.txt