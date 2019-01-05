#include "comboios.h"

Uint32 random_cor(void){
  int cor = rand()%10;
  switch(cor){
    case 0: return hexdec_CINZENTO;
    case 1: return hexdec_VERMELHO;
    case 2: return hexdec_ROXO;
    case 3: return hexdec_AZUL;
    case 4: return hexdec_CIANO;
    case 5: return hexdec_VERDE;
    case 6: return hexdec_AMARELO;
    case 7: return hexdec_CASTANHO;
    case 8: return hexdec_PRETO;
    case 9: return hexdec_BRANCO;
  }
  printf("Erro geracao de cor");
  fflush(stdout);
  exit(0);
}

void simular(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas, int dimJanela[]){
  LISTA_GRAF_BOIO *lista_graf_boios = NULL;
  SDL_Event event;
  Uint32 temporizador;
  int fim=0, pausa=0;
  int ticks_simulacao=0;
  char cronometro[10];
  int i=1;

  if ( inicializa_janela(dimJanela) == 0 ){
    exit(0);
  }

  reset_servicos_restantes(*topo_lista_comboios);

  temporizador = SDL_GetTicks();
  while (fim != 1){
    if (pausa!=1){
      lista_graf_boios = mexe_comboios3(lista_graf_boios);
      colisoes(lista_graf_boios);

      if(ticks_simulacao%10==0)
        pisca_comboios(lista_graf_boios);
      atualiza_render(*topo_lista_linhas, lista_graf_boios, dimJanela, pausa);
      ticks_simulacao++;
    }
    else {
      atualiza_render(*topo_lista_linhas, lista_graf_boios, dimJanela, pausa);
    }
    sprintf(cronometro, "%2d:%2d:%2d", ticks_simulacao/(30*60),(ticks_simulacao/30)%60, ((ticks_simulacao*TICKS_p_FRAME)%1000)/10);
    stringRGBA(pintor, 10, 10, cronometro, 0, 0, 0, 255);
    SDL_Delay(TICKS_p_FRAME - SDL_TICKS_PASSED(SDL_GetTicks(), temporizador));
    temporizador = SDL_GetTicks();
    SDL_RenderPresent(pintor);

    while (SDL_PollEvent(&event)) switch(eventos_sdl(&event, *topo_lista_linhas, lista_graf_boios, dimJanela)){
      case 2:
      pausa = 1 - pausa;
      if(i==1){
        menu(topo_lista_comboios, topo_lista_linhas);
        system("clear");
            }
      i=-i;
      break;
      case 1: fim = 1;
      default: break;
    }
  }
  SDL_Quit();
}

int inicializa_janela(int dimJanela[]){
  if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
    janela = SDL_CreateWindow("I like trains",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dimJanela[X], dimJanela[Y], SDL_WINDOW_SHOWN);
    if(janela != 0){
      pintor = SDL_CreateRenderer(janela, -1, 0);
      return 1;
    }
  }
  printf("Erro: na abertura do SDL2\n");
  fflush(stdout);
  return 0;
}

void reset_servicos_restantes(LISTA_COMBOIOS *topo_lista_comboios){
  for(; topo_lista_comboios!=NULL; topo_lista_comboios=topo_lista_comboios->pr){
    topo_lista_comboios->boio.servicos_restantes = topo_lista_comboios->boio.num_servicos;
  }
}

// LISTA_GRAF_BOIO *gera_novos_graf_boios(LISTA_GRAF_BOIO *lista_graf_boios, LISTA_COMBOIOS *comboios){
//   LISTA_GRAF_BOIO *aux;
//   int i, existe_boio_demasiado_perto;
//
//   for(; comboios!=NULL; comboios=comboios->pr){
//     if( procura_graf_boio_por_aquetipo(lista_graf_boios, &(comboios->boio) ) == NULL){
//       existe_boio_demasiado_perto=0;
//       for(aux=lista_graf_boios; aux!=NULL; aux=aux->pr){
//         for(i=0; i<N_CAR; i++){
//           if(pow(aux->graf.x[i]-comboios->boio.origem->pt.x,2)+pow(aux->graf.y[i]-comboios->boio.origem->pt.y,2) < 2*pow(2*RAIO_COMBOIO,2))
//            existe_boio_demasiado_perto=1;
//         }
//       }
//       if(existe_boio_demasiado_perto==0 && comboios->boio.servicos_restantes > 0) {
//         lista_graf_boios = cria_grafico_do_comboio(lista_graf_boios, &(comboios->boio));
//         comboios->boio.servicos_restantes--;
//       }
//     }
//   }
//   return lista_graf_boios;
// }

