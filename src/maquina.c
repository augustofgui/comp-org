#include "../includes/maquina.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

Instrucao nova_instrucao(int op, Endereco mem1, Endereco mem2, Endereco mem3){
    Instrucao temp = { op, mem1, mem2, mem3 };
    return temp;
}

Instrucao gera_instrucao_aleatoria(){
    int rand_op;
    Instrucao aleatoria;
    rand_op = rand() % 2;
    aleatoria = nova_instrucao(
        rand_op,
        novo_endereco_aleatorio(TAM_RAM),
        novo_endereco_aleatorio(TAM_RAM),
        novo_endereco_aleatorio(TAM_RAM));
    return aleatoria;
}

Instrucao nova_instrucao_nula(){
    Instrucao nula = {
        nulo,
        novo_endereco_nulo(),
        novo_endereco_nulo(),
        novo_endereco_nulo()};

    return nula;
}

void aloca_instrucoes_aleatorias_repetitivas(Instrucao * V, int tam){
    int i;
    FILE * out_file = fopen("instrucoes", "w");
    char inst_string[45] = "%2d  %4d %1d  %4d %1d  %4d %1d\n";

    Instrucao banco_instrucoes_aleatorias[20];
    aloca_instrucoes_aleatorias( banco_instrucoes_aleatorias, 20 );

    for( i = 0; i < tam - 1; i++ ){
        //V[i] = gera_instrucao_aleatoria();
        V[i] = banco_instrucoes_aleatorias[rand()%20];
        fprintf(out_file, inst_string,
            V[i].op,
            V[i].end_um.bloco, V[i].end_um.palavra,
            V[i].end_dois.bloco, V[i].end_dois.palavra,
            V[i].end_tres.bloco, V[i].end_tres.palavra);
    }

    V[i] = nova_instrucao_nula();
    fprintf(out_file, "%d  0 0  0 0  0 0\n",
            V[i].op);

    fclose(out_file);
}

void aloca_instrucoes_aleatorias(Instrucao * V, int tam){
    int i;

    for( i = 0; i < tam; i++ ){
        V[i] = gera_instrucao_aleatoria();
    }
}   

void le_instrucao( Instrucao * out, FILE * arq ){
    fscanf(arq, "%d %d %d %d %d %d %d",
            &out->op,
            &out->end_um.bloco, &out->end_um.palavra,
            &out->end_dois.bloco, &out->end_dois.palavra,
            &out->end_tres.bloco, &out->end_tres.palavra);
}

void maquina(Instrucao * V, BlocoMemoria * ram, Caches caches, Relatorio * info){
    int op;
    Instrucao atual = nova_instrucao_nula();

    FILE * instrucoes = fopen("90.txt", "r");
    if(instrucoes == NULL){
        printf("zuou");
        exit(1);
    }

    le_instrucao( &atual, instrucoes );
    op = atual.op;
    while( op != nulo ){
        executa_instrucao( &atual, ram, caches, info);
        le_instrucao( &atual, instrucoes );
        op = atual.op;
    }

    fclose( instrucoes );
    atualiza_ram( ram, caches );
}

