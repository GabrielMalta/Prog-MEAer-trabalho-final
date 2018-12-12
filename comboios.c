#include "comboios.h"

SDL_Window* janela = 0;
SDL_Renderer* pintor = 0;

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

Uint32 hexdec_cor_numero(int no){
  // converte o numero para um string de cor correspondente
  switch(no){
    case CINZENTO: return hexdec_CINZENTO;
    case VERMELHO: return hexdec_VERMELHO;
    case ROXO:     return hexdec_ROXO;
    case AZUL:     return hexdec_AZUL;
    case CIANO:    return hexdec_CIANO;
    case VERDE:    return hexdec_VERDE;
    case AMARELO:  return hexdec_AMARELO;
    case CASTANHO: return hexdec_CASTANHO;
    case PRETO:    return hexdec_PRETO;
    case BRANCO:   return hexdec_BRANCO;
  }
  printf("Erro hexdec_cor_numero\n");
  exit(0);
}

void leitor_configs(COMBOIO ***comboios, LINHA ***linhas, int *dim_X, int *dim_Y){
  FILE *config = fopen("config.txt", "r");
  LINHA *nova_linha = NULL;
  LISTA_PONTOS *aux_pt = NULL, *atual=NULL;
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
        fflush(stdout);
      }
      //se for ponto
      else if (sscanf(leitura, "%s %d %d %s %s", aux_string[0], aux_int, aux_int+1, aux_string[1], aux_string[2]) == 5){
        aux_pt = (LISTA_PONTOS*) calloc(1, sizeof(LISTA_PONTOS));
        // aux_string[0][5] = '\0';
        strcpy(aux_pt->pt.id, aux_string[0]);
        aux_pt->pt.x = aux_int[0];
        aux_pt->pt.y = aux_int[1];
        aux_pt->pt.cor = numero_cor(aux_string[1]);
        aux_pt->pt.tipo = numero_tipo(aux_string[2]);

        if ((*linhas)[numero_linhas-1]->l == NULL){
          (*linhas)[numero_linhas-1]->l = aux_pt;
          atual = aux_pt;
        }
        else{
          atual->pr[0] = aux_pt;
          atual = aux_pt;
        }
      }
      //Ligacao de pontos
      else if (sscanf(leitura, "LIGAR: %s %s %s %s", aux_string[0], aux_string[1], aux_string[2], aux_string[3]) == 4){
        liga_pontos(aux_string, linhas);
      }
      //se for comboio
      else if (sscanf(leitura, "COMBOIO: %s %d %s %s %s %d %d", aux_string[0], aux_int, aux_string[1], aux_string[2], aux_string[3], aux_int+1, aux_int+2) == 7){
        numero_comboios++;
        novo_boio = (COMBOIO*) calloc(1, sizeof(COMBOIO));
        strcpy(novo_boio->id, aux_string[0]);
        novo_boio->cor[0] = numero_cor(aux_string[1]);
        novo_boio->dim = aux_int[0];
        novo_boio->origem = procura_ponto(aux_string[2], aux_string[3], *linhas);
        novo_boio->tempo_spawn = aux_int[1];
        novo_boio->veloc = aux_int[2];

        *comboios = realloc(*comboios, (numero_comboios+1)*sizeof(LINHA*));
        (*comboios)[numero_comboios-1] = novo_boio;
        (*comboios)[numero_comboios] = NULL;

        printf("%p\n",(void*)  (*comboios)[numero_comboios-1]);
        fflush(stdout);

      }
    }
  }
}

void mostra_boio(COMBOIO boio){
  printf("\n/----COMBOIO %s----/\n", boio.id);
  fflush(stdout);
  printf("Dimensao:%d\n", boio.dim);
  fflush(stdout);
  printf("Cor locom.:%s\n", cor_numero(boio.cor[0]));
  fflush(stdout);
  printf("Ponto origem:%s\n", boio.origem->pt.id);
  fflush(stdout);
  printf("Tempo de spawn:%.2f\n", boio.tempo_spawn);
  fflush(stdout);
}

int inicializa_janela(int dim_X, int dim_Y){
  if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
    janela = SDL_CreateWindow("I like trains",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dim_X, dim_Y, SDL_WINDOW_SHOWN);
    if(janela != 0){
      pintor = SDL_CreateRenderer(janela, -1, 0);
      return 1;
    }
  }
  printf("Erro: na abertura do SDL2\n");
  return 0;
}

void liga_pontos(char aux_string[6][10], LINHA ***linhas){
  LISTA_PONTOS *aux_pt = NULL, *pt1 = NULL, *pt2 = NULL;
  int i;

  // printf("LIGAR: %s %s %s %s\n", aux_string[0], aux_string[1], aux_string[2], aux_string[3]);
  // fflush(stdout);

  pt1 = procura_ponto(aux_string[0], aux_string[1], *linhas);
  pt2 = procura_ponto(aux_string[2], aux_string[3], *linhas);
  if (pt1 == NULL || pt2 == NULL){
    printf("ERRO de ligacao de pontos\n");
    fflush (stdout);
    exit(0);
  }
  pt1->pr[1]=pt2;
  // mostra_ponto(pt1->pt);
  // mostra_ponto(pt1->pr[1]->pt);
}

