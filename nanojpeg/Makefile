LUA_CFLAGS=`pkg-config lua5.2 --cflags`
LUA_LDFLAGS=`pkg-config lua5.2 --libs`

SOURCES=nanojpeg.c luabinding.c colorRoutines.c

OBJECTS=$(SOURCES:%.c=%.o)
DEPS=$(OBJECTS:.o=.d)

.PHONY: all clean 

all:	nanojpeg.so

install:  nanojpeg.so 
	cp nanojpeg.so ..
clean: 
	rm -rf $(OBJECTS) $(DEPS) nanojpeg.so

nanojpeg.so:	$(OBJECTS)
	$(CC) -shared $(LUA_LDFLAGS) -o $@ $(OBJECTS)

%.o:	%.c
	$(CC) -MMD -fPIC $(LUA_CFLAGS) -c $< -o $@
