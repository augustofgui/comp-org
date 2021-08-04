#include "../includes/memoria.h"

#include <stdio.h>
#include <stdlib.h>

void incializa_memoria_vazia(BlocoMemoria * mem, int n){
    int i;
    for( i = 0; i < n; i++ ){
        mem[i] = novo_bloco_nulo();
    }
}

void aloca_valores_aleatorios_memoria(BlocoMemoria * mem, int n){
    int i, j;
    for( i = 0; i < n; i++ ){
        for( j = 0; j < 4; j++ )
            mem[i].palavras[j] = rand() % 100;
        mem[i].end = i;
        mem[i].atualizado = 0;
        mem[i].cache_hit = 0;
        mem[i].uso = 0;
        mem[i].custo = 0;
    }
}

BlocoMemoria novo_bloco_nulo(){
    BlocoMemoria nulo = {{ 0, 0, 0, 0}, -1, 0, 0, 0, 0};
    return nulo;
}

void printa_memoria( BlocoMemoria * mem, int tam, char * nome){
    int i;
    printf(TAB_TL TAB_HOR" %s ", (nome != NULL) ? nome : "Memoria");
    for( i = 0; i < 113; i++ )
        printf(TAB_HOR);
    printf(TAB_TR "\n");
    
    for( i = 0; i < tam; i++ ){
        if(!i)
            printf(TAB_VER);
        if(i%4 == 0 && i)
            printf("\n" TAB_VER);

        printf("%4d: %5d %5d %5d %5d " TAB_VER, mem[i].end, mem[i].palavras[0], mem[i].palavras[1], mem[i].palavras[2], mem[i].palavras[3]);
    }

    printf("\n" TAB_BL);
    for( i = 0; i < 123; i++ )
        printf(TAB_HOR);
    printf(TAB_BR "\n\n");

}