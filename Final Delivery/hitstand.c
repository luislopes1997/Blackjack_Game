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
*Funçao que permite ao jogador pedir mais uma carta
*no_card **head_card : ponteiro para a cabeça da lista de cartas
* no_card **remove : ponteiro para a carta removida da lista de cartas
* no_player **player_active ponteiro para o jogador ativo de momento
*/
void Hit_one_more_card(no_card **head_card, no_card **remove, no_player **player_active, int* number_of_cards_after_deal, int *deck_number)
{

    int temp=0;
    temp=(*player_active)->ptr.player_cards;
    if((*player_active)->next !=NULL)
    {
        if(temp<=10)// caso ainda tenha menos de 11 cartas na mao
        {
            Shuffle_deck(head_card, remove, number_of_cards_after_deal, deck_number);//invoca a funçao que permite retirar aleatoriamente uma carta da lista de cartas
            (*player_active)->ptr.hand_card[temp]=(*remove)->ctr; //coloca essa carta na mao do jogador
            Points_calculator(player_active);//invoca a funçao que permite efectuar a contagem de pontos
            if((*player_active)->ptr.hand_card[temp].points==11)  // efectua a contagem do numero de ases para o caso que a pontuaçao exceda 21 e exista um as na mao
            {
                (*player_active)->ptr.aces+=1;
            }
            printf("ases do jogador  %d\n", (*player_active)->ptr.aces);
            (*player_active)->ptr.player_cards = temp + 1;
            printf("*******************************************************************\n");
            printf("nome: %s   codigo= %d    naipe= %d     pontos%d     nº cartas:%d \n", (*player_active)->ptr.player_name, (*player_active)->ptr.hand_card[temp].code, (*player_active)->ptr.hand_card[temp].suit, (*player_active)->ptr.hand_card[temp].points, (*player_active)->ptr.player_cards);
            printf("*******************************************************************\n");
        }
        if(temp==10) // caso a mao do jogador exceda as 11 cartas passa ao jogador seguinte
        {
            *player_active=(*player_active)->next;
        }
        if((*player_active)->ptr.points>21) //caso exceda 21 potos e existam ases na mao
        {
            if((*player_active)->ptr.aces >0)
            {
                (*player_active)->ptr.points=(*player_active)->ptr.points - 10;
                (*player_active)->ptr.aces-=1;
                printf("PONTOS %d\n", (*player_active)->ptr.points);
            }
            else if((*player_active)->ptr.doubles==0)
            {
                (*player_active)->ptr.bust=1;
            }
            else
            {
                (*player_active)->ptr.bust=1;
            }
        }
    }
}
/**
* funçao que permite ao jogador fazer STAND, ou seja, passa para o jogador seguinte
*no_player **player_active : ponteiro para a estrutura do jogador activo
*/
void Stand_next_player_on(no_player **player_active)
{
    if((*player_active)->next!=NULL && (*player_active)->ptr.bust==0)
    {
        *player_active=(*player_active)->next;
    }

}
