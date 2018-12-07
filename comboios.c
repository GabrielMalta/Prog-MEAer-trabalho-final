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

int leitor_configs(TRAIN_LIST *train_list, LINHA *linhas, int *dim_X, int *dim_Y){
  FILE *config = fopen("config.txt", "r");
  char leitura[100];
  char aux_string[100];
  int aux_int[5];

  if(config==NULL){
    printf("Erro de leitura de config;\n");
    exit(0);
  }

  while( fgets(leitura, 100, config) != NULL){
    if ( (leitura[0] >= '0' && <='9') || (leitura[0] >= 'A' && <='Z') ){
      if (sscanf(leitura, "%d %d", aux_int[0], aux_int[1]) == 2){
        *dim_X = aux_int[0];
        *dim_Y = aux_int[1];
      }
      else if (sscanf(leitura, "LINHA: %s", aux_string) == 1){
        strcpy()
      };
      sscanf(leitura, "%s")
    }
  }
}
