/*
    TP 02 - BCC266
    Programa feito por 
    Augusto Ferreira Guilarducci
    Rafael . .
*/

#include "../includes/maquina.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    Instrucao instrucoes[TAM_INSTRUCOES];
    BlocoMemoria memoria_ram[TAM_RAM];
    BlocoMemoria cache1[TAM_CACHE_L1];
    BlocoMemoria cache2[TAM_CACHE_L2];
    BlocoMemoria cache3[TAM_CACHE_L3];

    Caches caches = { cache1, cache2, cache3 };
    Relatorio info = { 0, 0, 0, { 0, 0, 0, 0}, { 0, 0, 0 }};
    
    srand(time(NULL));

    aloca_valores_aleatorios_memoria(memoria_ram, TAM_RAM);
    incializa_memoria_vazia(caches.um, TAM_CACHE_L1);
    incializa_memoria_vazia(caches.dois, TAM_CACHE_L2);
    incializa_memoria_vazia(caches.tres, TAM_CACHE_L3);
    aloca_instrucoes_aleatorias_repetitivas(instrucoes, TAM_INSTRUCOES);
   
    maquina(instrucoes, memoria_ram, caches, &info);

    printa_memoria(caches.um, TAM_CACHE_L1, "CACHE 1");
    printa_memoria(caches.dois, TAM_CACHE_L2, "CACHE 2");
    printa_memoria(caches.tres, TAM_CACHE_L3, "CACHE 3");
    //printa_memoria(memoria_ram, TAM_RAM, "RAM 1  ");

    printa_relatorio(info);

    printf("\n");
    return 0;
}