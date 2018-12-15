// gcc -Wall -pedantic -c comboios.c; gcc -Wall -pedantic -c main.c; gcc main.o comboios.o -o comboios
// gcc comboios.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx; gcc main.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx; gcc comboios.o main.o -o comboios -g -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx

#include "comboios.h"

int main(int argc, char *argv[]){
  LISTA_LINHAS *topo_lista_linhas = NULL;
  LISTA_COMBOIOS *topo_lista_comboios = NULL;
  LISTA_GRAF_BOIO *boios_graficos = NULL;
  int dimensaoX, dimensaoY;
  SDL_Event event;
  Uint32 temporizador;
  int fim = 0;

  srand((unsigned long) &fim);

  leitor_configs(&topo_lista_comboios, &topo_lista_linhas, &dimensaoX, &dimensaoY, argv[1]);

  boios_graficos = inicializa_boios(boios_graficos, topo_lista_comboios);
  mostra_boios_ativos(boios_graficos);
  if ( inicializa_janela(dimensaoX,dimensaoY) == 0 ){
    exit(0);
  }

  while (fim != 1){
    temporizador = SDL_GetTicks();
    boios_graficos = mexe_comboios2(boios_graficos);
    atualiza_render(topo_lista_linhas);
    desenha_comboios(boios_graficos);
    SDL_PollEvent( &event );
    if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || event.type == SDL_QUIT) {
      fim = 1;
    }
    printf("\rA esperar%d ms", TICKS_p_FRAME - SDL_GetTicks() + temporizador);
    fflush(stdout);
    SDL_Delay(TICKS_p_FRAME - SDL_TICKS_PASSED(SDL_GetTicks(), temporizador));
    render();
	}
  SDL_Quit();

  return 0;
}