void executa_instrucao(Instrucao * I, BlocoMemoria * ram, Caches caches, Relatorio * info){
    BlocoMemoria numero_um = novo_bloco_nulo();
    BlocoMemoria numero_dois = novo_bloco_nulo();
    BlocoMemoria resultado = novo_bloco_nulo();

    numero_um = busca_nas_memoria( I->end_um.bloco, ram, caches );
    numero_dois = busca_nas_memoria( I->end_dois.bloco, ram, caches );
    resultado = busca_nas_memoria( I->end_tres.bloco, ram, caches );

    resultado.atualizado = 1;

    info->hit_total += ( numero_um.cache_hit != 4 );
    info->hit_total += ( numero_dois.cache_hit != 4 );
    info->hit_total += ( resultado.cache_hit != 4 );
    info->miss_total += ( numero_um.cache_hit + numero_dois.cache_hit + resultado.cache_hit ) - 3;
    info->custo +=  numero_um.custo +  numero_dois.custo +  resultado.custo;

    atualiza_relatorio( numero_um.cache_hit, info );
    atualiza_relatorio( numero_dois.cache_hit, info );
    atualiza_relatorio( resultado.cache_hit, info );

    //printf("Total de CH: %d\n", numero_um.cache_hit + numero_dois.cache_hit + resultado.cache_hit);
    //printf("N1:%d N2:%d R:%d\n", numero_um.cache_hit, numero_dois.cache_hit, resultado.cache_hit);

    printf("Total de Custo: %lld\n", info->custo);
    printf("Cache 1 hit: %lld miss: %lld\n", info->hit_cache[0], info->miss_cache[0]);
	printf("Cache 2 hit: %lld miss: %lld\n", info->hit_cache[1], info->miss_cache[1]);
    printf("Cache 3 hit: %lld miss: %lld\n", info->hit_cache[2], info->miss_cache[2]);

    atualiza_caches( &numero_um, ram, caches );
    atualiza_caches( &numero_dois, ram, caches );
    atualiza_caches( &resultado, ram, caches );

    switch(I->op){
        case soma:{

            resultado.palavras[I->end_tres.palavra] = nova_soma(
                numero_um.palavras[I->end_um.palavra],
                numero_dois.palavras[I->end_dois.palavra]
            );

            break;
        }
        case sub:{

            resultado.palavras[I->end_tres.palavra] = nova_sub(
                numero_um.palavras[I->end_um.palavra],
                numero_dois.palavras[I->end_dois.palavra]
            );

            break;
        }
    }

    printf("\n");
}

void atualiza_relatorio( int n, Relatorio * info ){
    switch(n){
        case 1:
            info->hit_cache[0]++;
        break;

        case 2:
            info->miss_cache[0]++;
            info->hit_cache[1]++;
        break;

        case 3:
            info->miss_cache[0]++;
            info->miss_cache[1]++;
            info->hit_cache[2]++;
        break;

        case 4:
            info->miss_cache[0]++;
            info->miss_cache[1]++;
            info->miss_cache[2]++;
            info->hit_cache[3]++;
        break;
    }

}

int nova_soma( int x, int y ){
    printf("Soma: %d + %d = %d\n", x, y, x+y);
    return x + y;
}

int nova_sub( int x, int y ){
    printf("Sub: %d - %d = %d\n", x, y, x-y);
    return x - y;
}

void printa_relatorio( Relatorio info ){
    int i;

    printf(TAB_TL" Relatorio ");
    for( i = 0; i < 50; i++ )
        printf(TAB_HOR);
    printf(TAB_TR"\n");

    for( i = 0; i < 3; i++ ){

    }

    

    printf(TAB_VER "  Tamanho da Cache 1: %-4d", TAM_CACHE_L1);
    printf("  \t Porcentagem de acerto: %5.2f", ((double)info.hit_cache[0] / (double)( TAM_INSTRUCOES * 3 )) * (double)100.0);
    printf(" "TAB_VER "\n");

    printf(TAB_VER "  Tamanho da Cache 2: %-4d", TAM_CACHE_L2);
    printf("  \t Porcentagem de acerto: %5.2f", ((double)info.hit_cache[1] / (double)( TAM_INSTRUCOES * 3 )) * (double)100.0);
    printf(" "TAB_VER "\n");

    printf(TAB_VER "  Tamanho da Cache 3: %-4d", TAM_CACHE_L3);
    printf("\t Porcentagem de acerto: %5.2f", ((double)info.hit_cache[2] / (double)( TAM_INSTRUCOES * 3 )) * (double)100.0);
    printf(" "TAB_VER "\n");

    printf(TAB_VER "  Tamanho da ram: %-4d", TAM_RAM);
    printf("\t\t Porcentagem de acerto: %5.2f", ((double)info.hit_cache[3] / (double)( TAM_INSTRUCOES * 3 )) * (double)100.0);
    printf(" "TAB_VER "\n");   

    printf(TAB_VER "  Custo total: %-32lld", info.custo);
    printf("\t      "TAB_VER "\n");

    printf(TAB_VER "  Cache hits: %-8d", info.hit_total);
    printf("\t\t Cache misses: %-8d", info.miss_total);
    printf("\t      " TAB_VER "\n");

    printf(TAB_BL);
    for( i = 0; i < 61; i++ )
        printf(TAB_HOR);
    printf(TAB_BR"\n");
}



