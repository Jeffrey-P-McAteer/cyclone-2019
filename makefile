
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
    CFLAGS = -g -fPIC -rdynamic -shared -Lstatic -I./cyclone-headers -Wall
else ifeq ($(PLATFORM),WIN)
    # Windows only gets to build a static library build/cyclone.lib
    dylib: build/cyclone.lib
    lib: build/cyclone.lib
    clean: winclean
    CFLAGS = -g -fPIC -shared -Lstatic -I./cyclone-headers
else
    dylib: build/libcyclone.so
    lib: build/libcyclone.a
    clean: linuxclean
    CFLAGS = -g -fPIC -rdynamic -shared -Lstatic -I./cyclone-headers
endif

# Each src/*.cpp file should have a corresponding object it builds to
_OBJS = body.o collide_coarse.o collide_fine.o contacts.o \
		core.o fgen.o joints.o particle.o pcontacts.o \
		pfgen.o plinks.o pworld.o random.o world.o timing.o app.o
OBJS = $(patsubst %,build/%,$(_OBJS))

build/%.o: cyclone-src/%.cpp
ifeq ($(PLATFORM),WIN)
	if not exist "build" mkdir build
else
	[ -e ./build/ ] || mkdir ./build/
endif
	$(CC) -c $(CFLAGS) -o $@ $<

build/libcyclone.so: $(OBJS)
	$(CC) \
		$(CFLAGS) \
		-o build/libcyclone.so \
		$(OBJS)

build/libcyclone.a: $(OBJS)
	ar rvs build/libcyclone.a $^

build/cyclone.dll: $(OBJS)
	$(CC) \
        $(CFLAGS) \
        -o build/cyclone.dll \
        $(OBJS)

build/cyclone.lib: $(OBJS)
	ar rvs build/cyclone.lib $^

linuxclean:
	if [ -e ./build/ ] ; then rm -rf ./build/ ; fi

winclean:
	if exist "build" rmdir /Q /S build
