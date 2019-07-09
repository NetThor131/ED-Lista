#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef struct _no_cdup
{
    char nome[51];
    struct _no_cdup *prox;
    struct _no_cdup *ant;
} No;

typedef struct _lista_cdup
{
    No *cbc;
    No *fim;
} Lista;

No *CriaNo(char nome[51])
{
    No *p = (No *)malloc(sizeof(No));
    strcpy(p->nome, nome);
    p->prox = p->ant = p;
    return p;
}

Lista *CriaLista()
{
    Lista *L = (Lista *)malloc(sizeof(Lista));
    L->cbc = L->fim = NULL;
    return L;
}

int32_t ListaVazia(Lista *L)
{
    return (L->cbc == NULL);
}

void Remove(Lista *L, char nome[51], FILE **output)
{
    if (!ListaVazia(L))
    {
        No *p = L->cbc;
        if (strcmp(L->cbc->nome, nome) == 0)
        {
            if (L->cbc == L->fim)
            {
                L->cbc = L->fim = NULL;
                fprintf(*output, "[ OK  ] REMOVE %s\n", nome);
            }
            else
            {
                L->cbc = p->prox;
                L->cbc->ant = L->fim;
                L->fim->prox = L->cbc;
                fprintf(*output, "[ OK  ] REMOVE %s\n", nome);
            }
        }
        else
        {
            p = p->prox;
            while (p != L->cbc)
            {
                if (strcmp(p->nome, nome) == 0)
                {
                    p->ant->prox = p->prox;
                    p->prox->ant = p->ant;
                    if (L->fim == p)
                    {
                        L->fim = p->ant;
                    }
                    fprintf(*output, "[ OK  ] REMOVE %s\n", nome);
                    return;
                }
                else
                {
                    p = p->prox;
                }
            }
        }
        fprintf(*output, "[ERROR] REMOVE %s\n", nome);
    }
    else
    {
        fprintf(*output, "[ERROR] REMOVE %s\n", nome);
    }
}

int32_t add(Lista *L, char nome[51], FILE **output)
{
    No *h = CriaNo(nome);
    if (L->cbc == NULL)
    {
        L->cbc = h;
        L->fim = h;
        fprintf(*output, "[ OK  ] ADD %s\n", h->nome);
        return 1;
    }
    else
    {
        No *p = L->cbc;
        for (;;)
        {
            if (strcmp(p->nome, nome) == 0)
            {
                fprintf(*output, "[ERROR] ADD %s\n", h->nome);
                return 0;
            }
            else
            {
                p = p->prox;
                if (p == L->cbc)
                {
                    L->fim->prox = h;
                    h->ant = L->fim;
                    L->fim = h;
                    L->cbc->ant = L->fim;
                    L->fim->prox = L->cbc;
                    fprintf(*output, "[ OK  ] ADD %s\n", h->nome);
                    return 1;
                }
            }
        }
    }
}

int32_t main(int32_t argc, char *argv[])
{
    Lista *L;
    L = CriaLista();
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    char funcao[50], nome[50];

    while (fscanf(input, "%50s %50[^\n]s", funcao, nome) != EOF)
    {
        if (strcmp(funcao, "ADD") == 0)
        {
            add(L, nome, &output);
        }
        if (strcmp(funcao, "SHOW") == 0)
        {
            if (!ListaVazia(L))
            {
                No *p = L->cbc;
                while (1)
                {
                    if (strcmp(p->nome, nome) == 0)
                    {
                        fprintf(output, "[ OK  ] %s<-%s->%s\n", p->ant->nome, p->nome, p->prox->nome);
                        break;
                    }
                    else
                    {
                        p = p->prox;
                        if (p == L->cbc)
                        {
                            fprintf(output, "[ERROR] ?<-%s->?\n", nome);
                            break;
                        }
                    }
                }
            }
            else
            {
                fprintf(output, "[ERROR] ?<-%s->?\n", nome);
            }
        }
        if (strcmp(funcao, "REMOVE") == 0)
        {
            Remove(L, nome, &output);
        }
    }

    fclose(input);
    fclose(output);

    system("pause");

    return 0;
}