// LISTA_GRAF_BOIO *procura_graf_boio_por_aquetipo(LISTA_GRAF_BOIO *lista_graf_boios, COMBOIO* arquetipo){
//   for(;lista_graf_boios!=NULL; lista_graf_boios=lista_graf_boios->pr){
//     if( lista_graf_boios->graf.pr == arquetipo)
//       return lista_graf_boios;
//   }
//   return NULL;
// }

// LISTA_GRAF_BOIO *cria_grafico_do_comboio(LISTA_GRAF_BOIO *lista_graf_boios, COMBOIO *comboio){
//   int i;
//   LISTA_GRAF_BOIO *novo_graf_boio = calloc(1, sizeof(LISTA_GRAF_BOIO));
//   if (novo_graf_boio == NULL){
//     printf("Erro falta de memória\n");
//     fflush(stdout);
//   }
//   novo_graf_boio->graf.arquetipo=*comboio;
//   novo_graf_boio->graf.pr=comboio;
//   for(i=0;i<N_CAR; i++){
//     novo_graf_boio->graf.cor[i]= random_cor();
//     novo_graf_boio->graf.x[i]=comboio->origem->pt.x;
//     novo_graf_boio->graf.y[i]=comboio->origem->pt.y;
//     novo_graf_boio->graf.ultimo_ponto[i]=comboio->origem;
//   }
//   novo_graf_boio->graf.cor[0]=comboio->cor;
//   novo_graf_boio->graf.veloc=SPEED;
//   novo_graf_boio->pr=lista_graf_boios;
//   return novo_graf_boio;
// }

