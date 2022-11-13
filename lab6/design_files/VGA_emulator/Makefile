CC = g++
CCFLAGS = -g
LIBS = -lX11
SRC = vga.c
TARGET = vga

.PHONY: all clean release test

all: $(TARGET)

release: CCFLAGS += -O3
release: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CCFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

