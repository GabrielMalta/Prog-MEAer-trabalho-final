#include "comboios.h"

void mostra_ponto(PONTO pt){
  char tipo[7];
  char cor[7];
  switch(pt.tipo):
    case 0: strcpy(tipo, "Via");      break;
    case 1: strcpy(tipo, "Estacao");  break;
    case 2: strcpy(tipo, "Entrada");  break;
    case 3: strcpy(tipo, "Saida");    break;
    default: printf("ERRO\n");        break;

  switch(pt.cor):
    case CINZENTO: strcpy(cor, "Cinzento"); break;
    case VERMELHO: strcpy(cor, "Cinzento"); break;
}
