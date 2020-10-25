LINKFLAGS=-lm -lSDL2 -lSDL2main -lGLEW -lGL
EXE=NonEuclidean



SRCS    := $(wildcard src/*.cpp)
OBJS    := $(patsubst src/%.cpp,comp/%.o,$(SRCS))

.PHONY: all clean

all: ${EXE}

clean:
	rm ${OBJS}

${EXE}: ${OBJS}
	c++ ${LINKFLAGS} comp/*.o -o final/${EXE}

#%.o : %.cpp ${HEADERS}

comp/%.o: include
	c++ -c -O3 src/$*.cpp -o comp/$*.o -I./include