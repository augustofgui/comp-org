#ifndef mmu_h
#define mmu_h

#include "../includes/memoria.h"

BlocoMemoria busca_nas_memoria( int end_buscado, BlocoMemoria * ram, Caches caches );

int procura_na_cache( BlocoMemoria * local_busca, int tam, BlocoMemoria * achado );

int encontrar_bloco_menos_usado( BlocoMemoria * busca, int tam_bloco );

void reinicia_contadores_blocos( BlocoMemoria * bloco );

void copia_palavras( BlocoMemoria * origem, BlocoMemoria * destino );

void atualiza_caches( BlocoMemoria * bloco, BlocoMemoria * ram, Caches caches);

void troca_bloco_de_memoria( BlocoMemoria * bloco_trocado, BlocoMemoria * origem, BlocoMemoria * destino, int tam_origem, int tam_destino );

void atualiza_ram( BlocoMemoria * ram, Caches caches );

#endif