#include "comboios.h"

void opcao_mostra_linha(LISTA_LINHAS *topo_lista_linhas){
  //opcao 1 - mostra as linhas e os pontos da que o utilizador escolher
  char linha[100];
  int i=0;
  LISTA_LINHAS *aux = topo_lista_linhas;
  LISTA_PONTOS *auxiliar=NULL;
  while (1){

    //get o nome da ferrovia que vai ser mostrada no ecra
    if((i=get_ferrovia_a_mostrar(linha, topo_lista_linhas))==-1)
      break;

    aux = topo_lista_linhas;
    for(;aux!=NULL;aux=aux->pr){
      if(strcmp(linha, aux->id)==0){
        //testa se a linha inserida pelo utilizador corresponde a alguma das guardadas na lista
        i=1;
        for(auxiliar=aux->linha; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
          mostra_ponto(auxiliar->pt); //mostra os pontos da ferrovia
        }
        while(getchar()!='\n');
      }
    }
    if (i==0){
      //se o id vor valido(menor que  4 carateres) mas nao encontrar a ferrovia
      printf("Erro, ferrovia inexistente\n");
      while(getchar()!='\n');
  }
  }
}

void mostra_ponto(PONTO pt){
  //faz printf de um ponto no ecra
  char tipo[8];
  switch(pt.tipo){
    case 1: strcpy(tipo, "Via");      break; //se tipo=1 vai imprimir via no ecra
    case 2: strcpy(tipo, "Estacao");  break;//se tipo=2 vai imprimir estacao no ecra
    default: printf("Erro traducao de tipo\n"); break;
  }
  printf("\nPonto: %s\nTipo: %s\nCor: %s\n", pt.id, tipo, cor_Uint32_para_string(pt.cor));
  fflush (stdout);
}

LISTA_LINHAS * opcao_elimina_linha(LISTA_LINHAS *topo_lista_linhas, LISTA_COMBOIOS **topo_lista_comboios){
  //opcao 2 - elimina uma linha escolhida pelo utilizador
  char linha[100];
  int i=0;
  LISTA_LINHAS *aux, *anterior;
  LISTA_PONTOS *elimina_lista_pontos;
  LISTA_COMBOIOS *comboio_a_eliminar;
  while (1){
    //get o nome da ferrovia que vai ser eliminada
    if((i=get_ferrovia_a_mostrar(linha, topo_lista_linhas))==-1)
      break;

    for(aux = topo_lista_linhas, anterior=NULL;aux!=NULL;aux=aux->pr){
      //verifica se ha alguma linha com o nome da linha escolhida
      if(strcmp(linha, aux->id)==0){
        i=1;
        while((comboio_a_eliminar = procura_comboios_na_linha(*topo_lista_comboios, aux->linha))!=NULL){
          //define servicos_restantes e num_servicos como -1 porque os comboios tem origem na linha a ser eliminada. quando chegarem ao fim da linha serao eliminados
          comboio_a_eliminar->boio.servicos_restantes=-1;
          comboio_a_eliminar->boio.num_servicos=-1;
        }
        //remove ligacoes que a linha tenha com outras linhas
        remove_ligacoes_para_a_linha_eliminada(topo_lista_linhas, aux);

        //elimina a linha
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
        break;
      }
      else
      anterior = aux;
    }
    //se o id inserido pelo utilizador for valido mas a ferrovia nao existir da erro
    if (i==0){
      printf("Erro, ferrovia inexistente\n");
    while(getchar()!='\n');
  }
  }
  return topo_lista_linhas;
}

LISTA_COMBOIOS *procura_comboios_na_linha(LISTA_COMBOIOS *lista_comboios, LISTA_PONTOS *linha){
  //devolve o primeiro comboio que encontrar que tenha partida na linha especificada
  LISTA_PONTOS *aux_pt = NULL;
  LISTA_COMBOIOS *aux_boios;
  int contador;
  for(aux_boios=lista_comboios;aux_boios!=NULL; aux_boios=aux_boios->pr){
    for(aux_pt = linha; aux_pt != NULL; aux_pt = aux_pt->pr[0]){
      //verifica se ha algum comboio com origem num ponto da linha escolhida
      if(aux_boios->boio.origem == aux_pt && aux_boios->boio.num_servicos!=-1)
        return aux_boios;
        //verifica se ha algum comboio a circular na linha
      for(contador=0;contador<N_CAR;contador++)
      //se houver um comboio a circular na linha volta ao seu ponto de origem e fica com menos um  servico restante:
        if(aux_boios->boio.ultimo_ponto[contador]==aux_pt || aux_boios->boio.ultimo_ponto[contador]->pr[1]==aux_pt || aux_boios->boio.ultimo_ponto[contador]->pr[0]==aux_pt)
          reset_movimento(&lista_comboios, aux_boios);
    }
  }

  return NULL;
}

