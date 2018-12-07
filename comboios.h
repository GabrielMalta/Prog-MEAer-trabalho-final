#include <stdio.h>
#include <stdlib.h>
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
  char id[4];
  float x, y;
  int cor;
  int tipo;
  float dim;
  int entradas;
  int saidas;
} PONTO;

typedef struct node{
  PONTO pt;
  struct node *ant[3];
  struct node *pr[3];
} LINHA;

typedef struct comboio{
  char id[2];
  int dim;
  float r_bolas;
  int cor[4];
} COMBOIO;

void mostra_ponto(PONTO pt);
