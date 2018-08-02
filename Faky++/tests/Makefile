GTEST_DIR=/usr/src/gtest

SOURCES=main.cpp ../Image.cpp Image_test.cpp ../FileSource.cpp Filesource_test.cpp ../ColorRoutines.cpp ColorRoutines_test.cpp ../HistogramRoutines.cpp HistogramRoutines_test.cpp ../ImageRoutines.cpp ImageRoutines_test.cpp ../ProcessingRoutines.cpp ProcessingRoutines_test.cpp ../FileSink.cpp ../LEDSink.cpp LEDSink_test.cpp

OBJECTS_1=$(SOURCES:../%.cpp=obj/%.o)
OBJECTS_2=$(OBJECTS_1:%.cpp=obj/%.o)
OBJECTS=$(OBJECTS_2:../%.c=obj/%.o) obj/gtest-all.o
CFLAGS=-g -Wall -Werror -I $(GTEST_DIR)/include -I ..
LDFLAGS= -lturbojpeg -pthread -lm

DEPS=$(OBJECTS:.o=.d)

.PHONY: all clean 

all:	test
	./test

clean: 
	rm -rf $(OBJECTS) $(DEPS) test

test:	$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) 

-include $(DEPS)

obj/%.o:	%.cpp
	$(CXX) -MMD $(CFLAGS) -c $< -o $@

obj/%.o:	../%.cpp
	$(CXX) -MMD $(CFLAGS) -c $< -o $@

obj/gtest-all.o:	$(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -MMD $(CFLAGS) -I $(GTEST_DIR) -c $< -o $@
