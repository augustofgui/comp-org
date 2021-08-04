#include "../includes/endereco.h"

#include <stdio.h>
#include <stdlib.h>

Endereco novo_endereco_nulo(){
    Endereco nulo = { -1, -1 };
    return nulo;
}

Endereco novo_endereco_aleatorio( int tam ){
    Endereco aleatorio;
    aleatorio.bloco = rand() % tam;
    aleatorio.palavra = rand() % 4;
    return aleatorio;
}