
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
    all: build/app
    LIB = ../build/libcyclone.a
    clean: linuxclean
    CFLAGS = -I../cyclone-headers -L../build -Wl,-Bstatic -lcyclone -Wl,-Bdynamic -lGL -lGLU -lglut
else ifeq ($(PLATFORM),WIN)
    all: build//app.exe
    LIB = ..//build//cyclone.lib
    clean: winclean
    CFLAGS = -I..//cyclone-headers -L..//build -Wl,-Bstatic -lcyclone -Wl,-Bdynamic -lglu32 -lopengl32 -lfreeglut
else
    dylib: build/libcyclone.so
    lib: build/libcyclone.a
    clean: linuxclean
    CFLAGS = -I../cyclone-headers -L../build -Wl,-Bstatic -lcyclone -Wl,-Bdynamic -lGL -lGLU -lglut
endif


build/app: main.cpp $(LIB)
	[ -e build ] || mkdir build
	$(CC) -g main.cpp $(CFLAGS) -o build/app

build//app.exe: main.cpp $(LIB)
	[ -e build ] || mkdir build
	$(CC) -g main.cpp $(CFLAGS) -o build//app.exe

linuxclean:
	if [ -e build ] ; then rm -rf build ; fi

winclean:
	if exist "build" rmdir /Q /S build

