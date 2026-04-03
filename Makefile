CC = gcc
# Aggiungiamo -Itests/include per permettere a test_suite.c di trovare test_util.h
CFLAGS = -Wall -g -Iinclude -Itests/include

# File comuni necessari (Logica ADT)
COMMON_SRCS = src/date.c src/report.c src/list.c

# --- Programma Principale ---
MAIN_SRCS = src/main.c
TARGET = gestione_report

# --- Programma dei Test ---
# Nota: i percorsi riflettono la tua nuova struttura tests/src/
TEST_SRCS = tests/src/test_suite.c tests/src/test_util.c
TEST_TARGET = run_tests

# Regola di default: compila entrambi
all: $(TARGET) $(TEST_TARGET)

# Compilazione Programma Principale
$(TARGET): $(COMMON_SRCS) $(MAIN_SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(COMMON_SRCS) $(MAIN_SRCS)

# Compilazione Suite di Test
$(TEST_TARGET): $(COMMON_SRCS) $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(COMMON_SRCS) $(TEST_SRCS)

# Pulizia (Compatibile con Windows se usi Git Bash/MinGW, altrimenti usa 'del')
clean:
	rm -f $(TARGET).exe $(TEST_TARGET).exe tests/*_output.txt