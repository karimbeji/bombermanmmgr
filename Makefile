LDFLAGS=-framework SDL -framework Cocoa -I/System/Library/Frameworks/OpenGL.framework/Headers -framework OpenGL
CFLAGS=-g -Wall -I/Library/Frameworks/SDL.framework/Headers  -I/System/Library/Frameworks/OpenGL.framework/Headers

bomberman: bomberman.o maze.o graphics.o SDLmain.o
	gcc -o $@ ${LDFLAGS} bomberman.o maze.o graphics.o SDLmain.o

SDLmain.o: SDLmain.m
	gcc -c ${CFLAGS} $<
bomberman.o: bomberman.c maze.h graphics.h
	gcc -c ${CFLAGS} bomberman.c
maze.o: maze.c maze.h
	gcc -c ${CFLAGS} maze.c
graphics.o: graphics.c graphics.h
	gcc -c ${CFLAGS} graphics.c

clean: 
	rm bomberman *.o