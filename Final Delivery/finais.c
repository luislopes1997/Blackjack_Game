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
void Empty_player_cards(no_player **head, no_player **player_active)
{
    no_player *aux_empty;
    (*player_active)=*head;
    for(aux_empty=*head; aux_empty!=NULL; aux_empty=aux_empty->next)
    {
        if(aux_empty->ptr.doubles==1)
        {
            aux_empty->ptr.bet=(aux_empty->ptr.bet)/2;
            printf("NOVA BET %d\n", aux_empty->ptr.bet);
        }
        //free(aux_empty->ptr.hand_card);
        aux_empty->ptr.points=0;
        aux_empty->ptr.player_cards=0;
        aux_empty->ptr.aces=0;
        aux_empty->ptr.blackjack=0;
        aux_empty->ptr.surrender=0;
        aux_empty->ptr.bust=0;
        aux_empty->ptr.doubles=0;
        printf("PONTOS NOVOS %d\n", aux_empty->ptr.points);
    }
}


void File_creator(no_player **head)
{
    no_player *aux_stats;
    no_player *aux_stats_dealer=*head;
    FILE*statistics = NULL;
    statistics= fopen("stats.txt","w");
    for(aux_stats=*head; aux_stats->next!=NULL; aux_stats=aux_stats->next)
    {
        fprintf(statistics, "\n\n  %s", aux_stats->ptr.player_name);
        fprintf(statistics, "\n TYPE %s", aux_stats->ptr.player_tipe);
        fprintf(statistics, "\n WINS %d", aux_stats->ptr.win);
        fprintf(statistics, "\n DRAWS %d", aux_stats->ptr.draw);
        fprintf(statistics, "\n LOSSES %d", aux_stats->ptr.loss);
        fprintf(statistics, "\n MONEY %d", aux_stats->ptr.money);
    }
    while(aux_stats_dealer->next==NULL)
    {
        aux_stats_dealer=aux_stats_dealer->next;
    }
    fprintf(statistics, "\n\n\n HOUSE MONEY %d\n", aux_stats->ptr.money);
    fclose(statistics);
}


