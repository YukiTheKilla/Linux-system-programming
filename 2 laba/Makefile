CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic 
LDFLAGS = -lstdc++fs -pthread -lzip
SOURCES = main.cpp
EXECUTABLE = backupd

all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(SOURCES) 
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)
clean:      
	rm -f $(EXECUTABLE)
