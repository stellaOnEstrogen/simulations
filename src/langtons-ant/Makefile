CC = gcc
CFLAGS = -lncurses
TARGET = langtons_ant
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
