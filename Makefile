CC = gcc
CFLAGS = -Wall -g -Iinclude

# File sorgenti (nella cartella src)
SRCS = src/date.c src/report.c src/list.c src/main.c

# Nome del programma finale
TARGET = gestione_report

# Regola per creare il programma
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Pulizia
clean:
	rm -f $(TARGET)