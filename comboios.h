#include <stdio.h>
#include <stdlib.h>

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
  int tipo;
  int cor;
  float x, y;
  int saidas;
} PONTO;

typedef struct node{
  PONTO pt;
  struct node *pr[3];
} LINHA;

typedef struct comboio{
  char id[2];
  int cor_locom;
  CARRUAGEM *car;
  int size;
  float x, y;
} TRAIN;

typedef struct carruagem{
  int cor;
  float x, y;
  struct carruagem *pr;
} CARRUAGEM;

void mostra_ponto(PONTO pt);
