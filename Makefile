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
# ===============================================

# Attribution: Makefile partially paraphrased from output of ChatGPT.

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic

OUTFILE = cpusim
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = CPU.h Controller.h

default: $(OUTFILE)

# .o and .h -> executable
$(OUTFILE): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY: clean
