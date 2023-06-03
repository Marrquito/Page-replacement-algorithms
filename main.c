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

Memoria *MemInit(int qtdRefMemoria);

int LeArquivo();

void Fifo();
void Otm();
void Lru();

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
int LeArquivo()
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

void Fifo()
{
    int *quadros    = NULL;
    int  idx        = 0;
    int  qtdFaltas  = 0;
    int  flag       = 0;

    quadros = (int *) calloc(memoria->qtdQuadros, sizeof(int) * memoria->qtdQuadros);
    assert(quadros);

    for(int i = 0; i < memoria->qtdMemRef; i++)
    {
        flag = 0;
        
        for(int j = 0; j < memoria->qtdQuadros; j++)
        {
            if(memoria->memRef[i] == quadros[j])
            {
                flag = 1;
                break;
            }
        }

        if(!flag)
        {
            quadros[idx] = memoria->memRef[i];
            idx          = (idx + 1) % memoria->qtdQuadros;
            qtdFaltas++;
        }
    }

    printf("FIFO %d\n", qtdFaltas);
    free(quadros);
}

void Otm()
{
    int *quadros    = NULL;
    int *proxRef    = NULL;
    int  idx        = 0;
    int  qtdFaltas  = 0;
    int  flag       = 0;
    int  maior      = 0;

    quadros = (int *) calloc(memoria->qtdQuadros, sizeof(int) * memoria->qtdQuadros);
    assert(quadros);

    proxRef = (int *) calloc(memoria->qtdQuadros, sizeof(int) * memoria->qtdQuadros);
    assert(proxRef);

    for(int i = 0; i < memoria->qtdMemRef; i++)
    {
        flag = 0;
        
        for(int j = 0; j < memoria->qtdQuadros; j++)
        {
            if(memoria->memRef[i] == quadros[j])
            {
                flag = 1;
                break;
            }
        }

        if(!flag)
        {
            if(qtdFaltas < memoria->qtdQuadros -1)
            {
                quadros[qtdFaltas] = memoria->memRef[i];
                qtdFaltas++;
                continue;
            }

            for(int j = 0; j < memoria->qtdQuadros; j++)
            {
                proxRef[j] = 0;
                
                for(int k = i + 1; k < memoria->qtdMemRef; k++)
                {
                    if(memoria->memRef[k] == quadros[j])
                    {
                        proxRef[j] = k;
                        break;
                    }
                }
            }

            maior = 0;
            idx   = 0;

            for(int j = 0; j < memoria->qtdQuadros; j++)
            {
                if(proxRef[j] == 0)
                {
                    idx = j;
                    break;
                }

                if(proxRef[j] > maior)
                {
                    maior = proxRef[j];
                    idx   = j;
                }
            }

            quadros[idx] = memoria->memRef[i];
            qtdFaltas++;
        }
    }

    printf("OTM %d\n", qtdFaltas);

    free(quadros);
    free(proxRef);
}

void Lru()
{
    int *quadros    = NULL;
    int *proxRef    = NULL;
    int  idx        = 0;
    int  qtdFaltas  = 0;
    int  flag       = 0;
    int  menor      = 0;

    quadros = (int *) calloc(memoria->qtdQuadros, sizeof(int) * memoria->qtdQuadros);
    assert(quadros);

    proxRef = (int *) calloc(memoria->qtdQuadros, sizeof(int) * memoria->qtdQuadros);
    assert(proxRef);

    for(int i = 0; i < memoria->qtdMemRef; i++)
    {
        flag = 0;
        
        for(int j = 0; j < memoria->qtdQuadros; j++)
        {
            if(memoria->memRef[i] == quadros[j])
            {
                flag = 1;
                break;
            }
        }

        if(!flag)
        {
            if(qtdFaltas < memoria->qtdQuadros - 1)
            {
                quadros[qtdFaltas] = memoria->memRef[i];
                qtdFaltas++;
                continue;
            }

            for(int j = 0; j < memoria->qtdQuadros; j++)
            {
                proxRef[j] = 0;
                
                for(int k = i - 1; k >= 0; k--)
                {
                    if(memoria->memRef[k] == quadros[j])
                    {
                        proxRef[j] = k;
                        break;
                    }
                }
            }

            menor = 0;
            idx   = 0;

            for(int j = 0; j < memoria->qtdQuadros; j++)
            {
                if(proxRef[j] == 0)
                {
                    idx = j;
                    break;
                }

                if(proxRef[j] < menor)
                {
                    menor = proxRef[j];
                    idx   = j;
                }
            }

            quadros[idx] = memoria->memRef[i];
            qtdFaltas++;
        }
    }

    printf("LRU %d\n", qtdFaltas);

    free(quadros);
    free(proxRef);
}

int main()
{
    int rc;

    rc = LeArquivo();
    
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

    Fifo();
    Otm();
    Lru();

    free(memoria->memRef);
    free(memoria);

    return 0;
}