LISTA_GRAF_BOIO * mexe_comboios3(LISTA_GRAF_BOIO *lista_graf_boios){
  float deltaX, deltaY;
  int i;
  LISTA_PONTOS *pt1 = NULL, *pt2 = NULL;
  LISTA_GRAF_BOIO *aux_boio = NULL, *ant_boio = NULL;

  for( aux_boio = lista_graf_boios; aux_boio!=NULL; ant_boio = aux_boio,aux_boio=aux_boio->pr){
    if (aux_boio->graf.veloc == 0) continue;
    for(i=0; i<N_CAR; i++){
      pt1 = aux_boio->graf.ultimo_ponto[i];

      if( (pt2 = pt1->pr[aux_boio->graf.alavanca[i]]) != NULL){}
      else if( (pt2 = pt1->pr[1-aux_boio->graf.alavanca[i]]) != NULL){}
      else{
        lista_graf_boios = remove_graf_boio(lista_graf_boios, aux_boio);
        if (lista_graf_boios == NULL) return NULL;
        if ((aux_boio = ant_boio) == NULL) aux_boio = lista_graf_boios;
        break;
      }
      // else continue;

      if (i!=0){ //se estivermos a mexer uma carruagem
        deltaX = aux_boio->graf.x[i] - aux_boio->graf.x[i-1];
        deltaY = aux_boio->graf.y[i] - aux_boio->graf.y[i-1];
        // e ela estiver demasiado perto da anterior, nao a mexer
        if (sqrt(deltaX*deltaX + deltaY*deltaY) < 2.1*aux_boio->graf.arquetipo.r_bolas) continue;
      }

      mexe_carruagem(aux_boio, i, pt1, pt2);

      verifica_se_chegou_ao_ponto(aux_boio, i, pt2);

      if (i!=0){
        deltaX = aux_boio->graf.x[i] - aux_boio->graf.x[i-1]; //reciclar variaveis
        deltaY = aux_boio->graf.y[i] - aux_boio->graf.y[i-1]; //para salvar o ambiente
        if(sqrt(deltaX*deltaX + deltaY*deltaY) > 2.2*aux_boio->graf.arquetipo.r_bolas)
        //se a carruagem que acabamos de mexer ainda estiver demasiado longe, vamos mexê-la outra vez
        i--;
      }
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
  return lista_graf_boios;
}

void mexe_carruagem(LISTA_GRAF_BOIO *aux_boio, int num_carruagem, LISTA_PONTOS *pt1, LISTA_PONTOS *pt2){
  // LISTA_PONTOS *pt1 = aux_boio->graf.ultimo_ponto[num_carruagem];
  // LISTA_PONTOS *pt2 = aux_boio->graf.ultimo_ponto[num_carruagem]->pr[aux_boio->graf.alavanca[num_carruagem]];


  float deltaX = pt2->pt.x - pt1->pt.x;
  float deltaY = pt2->pt.y - pt1->pt.y;
  float m; // declive
  float x_a_somar, y_a_somar;

  if (deltaX != 0){
    m = (float) deltaY/deltaX;

    x_a_somar = deltaX/abs(deltaX)  *  aux_boio->graf.veloc/sqrt(m*m+1);
    y_a_somar = m*x_a_somar;
  }
  else{
    x_a_somar=0;
    y_a_somar=deltaY/abs(deltaY) * aux_boio->graf.veloc;
  }

  aux_boio->graf.x[num_carruagem] += x_a_somar;
  aux_boio->graf.y[num_carruagem] += y_a_somar;
}

void verifica_se_chegou_ao_ponto(LISTA_GRAF_BOIO *aux_boio, int num_carruagem, LISTA_PONTOS *prox_pt){
  // LISTA_PONTOS *prox_pt = aux_boio->graf.ultimo_ponto[num_carruagem]->pr[aux_boio->graf.alavanca[num_carruagem]];
  float deltaX = prox_pt->pt.x - aux_boio->graf.x[num_carruagem];
  float deltaY = prox_pt->pt.y - aux_boio->graf.y[num_carruagem];

  //coloca a carruagem no proximo ponto da linha se a distancia entre os dois for muito pequena
  if( sqrt(deltaY*deltaY + deltaX*deltaX) < aux_boio->graf.veloc){
    aux_boio->graf.x[num_carruagem] = prox_pt->pt.x;
    aux_boio->graf.y[num_carruagem] = prox_pt->pt.y;
    aux_boio->graf.ultimo_ponto[num_carruagem] = prox_pt;

    //se for a locomotiva, assume o trajeto que estava selecionado pela alavanca
    if(num_carruagem==0) aux_boio->graf.alavanca[num_carruagem] = prox_pt->pt.alavanca;

    //se nao for a locomotiva, assume o trajeto da carruagem anterior
    //muda a cor para cinzento se a cor corresponder com a da estacao;
    if(num_carruagem!=0){
      if (aux_boio->graf.cor[num_carruagem] ==prox_pt->pt.cor && prox_pt->pt.tipo == EST)
      aux_boio->graf.cor[num_carruagem] = hexdec_CINZENTO;
      aux_boio->graf.alavanca[num_carruagem] = aux_boio->graf.alavanca[num_carruagem-1];
    }
  }
}

void colisoes(LISTA_GRAF_BOIO *lista_graf_boios){
  LISTA_GRAF_BOIO *atual, *comparar;
  int i,j;

  if (lista_graf_boios == NULL) return;

  for (atual=lista_graf_boios; atual->pr!=NULL; atual=atual->pr){
    for(comparar=atual->pr; comparar!=NULL; comparar=comparar->pr){
      for(i=0; i<N_CAR; i++){
        for(j=0; j<N_CAR; j++){
          if(pow(atual->graf.x[i]-comparar->graf.x[j],2)+pow(atual->graf.y[i]-comparar->graf.y[j],2) < 1.3 * pow(2*RAIO_COMBOIO,2)){
            if(i==0 && j==0 && atual->graf.veloc == 0){}
            if(i==0 && j==0 && comparar->graf.veloc == 0){}
            else if(i==0 && j==0){
              atual->graf.veloc = 0;
            }
            else if(i==0 && j != 0)
              atual->graf.veloc = 0;
            else if(j==0 && i != 0)
              comparar->graf.veloc = 0;
          }
        }
      }
    }
  }
}

void pisca_comboios(LISTA_GRAF_BOIO *lista_graf_boios){

  for(;lista_graf_boios!=NULL; lista_graf_boios=lista_graf_boios->pr){
    if(lista_graf_boios->graf.veloc == 0){
      if(lista_graf_boios->graf.cor[0]==hexdec_CINZENTO)
        lista_graf_boios->graf.cor[0]=lista_graf_boios->graf.arquetipo.cor;
      else
        lista_graf_boios->graf.cor[0]=hexdec_CINZENTO;
    }
  }
}

void atualiza_render(LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *boios_graficos, int dimJanela[], int pausa){

  SDL_SetRenderDrawColor(pintor, 235, 235, 235, 255);
  SDL_RenderClear(pintor);

  desenha_ligacoes(topo_lista_linhas);
  desenha_pontos(topo_lista_linhas);
  desenha_comboios(boios_graficos);
  desenha_botoes(dimJanela, pausa);
}

void desenha_pontos(LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *ap;

  for(; topo_lista_linhas !=NULL; topo_lista_linhas=topo_lista_linhas->pr){
    for(ap = topo_lista_linhas->linha; ap!= NULL; ap=ap->pr[0]){
      if (ap->pt.tipo == EST){
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 10, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 10, hexdec_PRETO);
      }
      else if (ap->pr[0] != NULL && ap->pr[1] != NULL){
        // filledCircleColor(pintor, ap->pt.x, ap->pt.y, 4, hexdec_PRETO);
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 2, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 2, hexdec_PRETO);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 6, hexdec_PRETO);
      }
      else {
        filledCircleColor(pintor, ap->pt.x, ap->pt.y, 2, ap->pt.cor);
        aacircleColor(pintor, ap->pt.x, ap->pt.y, 2, hexdec_PRETO);
      }
    }
  }
}

