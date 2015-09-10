SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)


main: main.o
	g++ $(SDL_LDFLAGS) main.o -o main

main.o: main.cpp
	g++ -c $(SDL_CFLAGS) main.cpp

run:
	./main

clean:	
	rm -f  main main.o
