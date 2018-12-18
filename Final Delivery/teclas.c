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
*funçao que permite ao joagador duplicar a sua aposta pedir mais uma carta e de seguida fazer stand
*no_player **player_active : ponteiro para a estrutura do jogador ativo
*no_card **head_card : ponteiro para a cabeça da lista de cartas
* no_card **remove : ponteiro para a carta removida da lista de cartas
*/
void Double_bet(no_player **player_active, no_card **head_card, no_card **remove, int* number_of_cards_after_deal, int *deck_number)
{
    if((*player_active)->ptr.player_cards==2)  //so e possivel caso o jogador so tenha duas cartas nao mao
    {
        (*player_active)->ptr.bet=(*player_active)->ptr.bet*2;  //duplica a aposta
        (*player_active)->ptr.doubles+=1; //incrementa a variavel que define que um determinado jogador efectuu um double
        Bet_update_double(player_active);   //invoca a funçao que permite atualizar a aposta
        Hit_one_more_card(head_card, remove, player_active, number_of_cards_after_deal, deck_number);//invoca a funçao que permite pedir mais uma carta
        Stand_next_player_on(player_active);  // invoca a funçao que permite nao pedir mais cartas e passar ao jogador seguinte
    }
}
/**
*funçao que permite descontar aos jogadores o valor apostado por estes
*no_player **head: ponteiro para a cabeça da lista de jogadores
*/
void Bet_update(no_player **head)
{
    no_player *j;
    for(j=*head; j!=NULL; j=j->next)
    {
        if(j->ptr.money >= j->ptr.bet)
        {
            j->ptr.money-=j->ptr.bet;
        }
        if(j->ptr.money < j->ptr.bet)
        {
            j->ptr.no_money=1;
        }
        else if(j->ptr.money >= j->ptr.bet)
        {
            j->ptr.no_money=0;
        }
    }
}
/**
*funçao que permite atualizar a aposta caso um jogador faça double
*no_player **player_active apontador para a estrutura do jogador ativo
*/
void Bet_update_double(no_player **player_active)
{
        (*player_active)->ptr.money-=((*player_active)->ptr.bet)/2;
}
/**
*funçao que permite ao jogador fazer surrender e receber metade do valor apostado
*no_player **player_active apontador para a estrutura do jogador ativo
*/
void Surrender(no_player **player_active)
{
    if((*player_active)->ptr.player_cards==2 && (*player_active)->ptr.surrender==0)  //so se o jogador so tiver duas cartas
    {
        (*player_active)->ptr.surrender=1;
        (*player_active)->ptr.money=(*player_active)->ptr.money + ((*player_active)->ptr.bet)/2;  //devolve metade do valor apostado ao jogador
    }
}
/**
*funçao que permite ao jogador no fim de uma ronda alterar o valor da sua aposta
*no_player **head : apontador para a cabeça da lista de jogadores
*/
void Bet_change(no_player **head)
{
    char aux[STRING_SIZE];
    no_player *aux_bet=*head;

    printf("Write the player name\n");
    fscanf(stdin, "%s", aux);
    printf("%s\n", aux);

    for(aux_bet=*head; aux_bet->next!=NULL;aux_bet=aux_bet->next)
    {
        if(strcmp(aux,aux_bet->ptr.player_name)==0) //compara o nome introduzido com o nome dos jogadores para ver se este existe
        {
            printf("Define the new bet value\n");
            scanf("%d", &aux_bet->ptr.bet);
            if(aux_bet->ptr.bet <2 || aux_bet->ptr.bet > 0.2*(aux_bet->ptr.money))
            {
                printf("Bet must be between 2 and 0.2*money\n");  //caso exista pede para inserir o novo valor de aposta
                printf("What is the bet of the new player?\n");
                scanf("%d", &aux_bet->ptr.bet);
            }
            break;
        }
    }
}
/**
*funçao que permite acrescentar um novo jogador caso exista um lugar vago na mesa
*no_player **head : apontador para a cabeça da lista de jogadres
*/
void New_player_arrives(no_player **head)
{
    char aux[STRING_SIZE];
    char aux1[STRING_SIZE];
    no_player *aux_new_player;

    for(aux_new_player=*head; aux_new_player->next!=NULL; aux_new_player=aux_new_player->next)
    {
        if(aux_new_player->ptr.no_money==1) //verifica se existe algum lugar vago na mesa
        {
            printf("What is the new player name?\n"); //caso afirmativo pergunta o nome, tipo, valor da aposta e dinheiro inicial
            fscanf(stdin, "%s", aux);
            strcpy(aux_new_player->ptr.player_name, aux);

            printf("What is the new player type?\n");
            fscanf(stdin, "%s", aux1);
            strcpy(aux_new_player->ptr.player_tipe, aux1);

            Is_it_a_EA(head);

            printf("What is the money of the new player?\n");
            scanf("%d", &aux_new_player->ptr.money);
            if(aux_new_player->ptr.money > 500 || aux_new_player->ptr.money<100)
            {
                printf("Money must be between 100 and 500\n");
                printf("What is the money of the new player?\n");
                scanf("%d", &aux_new_player->ptr.money);
            }

            printf("Player bet?\n");
            scanf("%d", &aux_new_player->ptr.bet);
            if(aux_new_player->ptr.bet <2 || aux_new_player->ptr.bet > 0.2*(aux_new_player->ptr.money))
            {
                printf("Bet must be between 2 and 0.2*money\n");
                printf("What is the bet of the new player?\n");
                scanf("%d", &aux_new_player->ptr.bet);
            }

            aux_new_player->ptr.aces=0;    //coloca tambem as variaveis dessa estrura onde vai ser inserido o novo jogador a zero
            aux_new_player->ptr.blackjack=0;
            aux_new_player->ptr.bust=0;
            aux_new_player->ptr.doubles=0;
            aux_new_player->ptr.draw=0;
            aux_new_player->ptr.win=0;
            aux_new_player->ptr.loss=0;
            aux_new_player->ptr.no_money=0;
            aux_new_player->ptr.points=0;
            aux_new_player->ptr.surrender=0;
            break;
        }
    }
}
