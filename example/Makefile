ROOT=..
-include $(ROOT)/Makefile.top

EXAMPLES=example1.cpp

OBJECTS=$(EXAMPLES:.cpp=.o)
PROGRAMS=$(EXAMPLES:.cpp=.exe)

all: $(PROGRAMS)

%.exe: %.o
	$(CC) $*.o $(LIBEASTL) -o $*.exe

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $*.cpp -o $*.o

clean:
	rm -f $(OBJECTS) $(PROGRAMS)

.phony: $(OBJECTS)
