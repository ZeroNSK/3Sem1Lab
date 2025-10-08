CXX := clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

BIN := ds
SRCS := main.cpp massive.cpp onelinked.cpp twolinked.cpp queue.cpp stack.cpp AvlTree.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all run clean rebuild

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@

onelinked.o: CXXFLAGS += -Dmain=__demo_onelinked_main -Dprint=__onelink_print
twolinked.o: CXXFLAGS += -Dmain=__demo_twolinked_main
queue.o:     CXXFLAGS += -Dmain=__demo_queue_main
stack.o:     CXXFLAGS += -Dmain=__demo_stack_main
AvlTree.o:   CXXFLAGS += -Dmain=__demo_avl_main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJS) $(BIN)

rebuild: clean all
