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
  printf("\nPonto: %s\nTipo: %s\nCor: %s\n", pt.id, tipo, cor_codigo(pt.cor));
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

Uint32 codigo_cor(char string[]){ //funciona
  // converte a string de cor para hexadecimal para armazenar
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
  // return NULL;
}

Uint32 random_cor(void){
 int random = rand()%10;
 switch(random){
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
 return 0;
}

char * cor_codigo(Uint32 no){
  // converte o numero para um string de cor correspondente
  switch(no){
    case hexdec_CINZENTO: return "Cinzento";
    case hexdec_VERMELHO: return "Vermelho";
    case hexdec_ROXO:     return "Roxo";
    case hexdec_AZUL:     return "Azul";
    case hexdec_CIANO:    return "Ciano";
    case hexdec_VERDE:    return "Verde";
    case hexdec_AMARELO:  return "Amarelo";
    case hexdec_CASTANHO: return "Castanho";
    case hexdec_PRETO:    return "Preto";
    case hexdec_BRANCO:   return "Branco";
  }
  printf("Erro cor_numero\n");
  exit(0);
}

void leitor_configs(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas, int *dim_X, int *dim_Y, char *nome_ficheiro){
  FILE *config = fopen(nome_ficheiro, "r");
  LISTA_LINHAS *nova_linha;
  LISTA_PONTOS *aux_pt = NULL, *atual=NULL;
  LISTA_COMBOIOS *novo_boio = NULL;

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
        strcpy(nova_linha->linha.id, aux_string[0]);
        nova_linha->pr=*topo_lista_linhas;
        *topo_lista_linhas=nova_linha;
      }
      //se for ponto
      else if (sscanf(leitura, "%s %d %d %s %s", aux_string[0], aux_int, aux_int+1, aux_string[1], aux_string[2]) == 5){

        aux_pt = (LISTA_PONTOS*) calloc(1, sizeof(LISTA_PONTOS));
        strcpy(aux_pt->pt.id, aux_string[0]);
        aux_pt->pt.x = aux_int[0];
        aux_pt->pt.y = aux_int[1];
        aux_pt->pt.cor = codigo_cor(aux_string[1]);
        aux_pt->pt.tipo = numero_tipo(aux_string[2]);

        if (nova_linha->linha.l==NULL){
          nova_linha->linha.l = aux_pt;
          atual = aux_pt;
        }
        else{
          atual->pr[0] = aux_pt;
          atual = aux_pt;
        }
      }
      //Ligacao de pontos
      else if (sscanf(leitura, "LIGAR: %s %s %s %s", aux_string[0], aux_string[1], aux_string[2], aux_string[3]) == 4){
        liga_pontos(aux_string, *topo_lista_linhas);
      }
      //se for comboio
      else if (sscanf(leitura, "COMBOIO: %s %d %s %s %s %d %d", aux_string[0], aux_int, aux_string[1], aux_string[2], aux_string[3], aux_int+1, aux_int+2) == 7){

        novo_boio = (LISTA_COMBOIOS*) calloc(1, sizeof(LISTA_COMBOIOS));
        strcpy(novo_boio->boio.id, aux_string[0]);
        novo_boio->boio.cor = codigo_cor(aux_string[1]);
        novo_boio->boio.dim = aux_int[0];
        novo_boio->boio.origem = procura_ponto(aux_string[2], aux_string[3], *topo_lista_linhas);
        novo_boio->boio.tempo_spawn = aux_int[1];
        novo_boio->boio.veloc = MULT_VELOC * aux_int[2];

        novo_boio->pr=*topo_lista_comboios;
        *topo_lista_comboios=novo_boio;
      }
    }
  }
}

void mostra_boio(COMBOIO boio){
  printf("\n/----COMBOIO %s----/\n", boio.id);
  printf("Dimensao:%d\n", boio.dim);
  printf("Cor locom.:%s\n", cor_codigo(boio.cor));
  printf("Ponto origem:%s\n", boio.origem->pt.id);
  printf("Tempo de spawn:%.2f\n", boio.tempo_spawn);
  printf("Velocidade:%.2fpixeis/tick\n", boio.veloc);
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

void atualiza_render(LISTA_LINHAS *topo_lista_linhas){

  SDL_SetRenderDrawColor(pintor, 235, 235, 235, 255);
  SDL_RenderClear(pintor);

  desenha_ligacoes(topo_lista_linhas);
  desenha_pontos(topo_lista_linhas);
}

void desenha_pontos(LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *ap;

  for(; topo_lista_linhas !=NULL; topo_lista_linhas=topo_lista_linhas->pr){
    for(ap = topo_lista_linhas->linha.l; ap!= NULL; ap=ap->pr[0]){
      if (ap->pt.tipo == EST){
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 10, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 10, hexdec_PRETO);
      }
      else{
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 2, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 2, hexdec_PRETO);
      }
    }
  }
}

