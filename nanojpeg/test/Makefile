GTEST_DIR=/usr/src/gtest

SOURCES=main.cpp ../colorRoutines.c colorRoutines_test.cpp

OBJECTS_1=$(SOURCES:%.cpp=obj/%.o)
OBJECTS=$(OBJECTS_1:../%.c=obj/%.o) obj/gtest-all.o
CFLAGS=-g -I $(GTEST_DIR)/include
LDFLAGS=-pthread

DEPS=$(OBJECTS:.o=.d)

.PHONY: all clean 

all:	test
	./test

clean: 
	rm -rf $(OBJECTS) $(DEPS) test

test:	$(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)

obj/%.o:	%.cpp
	$(CXX) -MMD $(CFLAGS) -c $< -o $@

obj/%.o:	../%.c
	$(CC) -MMD $(CFLAGS) -c $< -o $@

obj/gtest-all.o:	$(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -MMD $(CFLAGS) -I $(GTEST_DIR) -c $< -o $@