void remove_ligacoes_para_a_linha_eliminada(LISTA_LINHAS *lista_linhas, LISTA_LINHAS *eliminar){
  //remove ligacoes que a linha possa ter com outras
  LISTA_PONTOS *aux_pt;

  for(;lista_linhas != NULL; lista_linhas=lista_linhas->pr){
    if (lista_linhas == eliminar)
      continue;
    for(aux_pt = lista_linhas->linha ; aux_pt!=NULL; aux_pt=aux_pt->pr[0]){
      if(aux_pt->pr[1] != NULL && aux_pt->pr[1]->pt.linha == eliminar)
        aux_pt->pr[1]=NULL;
    }
  }
}

int get_ferrovia_a_mostrar(char* linha, LISTA_LINHAS *topo_lista_linhas){
  system("clear");
  linha[0]='\0';//inicializa a string para estar vazia
  if (mostra_linha(topo_lista_linhas, "Qual a ferrovia?(Voltar para regressar ao menu)", linha)==-1)
    return -1;

    //se o utilizador quiser voltar ao menu retorna -1:
  if (strcmp(linha, "Voltar")==0)
    return -1;
    //o id da ferrovia so pode ter 4 carateres no maximo
  else if (strlen(linha)>4){
    printf("Erro, ID invalido\n");
    while(getchar()!='\n');
    return 2;
  }


  return 0; //retorna 0 se o id da ferrovia inserido for valido
}

int mostra_linha(LISTA_LINHAS* topo_lista_linhas, char * texto, char*linha){
  char leitura[100];
  if(topo_lista_linhas== NULL){
    printf("\nNeste momento nao ha linhas\nPode adicionar linhas editando o ficheiro .txt e reiniciando o programa de seguida\n");
    //caso nao encontre linhas faz printf da mensagem no ecra e volta a funcao anterior retornando -1, pelo que a outra volta ao menu principal
    getchar();
    linha[0] = '\0';
    return -1;
  }
  printf("\nLista de ferrovias:\n");
  //faz printf do nome das ferrovias
  for(;topo_lista_linhas!=NULL;topo_lista_linhas=topo_lista_linhas->pr){
    printf("FERROVIA %s\n", topo_lista_linhas->id);
  }
  printf("\n%s\n", texto);
  fgets(leitura, 100, stdin);
  sscanf(leitura, "%s", linha);
  return 1;
}

void opcao_mostra_comboio(LISTA_COMBOIOS *topo_lista_comboios){
  //opcao 3 - mostra um comboio a escolha do utilizador
  char comboio[100];
  int id_valido=0;
  LISTA_COMBOIOS *aux=topo_lista_comboios;
    while(1){

      if ((id_valido=get_comboio_a_mostrar(topo_lista_comboios, comboio))==-1)//vai buscar o id do comboio
        break;

      //verifica se o comboio escolhido pelo utilizador existe, e se sim faz printf da informacao no ecra:
      for(aux=topo_lista_comboios;aux!=NULL;aux=aux->pr){
        if(strcmp(comboio, aux->boio.id)==0){
          mostra_boio(aux->boio);
          id_valido=1;
          }
      }
      //caso o id do comboio seja valido mas o mesmo nao exista
      if(id_valido==0)
        printf("Erro, comboio inexistente\n");
      while(getchar()!='\n');
    }
}

void mostra_boio(COMBOIO boio){
  //faz printf da informacao de um comboio
  printf("\n/----COMBOIO %s----/\n", boio.id);
  printf("Raio das bolas:%d\n", boio.r_bolas);
  printf("Cor locom.:%s\n", cor_Uint32_para_string(boio.cor[0]));
  //se num_servicos=-1 a linha de origem do comboio foi eliminada mas o comboio ainda se encontra em circulacao
  if(boio.num_servicos == -1)
    printf("A linha de origem deste comboio foi apagada, o comboio sera apagado assim que chegar ao fim da linha\n");
  else{
    printf("Linha origem:%s\n", boio.origem->pt.linha->id);
    printf("Ponto origem:%s\n", boio.origem->pt.id);
    printf("Numero de servicos:%d\n", boio.num_servicos);
    printf("Numero de servicos restantes: %d\n", boio.servicos_restantes);
  }
  fflush(stdout);
}

