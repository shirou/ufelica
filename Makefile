CC = gcc

SOFTWARE = ufelica
VERSION = 0.0.3

TAR = tar
INSTALL = install

#OS != uname -s
OS := $(shell uname -s)

ifeq ($(OS) , CYGWIN_NT-5.1)
TARGET = ufelica.exe
else
TARGET = ufelica
endif

SRCS   = main.c ufelica.c
OBJS   = main.o ufelica.o

INCLUDES = -I. -I../src
INCLUDES+= -I/usr/local/include
ifeq ($(OS) , NetBSD)
INCLUDES+= -I/usr/pkg/include
endif

LIBDIR = -L. -L..
LIBDIR+= -L/usr/local/lib
ifeq ($(OS) , NetBSD)
LIBDIR+= -L/usr/pkg/lib
endif

LIBS  = -lusb 
DEFS  = -DDEBUG -D$(OS)
CFLAGS = -g -Wall -O2 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(DEFS) $(CFLAGS) $(LIBDIR) $(INCLUDES) -o $@ $(OBJS) $(LIBS)  

.c.o: 
	$(CC) $(DEFS) $(CFLAGS) $(LIBDIR) $(INCLUDES) -c -o $@  $<

clean:	
	rm -rf $(TARGET) *.o *.tar.gz *~ *.core 
