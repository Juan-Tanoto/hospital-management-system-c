CC      = gcc
CFLAGS  = -Wall -std=c99
SRC     = $(wildcard src/*.c)
TARGET  = hms

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC) src/main.h
	$(CC) $(CFLAGS) -o $@ $(SRC)

# The program reads and writes its .txt "database" in the working directory
run: $(TARGET)
	cd data && ../$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
