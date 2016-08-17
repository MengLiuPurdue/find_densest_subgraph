TARGET = running_test
GCC = g++
CFLAGS = -g -Wall -Wshadow -Werror
CC = $(GCC) $(CFLAGS)
SRCS = densest_subgraph.cpp maxflow.cpp running_test.cpp
OBJS = $(SRCS:%.cpp=%.o)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

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

.cpp.o:
	$(GCC) $(CFLAGS) -c $*.cpp

clean:
	rm -f $(TARGET) $(OBJS)
