SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer


main: main.o
	g++ $(SDL_LDFLAGS) main.o -o main

main.o: main.cpp
	g++ -std=c++11 -c -Iincludes $(SDL_CFLAGS) main.cpp

run:
	./main

clean:	
	rm -f  main main.o
