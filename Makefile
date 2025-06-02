CXX=clang++
CXXFLAGS=-Wall -Wextra -I./glad/include/ -Og -ggdb -std=c++23 -pedantic
LIBS=-lglfw -lGL -lglm

OBJS=main.o

gl: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

%.o: %.cc Makefile $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: gl
	./$<

clean:
	rm *.o gl

.PHONY: clean
