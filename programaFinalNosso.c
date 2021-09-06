#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tree
{
    int numero;
    int altura;
    struct Tree *esquerda, *direita;
} Tree;

typedef struct lstItem
{
    int numero;
    struct lstItem *proximo;
} lista;

/*
Método responsável pode definir as alturas dos nós
Faz a análise de qual dos lados é maior
Retorna a Altura do Nó
*/
int definirAltura(Tree *noh)
{
    if (calcularAlturaNo(noh->esquerda) > calcularAlturaNo(noh->direita))
    {
        noh->altura = calcularAlturaNo(noh->esquerda) + 1;
    }
    else
    {
        noh->altura = calcularAlturaNo(noh->direita) + 1;
    }
    return noh->altura;
}
/*
Método responsável por calcular as alturas dos nós
Faz-se necessário caso eles estejam nulos, que sua alture seja 0
Retorna a altura do Nó
*/
int calcularAlturaNo(Tree *noh)
{
    if (noh == NULL)
    {
        return 0;
    }
    return noh->altura;
}
/*
Método responsável por verificar o desbalanceamento
Caso ele seja negativo (-2), a árvore se encontra desbalanceada para a esquerda
Caso ele seja positivo (+2), a árvore se encontra desbalanceada para a direita
Retorna 0 caso a árvore estive nula, ou seja, não há desbalanceamento
Retorna diferença das Alturas
*/
int verificarDesbalanceamento(Tree *arvore)
{
    if (arvore == NULL)
    {
        return 0;
    }
    Tree *arvoreApoio = arvore;
    int diferencaAlturas = calcularAlturaNo(arvoreApoio->direita) - calcularAlturaNo(arvoreApoio->esquerda);
    return diferencaAlturas;
}
/*
Método responsável por setar a altura para o Balanceamento 
*/
void setAlturaBalanceamento(Tree *nohA, Tree *nohC, Tree *arvore)
{
    nohC->altura = definirAltura(nohC);
    nohA->altura = definirAltura(nohA);
    arvore->altura = definirAltura(arvore);
}

/*
Seguem os Métodos responsáveis por fazer o balanceamento, conforme demonstrado nos slides de árvore
Aonde o Nó A é o menor valor, o Nó B é o valor médio e o Nó C é o maior valor
Caso o desbalanceamento seja para a direita (+2), o desbalanceamento ocorrerá no Nó A
Caso o desbalanceamento seja para a direita (-2), o desbalanceamento ocorrerá no Nó C
*/
Tree *balancearDireita(Tree *arvore)
{

    Tree *nohA, *nohB, *nohC;
    Tree *subArvore0;
    nohA = arvore;
    nohB = arvore->direita;
    nohC = (arvore->direita)->direita;

    if (nohC == NULL)
    {
        Tree *subArvore1;

        nohC = arvore->direita;
        nohB = (arvore->direita)->esquerda;
        subArvore0 = nohB->esquerda;
        subArvore1 = nohB->direita;

        rotacaoPadraoBalanceamento(arvore, nohA, nohB, nohC, subArvore0, subArvore1);

        setAlturaBalanceamento(nohA, nohC, arvore);

        return arvore;
    }

    subArvore0 = nohB->esquerda;
    arvore = nohB;
    nohB->esquerda = nohA;
    nohA->direita = subArvore0;

    setAlturaBalanceamento(nohA, nohC, arvore);

    return arvore;
}

