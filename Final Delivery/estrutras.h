#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE_OF_LINE 17
#define SIZE_OF_COLUMN 10
#define STRING_SIZE 100       // max size for some strings
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
#define MAX 8


typedef struct card
{
    int code;
    int suit;
    int points;
}card;

typedef struct no_card
{
    card ctr;
    struct no_card *next;
}no_card;

typedef struct player
{
    char player_tipe[3];
    char player_name[9];
    int money;
    int bet;
    int win;
    int loss;
    int draw;
    int aces;
    int EA;
    int blackjack;
    int surrender;
    int bust;
    int doubles;
    int no_money;
    int player_cards; //numero de cartas na mao
    int points;
    card *hand_card;
}player;

typedef struct no_player
{
    player ptr;
    struct no_player *next;
}no_player;

//declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(int [], TTF_Font *, SDL_Surface **, SDL_Renderer *, no_player **head );
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(int [], int , SDL_Surface **, SDL_Renderer * , no_player **head, int* pos_house_hand, no_player **player_active);
void RenderPlayerCards(int [][MAX_CARD_HAND], int [], SDL_Surface **, SDL_Renderer *, int *players_number, no_player **head, no_player **player_active);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
void Data_reader(int *deck_number, int *players_number, char file[STRING_SIZE], no_player **head, char* args[]);
void Card_list ( int *number_of_cards, no_card **head_card, int *deck_number);
void Shuffle_deck(no_card **head_card, no_card **remove, int* number_of_cards_after_deal, int *deck_number);
void Deal_cards(no_card **head_card, no_card **remove, no_player **head, int* pos_house_hand, int* number_of_cards_after_deal, int *deck_number);
void Hit_one_more_card(no_card **head_card, no_card **remove, no_player **player_active, int* number_of_cards_after_deal, int *deck_number);
void Stand_next_player_on(no_player **player_active);
void Deal_house_cards(no_player **head, no_card **head_card, no_card **remove, no_player **player_active, int* pos_house_hand, int* end_of_round, int* number_of_cards_after_deal, int *deck_number);
void Initial_player_points(no_player **head);
void Points_calculator(no_player **player_active);
void Make_money_function(no_player **head, int* end_of_round);
void Empty_player_cards(no_player **head, no_player **player_active);
void Double_bet(no_player **player_active, no_card **head_card, no_card **remove, int* number_of_cards_after_deal, int *deck_number);
void Bet_update(no_player **head);
void Bet_update_double(no_player **head);
void Surrender(no_player **player_active);
void Bet_change(no_player **player_active);
void Matrix_reader(int **matrix_pointer, char *args[]);
int** Matrix_allocation(int **matrix_pointer);
void Ea_Whats_Up(no_player **head, int **matrix_pointer, no_player **player_active, no_card ** head_card, no_card **remove, int* number_of_cards_after_deal, int *deck_number);
void Is_it_a_EA(no_player **head);
void New_player_arrives(no_player **head);
void Cards_in_deck(no_card **head_card, int* number_of_cards_after_deal);
void File_creator(no_player **head);





