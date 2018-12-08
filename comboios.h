#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <SDL.h>

#define DIMX 900
#define DIMY 600

#define SIM 1
#define NAO 0

#define VIA 0
#define EST 1
#define ENT 2
#define SAI 3

#define CINZENTO 0
#define VERMELHO 1
#define ROXO 2
#define AZUL 3
#define CIANO 4
#define VERDE 5
#define AMARELO 6
#define CASTANHO 7
#define PRETO 8
#define BRANCO 9

typedef struct{
  char id[5];
  int x, y;
  int cor;
  int tipo;
  float dim;
  int entradas;
  int saidas;
} PONTO;

typedef struct node{
  // char id[5];
  PONTO pt;
  // struct node *ant[3];
  struct node *pr;
} LISTA_PONTOS;

typedef struct {
  char id[5];
  LISTA_PONTOS *l;
} LINHA;

typedef struct{
  char id[3];
  int dim;
  int r_bolas;
  int cor[4];
  char orig_l[5];
  char orig_pt[5];
  char dest_l[5];
  char dest_pt[5];
} COMBOIO;

// typedef struct lista_comboios{
//   COMBOIO train;
//   struct lista_comboios *pr;
// } LISTA_COMBOIOS;

void mostra_ponto(PONTO pt);

void mostra_boio(COMBOIO boio);

int numero_cor(char string[]);

int numero_tipo(char string[]);

char * cor_numero(int no);

void leitor_configs(COMBOIO ***comboios, LINHA ***linhas, int *dim_X, int *dim_Y);
