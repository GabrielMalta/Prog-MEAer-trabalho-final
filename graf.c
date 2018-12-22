#include "comboios.h"

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
  fflush(stdout);
  return 0;
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

LISTA_GRAF_BOIO *gera_novos_graf_boios(LISTA_GRAF_BOIO *lista_graf_boios, LISTA_COMBOIOS *comboios, int ticks_simulacao){
  for(; comboios!=NULL; comboios=comboios->pr){
    if(ticks_simulacao% ((int) comboios->boio.tempo_spawn*FPS) == 0){
      lista_graf_boios = cria_grafico_do_comboio(lista_graf_boios, &(comboios->boio));
    }
  }
  return lista_graf_boios;
}

LISTA_GRAF_BOIO * remove_graf_boio(LISTA_GRAF_BOIO *lista_graf_boios, LISTA_GRAF_BOIO *eliminar){
  LISTA_GRAF_BOIO *aux_boio, *ant_boio=NULL;

  for( aux_boio = lista_graf_boios; aux_boio!=eliminar && aux_boio!=NULL; aux_boio=aux_boio->pr) {ant_boio=aux_boio;}

  if(aux_boio == NULL) return lista_graf_boios;

  if(ant_boio == NULL) lista_graf_boios=aux_boio->pr;
  else ant_boio->pr = aux_boio->pr;

  free(aux_boio);
  printf("Boio chegou ao fim de linha\n");
  fflush(stdout);
  // mostra_boios_ativos(lista_graf_boios);
  return lista_graf_boios;
}

void desenha_pontos(LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *ap;

  for(; topo_lista_linhas !=NULL; topo_lista_linhas=topo_lista_linhas->pr){
    for(ap = topo_lista_linhas->linha.l; ap!= NULL; ap=ap->pr[0]){
      if (ap->pt.tipo == EST){
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 10, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 10, hexdec_PRETO);
      }
      else if (ap->pr[0] != NULL && ap->pr[1] != NULL){
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 4, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 4, hexdec_PRETO);
      }
      else {
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 2, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 2, hexdec_PRETO);
      }
    }
  }
}

int sinal(int numero){
  if (numero>0) return 1;
  if (numero<0) return -1;
  return 0;
}

