# ifndef maquina_h
# define maquina_h

#include "../includes/endereco.h"
#include "../includes/mmu.h"

#define TAM_INSTRUCOES 5000

typedef struct instrucao {
    int op;
    Endereco end_um;
    Endereco end_dois;
    Endereco end_tres;
} Instrucao;

typedef struct relatorio {
    int hit_total, miss_total;
    long long int custo;
    long long int hit_cache[4], miss_cache[3];
} Relatorio;

enum op_codes  { nulo = -1, soma, sub };

Instrucao nova_instrucao( int op, Endereco mem1, Endereco mem2, Endereco mem3 );
Instrucao nova_instrucao_nula();
Instrucao gera_instrucao_aleatoria();
void aloca_instrucoes_aleatorias_repetitivas( Instrucao * V, int tam );
void aloca_instrucoes_aleatorias( Instrucao * V, int tam );
void maquina( Instrucao * V, BlocoMemoria * ram, Caches caches, Relatorio * info );
void executa_instrucao( Instrucao * I, BlocoMemoria * ram, Caches caches, Relatorio * info );
int nova_soma( int x, int y );
int nova_sub( int x, int y );
void atualiza_relatorio( int n, Relatorio * info );
void printa_relatorio( Relatorio info );

# endif