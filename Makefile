CC := cc
CFLAGS := -Wall -Wextra -pedantic
TARGET := bmblsh

$(TARGET): main.c process.c builtins.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET)