void desenha_ligacoes(LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *ap;

  SDL_SetRenderDrawColor(pintor, 0, 0, 0, 255);
  for(; topo_lista_linhas !=NULL; topo_lista_linhas=topo_lista_linhas->pr){
    for(ap = topo_lista_linhas->linha.l; ap!= NULL; ap=ap->pr[0]){
      if( ap->pr[0] != NULL){
        SDL_RenderDrawLine(pintor, ap->pt.x, (ap->pt.y), ap->pr[0]->pt.x, (ap->pr[0]->pt.y));
      }
      if( ap->pr[1] != NULL){
        SDL_RenderDrawLine(pintor, ap->pt.x, (ap->pt.y), ap->pr[1]->pt.x, (ap->pr[1]->pt.y));
      }
    }
  }
}

LISTA_PONTOS * procura_ponto(char *id_linha, char *id_ponto, LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *aux_pt;
  LISTA_LINHAS *linha_atual;

  for(linha_atual=topo_lista_linhas; linha_atual!=NULL; linha_atual=linha_atual->pr){
    if (strcmp(id_linha, linha_atual->linha.id) == 0){
      for(aux_pt=linha_atual->linha.l; aux_pt != NULL; aux_pt=aux_pt->pr[0]){
        if(strcmp(id_ponto, aux_pt->pt.id) == 0){
          return aux_pt;
        }
      }
    }
  }
  return NULL;
}

LISTA_GRAF_BOIO *cria_grafico_do_comboio(LISTA_GRAF_BOIO *lista_graf_boios, COMBOIO *comboio){
  int i;
  LISTA_GRAF_BOIO *novo_graf_boio = calloc(1, sizeof(LISTA_GRAF_BOIO));
  if (novo_graf_boio == NULL){
    printf("Erro falta de memória\n");
    fflush(stdout);
  }
  novo_graf_boio->graf.boio=comboio;
  for(i=0;i<comboio->dim; i++){
    novo_graf_boio->graf.cor[i]= random_cor();
    novo_graf_boio->graf.x[i]=comboio->origem->pt.x;
    novo_graf_boio->graf.y[i]=comboio->origem->pt.y;
    novo_graf_boio->graf.ultimo_ponto[i]=comboio->origem;
  }
  novo_graf_boio->graf.cor[0]=comboio->cor;
  novo_graf_boio->pr=lista_graf_boios;
  return novo_graf_boio;
}

LISTA_GRAF_BOIO * inicializa_boios(LISTA_GRAF_BOIO *boios_graficos, LISTA_COMBOIOS *lista_comboios){

  for(; lista_comboios!=NULL; lista_comboios=lista_comboios->pr){
    boios_graficos=cria_grafico_do_comboio(boios_graficos, &(lista_comboios->boio));
  }

  return boios_graficos;
}

