// gcc -Wall -pedantic -c comboios.c; gcc -Wall -pedantic -c main.c; gcc main.o comboios.o -o comboios
#include "comboios.h"

int main(void){
  LINHA **linha = NULL;
  LISTA_PONTOS *ap;
  COMBOIO **train = NULL;
  int dimensaoX, dimensaoY;
  int i;

  leitor_configs(&train, &linha, &dimensaoX, &dimensaoY);
  printf("cheguei\n");
  fflush(stdout);
  for(i=0; linha[i] !=NULL; i++){
    for(ap = linha[i]->l; ap!= NULL; ap=ap->pr){
      mostra_ponto(ap->pt);
    }
  }

  for(i=0; train[i] != NULL; i++){
    mostra_boio( *(train[i]));
  }

  return 0;
}