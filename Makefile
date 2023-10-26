# ==================== USAGE ====================
#
#   * To build the main executable:
#
#       make
#
#   * To clean the directory of binary files:
#
#       make clean
#
#   * To run individual tests:
#
#       make test-r
#       make test-sw
#       make test-all
#       make test1
#
# ===============================================

# Attribution: Makefile partially paraphrased from output of ChatGPT.

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic

OUTFILE = cpusim
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.h)

default: $(OUTFILE)

# .o and .h -> executable
$(OUTFILE): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTFILE)

test-r:
	./$(OUTFILE) traces/23instMem-r.txt

test-sw:
	./$(OUTFILE) traces/23instMem-sw.txt

test-all:
	./$(OUTFILE) traces/23instMem-all.txt

test1:
	./$(OUTFILE) traces/23instMem-test1.txt

.PHONY: clean test-r test-sw test-all test1