LISTA_GRAF_BOIO * mexe_comboios2(LISTA_GRAF_BOIO *lista_graf_boios){
  float deltaX, deltaY;
  float m;
  float x_a_somar, y_a_somar;
  int i;
  LISTA_PONTOS *pt1 = NULL, *pt2 = NULL;
  LISTA_GRAF_BOIO *aux_boio = NULL, *ant_boio = NULL;
  for( aux_boio = lista_graf_boios; aux_boio!=NULL; aux_boio=aux_boio->pr){
    for(i=0; i<aux_boio->graf.boio->dim; i++){
      pt1 = aux_boio->graf.ultimo_ponto[i];

      if( (pt2 = pt1->pr[pt1->pt.alavanca]) != NULL){}
      else if( (pt2 = pt1->pr[1-pt1->pt.alavanca]) != NULL){}
      else{
        if(aux_boio==lista_graf_boios){
          lista_graf_boios=aux_boio->pr;
          free(aux_boio);
          if( (aux_boio=lista_graf_boios) == NULL){
            return NULL;
          }
        }
        else{
        ant_boio->pr = aux_boio->pr;
        free(aux_boio);
        aux_boio=ant_boio;
        }
        printf("----------------------boio chegou ao fim\n");
        fflush(stdout);
        mostra_boios_ativos(lista_graf_boios);
        break;
      }
      // else continue;

      if (i!=0){ //se estivermos a mexer uma carruagem
        deltaX = aux_boio->graf.x[i] - aux_boio->graf.x[i-1];
        deltaY = aux_boio->graf.y[i] - aux_boio->graf.y[i-1];

        // e ela estiver demasiado perto da anterior, nao a mexer
        if (sqrt(deltaX*deltaX + deltaY*deltaY) < 2.1*RAIO_COMBOIO) continue;
      }

      deltaX = pt2->pt.x - pt1->pt.x;
      deltaY = pt2->pt.y - pt1->pt.y;

      if (deltaX != 0){
        m = (float) deltaY/deltaX;

        x_a_somar = deltaX/abs(deltaX)  *  aux_boio->graf.boio->veloc/sqrt(m*m+1);
        y_a_somar = m*x_a_somar;
      }
      else{
        x_a_somar=0;
        y_a_somar=deltaY/abs(deltaY) * aux_boio->graf.boio->veloc;
      }

      aux_boio->graf.x[i] += x_a_somar;
      aux_boio->graf.y[i] += y_a_somar;

      deltaX = pt2->pt.x - aux_boio->graf.x[i]; //reciclar variaveis
      deltaY = pt2->pt.y - aux_boio->graf.y[i]; //para salvar o ambiente

      if( sqrt(deltaY*deltaY + deltaX*deltaX) < aux_boio->graf.boio->veloc){
        aux_boio->graf.x[i] = pt2->pt.x;
        aux_boio->graf.y[i] = pt2->pt.y;
        aux_boio->graf.ultimo_ponto[i] = pt2;
        if(i!=0) aux_boio->graf.cor[i] = esvazia_vagao(pt2->pt, aux_boio->graf.cor[i]);
      }

      if (i!=0){
        deltaX = aux_boio->graf.x[i] - aux_boio->graf.x[i-1];
        deltaY = aux_boio->graf.y[i] - aux_boio->graf.y[i-1];
        if(sqrt(deltaX*deltaX + deltaY*deltaY) > 2.2*RAIO_COMBOIO){
          //se a carruagem que acabamos de mexer ainda estiver demasiado longe, vamos mexê-la outra vez
          i--;
        }
      }
    }
    ant_boio = aux_boio;
  }
  return lista_graf_boios;
}

void desenha_comboios(LISTA_GRAF_BOIO *lista_graf_boios){
  int i;
  for( ; lista_graf_boios!=NULL; lista_graf_boios=lista_graf_boios->pr){
    for(i=0; i<lista_graf_boios->graf.boio->dim; i++){
      filledCircleColor(pintor, lista_graf_boios->graf.x[i], lista_graf_boios->graf.y[i], RAIO_COMBOIO, lista_graf_boios->graf.cor[i]);
      aacircleColor(pintor, lista_graf_boios->graf.x[i], lista_graf_boios->graf.y[i], RAIO_COMBOIO, hexdec_PRETO);
    }
  }
}

void mostra_boios_ativos(LISTA_GRAF_BOIO *lista_graf_boios){
  int i=1;
  while(lista_graf_boios!=NULL){
    printf("\nGRAF_BOIO #%d\n", i++);
    printf("Prototipo:%s\n", lista_graf_boios->graf.boio->id);
    printf("Cor locom.:%s\n", cor_codigo(lista_graf_boios->graf.cor[0]));
    printf("X=%7.2fY=%7.2f\n", lista_graf_boios->graf.x[0], lista_graf_boios->graf.y[0]);
    fflush(stdout);
    lista_graf_boios=lista_graf_boios->pr;
  }
}

Uint32 esvazia_vagao(PONTO pt, Uint32 cor){
  if (cor == pt.cor) return hexdec_CINZENTO;
  return cor;
}

void render(void){
  SDL_RenderPresent(pintor);
}
