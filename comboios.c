#include "comboios.h"

void opcao_mostra_linha(LISTA_LINHAS *topo_lista_linhas){
  char linha[100];
  int i=0;
  LISTA_LINHAS *aux = topo_lista_linhas;
  LISTA_PONTOS *auxiliar=NULL;
  while (1){
    system("clear");
    i=0;
    linha[0] = '\0';
    mostra_linha(topo_lista_linhas, "Qual a lista a mostrar?(Fim para sair)", linha);
    if (strlen(linha)==0)
    break;
    if (strcmp(linha, "Fim")==0)
    break;
    if (strlen(linha)>4){
      printf("Erro, ID invalido\n");
      i=1;
    }
    aux = topo_lista_linhas;
    for(;aux!=NULL;aux=aux->pr){
      if(strcmp(linha, aux->id)==0){
        i=1;
        for(auxiliar=aux->linha; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
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
  char linha[100];
  int i=0;
  LISTA_LINHAS *aux = topo_lista_linhas, *anterior = NULL;
  LISTA_PONTOS *elimina_lista_pontos;
  while (1){
    system("clear");
    i=0;
    mostra_linha(topo_lista_linhas, "Qual a lista a mostrar?(Fim para sair)", linha);
    if (strlen(linha)==0)
      break;

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
      if(strcmp(linha, aux->id)==0){
        i=1;
        for(; aux->linha!=NULL; aux->linha = aux->linha->pr[0]){
          elimina_lista_pontos = aux->linha;
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

LISTA_COMBOIOS * opcao_novo_comboio(LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS * topo_lista_linhas){
  char string_aux[4][10];
  int int_aux[4];

  int_aux[0]=verifica(3, 9, "Raio das bolas?");
  int_aux[2]=verifica(10, 100, "Qual a velocidade?");
  int_aux[1]=verifica(1, 9999, "Quantos comboios gerar?");
  int_aux[3]=verifica(1, 9, "Qual a cor da locomotiva?\n1-Vermelho\n2-Roxo\n3-Azul\n4-Ciano\n5-Verde\n6-Amarelo\n7-Castanho\n8-Preto\n9-Branco");
  strcpy(string_aux[1], menu_cor_para_string(int_aux[3]));
  get_id_comboio(string_aux[0]);
  get_coords_origem(string_aux[2], string_aux[3], topo_lista_linhas);

  topo_lista_comboios = preenche_comboio(string_aux, int_aux, topo_lista_comboios, topo_lista_linhas);

  system("clear");
  printf("Comboio criado com sucesso!\n");
  getchar();

  return topo_lista_comboios;
}

int verifica(int n_min, int n_max, char *texto){
  char  leitura[10];
  float valor=0;
  while(1){
    system("clear");
    printf("%s\nInsira um numero inteiro entre %d e %d:", texto, n_min, n_max);
    fgets(leitura, 10, stdin);
    if (sscanf(leitura, "%f", &valor)==1){
    if (n_min<=valor && valor<=n_max && valor-(int)valor==0){
      return valor;
      while(getchar()!='\n');
    }
    else if(valor-(int)valor!=0){
      printf("Erro, insira um numero inteiro\n");

      while(getchar()!='\n');
    }
    else{
      printf("Erro, insira um valor valido");

      while(getchar()!='\n');
    }
    }
    else{
      printf("Erro, insira um valor valido");

      while(getchar()!='\n');
    }
  }
}

void get_id_comboio(char *id_comboio){
  char leitura[10];
  while(1){
    system("clear");
    printf("Qual o id do comboio?\nMax 2 carateres:");
    fgets(leitura, 10, stdin);
    if (sscanf(leitura, "%s",id_comboio)==1){
      if (strlen(id_comboio)==0 || 3<=strlen(id_comboio)){
        printf("Erro, insira um numero de carateres valido\n");
        getchar();
      }
      else
        break;
      }
    }
}

void get_coords_origem(char * linha, char *ponto, LISTA_LINHAS * topo_lista_linhas){
  LISTA_LINHAS * aux=NULL;
  LISTA_PONTOS *auxiliar=NULL;
  int i=0;
  char leitura[100];
  while(1){
    system("clear");
    aux=topo_lista_linhas;
    mostra_linha(topo_lista_linhas, "Qual a lista de origem?", linha);
    for(;aux!=NULL;aux=aux->pr){
      if(strcmp(linha, aux->id)==0){
        i=1;
        break;
        }
      }

      if (i==0){
        printf("Erro, linha inexistente\n");
        while(getchar()!='\n');
      }
      else
        break;
  }


  while(1){
    i=0;
    system("clear");
    printf("Pontos pertencentes a linha %s:\n", linha);
    for(auxiliar=aux->linha; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
      printf("\nPonto: %s", auxiliar->pt.id);
    }
    printf("\nQual o ponto de origem?");
    fgets(leitura, 10, stdin);
    sscanf(leitura, "%s", ponto);

    for(auxiliar=aux->linha; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
      if (strcmp(ponto, auxiliar->pt.id)==0){
        i=1;
        break;
      }
    }
    if (i==0){
      printf("Erro, ponto inexistente\n");
      while(getchar()!='\n');
    }
    else
      break;
  }
}

void mostra_linha(LISTA_LINHAS* topo_lista_linhas, char * texto, char*linha){
  char leitura[100];
  if(topo_lista_linhas== NULL){
    printf("\nNeste momento nao ha linhas\nPode adicionar linhas editando o ficheiro .txt e reiniciando o programa de seguida\n");
    getchar();
    linha[0] = '\0';
    return;
  }
  printf("\nLista de linhas:\n");
  for(;topo_lista_linhas!=NULL;topo_lista_linhas=topo_lista_linhas->pr){
    printf("LINHA %s\n", topo_lista_linhas->id);
  }
  printf("\n%s\n", texto);
  fgets(leitura, 100, stdin);
  sscanf(leitura, "%s", linha);
}

void mostra_ponto(PONTO pt){
  char tipo[8];
  switch(pt.tipo){
    case 1: strcpy(tipo, "Via");      break;
    case 2: strcpy(tipo, "Estacao");  break;
    default: printf("Erro traducao de tipo\n"); break;
  }
  printf("\nPonto: %s\nTipo: %s\nCor: %s\n", pt.id, tipo, cor_Uint32_para_string(pt.cor));
  fflush (stdout);
}

void mostra_boio(COMBOIO boio){
  printf("\n/----COMBOIO %s----/\n", boio.id);
  printf("Raio das bolas:%d\n", boio.r_bolas);
  printf("Cor locom.:%s\n", cor_Uint32_para_string(boio.cor));
  printf("Ponto origem:%s\n", boio.origem->pt.id);
  printf("Numero de servicos:%d\n", boio.numero_de_servicos);
  printf("Velocidade:%.2fpixeis/segundo\n", boio.veloc*FPS);
  fflush(stdout);
}

void mostra_graf_boios_ativos(LISTA_GRAF_BOIO *lista_graf_boios){
  system("clear");
  while(lista_graf_boios!=NULL){
    printf("Comboio:%s\n", lista_graf_boios->graf.arquetipo->id);
    printf("Cor locomotiva:%s\n", cor_Uint32_para_string(lista_graf_boios->graf.cor[0]));
    printf("Numero de servicos restantes:%d\n\n", lista_graf_boios->graf.arquetipo->servicos_restantes);
    lista_graf_boios=lista_graf_boios->pr;
  }
  fflush(stdout);
}

void reset_servicos_restantes(LISTA_COMBOIOS *topo_lista_comboios){
  for(; topo_lista_comboios!=NULL; topo_lista_comboios=topo_lista_comboios->pr){
    topo_lista_comboios->boio.servicos_restantes = topo_lista_comboios->boio.numero_de_servicos;
  }
}
