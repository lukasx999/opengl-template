CXX=clang++
CXXFLAGS=-Wall -Wextra -I./glad/include/
LIBS=-lglfw -lGL -lglm

OBJS=main.o

gl: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

%.o: %.cc Makefile $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o gl

.PHONY: clean
