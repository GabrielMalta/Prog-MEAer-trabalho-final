#makefile V1

all: comboios

main.o: main.c comboios.h
	gcc main.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx;
comboios.o: comboios.c comboios.h
	gcc comboios.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx;
leitor_configs.o: leitor_configs.c comboios.h
	gcc leitor_configs.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx;
graf.o: graf.c comboios.h
	gcc graf.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx;
comboios: main.o comboios.o leitor_configs.o graf.o
	gcc comboios.o main.o graf.o leitor_configs.o -o comboios -g -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx
clean:
	rm main.o comboios.o leitor_configs.o graf.o comboios
