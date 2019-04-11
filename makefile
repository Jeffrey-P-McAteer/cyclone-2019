
all: dylib lib

CC = g++

# PLATFORM = One of WIN, LINUX, or MAC

ifeq ($(OS),Windows_NT)
    PLATFORM = WIN
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM = MAC
    endif
endif

ifeq ($(PLATFORM),)
    $(error Error unknown platform)
endif

# Here we define generic targets into specific targets based on OS detected
ifeq ($(PLATFORM),LINUX)
    dylib: build/libcyclone.so
    lib: build/libcyclone.a
    clean: linuxclean
    CFLAGS = -g -ggdb -fPIC -rdynamic -shared -Lstatic -I./cyclone-headers
else ifeq ($(PLATFORM),WIN)
    dylib: build/cyclone.dll
    lib: build/cyclone.lib
    clean: winclean
    CFLAGS = -g -ggdb -fPIC -rdynamic -shared -Lstatic -I./cyclone-headers
else
    dylib: build/libcyclone.so
    lib: build/libcyclone.a
    clean: linuxclean
    CFLAGS = -g -ggdb -fPIC -rdynamic -shared -Lstatic -I./cyclone-headers
endif

_OBJS = body.o collide_coarse.o collide_fine.o contacts.o \
		core.o fgen.o joints.o particle.o pcontacts.o \
		pfgen.o plinks.o pworld.o random.o world.o
OBJS = $(patsubst %,build/%,$(_OBJS))

build/%.o: cyclone-src/%.cpp
	[ -e ./build/ ] || mkdir ./build/
	$(CC) -c $(CFLAGS) -o $@ $<

build/libcyclone.so: $(OBJS)
	$(CC) \
		$(CFLAGS) \
		-o build/libcyclone.so \
		$(OBJS)

build/libcyclone.a: $(OBJS)
	ar rvs build/libcyclone.a $^

build/cyclone.dll: $(OBJS)
	echo TODO

build/cyclone.lib: $(OBJS)
	echo TODO

linuxclean:
	if [ -e ./build/ ] ; then rm -rf ./build/ ; fi

winclean:
	if exist "build" rmdir /Q /S build
