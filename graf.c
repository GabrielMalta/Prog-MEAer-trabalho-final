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
  // int deltaX, deltaY;
  // float m;
  // float d=40, h=48, l=40; //distancia, comprimento e largura da flecha
  // int i;
  // float base_flechaX, base_flechaY;
  // Sint16 cantos_trianguloX[3], cantos_trianguloY[3];

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

      // if(ap->pr[0] == NULL || ap->pr[1] == NULL) continue;

      // deltaX = ap->pr[ap->pt.alavanca]->pt.x - ap->pt.x;
      // deltaY = ap->pr[ap->pt.alavanca]->pt.y - ap->pt.y;
      // m= (float) deltaY/deltaX;
      // if (abs(deltaX) > 0){
      //   base_flechaX = ap->pt.x+sinal(deltaX)*d/sqrt(m*m+1);
      //   base_flechaY = ap->pt.y+m*(base_flechaX - ap->pt.x);
      // }
      // else{
      //   base_flechaX = ap->pt.x;
      //   base_flechaY = sinal(deltaY)*d;
      // }
      // cantos_trianguloX[0]=ap->pt.x+sinal(deltaX)*(d+h)/sqrt(m*m+1);
      // cantos_trianguloY[0]=ap->pt.y+m*(cantos_trianguloX[0]-ap->pt.x);
      //
      //
      // cantos_trianguloX[1]=base_flechaX+sinal(deltaX)*sinal(deltaY)*m*l*sqrt(1.0/(4*(m*m+1)));
      // cantos_trianguloY[1]=base_flechaY+l*sqrt(1/(4*(m*m+1)));
      //
      // cantos_trianguloX[2]=base_flechaX-sinal(deltaX)*sinal(deltaY)*m*l*sqrt(1.0/(4*(m*m+1)));
      // cantos_trianguloY[2]=base_flechaY-l*sqrt(1.0/(4*(m*m+1)));
      // filledCircleColor(pintor, base_flechaX, base_flechaY, 5, hexdec_PRETO);
      // for (i=0; i<3; i++){
      //   filledCircleColor(pintor, cantos_trianguloX[i], cantos_trianguloY[i], 5, hexdec_PRETO);
      // }
      //
      // SDL_SetRenderDrawColor(pintor, 0, 0, 255, 255);
      // SDL_RenderDrawLine(pintor, base_flechaX, base_flechaY, ap->pt.x, ap->pt.y);
      // // filledPolygonColor(pintor,cantos_trianguloX, cantos_trianguloY, 3, hexdec_AZUL);
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

void atualiza_render(LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *boios_graficos, int dimX, int dimY){

  SDL_SetRenderDrawColor(pintor, 235, 235, 235, 255);
  SDL_RenderClear(pintor);

  desenha_ligacoes(topo_lista_linhas);
  desenha_pontos(topo_lista_linhas);
  desenha_comboios(boios_graficos);
  desenha_butoes(dimX,dimY);
}

int eventos_sdl(SDL_Event *event, LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *topo_lista_graf_boios){
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
      return 0;
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
  int fim=0;
  boios_graficos = inicializa_boios(boios_graficos, topo_lista_comboios);

  mostra_boios_ativos(boios_graficos);
  if ( inicializa_janela(dimensaoX,dimensaoY) == 0 ){
    exit(0);
  }

  while (fim != 1){
    temporizador = SDL_GetTicks();
    boios_graficos = mexe_comboios2(boios_graficos);
    atualiza_render(topo_lista_linhas, boios_graficos, dimensaoX, dimensaoY);
    printf("\rA esperar%d ms", TICKS_p_FRAME - SDL_GetTicks() + temporizador);
    fflush(stdout);
    SDL_Delay(TICKS_p_FRAME - SDL_TICKS_PASSED(SDL_GetTicks(), temporizador));
    SDL_RenderPresent(pintor);
    while (SDL_PollEvent(&event)) fim = eventos_sdl(&event, topo_lista_linhas, boios_graficos);
	}
  SDL_Quit();
}

void desenha_butoes(int dimX, int dimY){
  SDL_Rect botao_sair, botao_pausa;

  botao_sair.w = 120;
  botao_sair.h = 40;
  botao_sair.x = dimX - 10 - botao_sair.w;
  botao_sair.y = 10;
  botao_pausa.w = 120;
  botao_pausa.h = 40;
  botao_pausa.x = dimX - 2*10 - 2*botao_pausa.w;
  botao_pausa.y = 10;

  SDL_SetRenderDrawColor(pintor, 253, 233, 170, 255);
  SDL_RenderFillRect(pintor, &botao_sair);
  SDL_RenderFillRect(pintor, &botao_pausa);
  SDL_SetRenderDrawColor(pintor, 0, 0, 0, 255);
  SDL_RenderDrawRect(pintor, &botao_sair);
  SDL_RenderDrawRect(pintor, &botao_pausa);

}
