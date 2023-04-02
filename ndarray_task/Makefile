CC = g++
CFLAGS = -std=c++17 -Wall -pedantic
SOURCES = main.cpp
DICT_PY = dict.py
GRAPHIC = main.py

ifeq ($(OS), Windows_NT)
	RM = del
	TARGET = main.exe
else
	RM = rm
	TARGET = main
endif

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	$(RM) $(TARGET)

run: all
	$(TARGET)