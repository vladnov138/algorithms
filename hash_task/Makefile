CC = g++
CFLAGS = -std=c++17 -Wall -pedantic
SOURCE = main.cpp

ifeq ($(OS), Windows_NT)
	RM = del
	TARGET = main.exe
else
	RM = rm
	TARGET = main
endif

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	$(RM) $(TARGET)

run: all
	$(TARGET)