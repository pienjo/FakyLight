COMMON_SOURCES=RoapSource.cpp Image.cpp ImageRoutines.cpp 
CALCULATE_SOURCES=$(COMMON_SOURCES) ColorRoutines.cpp HistogramRoutines.cpp ProcessingRoutines.cpp Scheduler.cpp LEDSink.cpp 
FETCH_SOURCES=$(COMMON_SOURCES) FileSink.cpp fetch-main.cpp
STREAM_SOURCES=$(CALCULATE_SOURCES) UDPStrip.cpp stream-main.cpp
STREAM_GPIO_SOURCES=$(CALCULATE_SOURCES) WS2812Strip.cpp MixingStrip.cpp GPIOPin.cpp stream_gpio-main.cpp

FETCH_OBJECTS=$(FETCH_SOURCES:%.cpp=obj/%.o)
STREAM_OBJECTS=$(STREAM_SOURCES:%.cpp=obj/%.o)
STREAM_GPIO_OBJECTS=$(STREAM_GPIO_SOURCES:%.cpp=obj/%.o)

CFLAGS=-g -Wall -Werror $(shell pkg-config --cflags curlpp)
LDFLAGS=-lcurl -lcurlpp -lm -lturbojpeg -lpthread -lgpiod

DEPS=$(FETCH_OBJECTS:.o=.d) $(STREAM_OBJECTS:.o=.d) $(STREAM_GPIO_OBJECTS:.o=.d)


.PHONY: all clean 

all:	fetch stream stream_gpio

clean: 
	rm -rf $(FETCH_OBJECTS) $(STREAM_OBJECTS) $(STREAM_GPIO_OBJECTS) $(DEPS) fetch stream stream_gpio

fetch:	$(FETCH_OBJECTS) 
	$(CXX) -o $@ $(FETCH_OBJECTS) $(LDFLAGS) 
	#strip $@

stream:	$(STREAM_OBJECTS) 
	$(CXX) -o $@ $(STREAM_OBJECTS) $(LDFLAGS)
	#strip $@

stream_gpio:	$(STREAM_GPIO_OBJECTS) 
	$(CXX) -o $@ $(STREAM_GPIO_OBJECTS) $(LDFLAGS)
	#strip $@

obj/%.o:	%.cpp
	$(CXX) -MMD $(CFLAGS) -c $< -o $@

-include $(DEPS)
