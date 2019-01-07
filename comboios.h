#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

SDL_Window* janela;
SDL_Renderer* pintor;

#define FPS 30 //numero de vezes por segundo que a janela e atualizada

#define TICKS_p_FRAME 1000/FPS
#define SPEED 3 // pixeis/tick

#define RAIO_ESTACAO 10 //raio bola estacao
#define N_CAR 3 // numero de carruagens

#define VIA 1
#define EST 2

//dimJanela[]
#define X 0
#define Y 1
//codigo das cores
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

#define LARGURA_BOTAO 120
#define ALTURA_BOTAO 40
#define ESPACAMENTO 10

//menu criacao comboio
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
  struct elem_lista_linhas *linha;
  int x, y;
  Uint32 cor;
  int tipo;
  int alavanca;
} PONTO //struct do ponto

typedef struct elem_ponto{
  PONTO pt;
  struct elem_ponto *pr[2];
} LISTA_PONTOS; //ferrovia - lista de pontos

typedef struct elem_lista_linhas{
  char id[5];
  LISTA_PONTOS *linha;
  struct elem_lista_linhas *pr;
} LISTA_LINHAS; //lista das ferrovias

typedef struct{
  char id[3];
  int r_bolas;
  int estado_piscar;
  Uint32 cor[N_CAR];
  LISTA_PONTOS *origem;
  int num_servicos;
  int servicos_restantes;
  float x[N_CAR], y[N_CAR];
  LISTA_PONTOS *ultimo_ponto[N_CAR];
  int alavanca[N_CAR];
  float veloc;
} COMBOIO; // struct do comboio com toda a informacao do config e grafica

typedef struct elem_lista_comboios{
  COMBOIO boio;
  struct elem_lista_comboios *pr;
} LISTA_COMBOIOS; //lista dos comboios


/* Ficheiro: leitor_configs.c */

void leitor_configs( LISTA_COMBOIOS **comboios, LISTA_LINHAS **linhas, int dimJanela[], char *nome_ficheiro);

LISTA_COMBOIOS * preenche_comboio(char aux_string[][10], int *aux_int, LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS *topo_lista_linhas);

void preenche_linha(char *aux_string, int *aux_int, LISTA_LINHAS * nova_linha, LISTA_PONTOS **atual);

void liga_pontos(char aux_string[][10], LISTA_LINHAS *topo_lista_linhas);

LISTA_PONTOS * procura_ponto(char *id_linha, char *id_ponto, LISTA_LINHAS *topo_lista_linhas);

int numero_tipo( char string[]);

Uint32 cor_string_para_Uint32(char string[]);


/* Ficheiro: comboios.c */

void opcao_mostra_linha(LISTA_LINHAS *topo_lista_linhas);

LISTA_LINHAS * opcao_elimina_linha(LISTA_LINHAS *topo_lista_linhas, LISTA_COMBOIOS **topo_lista_comboios);

int get_ferrovia_a_mostrar(char* linha, LISTA_LINHAS *topo_lista_linhas);

void opcao_mostra_comboio(LISTA_COMBOIOS *topo_lista_comboios);

LISTA_COMBOIOS * opcao_elimina_comboio(LISTA_COMBOIOS *topo_lista_comboios);

LISTA_COMBOIOS * opcao_novo_comboio(LISTA_COMBOIOS *topo_lista_comboios, LISTA_LINHAS * topo_lista_linhas);

int get_comboio_a_mostrar(LISTA_COMBOIOS * topo_lista_comboios, char*comboio);

int verifica(int n_min, int n_max, char *texto);

char *menu_cor_para_string(int cor);

void get_id_comboio(char * id_comboio, LISTA_COMBOIOS * topo_boios);

void get_coords_origem(char * linha, char *ponto, LISTA_LINHAS * topo_lista_linhas);

void mostra_linha(LISTA_LINHAS* topo_lista_linhas, char * texto, char*linha);

void mostra_ponto( PONTO pt);

void mostra_boio( COMBOIO boio);

char *cor_Uint32_para_string(Uint32 cor);

LISTA_COMBOIOS *procura_comboios_na_linha(LISTA_COMBOIOS *lista_comboios, LISTA_PONTOS *linha);

LISTA_COMBOIOS *elimina_comboio(LISTA_COMBOIOS *lista_comboios, LISTA_COMBOIOS *eliminar);

void remove_ligacoes_para_a_linha_eliminada(LISTA_LINHAS *lista_linhas, LISTA_LINHAS *eliminar);


/* Ficheiro: graf.c */

Uint32 random_cor(void);

void simular(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas, int dimJanela[]);

int inicializa_janela( int dimJanela[]);

void reset_servicos_restantes(LISTA_COMBOIOS *topo_lista_comboios);

LISTA_COMBOIOS * mexe_comboios3(LISTA_COMBOIOS **topo_lista_boios);

void mexe_carruagem(LISTA_COMBOIOS *aux_boio, int num_carruagem, LISTA_PONTOS *pt1, LISTA_PONTOS *pt2);

void verifica_se_chegou_ao_ponto(LISTA_COMBOIOS *aux_boio, int num_carruagem, LISTA_PONTOS *prox_pt);

void colisoes(LISTA_COMBOIOS *lista_boios);

float dist_carruagens(LISTA_COMBOIOS *comboio1, int i, LISTA_COMBOIOS *comboio2, int j);

void pisca_comboios(LISTA_COMBOIOS *lista_boios);

void atualiza_render(LISTA_LINHAS *topo_lista_linhas, LISTA_COMBOIOS *boios_graficos, int dimJanela[], int pausa);

void desenha_pontos(LISTA_LINHAS *linha);

void desenha_ligacoes(LISTA_LINHAS *linha);

void desenha_comboios(LISTA_COMBOIOS *lista_boios);

void desenha_botoes(int dimJanela[], int pausa);

int eventos_sdl(SDL_Event *event, LISTA_LINHAS *topo_lista_linhas, LISTA_COMBOIOS *lista_boios, int dimJanela[]);

int carregou_botao(int dimJanela[], int x, int y);

LISTA_COMBOIOS * procura_locomotiva_por_coords(LISTA_COMBOIOS *lista_boios, int x, int y);

LISTA_PONTOS * procura_ponto_por_coords(LISTA_LINHAS *topo_lista_linhas, int x, int y);

void toggle_andamento_comboio(LISTA_COMBOIOS *boio_a_parar, LISTA_COMBOIOS *boios);

void menu_de_pausa(LISTA_COMBOIOS **topo_lista_comboios, LISTA_LINHAS **topo_lista_linhas);

void reset_movimento(LISTA_COMBOIOS **topo_lista_boios, LISTA_COMBOIOS *aux_boio);

void mostra_boios_ativos(LISTA_COMBOIOS *lista_comboios);
