#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct mem {
    int qtdQuadros;
    int qtdMemRef;
    int *memRef;
}Memoria;

Memoria *memoria  = NULL;

int leArquivo();
Memoria *MemInit(int qtdRefMemoria);


Memoria *MemInit(int qtdRefMemoria)
{
    memoria = (Memoria *) malloc(sizeof(Memoria));
    assert(memoria);
    
    memoria->qtdMemRef  = qtdRefMemoria;
    memoria->memRef     = (int *) calloc(qtdRefMemoria, sizeof(int) * qtdRefMemoria);
    assert(memoria->memRef);

    return memoria;
}

// QTD QUADROS
// SEQ REF MEMORIA
int leArquivo()
{
    FILE *ARQ    = NULL;
    char *result = NULL;
    char aux[20] = {'\0'};
    int  linhas  = 0;
    int  i       = 0;

    ARQ = fopen("paginas.txt", "r");
    
    if (ARQ == NULL)
    {
        printf("Problemas na abertura do arquivo");
        return -2;
    }
    
    fseek(ARQ, 0, SEEK_END);
    linhas = ftell(ARQ);
    rewind(ARQ);

    if(!linhas) return -1;

    linhas  = ceil(linhas/2.0);
    memoria = MemInit(linhas-1);
    
    fscanf(ARQ, "%d", &memoria->qtdQuadros);

    while (!feof(ARQ))
    {
        result = fgets(aux, 20, ARQ);

        if(result && aux[0] != '\n') memoria->memRef[i++] = atoi(aux);         
    }

    fclose(ARQ);

    return 0;
}


int main()
{
    int rc;

    rc = leArquivo();
    
    switch(rc)
    {
        case 0:
            break;
        case -1:
            puts("Arquivo vazio!");
            return -1;
        case -2:
            puts("Erro na abertura do arquivo!");
            return -2;
    }


    return 0;
}