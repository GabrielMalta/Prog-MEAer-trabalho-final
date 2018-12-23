#include "comboios.h"

void leitor_configs(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas, int *dim_X, int *dim_Y, char *nome_ficheiro){
  FILE *config = fopen(nome_ficheiro, "r");
  LISTA_LINHAS *nova_linha;
  LISTA_PONTOS *atual=NULL;

  char leitura[100];
  char aux_string[6][10];
  int aux_int[5];

  if(config==NULL){
    printf("Erro de abertura de config;\n\"%s\" nao encontrado\n", nome_ficheiro);
    printf("Utilizacao: ./comboios <ficheiro.txt>\n");
    exit(0);
  }

  while( fgets(leitura, 100, config) != NULL){
    if ( (leitura[0] >= '0' && leitura[0] <='9') || (leitura[0] >= 'A' && leitura[0] <='Z') ){
      if (sscanf(leitura, "%d %d", aux_int, aux_int+1) == 2){
        *dim_X = aux_int[0];
        *dim_Y = aux_int[1];
      }
      //se for linha
      else if (sscanf(leitura, "LINHA: %s", aux_string[0]) == 1){

        nova_linha = (LISTA_LINHAS*) calloc(1, sizeof(LISTA_LINHAS));
        strcpy(nova_linha->id, aux_string[0]);
        nova_linha->pr=*topo_lista_linhas;
        *topo_lista_linhas=nova_linha;
      }
      //se for ponto
      else if (sscanf(leitura, "%s %d %d %s %s", aux_string[0], aux_int, aux_int+1, aux_string[1], aux_string[2]) == 5){
        preenche_linha(aux_string[0], aux_int, nova_linha, &atual);
      }
      //Ligacao de pontos
      else if (sscanf(leitura, "LIGAR: %s %s %s %s", aux_string[0], aux_string[1], aux_string[2], aux_string[3]) == 4){
        liga_pontos(aux_string, *topo_lista_linhas);
      }
      //se for comboio
      else if (sscanf(leitura, "COMBOIO: %s %d %s %s %s %d %d", aux_string[0], aux_int, aux_string[1], aux_string[2], aux_string[3], aux_int+1, aux_int+2) == 7){

        *topo_lista_comboios = preenche_comboio(aux_string[0], aux_int, *topo_lista_comboios, *topo_lista_linhas);
      }
    }
  }
}

LISTA_COMBOIOS * preenche_comboio(char *aux_string, int *aux_int, LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS *topo_lista_linhas){
  LISTA_COMBOIOS *novo_boio=NULL;
  novo_boio = (LISTA_COMBOIOS*) calloc(1, sizeof(LISTA_COMBOIOS));

  strcpy(novo_boio->boio.id, aux_string);
  novo_boio->boio.cor = codigo_cor(aux_string+10);
  novo_boio->boio.dim = aux_int[0];
  novo_boio->boio.origem = procura_ponto(aux_string+20, aux_string+30, topo_lista_linhas);
  novo_boio->boio.tempo_spawn = aux_int[1];
  novo_boio->boio.veloc = (float) aux_int[2] / FPS;

  novo_boio->pr=topo_lista_comboios;
  topo_lista_comboios=novo_boio;

  return topo_lista_comboios;
}

void preenche_linha(char *aux_string, int *aux_int, LISTA_LINHAS * nova_linha, LISTA_PONTOS **atual){
  LISTA_PONTOS *aux_pt = NULL;

  aux_pt = (LISTA_PONTOS*) calloc(1, sizeof(LISTA_PONTOS));
  strcpy(aux_pt->pt.id, aux_string);
  aux_pt->pt.x = aux_int[0];
  aux_pt->pt.y = aux_int[1];
  aux_pt->pt.cor = codigo_cor(aux_string+10);
  aux_pt->pt.tipo = numero_tipo(aux_string+20);

  if (nova_linha->linha==NULL){
    nova_linha->linha = aux_pt;
    *atual = aux_pt;
  }
  else{
    (*atual)->pr[0] = aux_pt;
    *atual = aux_pt;
  }
}

void liga_pontos(char aux_string[6][10], LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *pt1 = NULL, *pt2 = NULL;

  pt1 = procura_ponto(aux_string[0], aux_string[1], topo_lista_linhas);
  pt2 = procura_ponto(aux_string[2], aux_string[3], topo_lista_linhas);
  if (pt1 == NULL || pt2 == NULL){
    printf("ERRO de ligacao de pontos\n");
    fflush (stdout);
    exit(0);
  }
  pt1->pr[1]=pt2;
  // mostra_ponto(pt1->pt);
  // mostra_ponto(pt1->pr[1]->pt);
}
