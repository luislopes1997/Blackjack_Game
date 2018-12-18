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
* Funçao que aloca memoria para a matriz
* matrix_pointer : ponteiro para uma determinada posiçao da matriz
*/
int** Matrix_allocation(int **matrix_pointer)
{
    int aux_pointer=0;
    matrix_pointer=(int *) calloc(18, sizeof(int *)); //alocaçao de memoria para as linhas da matriz
    for(aux_pointer=0; aux_pointer<18; aux_pointer++)
    {
        matrix_pointer[aux_pointer]=(int *) malloc(10*sizeof(10)); //alocaçao de memoria para as colunas da matriz
    }
    return matrix_pointer;
}
/**
*funçao que efectua a leitura da matriz
* matrix_pointer : ponteiro para uma determina posiçao da matriz
*/
void Matrix_reader(int **matrix_pointer, char *args[])
{
    FILE * fp;
    char string_aux[STRING_SIZE];
    int j=0;
    int a=0;
    fp=fopen(args[2], "r");  //abertura do ficheiro da matriz (matriz.txt)
    for(j=0; j<18; j++)
    {
            fgets(string_aux, STRING_SIZE, fp);
            sscanf(string_aux, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", &matrix_pointer[j][a],  &matrix_pointer[j][1],  &matrix_pointer[j][2],  &matrix_pointer[j][3],  &matrix_pointer[j][4],  &matrix_pointer[j][5],  &matrix_pointer[j][6],  &matrix_pointer[j][7],  &matrix_pointer[j][8],  &matrix_pointer[j][9]);
    }
    for(j=0; j<18; j++)
    {
        printf("%d-%d-%d-%d-%d-%d-%d-%d-%d-%d\n", matrix_pointer[j][0],  matrix_pointer[j][1],  matrix_pointer[j][2],  matrix_pointer[j][3],  matrix_pointer[j][4],  matrix_pointer[j][5],  matrix_pointer[j][6],  matrix_pointer[j][7],  matrix_pointer[j][8], matrix_pointer[j][9]);
    }
}
/**
* funçao que verifica se o jogador e uma identidade artificial ou nao
* no_player **head : apontador para a cabeça da lista dos jogadores
*/
void Is_it_a_EA(no_player **head)
{
    no_player *aux_ea;
    for(aux_ea=*head; aux_ea->next!=NULL;aux_ea=aux_ea->next)
    {
        if(strcmp("EA", aux_ea->ptr.player_tipe)==0) // compara o tipo de jogador lido do ficheiro com a string EA
        {
            aux_ea->ptr.EA=1;
            printf("JOGADOR É EA!!!\n");
        }
    }
}
/**
* Funçao que determina o comportamento de um jogador do tipo identidade artificial
* no_player **head : ponteiro para a cabeça da lista de jogadores
* matrix_pointer  : ponteiro para uma determinada posiçao da matriz
* no_card **head_card : ponteiro para a cabeça da lista de cartas
* no_card **remove : ponteiro para a carta removida
*/
void Ea_Whats_Up(no_player **head, int **matrix_pointer, no_player **player_active, no_card ** head_card, no_card **remove, int* number_of_cards_after_deal, int *deck_number)
{
    no_player *aux_ea=*player_active;
    no_player *aux_dealer=*head;
    int line =0;
    int column=0;
    int decision=0;

    while(aux_dealer->next!=NULL)  // percorre a lista de jogadores ate chegar ao dealer
    {
        aux_dealer=aux_dealer->next;
    }
    if((aux_ea->ptr.hand_card[0].points!=11) && (aux_ea->ptr.hand_card[1].points!=11))  // situaçao de hard hand (sem ases)
    {
        if(aux_ea->ptr.points<=8)
        {
            line=0; // se os pontos da mao do jogador forem mens de 8 entao a linha da matriz e zero
        }
        else if(aux_ea->ptr.points>=17)
        {
            line=9; // se os pontos na mao d jogador forem maiores que 16 entao a linha e 9
        }
        else if(aux_ea->ptr.points<17 && aux_ea->ptr.points>8)
        {
            line=aux_ea->ptr.points-8; // para as restantes pontuaçoes a linha e dada pelo nunmero de pontos menos 8
        }
    }
    if(aux_ea->ptr.hand_card[0].points==11 || aux_ea->ptr.hand_card[1].points==11)  //situaçao de soft hand
    {
        if(aux_ea->ptr.points>=19)
        {
            line=17;  // caso o nuemro de pontos seja superior a 19 entao a linha e 17
        }
        else
        {
            line=aux_ea->ptr.points-2;   // para as restantes pontuações a linha e dada pela pontuaçao menos 2
        }
    }
    printf("PONTOS EA %d\n", aux_ea->ptr.points);
    column=aux_dealer->ptr.hand_card[0].points - 2; // a coluna da matriz e dada pela pontuaçao da primeira carta da mao do dealer menos 2
    printf("LINE %d\n", line);
    printf("COLUMN %d\n", column);

    decision=matrix_pointer[line][column];  // a decision e dada pelo numero lindo da matriz tendo por base a linha e a coluna calculadas anteriormente
    printf("DECISION %d\n", decision);
    switch(decision)
    {
        case 0: // decision =0 entao efectua STAND
        {
            Stand_next_player_on(player_active);
            break;
        }
        case 1: //  decision =1 entao efectua HIT
        {
             Hit_one_more_card(head_card, remove, player_active, number_of_cards_after_deal, deck_number);
             break;
        }
        case 2: // decision =2 se so tiver duas cartas na mao DOUBLE, se tiver mais de duas cartas HIT
        {
            if(aux_ea->ptr.player_cards==2)
            {
                Double_bet(player_active, head_card, remove, number_of_cards_after_deal, deck_number);
            }
            else
            {
                 Hit_one_more_card(head_card, remove, player_active, number_of_cards_after_deal, deck_number);
            }
            break;
        }
        case 3:  // decision =3 se so tiver duas cartas na mao DOUBLE, se tiver mais de duas cartas STAND
        {
            if(aux_ea->ptr.player_cards==2)
            {
                Double_bet(player_active, head_card, remove, number_of_cards_after_deal, deck_number);
            }
            else
            {
                Stand_next_player_on(player_active);
            }
            break;
        }
        case 4:   // decision =4 efetua SURRENDER
        {
            if(aux_ea->ptr.player_cards==2)
            {
                 Surrender(player_active);
            }
            else
            {
                 Hit_one_more_card(head_card, remove, player_active, number_of_cards_after_deal, deck_number);
            }
            break;
        }
    }

}