LISTA_COMBOIOS * opcao_elimina_comboio(LISTA_COMBOIOS *topo_lista_comboios){
  //opcao 4 - elimina um comboio a escolha do utilizador
  char comboio[100];
  int i=0;
  LISTA_COMBOIOS *aux=topo_lista_comboios;
    while(1){
      //vai buscar o id do comboio, inserido pelo utilizador
      if ((i=get_comboio_a_mostrar(topo_lista_comboios, comboio))==-1)
        break;

      for(aux=topo_lista_comboios;aux!=NULL;aux=aux->pr){
        if(strcmp(comboio, aux->boio.id)==0){
          //verifica se o comboio inserido existe, se sim elimina-o
          i=1;
          topo_lista_comboios = elimina_comboio(topo_lista_comboios, aux);
        }
      }
      if(i==0){
        //se o id for valido mas o comboio nao existir da erro
        printf("Erro, comboio inexistente\n");
      while(getchar()!='\n');
      }
    }
    return topo_lista_comboios;
}

LISTA_COMBOIOS *elimina_comboio(LISTA_COMBOIOS *lista_comboios, LISTA_COMBOIOS *eliminar){
  LISTA_COMBOIOS *anterior, *atual;
  //recebe um comboio a ser eliminado
  if(eliminar == lista_comboios){
    lista_comboios=lista_comboios->pr;
    free(eliminar);
    return lista_comboios;
  }

  for(atual=lista_comboios; atual != NULL && atual != eliminar; anterior=atual, atual=atual->pr){}

  if(atual == NULL){
    printf("Erro eliminacao comboio");
    fflush(stdout);
  }
  else{
    anterior->pr=atual->pr;
    free(atual);
  }

  return lista_comboios;
}

int get_comboio_a_mostrar(LISTA_COMBOIOS * topo_lista_comboios, char*comboio){
  LISTA_COMBOIOS* aux;
  char leitura[100];
  int i=0;
  //vai buscar o nome do comboio e testa o id inserido pelo utilizador

  comboio[0]='\0';//inicia a string vazia
  system("clear");

  if(topo_lista_comboios== NULL){
    //caso a lista de comboios esteja vazia avisa o utilizador e retorna -1
    printf("\nNeste momento nao ha comboios\nPode adicionar comboios utilizando a opcao 5 do menu principal\n");
    getchar();
    return -1;
  }
  printf("\nLista de comboios:\n");
  for(aux=topo_lista_comboios;aux!=NULL;aux=aux->pr)
    printf("COMBOIO %s\n", aux->boio.id);

  printf("\nQual o comboio?(Voltar para regressar ao menu)\n");
  fgets(leitura, 100, stdin);
  sscanf(leitura, "%s", comboio);
  //testa o string, se o utilizador escrever Voltar retorna ao menu
  if (strcmp(comboio, "Voltar")==0)
    i=-1;

  // se o id for demasiado comprido da erro e retorna 1
  else if (strlen(comboio)>2){
    printf("Erro, ID invalido\n");
    i=1;
  }

  return i;
}

LISTA_COMBOIOS * opcao_novo_comboio(LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS * topo_lista_linhas){
  char string_aux[4][10];
  int int_aux[4];
  //opcao 5 - criacao de um comboio com os parametros inseridos pelo utilizador:
  if(topo_lista_linhas==NULL){
    system("clear");
    printf("Nao e possivel criar comboios visto que nao ha linhas\nPode inserir linhas atraves do ficheiro de configs e reiniciando o programa\n");
    getchar();
    return NULL;
  }
  //vai buscar o id do novo comboio
  get_id_comboio( string_aux[0], topo_lista_comboios);
  //vai buscar e testa o raio, numero de servicos e a cor
  int_aux[0]=verifica(3, 9, "Raio das bolas?");
  int_aux[1]=verifica(1, 9999, "Qual o numero de servicos?");
  int_aux[3]=verifica(1, 9, "Qual a cor da locomotiva?\n1-Vermelho\n2-Roxo\n3-Azul\n4-Ciano\n5-Verde\n6-Amarelo\n7-Castanho\n8-Preto\n9-Branco");
  //converte o numero da cor inserido para a cor
  strcpy(string_aux[1], menu_cor_para_string(int_aux[3]));
  //vai buscar e testa o ponto e linha de origem
  get_coords_origem(string_aux[2], string_aux[3], topo_lista_linhas);
  //preenche o comboio com os dados inseridos
  topo_lista_comboios = preenche_comboio(string_aux, int_aux, topo_lista_comboios, topo_lista_linhas);

  system("clear");
  printf("Comboio criado com sucesso!\n");
  getchar();

  return topo_lista_comboios;
}