void desenha_ligacoes(LISTA_LINHAS *topo_lista_linhas){
  LISTA_PONTOS *ap;

  for(; topo_lista_linhas !=NULL; topo_lista_linhas=topo_lista_linhas->pr){
    for(ap = topo_lista_linhas->linha; ap!= NULL; ap=ap->pr[0]){

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
    for(i=N_CAR-1; i>=0; i--){
      filledCircleColor(pintor, lista_graf_boios->graf.x[i], lista_graf_boios->graf.y[i], lista_graf_boios->graf.arquetipo.r_bolas, lista_graf_boios->graf.cor[i]);
      aacircleColor(pintor, lista_graf_boios->graf.x[i], lista_graf_boios->graf.y[i], lista_graf_boios->graf.arquetipo.r_bolas, hexdec_PRETO);
    }
  }
}

void desenha_botoes(int dimJanela[], int pausa){
  SDL_Rect botao_sair, botao_pausa;

  botao_sair.w = LARGURA_BOTAO;
  botao_sair.h = ALTURA_BOTAO;
  botao_sair.x = dimJanela[X] - ESPACAMENTO - LARGURA_BOTAO;
  botao_sair.y = ESPACAMENTO;
  botao_pausa.w = LARGURA_BOTAO;
  botao_pausa.h = ALTURA_BOTAO;
  botao_pausa.x = dimJanela[X] - 2*ESPACAMENTO - 2*LARGURA_BOTAO;
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

int eventos_sdl(SDL_Event *event, LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *topo_lista_graf_boios, int dimJanela[]){
  LISTA_PONTOS *aux_pt;
  LISTA_GRAF_BOIO *comboio_a_parar;
  int x=0, y=0;

  switch(event->type){
    case SDL_MOUSEMOTION:
    return 0;
    case SDL_MOUSEBUTTONDOWN:
      SDL_GetMouseState( &x, &y);
      aux_pt = procura_ponto_por_coords(topo_lista_linhas, x, y);
      comboio_a_parar = procura_locomotiva_por_coords(topo_lista_graf_boios, x, y);
      if(comboio_a_parar!=NULL) toggle_andamento_comboio(comboio_a_parar, topo_lista_graf_boios);
      else if (aux_pt !=NULL && aux_pt->pr[0] != NULL && aux_pt->pr[1] != NULL){
        aux_pt->pt.alavanca = 1 - aux_pt->pt.alavanca;
      }
      else return carregou_botao(dimJanela, x, y);
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

int carregou_botao(int dimJanela[], int x, int y){
  if ( y > ESPACAMENTO && y < ESPACAMENTO+ALTURA_BOTAO){
    if( x > dimJanela[X] - ESPACAMENTO - LARGURA_BOTAO && x < dimJanela[X] - ESPACAMENTO){
      // botao de sair
      return 1;
    }
    else if( x > dimJanela[X] - 2*ESPACAMENTO - 2*LARGURA_BOTAO && x < dimJanela[X] - 2*ESPACAMENTO - LARGURA_BOTAO){
      //botao de pausa
      return 2;
    }
  }
  return 0;
}

LISTA_GRAF_BOIO * procura_locomotiva_por_coords(LISTA_GRAF_BOIO *graf_boios, int x, int y){
  for(; graf_boios!=NULL; graf_boios=graf_boios->pr){
    if (pow(x - graf_boios->graf.x[0],2) + pow(y- graf_boios->graf.y[0],2) < pow(RAIO_ESTACAO,2)){
      return graf_boios;
    }
  }
  return NULL;
}

LISTA_PONTOS * procura_ponto_por_coords(LISTA_LINHAS *topo_lista_linhas, int x, int y){
  LISTA_PONTOS *aux=NULL;
  int deltaX, deltaY;

  for(; topo_lista_linhas!=NULL; topo_lista_linhas=topo_lista_linhas->pr){

    for(aux=topo_lista_linhas->linha; aux!=NULL; aux=aux->pr[0]){
      deltaX = aux->pt.x - x;
      deltaY = aux->pt.y - y;
      if(deltaX*deltaX + deltaY*deltaY < RAIO_ESTACAO*RAIO_ESTACAO){
        return aux;
      }
    }
  }
  return NULL;
}

void toggle_andamento_comboio(LISTA_GRAF_BOIO *boio_a_parar, LISTA_GRAF_BOIO *boios){
  int i;
  int raio1 = boio_a_parar->graf.arquetipo.r_bolas;
  int raio2;
  if (boio_a_parar->graf.veloc != 0){
    boio_a_parar->graf.veloc =0;
    return;
  }
  for(;boios!=NULL; boios=boios->pr){
    raio2 = boios->graf.arquetipo.r_bolas;
    for(i=0; i<N_CAR; i++){
      if(pow(boios->graf.x[i]-boio_a_parar->graf.x[0], 2)+pow(boios->graf.y[i]-boio_a_parar->graf.y[0], 2) < 1.5 * pow(raio1+raio2, 2) && boio_a_parar!=boios)
      return;
    }
  }
  boio_a_parar->graf.veloc=SPEED;
  boio_a_parar->graf.cor[0]=boio_a_parar->graf.arquetipo.cor;
}

void menu(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas){
  char leitura[100];
  int opcao;

    system("clear");
    printf("Lista de opções:\n0 – Retomar a simulacao\n1 – Mostrar a informação de uma ferrovia\n2 – Eliminar uma ferrovia\n3 – Mostrar a informação de um comboio\n4 – Eliminar um comboio\n5 – Criar um comboio\nSelecione a opção:");
    fgets(leitura, 100, stdin);
    if (sscanf(leitura, "%d", &opcao)==1){
    switch(opcao){
      case 0: return; break;
      case 1: opcao_mostra_linha(*topo_lista_linhas); return; break;
      case 2: *topo_lista_linhas = opcao_elimina_linha(*topo_lista_linhas, topo_lista_comboios); return; break;
      case 3: opcao_mostra_comboio(*topo_lista_comboios); return; break;
      case 4: *topo_lista_comboios = opcao_elimina_comboio(*topo_lista_comboios); return; break;
      case 5: *topo_lista_comboios = opcao_novo_comboio(*topo_lista_comboios, *topo_lista_linhas); return; break;
      default: break;
      }
    }
}
