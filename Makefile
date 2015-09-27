SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL_image -lSDL_ttf


main: main.o
	g++ $(SDL_LDFLAGS) main.o -o main

main.o: main.cpp
	g++ -c -Iincludes $(SDL_CFLAGS) main.cpp

run:
	./main

clean:	
	rm -f  main main.o
