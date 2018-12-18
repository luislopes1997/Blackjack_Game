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
* funçao que distribuiu as cartas iniciais pelos jogadores e delaer
* no_card **head : apontador para a cabeça da lista de cartas
* no_card **remove : apontador para a carta removida da lista de cartas
* no_player **head : apontador para a cabeça da lista de jogadores
* pos_house_hand : numero de cartas voltadas para cima na mao do dealer
* number_of_cards_after_deal : numero de cartas ainda existentes no baralho
* deck_number : numero de baralhos
*/
void Deal_cards(no_card **head_card, no_card **remove, no_player **head, int* pos_house_hand, int* number_of_cards_after_deal, int *deck_number)
{
    int temp=0;
    no_player *aux;
    *pos_house_hand=1;

    for(temp=0; temp<2; temp++)
    {
        for(aux=*head; aux!=NULL; aux=aux->next)
        {
            while(aux->ptr.no_money==1)   //permite nao distribuir cartas ao jogadores que estao falidos
            {
                if(aux->next !=NULL)
                {
                    aux=aux->next;
                }
                else if(aux->next == NULL)
                {
                    break;
                }
            }

            if(temp==0)
            {

                aux->ptr.hand_card=(card*)calloc(11, sizeof(card));   //alocaçao de memória para as 11 cartas possiveis na mao de cada um dos jogadores
                if(aux->ptr.hand_card==NULL)
                {
                    printf("ERRO NA ALOCAÇÃO\n");
                    exit(EXIT_FAILURE);
                }

                }
            Shuffle_deck(head_card,remove,number_of_cards_after_deal, deck_number); //invoca a funçao que remove uma carta aleatoriamente do baralho
            aux->ptr.hand_card[temp]=(*remove)->ctr;
            aux->ptr.player_cards = temp + 1;  //incremnta o numero de cartas na mao do jogador
            printf("*******************************************************************\n");
            printf("codigo= %d    naipe= %d     pontos%d     nº cartas:%d \n", aux->ptr.hand_card[temp].code, aux->ptr.hand_card[temp].suit, aux->ptr.hand_card[temp].points, aux->ptr.player_cards);
            printf("*******************************************************************\n");

        }
    }
}
/**
*Função que efectua a distruibuiçao das cartas para o dealer depois de todos os jogadores terem jogado
* no_player **head : apontador para a cabeça da lista de jogadores
* no_card **head_card : apontador para a cabeça da lista de cartas
* no_card **remove : apontador para a carta que foi removida do baralho
* pos_house_hand : numero de cartas voltadas para cima na mao do delaer
* end_of round : define quando e que a ronda acabou
*/
void Deal_house_cards(no_player **head, no_card **head_card, no_card **remove, no_player **player_active, int* pos_house_hand, int* end_of_round, int* number_of_cards_after_deal, int *deck_number)
{

    int temp=0;
    *pos_house_hand=2;
    if(*end_of_round==0)
    {
        if((*player_active)->next==NULL)  //player_active -> next ==NULL coresponde a situaçao em que e a vez de o dealer jogar
        {
            while((*player_active)->ptr.points<17)
            {
                temp=(*player_active)->ptr.player_cards;
                Shuffle_deck(head_card, remove, number_of_cards_after_deal, deck_number);  //invoca a funçao que remove a uma carta do baralho
                (*player_active)->ptr.hand_card[temp]=(*remove)->ctr;
                Points_calculator(player_active);  //invoca a funçao que efectua a contagem de pontos
                if((*player_active)->ptr.hand_card[temp].points==11)
                {
                    (*player_active)->ptr.aces+=1;
                }
                printf("ases do jogador  %d\n", (*player_active)->ptr.aces);
                (*player_active)->ptr.player_cards = temp + 1; //contagem do numero de cartas na mao do dealer
                if((*player_active)->ptr.points>21)
                {
                    if((*player_active)->ptr.aces>0)    // caso tenha mais de 21 pontos e ases na mao os ases passam a valer 1
                    {
                        (*player_active)->ptr.points=(*player_active)->ptr.points - 10;
                        (*player_active)->ptr.aces-=1;
                        printf("PONTOS %d\n", (*player_active)->ptr.points);
                    }
                    else
                    {
                        (*player_active)->ptr.bust=0;   //caso nao tenham ases entao o delaer BUST
                        printf("Dealer BUSTED\n");
                    }
                }
            }
        }
    }
}
/**
*Funçao que efectua a contagem dos pontos iniciais de cada jogador e dealer
* no_player **head : apontador para a cabeça da lista de jogadores
*/
void Initial_player_points(no_player **head)
{
    no_player * aux_points;
    int cards=0;
    int a=0;

    for(aux_points=*head; aux_points!=NULL; aux_points=aux_points->next)  //percorre a lista de jogadores
    {
        cards=aux_points->ptr.player_cards;
        for(a=0 ; a < cards ; a++) // entra neste loop enquanto nao forem contadas todas as cartas da mao do jogador
        {
            aux_points->ptr.points=aux_points->ptr.points + aux_points->ptr.hand_card[a].points;
            if(aux_points->ptr.hand_card[a].points==11)  //efetua contagem do numero de ases na mao
            {
                aux_points->ptr.aces+=1;
            }
        }
        if(aux_points->ptr.points==21)  // caso o numero de pontos iniciais seja 21 entao o jogador fex BLACKJACK
        {
            aux_points->ptr.blackjack=1;
        }
        if(aux_points->ptr.points==22 && aux_points->ptr.aces==2) //caso as duas cartas iniciais sejam dois ases
        {
            aux_points->ptr.points=aux_points->ptr.points-10;
            aux_points->ptr.aces-=1;
        }

        printf("pontos do jogador %d\n",  aux_points->ptr.points);
        printf("ases do jogador %d\n", aux_points->ptr.aces);
    }
}
/**
* Funçao que efectua contagem dos restantes pontos
* no_player **player_active : apontador para  a estrutura do jogador ativo no momento
*/
void Points_calculator(no_player **player_active)
{
    (*player_active)->ptr.points=(*player_active)->ptr.points + (*player_active)->ptr.hand_card[(*player_active)->ptr.player_cards].points;
    printf("PONTOS %d\n", (*player_active)->ptr.points);
}
/**
* Funçao que determina quem ganhou e quem perdeu e atribuiu o dinheiro aos jogadores
* no_player **head : apontador para a cabeça da lista dos jogadores
* end_of_round : determina quando acabou a ronda
*/
void Make_money_function(no_player **head, int*end_of_round)
{
    no_player *aux_money=*head;
    no_player *aux_dealer=*head;
    if(*end_of_round==0)
    {
        while(aux_dealer->next!=NULL) //permite percorrer a lista de jogadores ate chegar ao delaer
        {
            aux_dealer=aux_dealer->next;
        }
        while(aux_money!=aux_dealer)
        {
            while(aux_money->ptr.no_money==1) //permite so fazer a contagem de dinheiro caso os jogadores nao estejam falidos
            {
                if(aux_money->next != aux_dealer)
                {
                      aux_money=aux_money->next;
                }
                else if(aux_money->next == aux_dealer)
                {
                    break;
                }
            }
            if(aux_money->ptr.no_money==0 && aux_money->ptr.surrender==0)
            {
                if(aux_money->ptr.blackjack==1)
                {
                    aux_money->ptr.money=aux_money->ptr.money + aux_money->ptr.bet*2.5;
                    aux_dealer->ptr.money=aux_dealer->ptr.money - aux_money->ptr.bet *1.5;
                    aux_money->ptr.win=aux_money->ptr.win +1;
                }
                if((aux_money->ptr.blackjack==0 && aux_money->ptr.points<=21)  && aux_dealer->ptr.points>21)  //jogador faz menos que 22 pontos e dealer faz BUST
                {
                    aux_money->ptr.money=aux_money->ptr.money + aux_money->ptr.bet*2;
                    aux_dealer->ptr.money=aux_dealer->ptr.money - aux_money->ptr.bet;
                    aux_money->ptr.win=aux_money->ptr.win +1;
                }
                if(aux_money->ptr.points>21)
                {
                    aux_dealer->ptr.money=aux_dealer->ptr.money + aux_money->ptr.bet;
                    aux_money->ptr.loss=aux_money->ptr.loss +1;
                }
                if((aux_money->ptr.points<=21 && aux_dealer->ptr.points<=21) && aux_money->ptr.blackjack == 0)  //caso nem o delaer nem nenhum dos jogadores faça mais de 22 pontos
                {
                    if(aux_dealer->ptr.points > aux_money->ptr.points)  //situaçao em que o delaer tem mais pontos que os jogadores
                    {
                        aux_dealer->ptr.money=aux_dealer->ptr.money + aux_money->ptr.bet;
                        aux_money->ptr.loss=aux_money->ptr.loss +1;
                    }
                    if(aux_dealer->ptr.points < aux_money->ptr.points)   //situaçao em que o delaer tem menos pontos que os jogadores
                    {
                        aux_money->ptr.money=aux_money->ptr.money + aux_money->ptr.bet*2;
                        aux_dealer->ptr.money=aux_dealer->ptr.money - aux_money->ptr.bet;
                        aux_money->ptr.win=aux_money->ptr.win +1;
                    }
                    if(aux_dealer->ptr.points == aux_money->ptr.points)  //situaçao de empate
                    {
                        aux_money->ptr.money=aux_money->ptr.money + aux_money->ptr.bet;
                        aux_money->ptr.draw=aux_money->ptr.draw +1;
                    }
                }
            }
            else if(aux_money->ptr.no_money==0 && aux_money->ptr.surrender==1) // situaçao em que o jogador faz surrender
            {
                aux_dealer->ptr.money=aux_dealer->ptr.money + (aux_money->ptr.bet)/2;
                aux_money->ptr.loss=aux_money->ptr.loss +1;
            }
            aux_money=aux_money->next;
        }
    }
}

