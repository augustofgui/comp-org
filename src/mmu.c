#include "../includes/mmu.h"

#include <stdio.h>

BlocoMemoria busca_nas_memoria( int end_buscado, BlocoMemoria * ram, Caches caches ){
    BlocoMemoria achado = novo_bloco_nulo();
    achado.end = end_buscado;

    if(procura_na_cache( caches.um, TAM_CACHE_L1, &achado ))
        return achado;

    else if(procura_na_cache( caches.dois, TAM_CACHE_L2, &achado ))
        return achado;

    else if(procura_na_cache( caches.tres, TAM_CACHE_L3, &achado ))
        return achado;
    
    achado.end = ram[end_buscado].end;
    copia_palavras( &ram[end_buscado], &achado );
    achado.cache_hit++;
    achado.custo *= 100;
    achado.uso = 1;
    return achado;
}

int procura_na_cache( BlocoMemoria * local_busca, int tam, BlocoMemoria * achado ){
    achado->cache_hit++;
    achado->custo = (achado->custo * 10) + 10;

    int i;
    for ( i = 0; i < tam; i++ ){
        if( local_busca[i].end == achado->end){
            copia_palavras( &local_busca[i], achado );
            achado->uso = ++local_busca[i].uso;
            achado->atualizado = local_busca[i].atualizado;
            return 1;
        }
    }

    return 0;
}

int encontrar_bloco_menos_usado( BlocoMemoria * busca, int tam_bloco ){
    int i, j = 0;
    int menos_usado = busca[j].uso;

    for ( i = 0; i < tam_bloco; i++ ){
        if(busca[i].end == -1)
            return i;

        if( menos_usado < busca[i].uso ){
            menos_usado = busca[i].uso;
            j = i;
        }
    }

    return j;
}

void reinicia_contadores_blocos( BlocoMemoria * bloco ){
    bloco->atualizado = 0;
    bloco->cache_hit = 0;
    bloco->custo = 0;
    bloco->uso = 0;
}

void copia_palavras( BlocoMemoria * origem, BlocoMemoria * destino ){
    destino->palavras[0] = origem->palavras[0];
    destino->palavras[1] = origem->palavras[1];
    destino->palavras[2] = origem->palavras[2];
    destino->palavras[3] = origem->palavras[3];
}

void atualiza_caches( BlocoMemoria * bloco, BlocoMemoria * ram, Caches caches){
    switch (bloco->cache_hit)
    {
        case 4: 
            troca_bloco_de_memoria( bloco, ram, caches.tres, TAM_RAM, TAM_CACHE_L3);
        case 3: 
            troca_bloco_de_memoria( bloco, caches.tres, caches.dois, TAM_CACHE_L3, TAM_CACHE_L2);
        case 2: 
            troca_bloco_de_memoria( bloco, caches.dois, caches.um, TAM_CACHE_L2, TAM_CACHE_L1);
            break;
            
        case 1: break;
        default: break;
    }
}

void troca_bloco_de_memoria( BlocoMemoria * bloco_trocado, BlocoMemoria * origem, BlocoMemoria * destino, int tam_origem, int tam_destino ){
    BlocoMemoria aux;
    
    int i;
    for ( i = 0; i < tam_origem; i++ )
        if(origem[i].end == bloco_trocado->end)
            break;
    
    int menos_usado = encontrar_bloco_menos_usado( destino, tam_destino );
    
    aux = destino[menos_usado];
    destino[menos_usado] = origem[i];

    if( bloco_trocado->cache_hit != 4 )
        origem[i] = aux;
    else if( aux.atualizado ){
        copia_palavras( &aux, &origem[i] );
    }

    bloco_trocado->cache_hit = bloco_trocado->cache_hit - 1;
}

void atualiza_ram( BlocoMemoria * ram, Caches caches){
    for (int i = 0; i < TAM_CACHE_L1; i++)
    {
        if ( caches.um[i].atualizado ){
            copia_palavras( &caches.um[i], &ram[caches.um[i].end] );
            reinicia_contadores_blocos( &caches.um[i] );
        }
    }

    for (int i = 0; i < TAM_CACHE_L2; i++)
    {
        if ( caches.dois[i].atualizado ){
            copia_palavras( &caches.dois[i], &ram[caches.dois[i].end] );
            reinicia_contadores_blocos( &caches.dois[i] );
        }
    }

    for (int i = 0; i < TAM_CACHE_L3; i++)
    {
        if ( caches.tres[i].atualizado ){
            copia_palavras( &caches.tres[i], &ram[caches.tres[i].end] );
            reinicia_contadores_blocos( &caches.tres[i] );
        }
    }
}
