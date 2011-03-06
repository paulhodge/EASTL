
CC=g++
CFLAGS=-c -I../include -DEASTL_ASSERT_ENABLED
LDFLAGS=

EASTL_SRC_DIR=../src
EASTL_SOURCES=$(EASTL_SRC_DIR)/allocator.cpp \
    $(EASTL_SRC_DIR)/assert.cpp \
    $(EASTL_SRC_DIR)/fixed_pool.cpp \
    $(EASTL_SRC_DIR)/hashtable.cpp \
    $(EASTL_SRC_DIR)/red_black_tree.cpp \
    $(EASTL_SRC_DIR)/string.cpp

SOURCES=example.cpp $(EASTL_SOURCES)

OBJECTS=$(SOURCES:.cpp=.o)
EXE=example

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
