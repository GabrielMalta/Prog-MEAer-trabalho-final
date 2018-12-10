// gcc -Wall -pedantic -c comboios.c; gcc -Wall -pedantic -c main.c; gcc main.o comboios.o -o comboios
// gcc comboios.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx; gcc main.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -c -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx; gcc comboios.o main.o -o comboios -g -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/i386-linux-gnu -lSDL2 -lm -lSDL2_image -lSDL2_gfx

#include "comboios.h"

int main(void){
  LINHA **linhas = NULL;
  LISTA_PONTOS *ap;
  COMBOIO **trains = NULL;
  int dimensaoX, dimensaoY;
  int i;

  leitor_configs(&trains, &linhas, &dimensaoX, &dimensaoY);
  for(i=0; linhas[i] !=NULL; i++){
    printf("/----LINHA %s----/\n", linhas[i]->id);
    for(ap = linhas[i]->l; ap!= NULL; ap=ap->pr){
      mostra_ponto(ap->pt);
    }
  }

  for(i=0; trains[i] != NULL; i++){
    mostra_boio( *(trains[i]));
  }

  if ( inicializa_janela(dimensaoX,dimensaoY) != 0 ){
    atualiza_render(trains, linhas);
  }

  return 0;
}
