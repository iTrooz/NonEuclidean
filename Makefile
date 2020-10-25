
SRCS		:= $(wildcard src/*.cpp)
HEADERS		:= $(wildcard headers/*.h)
OBJS		:= $(patsubst src/%.cpp,comp/%.o,$(SRCS))

.PHONY: all clean

all: final/NonEuclidean

clean:
	rm ${OBJS}

final/NonEuclidean: ${OBJS}
	c++ -lm -lSDL2 -lSDL2main -lGLEW -lGL comp/* -o final/NonEuclidean

comp/%.o: src/%.cpp ${HEADERS}
	c++ -c -O3 src/$*.cpp -o comp/$*.o -I./include