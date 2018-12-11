// gcc -Wall -pedantic -c comboios.c; gcc -Wall -pedantic -c main.c; gcc main.o comboios.o -o comboios
// gcc comboios.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx; gcc main.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx; gcc comboios.o main.o -o comboios -g -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx

#include "comboios.h"

int main(void){
  LINHA **linhas = NULL;
  LISTA_PONTOS *ap;
  COMBOIO **trains = NULL;
  GRAF_BOIO **boios_graficos = NULL;
  int dimensaoX, dimensaoY;
  int i;
  SDL_Event event;
  int fim = 0;

  leitor_configs(&trains, &linhas, &dimensaoX, &dimensaoY);
  // for(i=0; linhas[i] !=NULL; i++){
  //   printf("/----LINHA %s----/\n", linhas[i]->id);
  //   for(ap = linhas[i]->l; ap!= NULL; ap=ap->pr[0]){
  //     mostra_ponto(ap->pt);
  //   }
  // }
  printf("chegou1\n");
  fflush(stdout);
  for(i=0; trains[i] != NULL; i++){
    printf("%d comboio\n", i);
    fflush(stdout);
    mostra_boio( *(trains[i]));
  }

  printf("chegou2\n");
  fflush(stdout);
  inicializa_boios(&boios_graficos, trains, linhas);
  printf("chegou3\n");
  fflush(stdout);
  if ( inicializa_janela(dimensaoX,dimensaoY) == 0 ){
    exit(0);
  }
  while (fim != 1){
    atualiza_render(trains, linhas);
    mexe_comboio(boios_graficos[0], linhas);
		// SDL_WaitEvent(&event);
    SDL_PollEvent( &event );
    if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || event.type == SDL_QUIT) {
      fim = 1;
    }
	}
  SDL_Quit();

  return 0;
}
