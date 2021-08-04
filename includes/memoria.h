# ifndef memoria_h
# define memoria_h

#define TAM_RAM 8192
#define TAM_CACHE_L1 32
#define TAM_CACHE_L2 64
#define TAM_CACHE_L3 128

// Caracteres para tabelas
#define TAB_HOR "\u2501" // ━ (horizontal)
#define TAB_VER "\u2503" // ┃ (vertical)
#define TAB_TL "\u250F"  // ┏ (top-left)
#define TAB_BL "\u2517"  // ┗ (bottom-left)
#define TAB_TR "\u2513"  // ┓ (top-right)
#define TAB_BR "\u251B"  // ┛ (bottom-right)

typedef struct blocoMemoria {
    int palavras[4];
    int end;
    int atualizado;
    int custo;
    int cache_hit;
    int uso;
} BlocoMemoria;

typedef struct junta_caches
{
    BlocoMemoria * um;
    BlocoMemoria * dois;
    BlocoMemoria * tres;
} Caches;

void incializa_memoria_vazia(BlocoMemoria * mem, int n);

void aloca_valores_aleatorios_memoria(BlocoMemoria * mem, int n);

BlocoMemoria novo_bloco_nulo();

void printa_memoria( BlocoMemoria * mem, int tam, char * nome);

# endif