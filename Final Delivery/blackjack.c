#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "estrutras.h"

#define STRING_SIZE 100       // max size for some strings
#define STRING_NAME 8
#define STRING_TYPE 2
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define MAX_DECK_SIZE 52      // number of max cards in the deck
#define MAX_CARD_HAND 11      // 11 cards max. that each player can hold
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // number of maximum players
/**
*Função que permite ler os parametros de jogo para os diferentes jogadores
* deck_number: numero de bralhos
* player_number: numero de jogadores;
* no_player **head: ponteiro para a cabeça da lista contendo a estruturas para cada um dos diferentes jogadores
*/
void Data_reader(int *deck_number, int *players_number, char file[STRING_SIZE], no_player **head, char *args[])
{
    FILE *parametros;
    int j=0;
    char aux[STRING_SIZE];
    char j_aux[STRING_SIZE];
    char *auxaux;

    parametros=fopen(args[1], "r"); //Efectua a abertura do ficheiro

    fgets(aux, STRING_SIZE, parametros);
    sscanf(aux, "%d-%d", deck_number, players_number);  //leitura do numero de baralhos e numero de jogadores
    *players_number=*players_number+1;

    if(*deck_number<1 || *deck_number>8)
    {
        printf("Numero de baralhos nao permitido!! (entre 4 e 8)\n");
        exit(EXIT_FAILURE);
    }

    if(*players_number<4 || *players_number>5)
    {
        printf("Numero de jogadores nao permitido!! (entre 1 e 4)\n");
        exit(EXIT_FAILURE);
    }

    printf("Numero de Baralhos:%d\n", *deck_number);
    printf("Numero de jogadores:%d\n", *players_number);

    player * jp=(player*)malloc((*players_number)*sizeof(player));

    for(j=0; j<*players_number-1; j++)  //leitura dos parametros de jogo para os diferentes jogadores (tipo, nome, dinheiro e aposta)
    {
        fgets(j_aux, 100, parametros);

        auxaux = strtok(j_aux, "-");
        strcpy(jp[j].player_tipe, auxaux);

        auxaux=strtok(NULL, "-");
        strcpy(jp[j].player_name, auxaux);

        auxaux=strtok(NULL, "-");
        sscanf(auxaux,"%d", &jp[j].money);

        if(jp[j].money<100 || jp[j].money>500)
        {
            printf("Quantia de dinheiro do jogador %d inválida\n", j+1);
            exit(EXIT_FAILURE);
        }

        auxaux=strtok(NULL,"-");
        sscanf(auxaux, "%d", &jp[j].bet);

        if(jp[j].bet > (0.2*(jp[j].money)) || jp[j].bet<2)
        {
            printf("Quantia apostada pelo jogador %d inválida\n", j+1);
            exit(EXIT_FAILURE);
        }

        printf("ativo: %s    nome:%s   money:%d    aposta:%d\n",jp[j].player_tipe ,jp[j].player_name,jp[j].money ,jp[j].bet);

    }
    //coloca os parametros obtidos na lista dos jogadores
    for(j=0; j<*players_number; j++)
    {
        no_player *aux1=(no_player*)malloc(sizeof(no_player));
        no_player *aux2;

        if(*head==NULL)
        {
            *head=aux1;
            (*head)->next = NULL;
            aux1->ptr= jp[0];
            aux2=*head;
            printf("tipo:%s    nome:%s   money:%d    aposta:%d\n", aux1->ptr.player_tipe, aux1->ptr.player_name, aux1->ptr.money, aux1->ptr.bet);
        }
        else
        {
            aux1->next=NULL;
            aux2->next=aux1;
            aux1->ptr=jp[j];
            aux2=aux1;
            printf("tipo:%s   nome:%s    money:%d    aposta:%d\n", aux1->ptr.player_tipe, aux1->ptr.player_name, aux1->ptr.money, aux1->ptr.bet);
        }
    }

}
/**
*Função que coloca as cartas numa lista onde cada carta e identificada por um codigo de 0 a 12 um naipe de 0 a 3 e a respectiva pontuaçao de 0 a 11.
*deck_numer: numero de baralhos
*numero de cartas: numero total de cartas tendo em conta o numero de baralhos
* no_card **head ponteiro para a cabeça da lista do megabaralho
*/
void Card_list ( int *number_of_cards, no_card **head_card, int *deck_number)
{

    int tmp=0;
    int suit=0;
    *head_card=NULL;

    *number_of_cards= 52* (*deck_number); //calculo do numero total de cartas
    card * cardpointer = (card*)malloc((*number_of_cards)*sizeof(card));  //alocaçao de memória para o numero total de cartas
    for(tmp=0; tmp < *number_of_cards; tmp++)
    {
        cardpointer[tmp].code= tmp%13; //efectua a convesao para o codigo da cartas
        cardpointer[tmp].suit= suit;
        if(cardpointer[tmp].code==12)  //sempre o que o codigo chege ao numero 12 , significa que se chegou ao fim do naipe e entao incrementa o naipe
        {
            suit+=1;
            cardpointer[tmp].points=11;
        }
        if(cardpointer[tmp].code==11 || cardpointer[tmp].code==10|| cardpointer[tmp].code==9 || cardpointer[tmp].code==8)
        {
            cardpointer[tmp].points=10;
        }
        if(cardpointer[tmp].code==7 ||cardpointer[tmp].code==6||cardpointer[tmp].code==5||cardpointer[tmp].code==4||cardpointer[tmp].code==3||cardpointer[tmp].code==2||cardpointer[tmp].code==1 ||cardpointer[tmp].code==0)
        {
            cardpointer[tmp].points= cardpointer[tmp].code+2; //a pontuação das restantes cartas e dada pelo codigo +2;
        }
        if(suit==4)
        {
            suit=0;  //sempre que a contagem dos naipes chegar a 4 entao significa que o baralho acabou e o naipe volta ao numero 0.
        }
    }
    for(tmp=0; tmp < *number_of_cards; tmp++) //depois de atribuidas os diferentes codigos e pontuação as cartas estas são colocadas numa lista
    {
        no_card *ptr1 = (no_card*)malloc(sizeof(no_card));  //alocação de memória para o numero total de cartas
        if(ptr1==NULL)
        {
            printf("ERRO NA ALOCAÇÃO DE MEMÓRIA\n");
            exit(EXIT_FAILURE);
        }
        no_card *ptr2;

        if(*head_card == NULL)
        {
            *head_card=ptr1;
            (*head_card)->next=NULL;
            ptr1->ctr=cardpointer[tmp];
            ptr2=*head_card;
            printf("codigo:%d   naipe:%d     pontos:%d\n", ptr1->ctr.code, ptr1->ctr.suit, ptr1->ctr.points);
        }

        else
        {
            ptr1->next = NULL;
            ptr2->next = ptr1;
            ptr1->ctr = cardpointer[tmp];
            ptr2=ptr1;
            printf("codigo:%d   naipe:%d     pontos:%d\n", ptr1->ctr.code, ptr1->ctr.suit, ptr1->ctr.points);
        }
    }
}
/**
*função que permite saber quantas cartas ainda existem no mega baralho
*no_card **head_card : apontador para a cabeça da lista de cartas
*number_of_cards_after_deal : numero de cartas existente no baralho depois de distribuidas as cartas
*/
void Cards_in_deck(no_card **head_card, int* number_of_cards_after_deal)
{
    no_card *ptr= *head_card;
    *number_of_cards_after_deal=0;
    int i=0;
    for(i=0; ptr!=NULL; i++, ptr=ptr->next)
    {
        (*number_of_cards_after_deal)++;
    }
    printf("N %d\n", *number_of_cards_after_deal);
    printf("i = %d\n", i);
}
/**
*função que efectua a remoção aleotoria de uma carta do baralho
*no_card **head_card : apontador para a cabeça da lista de cartas
*no_card **remove : ponteiro para a carta removida aleatoriamente da lista de cartas
*number_of_cards_after_deal : numero de cartas restantes no baralho
* deck_number : numero de baralhos(lidos do ficheiro de parametros)
*/
void Shuffle_deck(no_card **head_card, no_card **remove, int* number_of_cards_after_deal, int *deck_number)
{
    *remove= *head_card;
    no_card *ptr= *head_card;
    int x=0;
    int j=0;
    int i=0;

    Cards_in_deck(head_card, number_of_cards_after_deal);

    i=*number_of_cards_after_deal;
    printf("N %d\n", *number_of_cards_after_deal);
    printf("i = %d\n", i);


    srand(time(NULL));
    x=rand()%i; //gera um numero aleatorio e divide-o pelo numero de cartas restantes no baralho de forma a garantir que o numero se encontra entre o e o numero de cartas ainda existentes no baralho

    printf("x = %d\n", x);

    if(x==0) // caso a carta a remover seja do cabeça da lista;
    {
        *head_card= (*remove)->next; //a cabeça passa a apontar para a carta seguinte
        (*remove)->next=NULL;
        printf("codigo= %d    naipe= %d     pontos:%d\n", (*remove)->ctr.code, (*remove)->ctr.suit, (*remove)->ctr.points);
    }
    else if(x>0 && x<i-1) //caso a carta a remover nao esteja nem na cabeça nem no fim da lista de cartas
    {
        for(j=0; j<x-1; j++)
        {
            ptr=ptr->next; //percorre a lista de cartas ate à carta anterior a carta a remover

        }
        *remove=ptr->next;
        ptr->next=(*remove)->next;
        printf("codigo= %d    naipe= %d     pontos:%d\n", (*remove)->ctr.code, (*remove)->ctr.suit, (*remove)->ctr.points);

    }
    else if(x==i-1)  //caso a carta a remover seja do fim da lista de cartas
    {
        for(j=0; j<x-1; j++)
        {
            ptr=ptr->next; //percorre a lista ate a carta anterior a aquela que se pretende remover
        }
        *remove=ptr->next;  //remove a carta do baralho
        ptr->next=NULL;
        printf("codigo= %d    naipe= %d     pontos:%d\n", (*remove)->ctr.code, (*remove)->ctr.suit, (*remove)->ctr.points);
    }
}