int verifica(int n_min, int n_max, char *texto){
  char  leitura[10], lixo[10];
  float valor=0;
  //recebe o numero minimoe maximo a serem inseridos, assim como texto a ser escrito
  while(1){
    system("clear");
    printf("%s\nInsira um numero inteiro entre %d e %d:", texto, n_min, n_max);
    fgets(leitura, 10, stdin);
    //so avanca se o utilizador tiver inserido apenas um numero, caso contrario imprime uma mensagem de erro
    if (sscanf(leitura, "%f", &valor)==1 && sscanf(leitura, "%f %s", &valor, lixo)==1){
      //se o valor inserido estiver dentro dos limites e for um num inteiro faz return do valor
      if (n_min<=valor && valor<=n_max && valor-(int)valor==0){
        return valor;
        while(getchar()!='\n');
      }
      //mensagem de erro se o numero nao for inteiro
      else if(valor-(int)valor!=0){
        printf("Erro, insira um numero inteiro\n");
        while(getchar()!='\n');
      }
      //mensagem de erro se o valor nao estiver dentro dos intervalos
      else{
        printf("Erro, insira um valor valido dentro do intervalo pedido");
        while(getchar()!='\n');
      }
    }
    else{
      printf("Erro, insira um valor valido");
      while(getchar()!='\n');
    }
  }
}

char *menu_cor_para_string(int cor){
  // converte o numero inserido pelo utilizador na string a ser guardada na struct
  switch(cor){
    case CINZENTO: return "CINZENTO";
    case VERMELHO: return "VERMELHO";
    case ROXO:     return "ROXO";
    case AZUL:     return "AZUL";
    case CIANO:    return "CIANO";
    case VERDE:    return "VERDE";
    case AMARELO:  return "AMARELO";
    case CASTANHO: return "CASTANHO";
    case PRETO:    return "PRETO";
    case BRANCO:   return "BRANCO";
  }
  return 0;
}

void get_id_comboio(char * id_comboio, LISTA_COMBOIOS * topo_boios){
  char leitura[10];
  int boio_valido=1;
  LISTA_COMBOIOS *aux;
  //funcao para ler o id dom comboio
  while(1){
    boio_valido=1;
    system("clear");
    printf("Qual o id do comboio?\nMax 2 carateres:");
    fgets(leitura, 10, stdin);
    if (sscanf(leitura, "%s",id_comboio)==1){
      //se for inserido um numero de carateres invalido da uma mensagem de erro
      if (strlen(id_comboio)==0 || 3<=strlen(id_comboio)){
        printf("Erro, insira um numero de carateres valido\n");
        getchar();
        boio_valido=0;
      }
      //testa se o id do comboio ja existe, se sim faz printf de uma mensagem
      for(aux=topo_boios;aux!=NULL; aux=aux->pr){
        if(strcmp(aux->boio.id, id_comboio)==0){
          printf("Comboio ja existente, insira outro ID\n");
          getchar();
          boio_valido=0;
        }
      }
      //se o numero de carateres for valido e o comboio nao existir a funcao termina
      if(boio_valido==1)
        break;
      }
    }
}

void get_coords_origem(char * linha, char *ponto, LISTA_LINHAS * topo_lista_linhas){
  LISTA_LINHAS * aux=NULL;
  LISTA_PONTOS *auxiliar=NULL;
  int id_valido=0;
  char leitura[100];
  //funcao para receber e testar as coords da origem do comboio
  //ciclo para receber a linha de origem:
  while(1){
    system("clear");

    mostra_linha(topo_lista_linhas, "Qual a ferrovia de origem?", linha);
    //testa o id da ferrovia recebido, se existir continua
    for(aux=topo_lista_linhas;aux!=NULL;aux=aux->pr){
      if(strcmp(linha, aux->id)==0){
        id_valido=1;
        break;
        }
      }
      //se o id nao existir imprime a mensagem de erro
      if (id_valido==0){
        printf("Erro, linha inexistente\n");
        while(getchar()!='\n');
      }
      else
        break;
  }

  //ciclo para receber o ponto de origem
  while(1){
    id_valido=0;
    system("clear");
    printf("Pontos pertencentes a linha %s:\n", linha);
    for(auxiliar=aux->linha; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
      printf("\nPonto: %s", auxiliar->pt.id);
    }
    printf("\nQual o ponto de origem?");
    fgets(leitura, 10, stdin);
    sscanf(leitura, "%s", ponto);
    //testa o ponto inserido, se existir continua
    for(auxiliar=aux->linha; auxiliar!=NULL; auxiliar = auxiliar->pr[0]){
      if (strcmp(ponto, auxiliar->pt.id)==0){
        id_valido=1;
        break;
      }
    }
    //se o id nao existir imprime a mensagem de erro
    if (id_valido==0){
      printf("Erro, ponto inexistente\n");
      while(getchar()!='\n');
    }
    else
      break;
  }
}

char * cor_Uint32_para_string(Uint32 cor){
  // converte o Uint32 para um string com o nome da cor
  switch(cor){
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