void atualiza_render(COMBOIO **comboios, LINHA **linhas){

  SDL_SetRenderDrawColor(pintor, 235, 235, 235, 255);
  SDL_RenderClear(pintor);

  desenha_ligacoes(linhas);
  desenha_pontos(linhas);

  // SDL_RenderPresent(pintor);
}

void desenha_pontos( LINHA **linhas){
  LISTA_PONTOS *ap;
  int i;
  for(i=0; linhas[i] !=NULL; i++){
    for(ap = linhas[i]->l; ap!= NULL; ap=ap->pr[0]){
      if (ap->pt.tipo == EST){
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 10, hexdec_cor_numero(ap->pt.cor));
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 10, hexdec_PRETO);
      }
      else{
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 2, hexdec_cor_numero(ap->pt.cor));
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 2, hexdec_PRETO);
      }
    }
  }
}

void desenha_ligacoes( LINHA **linhas){
  LISTA_PONTOS *ap;
  int i;

  SDL_SetRenderDrawColor(pintor, 0, 0, 0, 255);
  for(i=0; linhas[i] !=NULL; i++){
    for(ap = linhas[i]->l; ap!= NULL; ap=ap->pr[0]){
      if( ap->pr[0] != NULL){
        SDL_RenderDrawLine(pintor, ap->pt.x, (ap->pt.y), ap->pr[0]->pt.x, (ap->pr[0]->pt.y));
      }
      if( ap->pr[1] != NULL){
        SDL_RenderDrawLine(pintor, ap->pt.x, (ap->pt.y), ap->pr[1]->pt.x, (ap->pr[1]->pt.y));
      }
    }
  }
}

LISTA_PONTOS * procura_ponto(char *id_linha, char *id_ponto, LINHA **linhas){
  LISTA_PONTOS *aux_pt;
  int i;

  for(i = 0; linhas[i] != NULL; i++){
    if (strcmp(id_linha, linhas[i]->id) == 0){
      for(aux_pt=linhas[i]->l; aux_pt != NULL; aux_pt=aux_pt->pr[0]){
        if(strcmp(id_ponto, aux_pt->pt.id) == 0){
          return aux_pt;
        }
      }
    }
  }
  return NULL;
}

void inicializa_boios(GRAF_BOIO ***boios_graficos, COMBOIO **comboios, LINHA **linhas){
  GRAF_BOIO *auxiliar;
  int numero_comboios, j;
  for (numero_comboios=0; comboios[numero_comboios] != NULL; numero_comboios++){}

  *boios_graficos = (GRAF_BOIO**) calloc(numero_comboios, sizeof(GRAF_BOIO*));

  for (j=0; j<numero_comboios; j++){
    auxiliar = (GRAF_BOIO*) calloc(1, sizeof(GRAF_BOIO));

    auxiliar->boio = comboios[j];
    auxiliar->x = auxiliar->boio->origem->pt.x;
    auxiliar->y = auxiliar->boio->origem->pt.y;
    auxiliar->ultimo_ponto = auxiliar->boio->origem;
    (*boios_graficos)[j] = auxiliar;

  }
}

void mexe_comboio(GRAF_BOIO *comboio, LINHA **linhas){
  float deltaX, deltaY;
  float m;
  float x_a_somar, y_a_somar;
  LISTA_PONTOS *pt1 = NULL, *pt2 = NULL;

  pt1 = comboio->ultimo_ponto;
  pt2 = pt1->pr[0];


  if( pt2 == NULL){
    pt2 = pt1->pr[1];
  }
  if( pt2 == NULL){
    printf("boio chegou ao fim");
    SDL_Quit();
    exit(0);
  }

  deltaX = pt2->pt.x - pt1->pt.x;
  deltaY = pt2->pt.y - pt1->pt.y;

  m = (float) deltaY/deltaX;

  x_a_somar = deltaX/abs(deltaX)  *  comboio->boio->veloc/sqrt(m*m+1);
  y_a_somar = m*x_a_somar;

  filledCircleColor(pintor, comboio->x, comboio->y, 6, hexdec_cor_numero( comboio->boio->cor[0]));
  aacircleColor(pintor, comboio->x, comboio->y, 6, hexdec_PRETO);

  comboio->x += x_a_somar;
  comboio->y += y_a_somar;

  deltaX = pt2->pt.x - comboio->x;
  deltaY = pt2->pt.y - comboio->y;

  if( deltaY * deltaY + deltaX * deltaX < comboio->boio->veloc * comboio->boio->veloc ){
    comboio->x = pt2->pt.x;
    comboio->y = pt2->pt.y;
    comboio->ultimo_ponto = pt2;
  }
}

void render(void){
  SDL_RenderPresent(pintor);
  SDL_Delay(33);
}
