CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
LDFLAGS = -lm

TARGET  = graphics_editor
SRC     = graphics_editor.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
