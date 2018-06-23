SOURCES=RoapSource.cpp FileSink.cpp ColorRoutines.cpp HistogramRoutines.cpp Image.cpp ImageRoutines.cpp ProcessingRoutines.cpp

#OBJECTS_1=$(SOURCES:../%.cpp=obj/%.o)
OBJECTS_2=$(SOURCES:%.cpp=obj/%.o)
OBJECTS=$(OBJECTS_2:../%.c=obj/%.o) 
CFLAGS=-O3 -Wall -Werror $(shell pkg-config --cflags curlpp)
#LDFLAGS=$(shell pkg-config --libs curlpp)
LDFLAGS=-lcurl -lcurlpp -lm -lturbojpeg
DEPS=$(OBJECTS:.o=.d)

.PHONY: all clean 

all:	fetch stream

clean: 
	rm -rf $(OBJECTS) $(DEPS) fetch obj/fetch-main.*

fetch:	$(OBJECTS) obj/fetch-main.o
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) obj/fetch-main.o
	strip $@

stream:	$(OBJECTS) obj/stream-main.o
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) obj/stream-main.o
	strip $@

-include $(DEPS)
-include obj/fetch-main.d stream-main.d

obj/%.o:	%.cpp
	$(CXX) -MMD $(CFLAGS) -c $< -o $@
