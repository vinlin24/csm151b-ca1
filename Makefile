# ==================== USAGE ==================== #
#                                                 #
#   * To build the main executable:               #
#                                                 #
#       make                                      #
#                                                 #
#   * To clean the directory of main executable   #
#     and intermediate object files:              #
#                                                 #
#       make clean                                #
#                                                 #
#   * To run individual tests:                    #
#                                                 #
#       make test-r                               #
#       make test-sw                              #
#       make test-all                             #
#       make test1                                #
#                                                 #
#   * To run all tests:                           #
#                                                 #
#       make test                                 #
#       make test 2>/dev/null # Discard stderr    #
#                                                 #
# =============================================== #

# Attribution: SRCS and OBJS trick taken from output of ChatGPT.

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic

OUTFILE = cpusim
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Directory containing the trace files.
TRACES = traces

default: $(OUTFILE)

# .o -> executable
$(OUTFILE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTFILE)

test-r: $(OUTFILE)
	@./$(OUTFILE) $(TRACES)/23instMem-r.txt
	@[ "$$(./$(OUTFILE) $(TRACES)/23instMem-r.txt 2>/dev/null)" = "(-8,23)" ] \
		|| (echo "FAIL: Expected (-8,23)" && false)

test-sw: $(OUTFILE)
	@./$(OUTFILE) $(TRACES)/23instMem-sw.txt
	@[ "$$(./$(OUTFILE) $(TRACES)/23instMem-sw.txt 2>/dev/null)" = "(9,17)" ] \
		|| (echo "FAIL: Expected (9,17)" && false)

test-all: $(OUTFILE)
	@./$(OUTFILE) $(TRACES)/23instMem-all.txt
	@[ "$$(./$(OUTFILE) $(TRACES)/23instMem-all.txt 2>/dev/null)" = "(40,1)" ] \
		|| (echo "FAIL: Expected (40,1)" && false)

test1: $(OUTFILE)
	@./$(OUTFILE) $(TRACES)/23instMem-test1.txt
	@[ "$$(./$(OUTFILE) $(TRACES)/23instMem-test1.txt 2>/dev/null)" \
		= "(46,-10)" ] || (echo "FAIL: Expected (46,-10)" && false)

# NOTE: test1 is the Gradescope Test 1 case. If you don't have the instMem for
# that, remove it from the prerequisites.
test: test-r test-sw test-all test1

.PHONY: clean test-r test-sw test-all test1 test
