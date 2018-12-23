#include "comboios.h"


LISTA_GRAF_BOIO * mexe_comboios2(LISTA_GRAF_BOIO *lista_graf_boios){
  float deltaX, deltaY;
  float m;
  float x_a_somar, y_a_somar;
  int i;
  LISTA_PONTOS *pt1 = NULL, *pt2 = NULL;
  LISTA_GRAF_BOIO *aux_boio = NULL;
  for( aux_boio = lista_graf_boios; aux_boio!=NULL; aux_boio=aux_boio->pr){
    if (aux_boio->graf.veloc == 0) continue;
    for(i=0; i<aux_boio->graf.boio->dim; i++){
      pt1 = aux_boio->graf.ultimo_ponto[i];

      if( (pt2 = pt1->pr[aux_boio->graf.alavanca[i]]) != NULL){}
      else if( (pt2 = pt1->pr[1-aux_boio->graf.alavanca[i]]) != NULL){}
      else{
        lista_graf_boios = remove_graf_boio(lista_graf_boios, aux_boio);
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

        x_a_somar = deltaX/abs(deltaX)  *  aux_boio->graf.veloc/sqrt(m*m+1);
        y_a_somar = m*x_a_somar;
      }
      else{
        x_a_somar=0;
        y_a_somar=deltaY/abs(deltaY) * aux_boio->graf.veloc;
      }

      aux_boio->graf.x[i] += x_a_somar;
      aux_boio->graf.y[i] += y_a_somar;

      deltaX = pt2->pt.x - aux_boio->graf.x[i]; //reciclar variaveis
      deltaY = pt2->pt.y - aux_boio->graf.y[i]; //para salvar o ambiente

      if( sqrt(deltaY*deltaY + deltaX*deltaX) < aux_boio->graf.veloc){
        aux_boio->graf.x[i] = pt2->pt.x;
        aux_boio->graf.y[i] = pt2->pt.y;
        aux_boio->graf.ultimo_ponto[i] = pt2;
        if(i==0) aux_boio->graf.alavanca[i] = pt2->pt.alavanca;
        if(i!=0) {
        if (aux_boio->graf.cor[i] == pt2->pt.cor) aux_boio->graf.cor[i] = hexdec_CINZENTO;
        aux_boio->graf.alavanca[i] = aux_boio->graf.alavanca[i-1];}
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
  }
  return lista_graf_boios;
}

void opcao_mostra_comboio(LISTA_COMBOIOS *topo_lista_comboios){
  char comboio[100], leitura[100];
  int i=0;
  LISTA_COMBOIOS *aux=topo_lista_comboios;
    while(1){
      system("clear");
      i=0;
      aux=topo_lista_comboios;
      if(topo_lista_comboios== NULL){
        printf("\nNeste momento nao ha comboios\nPode adicionar comboios utilizando a opcao 5 do menu principal\n");
        getchar();
        break;
      }
      printf("\nLista de comboios:\n");
      for(;aux!=NULL;aux=aux->pr)
        printf("COMBOIO %s\n", aux->boio.id);
      printf("\nQual o comboio a mostrar?(Fim para sair)\n");

      fgets(leitura, 100, stdin);
      sscanf(leitura, "%s", comboio);

      if (strcmp(comboio, "Fim")==0)
        break;

      if (strlen(comboio)>2){
        printf("Erro, ID invalido\n");
        i=1;
      }

      aux=topo_lista_comboios;

      for(;aux!=NULL;aux=aux->pr){
        if(strcmp(comboio, aux->boio.id)==0){
          mostra_boio(aux->boio);
          i=1;
          }
      }
      if(i==0)
        printf("Erro, comboio inexistente\n");
      while(getchar()!='\n');
    }
}

LISTA_COMBOIOS * opcao_elimina_comboio(LISTA_COMBOIOS *topo_lista_comboios){
  char comboio[100], leitura[100];
  int i=0;
  LISTA_COMBOIOS *aux=topo_lista_comboios, *anterior=NULL;
    while(1){
      system("clear");
      i=0;
      aux=topo_lista_comboios;
      if(topo_lista_comboios== NULL){
        printf("\nNeste momento nao ha comboios\nPode adicionar comboios utilizando a opcao 5 do menu principal\n");
        getchar();
        break;
      }
      printf("\nLista de comboios:\n");
      for(;aux!=NULL;aux=aux->pr)
        printf("COMBOIO %s\n", aux->boio.id);

      printf("\nQual o comboio a eliminar?(Fim para sair)\n");
      fgets(leitura, 100, stdin);
      sscanf(leitura, "%s", comboio);

      if (strcmp(comboio, "Fim")==0)
        break;

      if (strlen(comboio)>2){
        printf("Erro, ID invalido\n");
        i=1;
        while(getchar()!='\n');
      }

      aux=topo_lista_comboios;
      anterior=NULL;
      for(;aux!=NULL;aux=aux->pr){
        if(strcmp(comboio, aux->boio.id)==0){
          i=1;
          if (anterior == NULL){
            topo_lista_comboios = topo_lista_comboios->pr;
            free(aux);
          }
          else{
            anterior->pr=aux->pr;
            free(aux);
          }
        }
        else
        anterior = aux;
      }
      if(i==0){
        printf("Erro, comboio inexistente\n");
        while(getchar()!='\n');
      }
    }
    return topo_lista_comboios;
}

void opcao_mostra_linha(LISTA_LINHAS *topo_lista_linhas){
  char linha[100], leitura[100];
  int i=0;
  LISTA_LINHAS *aux = topo_lista_linhas;
  LISTA_PONTOS *auxiliar=NULL;
  while (1){
    system("clear");
    i=0;
    aux = topo_lista_linhas;
    if(topo_lista_linhas== NULL){
      printf("\nNeste momento nao ha linhas\nPode adicionar linhas editando o ficheiro .txt e reiniciando o programa de seguida\n");
      getchar();
      break;
    }
    printf("\nLista de linhas:\n");
    for(;aux!=NULL;aux=aux->pr){
      printf("LINHA %s\n", aux->linha.id);
    }
    printf("\nQual a linha a mostrar?(Fim para sair)\n");
    fgets(leitura, 100, stdin);
    sscanf(leitura, "%s", linha);
      if (strcmp(linha, "Fim")==0)
        break;
      if (strlen(linha)>4){
        printf("Erro, ID invalido\n");
        i=1;
      }
      aux = topo_lista_linhas;
      for(;aux!=NULL;aux=aux->pr){
        if(strcmp(linha, aux->linha.id)==0){
          i=1;
          for(auxiliar=aux->linha.l; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
            mostra_ponto(auxiliar->pt);
          }
        }
      }
      if (i==0)
        printf("Erro, linha inexistente\n");
      while(getchar()!='\n');
  }
}

LISTA_LINHAS * opcao_elimina_linha(LISTA_LINHAS *topo_lista_linhas){
  char linha[100], leitura[100];
  int i=0;
  LISTA_LINHAS *aux = topo_lista_linhas, *anterior = NULL;
  LISTA_PONTOS *elimina_lista_pontos;
  while (1){
    system("clear");
    i=0;
    aux = topo_lista_linhas;
    printf("\nLista de linhas:\n");
      for(;aux!=NULL;aux=aux->pr){
        printf("LINHA %s\n", aux->linha.id);
      }
    if(topo_lista_linhas== NULL){
      printf("\nNeste momento nao ha linhas\nPode adicionar linhas editando o ficheiro .txt e reiniciando o programa de seguida\n");
      getchar();
      break;
    }
    printf("\nQual a linha a eliminar?(Fim para sair)\n");
    fgets(leitura, 100, stdin);
    sscanf(leitura, "%s", linha);

    if (strcmp(linha, "Fim")==0)
      break;

    if (strlen(linha)>4){
      printf("Erro, ID invalido\n");
      while(getchar()!='\n');
      i=1;
    }

    aux = topo_lista_linhas;
    anterior = NULL;

    for(;aux!=NULL;aux=aux->pr){
      if(strcmp(linha, aux->linha.id)==0){
        i=1;
        for(; aux->linha.l!=NULL; aux->linha.l = aux->linha.l->pr[0]){
          elimina_lista_pontos = aux->linha.l;
          free(elimina_lista_pontos);
        }
        if (anterior == NULL){
           topo_lista_linhas=aux->pr;
           free(aux);
        }
        else{
          anterior->pr=aux->pr;
          free(aux);
        }
      }
      else
      anterior = aux;
    }
    if (i==0){
      printf("Erro, linha inexistente\n");
    while(getchar()!='\n');
    }
  }
  return topo_lista_linhas;
}

LISTA_COMBOIOS * opcao_novo_comboio(LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS * topo_lista_linhas){
  char string_aux[4][10];
  int int_aux[4];

  printf("Qual o id do comboio?(max 2 carateres)");
  scanf("%s", string_aux[0]);
  printf("Qual a cor da locomotiva?");
  scanf("%s", string_aux[1]);
  printf("Quantas carruagens?(max 3)");
  scanf("%d", &int_aux[0]);
  printf("Velocidade?");
  scanf("%d", &int_aux[2]);
  printf("Tempo de spawn?");
  scanf("%d", &int_aux[1]);
  printf("Qual a linha e ponto de origem?");
  scanf("%s %s", string_aux[2], string_aux[3]);

  topo_lista_comboios = preenche_comboio(string_aux[0], int_aux, topo_lista_comboios, topo_lista_linhas);

  while(getchar()!='\n');

  return topo_lista_comboios;
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

    for(aux=topo_lista_linhas->linha.l; aux!=NULL; aux=aux->pr[0]){
      deltaX = aux->pt.x - x;
      deltaY = aux->pt.y - y;
      if(deltaX*deltaX + deltaY*deltaY < RAIO_ESTACAO*RAIO_ESTACAO){
        return aux;
      }
    }
  }
  return NULL;
}

void toggle_andamento_comboio(LISTA_GRAF_BOIO *boio_a_parar){
  if (boio_a_parar->graf.veloc != 0){
    boio_a_parar->graf.veloc =0;
    return;
  }
  boio_a_parar->graf.veloc=boio_a_parar->graf.boio->veloc;
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

void mostra_boio(COMBOIO boio){
  printf("\n/----COMBOIO %s----/\n", boio.id);
  printf("Dimensao:%d\n", boio.dim);
  printf("Cor locom.:%s\n", cor_codigo(boio.cor));
  printf("Ponto origem:%s\n", boio.origem->pt.id);
  printf("Tempo de spawn:%.2f\n", boio.tempo_spawn);
  printf("Velocidade:%.2fpixeis/segundo\n", boio.veloc*FPS);
  fflush(stdout);
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
