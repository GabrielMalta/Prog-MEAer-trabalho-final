// #ifndef COMBOIOS_H_
//
// #define COMBOIOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

SDL_Window* janela;
SDL_Renderer* pintor;

#define FPS 30
#define TICKS_p_FRAME 1000/FPS

#define RAIO_ESTACAO 10
#define RAIO_COMBOIO 7
#define MULT_VELOC 0.7

#define VIA 0
#define EST 1

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
  int alavanca;
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

typedef struct elem_lista_linhas{
  LINHA linha;
  struct elem_lista_linhas *pr;
} LISTA_LINHAS;

typedef struct{
  char id[3];
  int dim;
  Uint32 cor;
  LISTA_PONTOS *origem;
  float tempo_spawn;
  float veloc;
} COMBOIO;

typedef struct elem_lista_comboios{
  COMBOIO boio;
  struct elem_lista_comboios *pr;
} LISTA_COMBOIOS;

typedef struct grafico_comboio{
  COMBOIO *boio;
  float x[4], y[4];
  Uint32 cor[4];
  LISTA_PONTOS *ultimo_ponto[4];
  int alavanca[4];
} GRAF_BOIO;


typedef struct lista_graf_boios{
  GRAF_BOIO graf;
  struct lista_graf_boios *pr;
} LISTA_GRAF_BOIO;

void mostra_ponto( PONTO pt);

void mostra_boio( COMBOIO boio);

Uint32 codigo_cor( char string[]);

int numero_tipo( char string[]);

Uint32 random_cor(void);

char * cor_codigo( Uint32 no);

void leitor_configs( LISTA_COMBOIOS **comboios, LISTA_LINHAS **linhas, int *dim_X, int *dim_Y, char *nome_ficheiro);

void liga_pontos(char aux_string[6][10], LISTA_LINHAS *topo_lista_linhas);

int inicializa_janela( int dim_X, int dim_Y);

void atualiza_render(LISTA_LINHAS *topo_lista_linhas);

void desenha_pontos(LISTA_LINHAS *linha);

void desenha_ligacoes(LISTA_LINHAS *linha);

void desenha_comboios(LISTA_GRAF_BOIO *lista_graf_boios);

LISTA_PONTOS * procura_ponto(char *id_linha, char *id_ponto, LISTA_LINHAS *topo_lista_linhas);

LISTA_GRAF_BOIO *cria_grafico_do_comboio(LISTA_GRAF_BOIO *lista_graf_boios, COMBOIO *comboio);

LISTA_GRAF_BOIO * inicializa_boios(LISTA_GRAF_BOIO *boios_graficos, LISTA_COMBOIOS *lista_comboios);

LISTA_GRAF_BOIO * mexe_comboios2(LISTA_GRAF_BOIO *lista_graf_boios);

void mostra_boios_ativos(LISTA_GRAF_BOIO *lista_graf_boios);

Uint32 esvazia_vagao(PONTO pt, Uint32 cor);

LISTA_GRAF_BOIO * remove_graf_boio(LISTA_GRAF_BOIO *lista_graf_boios, LISTA_GRAF_BOIO *eliminar);

LISTA_PONTOS * procura_ponto_por_coords(LISTA_LINHAS *topo_lista_linhas, int x, int y);

int eventos_sdl(SDL_Event *event, LISTA_LINHAS *topo_lista_linhas, LISTA_GRAF_BOIO *topo_lista_graf_boios);

// #endif
