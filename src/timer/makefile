CXX	= g++
CONFIG  = ``
PROG_NAME = fmath_test
OBJS = timer.o
MAIN = fmath_test.cpp
.SUFFIXES: .o .cpp 

$(PROG_NAME): $(MAIN) $(OBJS)
	$(CXX) $(GLFLAGS) -o $@ $^ $(CONFIG)

.cpp.o:
	$(CXX) $(CONFIG) -c $<

clean:
	rm -f *.o $(PROG_NAME)
