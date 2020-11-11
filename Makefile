CC = g++
AR = ar crv

FLAGS = -g -std=c++11

LIBS = -lpmem

OBJECT := arena.o \
          btree.o

TARGET = libpmbtree.a

$(TARGET) : $(OBJECT)
	$(AR) $@ $^

.cc.o :
	$(CC) -c $(FLAGS) -o $@ $< $(LIBS)

clean :
	rm $(TARGET) *.o