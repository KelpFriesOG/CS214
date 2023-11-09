CC = gcc
# Flags include address and undefined sanitizer
CFLAGS = -Wall -Werror -fsanitize=address,undefined
TARGET = words

$(TARGET): *.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET) testdir