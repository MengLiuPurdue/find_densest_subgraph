UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
	SHAREDLIB_EXT := so
	SHAREDLIB_FLAG := -shared
endif
ifeq ($(UNAME),Darwin)
	SHAREDLIB_EXT := dylib
	SHAREDLIB_FLAG := -dynamiclib
endif

TARGET := find_densest_subgraph
LIB := libdensest_subgraph.$(SHAREDLIB_EXT)

CC := $(CXX)
CXXFLAGS := -Wall -O3 -Wshadow -Werror

SRCS = densest_subgraph.cpp maxflow.cpp running_test.cpp
OBJS = $(SRCS:%.cpp=%.o)

LIBOBJS = densest_subgraph.o maxflow.o

all: $(TARGET) $(LIB)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(LIB): $(LIBOBJS)
	$(CC) $(SHAREDLIB_FLAG) -o $(LIB) $(LIBOBJS)

test:
	./running_test yeast-cc.smat
	./running_test pgp-cc.smat
	./running_test musm-cc.smat
	./running_test marvel-comics-cc.smat
	./running_test marvel-chars-cc.smat
	./running_test homo-cc.smat
	./running_test Erdos02-cc.smat
	./running_test as-caida20060911.smat
	./running_test as19991115.smat

.PHONY: all clean test

clean:
	$(RM) $(TARGET) $(LIB) $(OBJS)
