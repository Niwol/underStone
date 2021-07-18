CC = g++
CFLAGS = -Wall -std=c++20
LDFLAGS = -lSDL2 

EXEC = game
SRC = $(wildcard *.cpp)
HDR = $(wildcard *.hpp)
OBJ = $(SRC:.cpp=.o)

%o: %cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f game
