CC = gcc
# Adding -Itests/include so test_suite.c can find test_util.h
CFLAGS = -Wall -g -Iinclude -Itests/include

# .c files, necessary for compiling 
COMMON_SRCS = src/date.c src/report.c src/list.c src/p_queue.c src/gestionale.c tests/src/test_util.c

# --- Main program ---
MAIN_SRCS = src/main.c
TARGET = build/gestione_report

# --- Test program ---
TEST_SRCS = tests/src/test_suite.c  tests/src/test_cases.c
TEST_TARGET = build/run_tests

# default rule: compile both test and main program
all: $(TARGET) $(TEST_TARGET)

# Compiling main program
$(TARGET): $(COMMON_SRCS) $(MAIN_SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(COMMON_SRCS) $(MAIN_SRCS)

# Compiling the test suite
$(TEST_TARGET): $(COMMON_SRCS) $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(COMMON_SRCS) $(TEST_SRCS)
