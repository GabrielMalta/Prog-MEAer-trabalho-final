#include "comboios.h"

void mostra_ponto(PONTO pt){
  char tipo[8];
  switch(pt.tipo){
    case 0: strcpy(tipo, "Erro");      break;
    case 1: strcpy(tipo, "Estacao");  break;
    case 2: strcpy(tipo, "Entrada");  break;
    case 3: strcpy(tipo, "Saida");    break;
    case 4: strcpy(tipo, "Via");    break;
    default: printf("Erro traducao de tipo\n"); break;
  }
  printf("\nPonto: %s\nTipo: %s\nCor: %s\n", pt.id, tipo, cor_numero(pt.cor));
  fflush (stdout);
}

int numero_tipo(char string[]){ //funciona
  // converte a string de tipo num inteiro para armazenar
  if (strcmp("EST", string) == 0) return 1;
  if (strcmp("ENT", string) == 0) return 2;
  if (strcmp("SAI", string) == 0) return 3;
  if (strcmp("VIA", string) == 0) return 4;
  return 0;
}

int numero_cor(char string[]){ //funciona
  // converte a string de cor num inteiro para armazenar
  if (strcmp("CINZENTO", string) == 0) return CINZENTO;
  if (strcmp("VERMELHO", string) == 0) return VERMELHO;
  if (strcmp("ROXO", string) == 0) return ROXO;
  if (strcmp("AZUL", string) == 0) return AZUL;
  if (strcmp("CIANO", string) == 0) return CIANO;
  if (strcmp("VERDE", string) == 0) return VERDE;
  if (strcmp("AMARELO", string) == 0) return AMARELO;
  if (strcmp("CASTANHO", string) == 0) return CASTANHO;
  if (strcmp("PRETO", string) == 0) return PRETO;
  if (strcmp("BRANCO", string) == 0) return BRANCO;
  printf("Erro leitura de cor\n");
  exit(0);
  return 0;
}

char * cor_numero(int no){
  // converte o numero para um string de cor correspondente
  switch(no){
    case CINZENTO: return "Cinzento";
    case VERMELHO: return "Vermelho";
    case ROXO:     return "Roxo";
    case AZUL:     return "Azul";
    case CIANO:    return "Ciano";
    case VERDE:    return "Verde";
    case AMARELO:  return "Amarelo";
    case CASTANHO: return "Castanho";
    case PRETO:    return "Preto";
    case BRANCO:   return "Branco";
  }
  printf("Erro cor_numero\n");
  exit(0);
}

void leitor_configs(COMBOIO ***comboios, LINHA ***linhas, int *dim_X, int *dim_Y){
  FILE *config = fopen("config.txt", "r");
  LINHA *nova_linha = NULL;
  LISTA_PONTOS *nova_lista_pt = NULL, *atual=NULL;
  COMBOIO *novo_boio = NULL;

  int numero_linhas=0;
  int numero_comboios=0;

  char leitura[100];
  char aux_string[6][10];
  int aux_int[5];

  if(config==NULL){
    printf("Erro de abertura de config;\n");
    exit(0);
  }

  while( fgets(leitura, 100, config) != NULL){
    if ( (leitura[0] >= '0' && leitura[0] <='9') || (leitura[0] >= 'A' && leitura[0] <='Z') ){
      if (sscanf(leitura, "%d %d", aux_int, aux_int+1) == 2){
        *dim_X = aux_int[0];
        *dim_Y = aux_int[1];
      }
      else if (sscanf(leitura, "LINHA: %s", aux_string[0]) == 1){
        numero_linhas++;
        nova_linha = (LINHA*) calloc(1, sizeof(LINHA));
        strcpy(nova_linha->id, aux_string[0]);
        *linhas = realloc(*linhas, (numero_linhas+1)*sizeof(LINHA*));
        (*linhas)[numero_linhas-1] = nova_linha;
        (*linhas)[numero_linhas-1]->l = NULL;
        (*linhas)[numero_linhas] = NULL;
        printf("%p\n", (void*) (*linhas)[numero_linhas-1]);
        fflush(stdout);
      }
      //se for ponto
      else if (sscanf(leitura, "%s %d %d %s %s %d %d %d", aux_string[0], aux_int, aux_int+1, aux_string[1], aux_string[2], aux_int+2, aux_int+3, aux_int+4) == 8){

        printf("%s %d %d %s %s %d %d %d\n", aux_string[0], aux_int[0], aux_int[1], aux_string[1], aux_string[2], aux_int[2], aux_int[3], aux_int[4]);

        nova_lista_pt = (LISTA_PONTOS*) calloc(1, sizeof(LISTA_PONTOS));
        // aux_string[0][5] = '\0';
        strcpy(nova_lista_pt->pt.id, aux_string[0]);
        nova_lista_pt->pt.x = aux_int[0];
        nova_lista_pt->pt.y = aux_int[1];
        nova_lista_pt->pt.cor = numero_cor(aux_string[1]);
        nova_lista_pt->pt.tipo = numero_tipo(aux_string[2]);
        nova_lista_pt->pt.dim = aux_int[2];
        nova_lista_pt->pt.entradas = aux_int[3];
        nova_lista_pt->pt.saidas = aux_int[4];
        if ((*linhas)[numero_linhas-1]->l == NULL){
          (*linhas)[numero_linhas-1]->l = nova_lista_pt;
          atual = nova_lista_pt;
        }
        else{
          atual->pr = nova_lista_pt;
          atual = nova_lista_pt;
        }
      }
      //se for comboio
      else if (sscanf(leitura, "COMBOIO: %s %d %d %s %s %s %s %s", aux_string[0], aux_int, aux_int+1, aux_string[1], aux_string[2], aux_string[3], aux_string[4], aux_string[5]) == 8){
        numero_comboios++;
        novo_boio = (COMBOIO*) calloc(1, sizeof(COMBOIO));
        printf("COMBOIO: %s %d %d %s %s %s %s %s",aux_string[0], aux_int[0], aux_int[1], aux_string[1], aux_string[2], aux_string[3], aux_string[4], aux_string[5]);

        strcpy(novo_boio->id, aux_string[0]);
        novo_boio->dim = aux_int[0];
        novo_boio->r_bolas = aux_int[1];
        novo_boio->cor[0] = numero_cor(aux_string[1]);
        strcpy(novo_boio->orig_l, aux_string[2]);
        strcpy(novo_boio->orig_pt, aux_string[3]);
        strcpy(novo_boio->dest_l, aux_string[4]);
        strcpy(novo_boio->dest_pt, aux_string[5]);

        *comboios = realloc(*comboios, (numero_comboios+1)*sizeof(LINHA*));
        (*comboios)[numero_comboios-1] = novo_boio;
        (*comboios)[numero_comboios] = NULL;

      }
    }
  }
}

void mostra_boio(COMBOIO boio){
  printf("\nCOMBOIO: %s \nDimensao:%d \nR_bolas:%d \nCor locom.:%s \nLinha,Ponto origem:%s,%s \nLinha,ponto destino:%s,%s \n", boio.id, boio.dim, boio.r_bolas, cor_numero(boio.cor[0]), boio.orig_l, boio.orig_pt, boio.dest_l, boio.dest_pt);
  fflush(stdout);
}
