#include "estrutras.h"

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( int argc, char* args[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int money[MAX_PLAYERS] = {110, 110, 110, 110};
    int player_cards[MAX_PLAYERS][MAX_CARD_HAND] = {{0}};
    int house_cards[MAX_CARD_HAND] = {0};
    int pos_house_hand = 0;     //numero de cartas na  voltadas para cima na mao do dealer
    char file[STRING_SIZE];
    int deck_number=0;             //numero de baralhos
    int players_number=0;           //numero de jogadores
    int end_of_round=0;             //fim de ronda
    int pos_player_hand=0;
    int number_of_cards=0;          //numero total de cartas
    int number_of_cards_after_deal=0;       //numero de cartas ainda existentes no baralho
    int **matrix_pointer=NULL;              //ponteiro para uma dada posicao da matriz
    int delay_ea=1000;

    no_player * head=NULL;          //ponteiro para a cabeça da lista de jogadores
    no_card * head_card=NULL;       //ponteiro para a cabeça da lista de cartas
    no_card * remove=NULL;          //ponteiro para a carta removida da lista de cartas

    //leitura de paramtros
    Data_reader (&deck_number, &players_number, file, &head, args);
    //alocaçao de memoria para a matriz da inteligencia artificial
    matrix_pointer=Matrix_allocation(matrix_pointer);
    //leitura da matriz da inteligencia artificial
    Matrix_reader(matrix_pointer, args);
     //funçao que controla o comportamento de um jogador do tipo de inteligencia artificial
    Is_it_a_EA(&head);
     //funçao que coloca as cartas numa lista
    Card_list (&number_of_cards, &head_card, &deck_number);
    //funçao que distribui as cartas pelos jogadores
    Deal_cards (&head_card, &remove, &head, &pos_house_hand, &number_of_cards_after_deal, &deck_number);
     //funçao que calcula os pontos inciais de cada jogador
    Initial_player_points (&head);
     //funçao que actualiza as apostas
    Bet_update(&head);

    // initialize graphics
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);
    no_player *x=head;
    for(int j=0; j<2; j++)
    {
        printf("codigo= %d    naipe= %d     pontos%d\n", x->ptr.hand_card[j].code, x->ptr.hand_card[j].suit, x->ptr.hand_card[j].points);

    }
    no_player * player_active=head;
    while( quit == 0 )
    {
        if(number_of_cards_after_deal < 20)
        {
            printf("ANTES DE BARALHAR\n");
            Card_list (&number_of_cards, &head_card, &deck_number);
            Cards_in_deck(&head_card, &number_of_cards_after_deal);
            printf("N main %d\n", number_of_cards_after_deal);
            printf(" Baralhou!!!!!!!!!!!!!!!!!!!!!\n");
        }
        if(player_active->ptr.EA==1)
        {
            Ea_Whats_Up(&head, matrix_pointer, &player_active, &head_card, &remove, &number_of_cards_after_deal, &deck_number);
        }
        if(player_active->next!=NULL)
        {
            if(player_active->ptr.blackjack==1 || player_active->ptr.surrender==1 || player_active->ptr.bust==1 || player_active->ptr.no_money==1)
            {
                player_active=player_active->next;
            }
        }
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit=1;// quit the program
            }

            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                case SDLK_s:
                    // stand !
                    if(end_of_round==0)
                    {
                        Stand_next_player_on(&player_active);
                    }
                    //Player_points(&head, &player_active, &players_number);
                    break;
                // todo
                case SDLK_h:
                    // hit !
                    if(end_of_round==0)
                    {
                         Hit_one_more_card(&head_card, &remove, &player_active, &number_of_cards_after_deal, &deck_number);
                    }
                    break;
                // todo
                case SDLK_q:
                    quit=1;
                    break;
                case SDLK_d: //double
                    if(end_of_round==0)
                    {
                        Double_bet(&player_active, &head_card, &remove, &number_of_cards_after_deal, &deck_number);
                    }
                    break;
                case SDLK_r: //surrender
                    if(end_of_round==0)
                    {
                        Surrender(&player_active);
                    }
                    break;
                case SDLK_n: //new game
                    if(end_of_round==1)
                    {
                        printf("ENTROU NO MORGADO\n");
                        Empty_player_cards(&head, &player_active);
                        Bet_update(&head);
                        Deal_cards (&head_card, &remove, &head, &pos_house_hand, &number_of_cards_after_deal, &deck_number);
                        Initial_player_points (&head);
                        end_of_round=0;
                    }
                    break;
                case SDLK_b: //bet
                    if(end_of_round==1)
                    {
                        Bet_change(&head);
                    }
                    break;
                case SDLK_a: //new player
                    if(end_of_round==1)
                    {
                        New_player_arrives(&head);
                    }
                    break;
                case SDLK_UP: //delay up
                    delay_ea=delay_ea+50;
                    printf("%d\n", delay_ea);
                    break;
                case SDLK_DOWN: //delay down
                    if(delay_ea>50)
                    {
                        delay_ea=delay_ea-50;
                        printf("%d\n", delay_ea);
                    }
                    break;
                default:
                    break;
                }
            }
            if(player_active->next!=NULL)
            {
                if(player_active->ptr.blackjack==1 || player_active->ptr.surrender==1 || player_active->ptr.bust==1 || player_active->ptr.no_money==1)
                {
                    player_active=player_active->next;
                }
            }
            if(player_active->next==NULL)
            {
                Deal_house_cards(&head, &head_card, &remove, &player_active, &pos_house_hand, &end_of_round, &number_of_cards_after_deal, &deck_number);
                Make_money_function(&head, &end_of_round);
                end_of_round=1;
                break;
            }

        }
        // render game table
        RenderTable(money, serif, imgs, renderer, &head);
        // render house cards
        RenderHouseCards(house_cards, pos_house_hand, cards, renderer, &head, &pos_player_hand, &player_active);
        // render player cards
        RenderPlayerCards(player_cards, &pos_player_hand, cards, renderer, &players_number, &head, &player_active);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        if(player_active->ptr.EA==1)
        {
            SDL_Delay( delay_ea);
        }
        else
        {
             SDL_Delay( delay );
        }
        //criaçao do ficheiro de estatisticas
        File_creator(&head);
    }

    // free memory allocated for images and textures and close everything including fonts
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}