void desenha_ligacoes(LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *ap;

  for(; topo_lista_linhas !=NULL; topo_lista_linhas=topo_lista_linhas->pr){
    for(ap = topo_lista_linhas->linha.l; ap!= NULL; ap=ap->pr[0]){

      SDL_SetRenderDrawColor(pintor, 0, 0, 0, 255);
      if(ap->pr[0] == NULL && ap->pr[1] == NULL) {
        //fim de linha
        continue;
      }
      if(ap->pr[0] == NULL && ap->pr[1] != NULL){
        //meio da linha
        SDL_RenderDrawLine(pintor,ap->pt.x, ap->pt.y, ap->pr[1]->pt.x, ap->pr[1]->pt.y);
        continue;
      }
      else if(ap->pr[1] == NULL && ap->pr[0] != NULL){
        //linha acaba noutra
        SDL_RenderDrawLine(pintor,ap->pt.x, ap->pt.y, ap->pr[0]->pt.x, ap->pr[0]->pt.y);
        continue;
      }

      //troca de linha
      SDL_RenderDrawLine(pintor, ap->pt.x, ap->pt.y, ap->pr[ap->pt.alavanca]->pt.x, ap->pr[ap->pt.alavanca]->pt.y);
      SDL_SetRenderDrawColor(pintor, 185, 185, 185, 255);
      SDL_RenderDrawLine(pintor, ap->pt.x, ap->pt.y, ap->pr[1-ap->pt.alavanca]->pt.x, ap->pr[1-ap->pt.alavanca]->pt.y);
    }
  }
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

void atualiza_render(LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *boios_graficos, int dimX, int dimY, int pausa){

  SDL_SetRenderDrawColor(pintor, 235, 235, 235, 255);
  SDL_RenderClear(pintor);

  desenha_ligacoes(topo_lista_linhas);
  desenha_pontos(topo_lista_linhas);
  desenha_comboios(boios_graficos);
  desenha_botoes(dimX,dimY, pausa);
}

int eventos_sdl(SDL_Event *event, LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *topo_lista_graf_boios, int dimX, int dimY){
  LISTA_PONTOS *aux_pt;
  int x=0, y=0;

  switch(event->type){
    case SDL_MOUSEMOTION:
    return 0;
    case SDL_MOUSEBUTTONDOWN:
      SDL_GetMouseState( &x, &y);
      aux_pt = procura_ponto_por_coords(topo_lista_linhas, x, y);
      if (aux_pt !=NULL && aux_pt->pr[0] != NULL && aux_pt->pr[1] != NULL){
        aux_pt->pt.alavanca = 1 - aux_pt->pt.alavanca;
      }
      else return carregou_botao(dimX, dimY, x, y);
    case SDL_KEYDOWN:
      switch(event->key.keysym.sym){
        case SDLK_ESCAPE: return 1;
        default: return 0;
        // case SDLK_SPACE: pausar a simulacao
      }
    case SDL_QUIT: return 1;
    default: return 0;
  }
  return 0;
}

void simular(LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS *topo_lista_linhas, int dimensaoX, int dimensaoY){
  LISTA_GRAF_BOIO *boios_graficos = NULL;
  SDL_Event event;
  Uint32 temporizador;
  int fim=0, pausa=0;
  int ticks_simulacao=0;
  // boios_graficos = inicializa_boios(boios_graficos, topo_lista_comboios);

  mostra_boios_ativos(boios_graficos);
  if ( inicializa_janela(dimensaoX,dimensaoY) == 0 ){
    exit(0);
  }

  temporizador = SDL_GetTicks();
  while (fim != 1){
    if (pausa!=1){
      boios_graficos = gera_novos_graf_boios(boios_graficos, topo_lista_comboios, ticks_simulacao);
      boios_graficos = mexe_comboios2(boios_graficos);
      atualiza_render(topo_lista_linhas, boios_graficos, dimensaoX, dimensaoY, pausa);
      ticks_simulacao++;
    }
    else {
      atualiza_render(topo_lista_linhas, boios_graficos, dimensaoX, dimensaoY, pausa);
    }
    SDL_Delay(TICKS_p_FRAME - SDL_TICKS_PASSED(SDL_GetTicks(), temporizador));
    SDL_RenderPresent(pintor);
    temporizador = SDL_GetTicks();

    while (SDL_PollEvent(&event)) switch(eventos_sdl(&event, topo_lista_linhas, boios_graficos, dimensaoX, dimensaoY)){
      case 2:
        pausa = 1 - pausa;
        break;
      case 1: fim = 1;
      default: break;
    }
	}
  SDL_Quit();
}

void desenha_botoes(int dimX, int dimY, int pausa){
  SDL_Rect botao_sair, botao_pausa;

  botao_sair.w = LARGURA_BOTAO;
  botao_sair.h = ALTURA_BOTAO;
  botao_sair.x = dimX - ESPACAMENTO - LARGURA_BOTAO;
  botao_sair.y = ESPACAMENTO;
  botao_pausa.w = LARGURA_BOTAO;
  botao_pausa.h = ALTURA_BOTAO;
  botao_pausa.x = dimX - 2*ESPACAMENTO - 2*LARGURA_BOTAO;
  botao_pausa.y = ESPACAMENTO;

  SDL_SetRenderDrawColor(pintor, 253, 233, 170, 255);
  SDL_RenderFillRect(pintor, &botao_sair);
  SDL_RenderFillRect(pintor, &botao_pausa);
  SDL_SetRenderDrawColor(pintor, 0, 0, 0, 255);
  SDL_RenderDrawRect(pintor, &botao_sair);
  SDL_RenderDrawRect(pintor, &botao_pausa);
  stringRGBA(pintor, botao_sair.x+0.35*botao_sair.w, botao_sair.y+0.4*botao_sair.h, "SAIR", 0, 0, 0, 255);
  if(pausa == 0) stringRGBA(pintor, botao_pausa.x+botao_pausa.w*0.35, botao_pausa.y+0.4*botao_pausa.h, "PAUSA", 0, 0, 0, 255);
  else stringRGBA(pintor, botao_pausa.x+botao_pausa.w*0.29, botao_pausa.y+0.4*botao_pausa.h, "RETOMAR", 0, 0, 0, 255);
}

int carregou_botao(int dimX, int dimY, int x, int y){
  if ( y > ESPACAMENTO && y < ESPACAMENTO+ALTURA_BOTAO){
    if( x > dimX - ESPACAMENTO - LARGURA_BOTAO && x < dimX - ESPACAMENTO){
      // botao de sair
      return 1;
    }
    else if( x > dimX - 2*ESPACAMENTO - 2*LARGURA_BOTAO && x <dimX - 2*ESPACAMENTO - LARGURA_BOTAO){
      //botao de pausa
      return 2;
    }
  }
  return 0;
}