Tree *balancearEsquerda(Tree *arvore)
{
    Tree *nohA, *nohB, *nohC;
    Tree *subArvore0;

    nohA = arvore->esquerda->esquerda;
    nohB = arvore->esquerda;
    nohC = arvore;
    if (nohA == NULL)
    {
        Tree *subArvore1;
        nohA = arvore->esquerda;
        nohB = (arvore->esquerda)->direita;

        subArvore0 = nohB->direita;
        subArvore1 = nohB->esquerda;
        rotacaoPadraoBalanceamento(arvore, nohA, nohB, nohC, subArvore0, subArvore1);
        setAlturaBalanceamento(nohA, nohC, arvore);

        return arvore;
    }

    subArvore0 = nohB->direita;
    arvore = nohB;
    nohB->direita = nohC;
    nohC->esquerda = subArvore0;

    setAlturaBalanceamento(nohA, nohC, arvore);

    return arvore;
}
/*
Método responsável por fazer rotações padrões em ambos os tipos de balanceamento
*/
void rotacaoPadraoBalanceamento(Tree *arvore, Tree *nohA, Tree *nohB, Tree *nohC, Tree *subArvore0, Tree *subArvore1)
{
    arvore = nohB;
    nohB->direita = nohC;
    nohB->esquerda = nohA;
    nohC->esquerda = subArvore0;
    nohA->direita = subArvore1;
}
/*
Método responsável por instanciar uma árvore AVL, ou seja, fazendo as verificações de balanceamento
A partir de uma lista encadeada
*/
Tree *toAVL(Tree *arvore, int dado)
{
    if (arvore != NULL)
    {
        Tree *arvoreApoio = arvore;
        if (dado < arvore->numero)
        {
            arvore->esquerda = toAVL(arvoreApoio->esquerda, dado);
            arvoreApoio->altura = definirAltura(arvoreApoio);
            if (verificarDesbalanceamento(arvoreApoio) == -2)
            {
                arvore = balancearEsquerda(arvoreApoio);
            }
        }
        else
        {
            arvore->direita = toAVL(arvoreApoio->direita, dado);
            arvoreApoio->altura = definirAltura(arvoreApoio);
            if (verificarDesbalanceamento(arvoreApoio) == 2)
            {
                arvore = balancearDireita(arvoreApoio);
            }
        }
    }
    else
    {
        arvore = malloc(sizeof(Tree));
        arvore->numero = dado;
        arvore->esquerda = NULL;
        arvore->direita = NULL;
        arvore->altura = 1;
    }
    return arvore;
}
/*
Método responsável por printar a árvore AVL instanciada
*/
void mostrarArvore(Tree *arvore, char lado)
{
    if (arvore != NULL)
    {
        printf("> %d (%c) (%d)\n", arvore->numero, lado, arvore->altura - 1);
        mostrarArvore(arvore->esquerda, 'E');
        mostrarArvore(arvore->direita, 'D');
    }
}
/*
Método responsável por printar a lista encadeada
*/
void imprimir(lista *l)
{
    lista *listaImpressao = l;
    printf("Lista: [");
    while (listaImpressao != NULL)
    {
        printf("%d", listaImpressao->numero);
        if (listaImpressao->proximo == NULL)
        {
            printf("]");
        }
        else
        {
            printf(",");
        }
        listaImpressao = listaImpressao->proximo;
    }
}
/*
Método responsável por fazer a inserção no início da lista
*/
lista *inserirInicio(lista *l, int input)
{
    lista *list = malloc(sizeof(lista));
    list->numero = input;
    list->proximo = l;
    l = list;
}
/*
Método responsável por fazer a inserção no final da lista
*/
lista *inserirFinal(lista *l, int input)
{
    lista *list = malloc(sizeof(lista));
    list->numero = input;
    list->proximo = NULL;
    if (l != NULL)
    {
        lista *ultimo = l;
        while (ultimo->proximo != NULL)
        {
            ultimo = ultimo->proximo;
        }
        ultimo->proximo = list;
    }
    else
    {
        l = list;
    }
    return l;
}
/*
Método responsável por fazer a inserção após algum valor da lista
*/
lista *inserirApos(lista *l, int input, int numeroApos)
{
    lista *list = l;
    lista *novaLista = malloc(sizeof(lista));
    novaLista->numero = input;
    if (list->numero == numeroApos)
    {
        novaLista->proximo = list->proximo;
        list->proximo = novaLista;
    }
    else if (list->proximo == NULL)
    {
        list->proximo = novaLista;
        novaLista->proximo = NULL;
    }
    else
    {
        while (list->numero != numeroApos)
        {
            list = list->proximo;
            if (list == NULL)
            {
                printf("Não foi encontrado essa posicao!\n");
                return l;
            }
        }
        novaLista->proximo = list->proximo;
        list->proximo = novaLista;
    }

    return l;
}

int main(void)
{

    lista *list = NULL;
    int auxiliar = 100;
    int comando = malloc(sizeof(int));
    while (comando != 3)
    {

        printf("Digite o Comando:\n");
        printf("1 -> Imprimir lista\n");
        printf("2 -> Entrar no modo de Inserção\n");
        printf("3 -> Tornar lista em Árvore\n");
        scanf("%d", &comando);
        switch (comando)
        {
        case 1:
        {
            if (list != NULL)
            {
                printf("\n\n");
                imprimir(list);
                printf("\n\n");
            }
            else
            {
                printf("A lista na qual você está tentando imprimir está vazia");
            }
            break;
        }
        case 2:
        {
            int entrada = malloc(sizeof(int));
            int tipoInsercao = malloc(sizeof(int));
            printf("Você entrou no modo de inserção\n");
            while (entrada != -1)
            {
                printf("Digite o número a ser adicionado\n");
                scanf("%i", &entrada);
                if (entrada != -1)
                {
                    printf("Para inserir no início digite 0, no final 1 e após 2\n");
                    scanf("%i", &tipoInsercao);
                    if (tipoInsercao != 0 && tipoInsercao != 1 && tipoInsercao != 2)
                    {
                        printf("Digite opção Válida\n");
                    }
                    else
                    {

                        switch (tipoInsercao)
                        {
                        case 0:
                            list = inserirInicio(list, entrada);
                            break;
                        case 1:
                            list = inserirFinal(list, entrada);
                            break;
                        case 2:
                            if (list != NULL)
                            {
                                int numeroApos = malloc(sizeof(int));
                                printf("Digite o número o qual você quer inserir após :");
                                scanf("%d", &numeroApos);
                                list = inserirApos(list, entrada, numeroApos);
                            }
                            else
                            {
                                printf("Impossível inserir após, pois a lista não possui nenhum elemento");
                            }
                            break;
                        }
                    }
                }
            }
        }
        }
    }
    Tree *arvoreInstanciada = NULL;
    lista *listaApoio = list;

    while (listaApoio != NULL)
    {
        arvoreInstanciada = toAVL(arvoreInstanciada, listaApoio->numero);
        listaApoio = listaApoio->proximo;
    }
    printf("\n");
    printf("Árvore Criada:\n");
    mostrarArvore(arvoreInstanciada, 'A');
    printf("\n");
    printf("Lista Informada: \n");
    imprimir(list);
    return 0;
}