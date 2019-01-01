#include "comboios.h"

int main(int argc, char *argv[]){
  LISTA_LINHAS *topo_lista_linhas = NULL;
  LISTA_COMBOIOS *topo_lista_comboios = NULL;
  int dimJanela[2];
  int opcao;
  char leitura[100];

  srand((unsigned long) &opcao);

  leitor_configs(&topo_lista_comboios, &topo_lista_linhas, dimJanela, argv[1]);

  while(1){
    system("clear");
    printf("Lista de opções:\n0 – Terminar o programa\n1 – Mostrar a informação de uma ferrovia\n2 – Eliminar uma ferrovia\n3 – Mostrar a informação de um comboio\n4 – Eliminar um comboio\n5 – Criar um comboio\n6 – Simulação dos comboios\nSelecione a opção:");
    fgets(leitura, 100, stdin);
    if (sscanf(leitura, "%d", &opcao)==1){
      switch(opcao){
        case 0: exit (0);
        case 1: opcao_mostra_linha(topo_lista_linhas); break;
        case 2: topo_lista_linhas = opcao_elimina_linha(topo_lista_linhas, &topo_lista_comboios); break;
        case 3: opcao_mostra_comboio(topo_lista_comboios); break;
        case 4: topo_lista_comboios = opcao_elimina_comboio(topo_lista_comboios); break;
        case 5: topo_lista_comboios = opcao_novo_comboio(topo_lista_comboios, topo_lista_linhas); break;
        case 6: simular(&topo_lista_comboios, &topo_lista_linhas, dimJanela); break;
        default: break;
      }
    }
  }
  return 0;
}
