#ifndef COMBOIOS_H_

#define COMBOIOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>


// SDL_Surface *superficie = 0;

#define PIXEIS_p_TICK 2

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

#define hexdec_CINZENTO 0xff808080
#define hexdec_VERMELHO 0xff0000ff
#define hexdec_ROXO 0xff800080
#define hexdec_AZUL 0xffff0000
#define hexdec_CIANO 0xffffff00
#define hexdec_VERDE 0xff00ff00
#define hexdec_AMARELO 0xff00ffff
#define hexdec_CASTANHO 0xff123159
#define hexdec_PRETO 0xff000000
#define hexdec_BRANCO 0xffffffff

typedef struct{
  char id[5];
  int x, y;
  Uint32 cor;
  int tipo;
} PONTO;

typedef struct node{
  PONTO pt;
  // struct node *ant[3];
  struct node *pr[2];
} LISTA_PONTOS;

typedef struct {
  char id[5];
  LISTA_PONTOS *l;
} LINHA;

typedef struct{
  char id[3];
  int dim;
  Uint32 cor[4];
  LISTA_PONTOS *origem;
  float tempo_spawn;
  float veloc;
} COMBOIO;

typedef struct{
  COMBOIO *boio;
  float x, y;
  LISTA_PONTOS *ultimo_ponto;
} GRAF_BOIO;

typedef struct lista_graf_boios{
  GRAF_BOIO *graf;
  struct lista_graf_boios *pr;
} LISTA_GRAF_BOIO;

void mostra_ponto( PONTO pt);

void mostra_boio( COMBOIO boio);

Uint32 codigo_cor( char string[]);

int numero_tipo( char string[]);

char * cor_codigo( Uint32 no);

void leitor_configs( COMBOIO ***comboios, LINHA ***linhas, int *dim_X, int *dim_Y, char *nome_ficheiro);

void liga_pontos( char aux_string[6][10], LINHA ***linhas);

int inicializa_janela( int dim_X, int dim_Y);

void atualiza_render(LINHA **linhas);

void desenha_pontos( LINHA **linhas);

void desenha_ligacoes( LINHA **linhas);

LISTA_PONTOS * procura_ponto( char *id_linha, char *id_ponto, LINHA **linhas);

void mexe_comboios(LISTA_GRAF_BOIO **comboio, LINHA **linhas);

void inicializa_boios(LISTA_GRAF_BOIO ***boios_graficos, COMBOIO **comboios, LINHA **linhas);

void render(void);

#endif
