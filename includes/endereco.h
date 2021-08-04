#ifndef endereco_h
#define endereco_h

typedef struct endereco {
    int bloco, palavra;
} Endereco;

Endereco novo_endereco_nulo();

Endereco novo_endereco_aleatorio( int tam );

#endif