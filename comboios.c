#include "comboios.h"

void mostra_ponto(PONTO pt){
  char tipo[7];
  char cor[8];
  switch(pt.tipo){
    case 0: strcpy(tipo, "Via");      break;
    case 1: strcpy(tipo, "Estacao");  break;
    case 2: strcpy(tipo, "Entrada");  break;
    case 3: strcpy(tipo, "Saida");    break;
    default: printf("ERRO\n");        break;
  }

  switch(pt.cor){
    case CINZENTO: strcpy(cor, "Cinzento"); break;
    case VERMELHO: strcpy(cor, "Cinzento"); break;
    case ROXO:     strcpy(cor, "Roxo");     break;
    case AZUL:     strcpy(cor, "Azul");     break;
    case CIANO:    strcpy(cor, "Ciano");    break;
    case VERDE:    strcpy(cor, "Verde");    break;
    case AMARELO:  strcpy(cor, "Amarelo");  break;
    case CASTANHO: strcpy(cor, "Castanho"); break;
    case PRETO:    strcpy(cor, "Preto");    break;
    case BRANCO:   strcpy(cor, "Branco");   break;
    default: printf("ERRO\n");              break;
  }

  printf("\nPonto: %s\nTipo: %s\nCor: %s\n", pt.id, tipo, cor);
}
