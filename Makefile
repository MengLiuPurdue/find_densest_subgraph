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
CXXFLAGS := -std=c++11 -Wall -O3 -Wshadow -Werror -fPIC

SRCS = densest_subgraph.cpp maxflow.cpp running_test.cpp
OBJS = $(SRCS:%.cpp=%.o)

LIBOBJS = densest_subgraph.o maxflow.o

all: $(TARGET) $(LIB)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(LIB): $(LIBOBJS)
	$(CC) $(SHAREDLIB_FLAG) -o $(LIB) $(LIBOBJS)

test:
	./find_densest_subgraph yeast-cc.smat
	./find_densest_subgraph pgp-cc.smat
	./find_densest_subgraph musm-cc.smat
	./find_densest_subgraph marvel-comics-cc.smat
	./find_densest_subgraph marvel-chars-cc.smat
	./find_densest_subgraph homo-cc.smat
	./find_densest_subgraph Erdos02-cc.smat
	./find_densest_subgraph as-caida20060911.smat
	./find_densest_subgraph as19991115.smat

.PHONY: all clean test

clean:
	$(RM) $(TARGET) $(LIB) $(OBJS)
