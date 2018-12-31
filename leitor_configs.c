#include "comboios.h"

void leitor_configs(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas, int dimJanela[], char *nome_ficheiro){
  FILE *config = fopen(nome_ficheiro, "r");
  LISTA_LINHAS *nova_linha;
  LISTA_PONTOS *atual=NULL;

  char leitura[100];
  char aux_string[4][10];
  int aux_int[5];

  if(config==NULL){
    printf("Erro de abertura de config;\n\"%s\" nao encontrado\n", nome_ficheiro);
    printf("Utilizacao: ./comboios <ficheiro.txt>\n");
    exit(0);
  }

  while( fgets(leitura, 100, config) != NULL){
    if ( (leitura[0] >= '0' && leitura[0] <='9') || (leitura[0] >= 'A' && leitura[0] <='Z') ){
      if (sscanf(leitura, "%d %d", aux_int, aux_int+1) == 2){
        dimJanela[X] = aux_int[0];
        dimJanela[Y] = aux_int[1];
      }

      else if (sscanf(leitura, "LINHA: %s", aux_string[0]) == 1){
        //se for linha
        nova_linha = (LISTA_LINHAS*) calloc(1, sizeof(LISTA_LINHAS));
        strcpy(nova_linha->id, aux_string[0]);
        nova_linha->pr=*topo_lista_linhas;
        *topo_lista_linhas=nova_linha;
      }

      else if (sscanf(leitura, "%s %d %d %s %s", aux_string[0], aux_int, aux_int+1, aux_string[1], aux_string[2]) == 5)
        //se for ponto
        preenche_linha(aux_string[0], aux_int, nova_linha, &atual);

      else if (sscanf(leitura, "LIGAR: %s %s %s %s", aux_string[0], aux_string[1], aux_string[2], aux_string[3]) == 4)
        //ligacao de pontos
        liga_pontos(aux_string, *topo_lista_linhas);

      else if (sscanf(leitura, "COMBOIO: %s %d %s %s %s %d", aux_string[0], aux_int, aux_string[1], aux_string[2], aux_string[3], aux_int+1) == 6)
        //se for comboio
        *topo_lista_comboios = preenche_comboio(aux_string, aux_int, *topo_lista_comboios, *topo_lista_linhas);
    }
  }
}

LISTA_COMBOIOS * preenche_comboio(char aux_string[][10], int *aux_int, LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS *topo_lista_linhas){
  LISTA_COMBOIOS *novo_boio=NULL;
  novo_boio = (LISTA_COMBOIOS*) calloc(1, sizeof(LISTA_COMBOIOS));

  strcpy(novo_boio->boio.id, aux_string[0]);
  novo_boio->boio.cor = cor_string_para_Uint32(aux_string[1]);
  novo_boio->boio.r_bolas = aux_int[0];
  novo_boio->boio.origem = procura_ponto(aux_string[2], aux_string[3], topo_lista_linhas);
  novo_boio->boio.num_servicos = aux_int[1];
  novo_boio->boio.servicos_restantes = aux_int[1];

  novo_boio->pr=topo_lista_comboios;
  topo_lista_comboios=novo_boio;

  return topo_lista_comboios;
}

void preenche_linha(char *aux_string, int *aux_int, LISTA_LINHAS * nova_linha, LISTA_PONTOS **atual){
  // adiciona uma linha a lista de linhas
  LISTA_PONTOS *aux_pt = NULL;

  aux_pt = (LISTA_PONTOS*) calloc(1, sizeof(LISTA_PONTOS));
  strcpy(aux_pt->pt.id, aux_string);
  aux_pt->pt.x = aux_int[0];
  aux_pt->pt.y = aux_int[1];
  aux_pt->pt.cor = cor_string_para_Uint32(aux_string+10);
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

void liga_pontos(char aux_string[4][10], LISTA_LINHAS *topo_lista_linhas){
  // invocada quando se lê o comando LIGAR no ficheiro de config
  LISTA_PONTOS *pt1 = NULL, *pt2 = NULL;

  pt1 = procura_ponto(aux_string[0], aux_string[1], topo_lista_linhas);
  pt2 = procura_ponto(aux_string[2], aux_string[3], topo_lista_linhas);
  if (pt1 == NULL || pt2 == NULL){
    printf("ERRO de ligacao de pontos\n");
    fflush (stdout);
    exit(0);
  }
  pt1->pr[1]=pt2;
}

LISTA_PONTOS * procura_ponto(char *id_linha, char *id_ponto, LISTA_LINHAS *topo_lista_linhas){
  // procura o ponto pelo identificador de linha e de ponto
  LISTA_PONTOS *aux_pt;
  LISTA_LINHAS *linha_atual;

  for(linha_atual=topo_lista_linhas; linha_atual!=NULL; linha_atual=linha_atual->pr){
    if (strcmp(id_linha, linha_atual->id) == 0){
      for(aux_pt=linha_atual->linha; aux_pt != NULL; aux_pt=aux_pt->pr[0]){
        if(strcmp(id_ponto, aux_pt->pt.id) == 0){
          return aux_pt;
        }
      }
    }
  }
  return NULL;
}

int numero_tipo(char string[]){
  // converte a string de tipo num inteiro para armazenar
  if (strcmp("EST", string) == 0) return 2;
  if (strcmp("VIA", string) == 0) return 1;
  return 0;
}

Uint32 cor_string_para_Uint32(char string[]){ //funciona
  // converte a string de cor para Uint32 para armazenar
  if (strcmp("CINZENTO", string) == 0) return hexdec_CINZENTO;
  if (strcmp("VERMELHO", string) == 0) return hexdec_VERMELHO;
  if (strcmp("ROXO", string) == 0) return hexdec_ROXO;
  if (strcmp("AZUL", string) == 0) return hexdec_AZUL;
  if (strcmp("CIANO", string) == 0) return hexdec_CIANO;
  if (strcmp("VERDE", string) == 0) return hexdec_VERDE;
  if (strcmp("AMARELO", string) == 0) return hexdec_AMARELO;
  if (strcmp("CASTANHO", string) == 0) return hexdec_CASTANHO;
  if (strcmp("PRETO", string) == 0) return hexdec_PRETO;
  if (strcmp("BRANCO", string) == 0) return hexdec_BRANCO;
  printf("Erro leitura de cor\n");
  exit(0);
}
