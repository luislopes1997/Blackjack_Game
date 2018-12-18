#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_DECK 312
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

// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(int _money[], TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer,int f_player1,int f_player2,int f_player3,int f_player4,
                 int money[],int i_player1,int i_player2, int i_player3,int i_player4, int aposta);
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(int [], int , SDL_Surface **, SDL_Renderer * );
void RenderPlayerCards(int [][MAX_CARD_HAND], int [], SDL_Surface **, SDL_Renderer * , int n_jogador);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
void definicao_parametros (int money[],int *n_baralhos,int *aposta);
void baralhar_cartas(int deck[], int *numero_cartas, int *n_baralhos);
void distribuir_cartas(int deck[], int player_cards[MAX_PLAYERS][MAX_CARD_HAND],int pos_player_hand[],int house_cards[],int *pos_house_hand, int v1, int v2, int v3, int v4);
void atualizacao_parametros(int money[], int aposta); //permite actualizar os parametros aps a aposta
void hit(int deck[],int player_cards[MAX_PLAYERS][MAX_CARD_HAND], int pos_player_hand[], int n_jogador, int *n_posicao, int *aux);
void stand(int *n_jogador, int *n_posicao);
int valores_cartas (int n_pontos, int deck);
void contagem_pontos(int deck[], int *n_jogador, int *n_posicao, int *f_player1, int *f_player2, int *f_player3, int *f_player4,
                     int *aux, int *as_p1, int *as_p2, int *as_p3, int *as_p4, int *i_player1, int *i_player2, int *i_player3, int *i_player4,
                     int *s1, int *s2, int *s3, int *s4,int v1, int v2, int v3, int v4);
int pontos_dealer(int deck[], int *i_dealer, int *f_dealer, int *s_dealer, int *as_dealer, int *n_posicao1, int *aux1);
void hit_dealer (int deck[], int house_cards[], int *pos_house_hand, int *aux1, int *n_posicao1, int *aux);
void atualizacao_apostas(int deck[], int f_player1, int f_player2, int f_player3, int f_player4,int f_dealer, int i_player1,
                         int i_player2, int i_player3, int i_player4, int i_dealer, int aposta, int money[], int *n_jogador,int v1, int v2, int v3, int v4, int Vitorias[], int Empates[],int Derrotas[]);
void banca_rota (int aposta, int money[], int *v1, int *v2, int *v3, int *v4);

// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Luis Afonso Lopes";
const char myNumber[] = "ist425416";
const char * playerNames[] = {"Player 1", "Player 2", "Player 3", "Player 4"};

/**
 * main function: entry point of the program
 * only to invoke other functions !
 *IMPORTANT: exitem três warnings que foram ignorados por nao interferirem com o funcionamento do programa. São devidos ao valor returnado pelos scanf não estar atribuido a nenhuma variável
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
    int money[MAX_PLAYERS];                                            //quantia inicial de dinheiro de cada jogador
    int player_cards[MAX_PLAYERS][MAX_CARD_HAND] = {{0}};              //cartas de cada jogador
    int house_cards[MAX_CARD_HAND] = {0};                              //cartas da casa
    int pos_house_hand = 0;                                            //numero de cartas viradas para cima da casa
    int pos_player_hand[MAX_PLAYERS] = {0};                            //numero de cartas viradas para cima de cada jogador
    int n_baralhos=0;                                                  //numero de baralhos
    int aposta=0 ;                                                     //valor que cada jogador aposta
    int deck[MAX_DECK];                                                //vetor onde são guardadas as cartas
    int numero_cartas = 0;                                             //numero total de cartas no jogo
    int n_jogador=0;                                                   //numero do jogador
    static int  n_posicao =2;                                          //posição da carta pedida
    int  aux=10;                                                       //identifica o valor da carta pedido pelo jogador
    static int i_player1=0, i_player2=0, i_player3=0, i_player4=0;     //soma dos pontos iniciais
    static int f_player1=0, f_player2=0, f_player3=0, f_player4=0;     //soma de pontos finais
    static int s1=0, s2=0, s3=0, s4=0;                                 //soma dos pontos após o joagdor fazer hit
    int as_p1=0, as_p2=0, as_p3=0, as_p4=0;                            //numero de ases de cada jogador
    static int i_dealer=0, f_dealer=0 ;                                //pontuação inicial e final da casa
    int as_dealer =0;                                                  //numero de ases da casa
    static int s_dealer=0;                                             //somas das cartas seguintes da casa
    static int n_posicao1 = 2 ;
    static int aux1 =0;
    int v1 =1, v2=1, v3=1, v4=1;                                       //define se o jogador e valido ou nao
    FILE *Estatisticas ;                                               //ficheiro de estatisticas
    int Vitorias[MAX_PLAYERS]= {0,0,0,0};                                 //numero de vitorias
    int Empates[MAX_PLAYERS]= {0,0,0,0};                                  //numero de empates
    int Derrotas[MAX_PLAYERS]= {0,0,0,0};                                 //numero de derrotas
    srand(456);

    //leitura de parametros
    definicao_parametros(money, &n_baralhos, &aposta);
    // initialize graphics
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);
    //baralhar cartas
    baralhar_cartas(deck, &numero_cartas, &n_baralhos);
    //Distribuição de cartas
    distribuir_cartas(deck, player_cards, pos_player_hand, house_cards, &pos_house_hand,v1,v2,v3,v4);
    //contagem de pontos
    contagem_pontos(deck, &n_jogador, &n_posicao, &f_player1, &f_player2,&f_player3,&f_player4,
                    &aux, &as_p1,&as_p2, &as_p3, &as_p4,&i_player1,&i_player2, &i_player3, &i_player4,
                    &s1, &s2, &s3, &s4, v1, v2,v3,v4);
    //actualização de parametros
    atualizacao_parametros(money,aposta);


    while( quit == 0 )
    {
        if(pos_player_hand[n_jogador]==0 &&n_jogador<4)
        {
            n_jogador++;
            contagem_pontos(deck, &n_jogador, &n_posicao, &f_player1, &f_player2,&f_player3,&f_player4,
                            &aux, &as_p1,&as_p2, &as_p3, &as_p4,&i_player1,&i_player2, &i_player3, &i_player4,
                            &s1, &s2, &s3, &s4, v1, v2, v3, v4);

            if(n_jogador==4)
            {
                pos_house_hand= 2 ;

                while(f_dealer<17)
                {
                    f_dealer = pontos_dealer(deck, &i_dealer, &f_dealer, &s_dealer, &as_dealer, &n_posicao1, &aux1);
                    hit_dealer (deck, house_cards, &pos_house_hand, &aux1, &n_posicao1, &aux);
                }
                atualizacao_apostas(deck, f_player1,f_player2, f_player3, f_player4,f_dealer,i_player1,
                                    i_player2, i_player3, i_player4, i_dealer, aposta, money,&n_jogador,v1,v2,v3,v4,Vitorias, Empates,Derrotas);

            }
        }

        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                exit(0); // quit the program
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                case SDLK_n:

                    n_jogador=0;
                    n_posicao =2;
                    aux=10;
                    i_player1=0, i_player2=0, i_player3=0, i_player4=0;
                    f_player1=0, f_player2=0, f_player3=0, f_player4=0;
                    s1=0, s2=0, s3=0, s4=0;
                    as_p1=0, as_p2=0, as_p3=0, as_p4=0;
                    i_dealer=0, f_dealer=0 ;
                    as_dealer =0;
                    s_dealer=0;
                    n_posicao1 = 2 ;
                    aux1 =0;
                    v1=v1;
                    v2=v2;
                    v3=v3;
                    v4=v4;

                    banca_rota (aposta, money, &v1, &v2, &v3, &v4);
                    atualizacao_parametros(money,aposta);
                    baralhar_cartas(deck, &numero_cartas, &n_baralhos);
                    distribuir_cartas(deck, player_cards, pos_player_hand, house_cards, &pos_house_hand,v1,v2,v3,v4);
                    contagem_pontos(deck, &n_jogador, &n_posicao, &f_player1, &f_player2,&f_player3,&f_player4,
                                    &aux, &as_p1,&as_p2, &as_p3, &as_p4,&i_player1,&i_player2, &i_player3, &i_player4,
                                    &s1, &s2, &s3, &s4, v1, v2, v3, v4);

                    break;

                case SDLK_s:

                    stand(&n_jogador, &n_posicao);
                    contagem_pontos(deck, &n_jogador, &n_posicao, &f_player1, &f_player2,&f_player3,&f_player4,
                                    &aux, &as_p1,&as_p2, &as_p3, &as_p4,&i_player1,&i_player2, &i_player3, &i_player4,
                                    &s1, &s2, &s3, &s4,v1, v2, v3, v4);

                    if(n_jogador==4)
                    {
                        pos_house_hand= 2 ;

                        while(f_dealer<17)
                        {
                            f_dealer = pontos_dealer(deck, &i_dealer, &f_dealer, &s_dealer, &as_dealer, &n_posicao1, &aux1);
                            hit_dealer (deck, house_cards, &pos_house_hand, &aux1, &n_posicao1, &aux);
                        }

                        atualizacao_apostas(deck, f_player1,f_player2, f_player3, f_player4,f_dealer,i_player1,
                                            i_player2, i_player3, i_player4, i_dealer, aposta, money,&n_jogador,v1,v2,v3,v4,Vitorias, Empates,Derrotas);


                    }

                    break;
                case SDLK_h:

                    hit(deck,player_cards,pos_player_hand, n_jogador, &n_posicao, &aux);
                    contagem_pontos(deck, &n_jogador, &n_posicao, &f_player1, &f_player2,&f_player3,&f_player4,
                                    &aux, &as_p1,&as_p2, &as_p3, &as_p4,&i_player1,&i_player2, &i_player3, &i_player4,
                                    &s1, &s2, &s3, &s4, v1, v2, v3, v4);

                    if(n_jogador==4)
                    {
                        pos_house_hand= 2 ;

                        while(f_dealer<17)
                        {
                            f_dealer = pontos_dealer(deck, &i_dealer, &f_dealer, &s_dealer, &as_dealer, &n_posicao1, &aux1);
                            hit_dealer (deck, house_cards, &pos_house_hand, &aux1, &n_posicao1, &aux);
                        }

                        atualizacao_apostas(deck, f_player1,f_player2, f_player3, f_player4,f_dealer,i_player1,
                                            i_player2, i_player3, i_player4, i_dealer, aposta, money,&n_jogador,v1,v2,v3,v4,Vitorias, Empates,Derrotas);



                    }
                    break;
                case SDLK_q:


                    Estatisticas = fopen("Estatisticas.txt", "w");
                    fprintf(Estatisticas, "%s: %d Vitorias || %d Empates || %d Derrotas\n", playerNames[0], Vitorias[0], Empates[0], Derrotas[0]);
                    fprintf(Estatisticas, "%s: %d Vitorias || %d Empates || %d Derrotas\n", playerNames[1], Vitorias[1], Empates[1], Derrotas[1]);
                    fprintf(Estatisticas, "%s: %d Vitorias || %d Empates || %d Derrotas\n", playerNames[2], Vitorias[2], Empates[2], Derrotas[2]);
                    fprintf(Estatisticas, "%s: %d Vitorias || %d Empates || %d Derrotas\n", playerNames[3], Vitorias[3], Empates[3], Derrotas[3]);

                    fclose(Estatisticas);

                    exit(0);


                    break;
                default:
                    break;
                }
            }
        }
        // render game table
        RenderTable(money, serif, imgs, renderer,f_player1,f_player2,f_player3,f_player4,
                    money, i_player1, i_player2,  i_player3, i_player4,  aposta);
        // render house cards
        RenderHouseCards(house_cards, pos_house_hand, cards, renderer);
        // render player cards
        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer, n_jogador);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );

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
/**Definição de parametros
 * Local onde são definidos os parametros de jogo
 * int money[] corresponde ao dinheiro de cada um dos jogadores
 * int n_baralhos corresponde ao numero de baralhos com que se joga
 * int aposta: valore apostado por cada um dos jogadores;
 */
void definicao_parametros(int money[], int *n_baralhos, int *aposta)
{

    int i=0;

    do
    {
        printf("Insira o número de baralhos:");
        scanf("%d", n_baralhos);

        if(*n_baralhos>6 || *n_baralhos<1)
        {
            printf("Número de baralhos não válido!\nO número de baralhos tem de ser entre 1 e 6.\n");
        }
    }

    while(*n_baralhos>6 ||*n_baralhos<1);

    do
    {
        for(i=0 ; i <= 3 ; i++)
        {
            printf("Escolha o dinheiro do %dº jogador:", i+1);
            scanf("%d" , &money[i]);
        }

        if(money[0]<10 || money[1]<10 || money[2]<10 || money[3]<10)
        {

            printf("Quantia de dinheiro inválida!\nA quantia de dinheiro para cada jogador não pode ser inferior a 10.\n");
        }
    }

    while(money[0]<10 || money[1]<10 || money[2]<10 || money[3]<10);

    do
    {
        printf("Escolha o valor das apostas:");
        scanf("%d", aposta);

        if(*aposta<1 || *aposta>(0.2*money[0]) ||*aposta>(0.2*money[1]) || *aposta>(0.2*money[2]) || *aposta>(0.2*money[3]))
        {
            printf("Aposta Inválida!\n");
        }
    }

    while(*aposta<1 || *aposta>(0.2*money[0]) || *aposta>(0.2*money[1]) || *aposta>(0.2*money[2]) || *aposta>(0.2*money[3]));

}
/**
 * Baralhar_cartas : efectua o baralhamento de cartas
 * int deck[]: vetor onde sao guardadas as cartas
 * int n_cartas: numero total de cartas ;
 * int n_baralhos: numero de baralhos escolhido na definição de parametros
 */
void baralhar_cartas(int deck[], int *numero_cartas, int *n_baralhos)
{
    int i, j, temp;

    *numero_cartas = *n_baralhos*MAX_DECK_SIZE;

    for(i=0; i<*numero_cartas; i++) //agrupa as cartas num vetor
    {
        deck[i]=i%52;
    }



    for(i=0; i<*numero_cartas; i++) //efetua o baralhamento das cartas
    {
        j=rand() %52;
        temp = deck[i];
        deck[i] = deck[j];
        deck[j]= temp ;
    }


}
/**distribuir_cartas: efectua a distribuição das cartas iniciais pelos diferentes jogadores e casa
 * int player_cards[][]: distribuição das cartas por cada jogador bem como definição das suas posições
 * int pos_player_hand: numero de cartas viradas para cima;
 * int house_cards: cartas da casa
 * int pos_house_hand: numero de cartas voltadas para cima
 * int v1, int v2, int v3, int v4 : quando toma o valor 1 o jogador é valido
 */
void distribuir_cartas(int deck[], int player_cards[MAX_PLAYERS][MAX_CARD_HAND], int pos_player_hand[], int house_cards[], int *pos_house_hand, int v1, int v2, int v3, int v4)
{

    player_cards[0][0]=deck[0];
    player_cards[0][1]=deck[5];
    pos_player_hand[0]= 2 ;

    if(v1==0)   //caso o jogador nao seja valido ou seja nao tem dinheiro suficiente para ir a jogo, não são distribuidas cartas;
    {
        pos_player_hand[0]=0;
    }

    player_cards[1][0]=deck[1];
    player_cards[1][1]=deck[6];
    pos_player_hand[1]= 2 ;

    if(v2==0)
    {
        pos_player_hand[1]=0;
    }

    player_cards[2][0]=deck[2];
    player_cards[2][1]=deck[7];
    pos_player_hand[2]= 2 ;


    if(v3==0)
    {
        pos_player_hand[2]=0;
    }

    player_cards[3][0]=deck[3];
    player_cards[3][1]=deck[8];
    pos_player_hand[3]= 2 ;


    if(v4==0)
    {
        pos_player_hand[3]=0;
    }

    house_cards[0]=deck[4];
    house_cards[1]=deck[9];
    *pos_house_hand = 1 ;
}
/**actualização de parametros: desconta o valor da aposta ao dinheiro inicial de cada um dos jogadores
 * int money[], dinheiro de cada um dos jogadores
 * int aposta: valor apostado por cada um dos jogadores;
 */
void atualizacao_parametros(int money[], int aposta)
{
    if(money[0]>=aposta)
    {
        money[0]=money[0]-aposta; //atualização do dinheiro player1
    }

    if(money[1]>=aposta)
    {
        money[1]=money[1]-aposta; //atualização do dinheiro player2
    }

    if(money[2]>=aposta)
    {
        money[2]=money[2]-aposta; //atualização do dinheiro player3
    }

    if(money[3]>=aposta)
    {
        money[3]=money[3]-aposta; //atualização do dinheiro player4
    }

}
/**hit: permite ao jogador pedir mais cartas;
 * int n_jogador: define que jogador esta activo;
 * int n_posicao: posicao onde vai ser colocada a carta pedida
 * int aux: permite calcular a posição de onde vai ser retirada a carta pedida;
 */
void hit(int deck[],int player_cards[MAX_PLAYERS][MAX_CARD_HAND], int pos_player_hand[], int n_jogador, int *n_posicao, int *aux)
{

    static int n_deck =0 ; //numero de cartas pedidas

    if(*n_posicao<11)
    {
        player_cards[n_jogador][*n_posicao]=deck[10 + n_deck];      //carta a atribuir ao jogador
        pos_player_hand[n_jogador]= 1 + *n_posicao;                 //definição de quantas cartas estão voltadas para cima


        *aux = 10 + n_deck ; //quando for a contagem de pontos permite saber em que posição do deck se encontra a carta que o jogador pediu
        n_deck++;
        (*n_posicao)++;
    }
}
/**stand: permite ao jogador não pedir mais cartas e assim acabar a sua aposta:
 * int n_jogador: jogador activo;
 * int n_posicao :posição onde vai ser coladas as cartas do jogador seguinte ;
 */
void stand(int *n_jogador, int *n_posicao)
{

    (*n_jogador)++; //incrementa o numero do jogador
    *n_posicao=2;   //a posição da carta volta a dois pois o proximo jogador ainda so tem duas cartas

}
/**valores_cartas: atribuir um valor em pontos a cada uma das cartas do deck[];
 * int n_pontos: valor em pontos da carta;
 */
int valores_cartas (int n_pontos, int deck)
{

    if(deck==0||deck==13||deck==26||deck==39)
        n_pontos=2;
    if(deck==1||deck==14||deck==27||deck==40)
        n_pontos=3;
    if(deck==2||deck==15||deck==28||deck==41)
        n_pontos=4;
    if(deck==3||deck==16||deck==29||deck==42)
        n_pontos=5;
    if(deck==4||deck==17||deck==30||deck==43)
        n_pontos=6;
    if(deck==5||deck==18||deck==31||deck==44)
        n_pontos=7;
    if(deck==6||deck==19||deck==32||deck==45)
        n_pontos=8;
    if(deck==7||deck==20||deck==33||deck==46)
        n_pontos=9;
    if(deck==8||deck==21||deck==34||deck==47)
        n_pontos=10;
    if(deck==9||deck==10||deck==11||deck==22||deck==23||deck==24||deck==35||deck==36||deck==37||deck==48||deck==49||deck==50)
        n_pontos=10;
    if(deck==12||deck==25||deck==38||deck==51)
        n_pontos=11;

    return n_pontos;
}
/**contagem de pontos: efectua a contagem de pontos das cartas iniciais bem como das seguintes;
 * int n_jogador: jogador activo;
 * int n_posicao : posicao da carta que se pretende calcular os pontos;
 * i_player1, i_player2, i_player3, i_player4: soma dos pontos iniciais de cada um dos jogadores
 * s1, s2, s3, s4 soma dos pontos das cartas seguintes;
 * f_player1, f_player2, f_player3, f_player4 soma final dos pontos de cada um dos jogadores;
 * as_p1, as_p2, as_p3, as_p4 numero de ases que cada um dos jogadores tem ;
 * v1, v2, v3, v4 so efectua a contagem de pontos caso o jogador esteja valido;
 */
void contagem_pontos(int deck[], int *n_jogador, int *n_posicao, int *f_player1, int *f_player2, int *f_player3, int *f_player4,
                     int *aux, int *as_p1, int *as_p2, int *as_p3, int *as_p4, int *i_player1, int *i_player2, int *i_player3, int *i_player4,
                     int *s1, int *s2, int *s3, int *s4, int v1, int v2, int v3, int v4)
{
    int pontos=0;

    if(*n_jogador==0 &&v1==1)
    {
        if(*n_posicao<3)                                                                        //enquanto o numero de cartas 2, ou seja o jogador nao pediu ainda mais cartas
        {

            (*i_player1)= valores_cartas(pontos, deck[0])+ valores_cartas(pontos, deck[5]);   //soma das cartas iniciais
            *f_player1 = *i_player1 ;                                                       //soma final de pontos caso não peça mais cartas


            if((deck[0]==12||deck[0]==25||deck[0]==38||deck[0]==51) && (deck[5]==12||deck[5]==25||deck[5]==38||deck[5]==51))   //caso as cartas iniciais sejam dois ases
            {
                (*as_p1)++;                                                                                                    //incrmenta a contagem de ases
                *i_player1 = *i_player1 -10;                                                                                   //subtrai 10 pontos aos pontos iniciais
                *f_player1= *i_player1 ;
            }

            if(deck[0]==12||deck[0]==25||deck[0]==38||deck[0]==51||deck[5]==12||deck[5]==25||deck[5]==38||deck[5]==51)         //caso exita apenas um AS nas cartas iniciais
            {
                (*as_p1)++ ;   //incremnta o numero de ases

            }

        }

        else
        {
            *s1 = *s1 + valores_cartas(pontos, deck[*aux]);                                 // calcula a soma das cartas pedidas além das cartas iniciais
            *f_player1 = *i_player1 + *s1 ;                                                 //pontuação final= pontuação inicial mais a soma das cartas pedidas

            if(deck[*aux]==12||deck[*aux]==25||deck[*aux]==38||deck[*aux]==51)
            {
                (*as_p1)++;                                                                 // caso exista um as nas cartas pedidas alem das iniciais deve incrementar o numero de ases
            }

            while((*f_player1>21)&&(*as_p1!=0))                                             //enquanto a pontuação for superior a 21 e o numero de ases diferente de 0
            {
                *s1 = *s1 -10 ;                                                             // retira dez pontos caso o AS saia após o jogador fazer hit
                *f_player1 = *i_player1 + *s1 ;                                             //pontos finais = pontos iniciais + a soma das cartas pedidas
                (*as_p1)-- ;                                                                //decrementa o numero de ases
            }
        }
        printf("pontos finais player1: %d\n", *f_player1);

        if(*f_player1>21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 1 rebentou!\n");
        }

        if(*i_player1==21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 1 fez blackjack!\n");
        }

        if(*f_player1==21 && *i_player1!=21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 1 fez 21 pontos!\n");
        }

    }

    if(*n_jogador==1 && v2==1)      //jogador 2
    {
        if(*n_posicao<3)
        {

            *i_player2 = valores_cartas(pontos, deck[1])+ valores_cartas(pontos, deck[6]);
            *f_player2 = *i_player2 ;


            if((deck[1]==12||deck[1]==25||deck[1]==38||deck[1]==51) && (deck[6]==12||deck[6]==25||deck[6]==38||deck[6]==51))
            {
                (*as_p2)++;
                *i_player2 = *i_player2 -10;
                *f_player2= *i_player2 ;
            }

            if(deck[1]==12||deck[1]==25||deck[1]==38||deck[1]==51||deck[6]==12||deck[6]==25||deck[6]==38||deck[6]==51)
            {
                (*as_p2)++ ;

            }

        }

        else
        {
            *s2 = *s2 + valores_cartas(pontos, deck[*aux]);
            *f_player2 = *i_player2 + *s2 ;

            if(deck[*aux]==12||deck[*aux]==25||deck[*aux]==38||deck[*aux]==51)
            {
                (*as_p2)++;
            }

            while((*f_player2>21)&&(*as_p2!=0))
            {
                *s2 = *s2 -10 ;
                *f_player2 = *i_player2 + *s2 ;
                (*as_p2)-- ;
            }
        }
        printf("pontos finais player2: %d\n", *f_player2);

        if(*f_player2>21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 2 rebentou!\n");
        }

        if(*i_player2==21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 2 fez blackjack!\n");
        }

        if(*f_player2==21 && *i_player2!=21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 2 fez 21 pontos!\n");
        }

    }

    if(*n_jogador==2 && v3==1)   //jogador 3
    {
        if(*n_posicao<3)
        {

            *i_player3= valores_cartas(pontos, deck[2])+ valores_cartas(pontos, deck[7]);
            *f_player3 = *i_player3 ;


            if((deck[2]==12||deck[2]==25||deck[2]==38||deck[2]==51) && (deck[7]==12||deck[7]==25||deck[7]==38||deck[7]==51))
            {
                (*as_p3)++;
                *i_player3 = *i_player3 -10;
                *f_player3= *i_player3 ;
            }

            if(deck[2]==12||deck[2]==25||deck[2]==38||deck[2]==51||deck[7]==12||deck[7]==25||deck[7]==38||deck[7]==51)
            {
                (*as_p3)++ ;
            }

        }

        else
        {
            *s3 = *s3 + valores_cartas(pontos, deck[*aux]);
            *f_player3 = *i_player3 + *s3 ;

            if(deck[*aux]==12||deck[*aux]==25||deck[*aux]==38||deck[*aux]==51)
            {
                (*as_p3)++;
            }

            while((*f_player3>21)&&(*as_p3!=0))
            {
                *s3 = *s3 -10 ;
                *f_player3 = *i_player3 + *s3 ;
                (*as_p3)-- ;
            }
        }
        printf("pontos finais player3: %d\n", *f_player3);

        if(*f_player3>21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 3 rebentou!\n");
        }

        if(*i_player3==21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 3 fez blackjack!\n");
        }

        if(*f_player3==21 && *i_player3!=21)
        {
            (*n_jogador)++;
            *n_posicao=2;
            printf("O jogador 3 fez 21 pontos!\n");
        }


    }

    if(*n_jogador==3 &&v4==1)       //jogador 4
    {
        if(*n_posicao<3)
        {

            *i_player4= valores_cartas(pontos, deck[3])+ valores_cartas(pontos, deck[8]);
            *f_player4 = *i_player4 ;


            if((deck[3]==12||deck[3]==25||deck[3]==38||deck[3]==51) && (deck[8]==12||deck[8]==25||deck[8]==38||deck[8]==51))
            {
                (*as_p4)++;
                *i_player4 = *i_player4 -10;
                *f_player4= *i_player4 ;
            }

            if(deck[3]==12||deck[3]==25||deck[3]==38||deck[3]==51||deck[8]==12||deck[8]==25||deck[8]==38||deck[8]==51)
            {
                (*as_p4)++ ;

            }

        }

        else
        {
            *s4 = *s4 + valores_cartas(pontos, deck[*aux]);
            *f_player4 = *i_player4 + *s4 ;

            if(deck[*aux]==12||deck[*aux]==25||deck[*aux]==38||deck[*aux]==51)
            {
                (*as_p4)++;
            }

            while((*f_player4>21)&&(*as_p4!=0))
            {
                *s4 = *s4 -10 ;
                *f_player4 = *i_player4 + *s4 ;
                (*as_p4)-- ;
            }
        }
        printf("pontos finais player4: %d\n", *f_player4);

        if(*f_player4>21)
        {
            (*n_jogador)++;
            printf("O jogador 4 rebentou!\n");
        }

        if(*i_player4==21)
        {
            (*n_jogador)++;
            printf("O jogador 4 fez blackjack!\n");
        }

        if(*f_player4==21 && *i_player4!=21)
        {
            (*n_jogador)++;

            printf("O jogador 4 fez 21 pontos!\n");
        }

    }


}
/**pontos_dealer: calcula a soma de pontos do dealer;
 * i_dealer: soma dos pontos inicias das cartas da casa;
 * as_dealer: numero de ases da casa;
 * s_dealer: soma dos pontos das cartas seguintes da casa;
 * f_dealer: pontos totais da casa;
 * aux1= definine a posição da carta pedida no baralho permitindo assim calcular os pontos;
 * n_posicao1: posicao onde vao ser colocadas as cartas pedidas pela casa;
 */
int pontos_dealer (int deck[], int *i_dealer, int *f_dealer, int *s_dealer, int *as_dealer, int *n_posicao1, int *aux1)
{
    int pontos=0;
    if(*n_posicao1<3)
    {

        *i_dealer= valores_cartas(pontos, deck[4])+ valores_cartas(pontos, deck[9]);
        *f_dealer = *i_dealer ;


        if((deck[4]==12||deck[4]==25||deck[4]==38||deck[4]==51) && (deck[9]==12||deck[9]==25||deck[9]==38||deck[9]==51))          //caso saiam dois ases nas cartas iniciais
        {
            (*as_dealer)++;                  //incrementa o numero de ases da casa
            *i_dealer = *i_dealer -10;       //subtrai 10 aos pontos iniciais
            *f_dealer= *i_dealer ;
        }

        if(deck[4]==12||deck[4]==25||deck[4]==38||deck[4]==51||deck[9]==12||deck[9]==25||deck[9]==38||deck[9]==51)
        {
            (*as_dealer)++ ;   // caso apenas saia um as ao inicio incrementa o numero de ases;

        }


    }

    else
    {
        *s_dealer = *s_dealer + valores_cartas(pontos, deck[*aux1]);
        *f_dealer = *i_dealer + *s_dealer ;

        if(deck[*aux1]==12||deck[*aux1]==25||deck[*aux1]==38||deck[*aux1]==51)
        {
            (*as_dealer)++;   // caso saia um as nas cartas seguintes
        }
        /*sempre que a pontuação for superior a 21 pontos
        e exitirem ases na mao da casa, decrementa se o numero de ases
        e substrai-se 10 à pontuação total do jogador*/
        while((*f_dealer>21)&&(*as_dealer!=0))
        {
            *s_dealer = *s_dealer -10 ;
            *f_dealer = *i_dealer + *s_dealer ;
            (*as_dealer)-- ;
        }

    }

    printf("pontos dealer: %d\n", *f_dealer);

    return *f_dealer ;
}
/**hit_dealer: pede mais cartas para a casa enquanto a pontuação for menor que 17
 * deck: local onde estao armazenadas as cartas
 * house_cards: cartas da casa
 * pos_house_hand: numero de cartas voltadas para cima
 * *aux1: posicao do baralho a que se deve aceder para acrescentar cartas a casa;
 * n_posicao1: posicao onde vao ser colacadas as cartas do dealer;
 *aux: permite ao aux1 começar onde acabou o aux para nao repetir cartas;
 */
void hit_dealer (int deck[], int house_cards[], int *pos_house_hand, int *aux1, int *n_posicao1, int *aux)
{
    static int i=1;

    if(*n_posicao1<11)      //enquanto o numero de cartas pedidas for menor que 12
    {
        *aux1=*aux+i ;      //continuar a retirar cartas das posições sucessivas do baralho baralho
        house_cards[*n_posicao1]=deck[*aux1] ;
        *pos_house_hand = *n_posicao1; //define onde vai ser colocada cada uma das cartas
        i++;
        (*n_posicao1)++;
    }
}
/**actualizaçao das apostas: actualiza o dinheiro dos jogador no final de cada ronda
 * i_player1, i_player2, i_player3, i_player4, i_dealer: pontos iniciais dos jogadores e do dealer,
 * serve para verificar se ocorreu blackjack ou nao;
 * f_player1, f_player2, f_player3, f_player4, f_dealer: pontuação final de cada um dos jogador e da casa
 * permite comparar as pontuações do dealer com as dos jogadores e assim determinar o vencedor;
 * v1, v2, v3, v4 : permite so actualizar as apostas se o jogador for valido ou seja tiver dinheiro sufuciente para apostar;
 * Vitorias, Empates, Derrotas: permite guardas quantas vitorias, empates e derrotas teve cada jogar durante o jogo.
 * aposta: valor apostado por cada um dos jogador
 * money[]: dinheiro de cada um dos jogadores;
 */
void atualizacao_apostas(int deck[], int f_player1, int f_player2, int f_player3, int f_player4,int f_dealer,int i_player1,
                         int i_player2, int i_player3, int i_player4, int i_dealer, int aposta, int money[], int *n_jogador,
                         int v1, int v2, int v3, int v4, int Vitorias[], int Empates[],int Derrotas[])
{

    /****************************************/
    if(v1==1)
    {
        if((f_player1>f_dealer) && f_dealer<=21 && f_player1<=21) //o jogador tem mais pontos que a casa e nenhum deles teve mais de 21
        {
            money[0]= money[0] + (2*aposta) ;       // o jogador ganha o valor que apostou
            Vitorias[0]+=1;                         //incrementa o numero de vitorias

        }
        if((f_player1<f_dealer)&&(f_dealer<=21))    //situaçao em que o dealer tem mais pontos que o jogador
        {
            Derrotas[0]+=1;                         //incremnta o numero de derrotas
        }
        if(f_player1>21 && f_dealer>21)               // se o jogador rebentar
        {
            Derrotas[0]+=1;                          //incrementa o numero de derrotas
        }

        if((f_player1==f_dealer)&&(f_dealer<=21))     //situçaõ em que tem os mesmo pontos
        {
            money[0]= money[0]+ aposta;               //mantem o dinheiro inicial
            Empates[0]+=1;                            //incremneta o numero de empates

        }

        if((f_player1<=21) &&(f_dealer>21))
        {
            money[0]= money[0] + (2*aposta);
            Vitorias[0]+=1;            //caso o dealer tenha mais de 21 pontos e jogador nao exceda incrementa o nuemro de vitorias
        }

        if(f_player1>21 && f_dealer<=21)
        {
            money[0]=money[0];
            Derrotas[0]+=1;
        }

        if(i_player1==21 && i_dealer!=21)                //situação de blackjack
        {
            money[0]= money[0]+ (0.5*aposta);


        }
        if(i_player1==21 && f_dealer==21)
        {
            money[0]= money[0]+ aposta;


        }
        if(i_dealer==21 && i_player1!=21)
        {
            money[0]= money[0] ;


        }
        if(i_dealer==21 && f_player1==21)
        {
            money[0]= money[0] - aposta;

        }
    }
    printf("Vitorias: %d\n", Vitorias[0]);
    printf("Derrotas: %d\n", Derrotas[0]);
    printf("Empates: %d\n", Empates[0]);

    /**********************************************/
    if(v2==1)
    {
        if((f_player2>f_dealer) && f_dealer<=21 && f_player2<=21)
        {
            money[1]= money[1] + (2*aposta) ;
            Vitorias[1]+=1;
            printf("ola8\n");
        }
        if((f_player2<f_dealer)&&(f_dealer<=21))
        {
            Derrotas[1]+=1;
        }
        if(f_player2>21 && f_dealer>21)
        {
            Derrotas[1]+=1;
        }

        if((f_player2==f_dealer)&&(f_dealer<=21))
        {
            money[1]= money[1]+ aposta;
            Empates[1]+=1;
            printf("ola7\n");
        }

        if((f_player2<=21) &&(f_dealer>21))
        {
            money[1]= money[1] + (2*aposta);
            printf("ola6\n");
            Vitorias[1]+=1;
        }

        if(f_player2>21 && f_dealer<=21)
        {
            money[1]=money[1];
            printf("ola5\n");
            Derrotas[1]+=1;
        }

        if(i_player2==21 && i_dealer!=21)
        {
            money[1]= money[1]+ (0.5*aposta);
            printf("ola4\n");

        }
        if(i_player2==21 && f_dealer==21)
        {
            money[1]= money[1]+ aposta;
            printf("ola3\n");

        }
        if(i_dealer==21 && i_player2!=21)
        {
            money[1]= money[1] ;
            printf("ola2\n");

        }
        if(i_dealer==21 && f_player2==21)
        {
            money[1]= money[1] - aposta;
            printf("ola1\n");
        }

        printf("Vitorias: %d\n", Vitorias[1]);
        printf("Derrotas: %d\n", Derrotas[1]);
        printf("Empates: %d\n", Empates[1]);
    }

    /**********************************************/
    if(v3==1)
    {
        if((f_player3>f_dealer) && f_dealer<=21 && f_player3<=21)
        {
            money[2]= money[2] + (2*aposta) ;
            Vitorias[2]+=1;
            printf("ola8\n");
        }
        if((f_player3<f_dealer)&&(f_dealer<=21))
        {
            Derrotas[2]+=1;
        }
        if(f_player3>21 && f_dealer>21)
        {
            Derrotas[2]+=1;
        }

        if((f_player3==f_dealer)&&(f_dealer<=21))
        {
            money[2]= money[2]+ aposta;
            Empates[2]+=1;
            printf("ola7\n");
        }

        if((f_player3<=21) &&(f_dealer>21))
        {
            money[2]= money[2] + (2*aposta);
            printf("ola6\n");
            Vitorias[2]+=1;
        }

        if(f_player3>21 && f_dealer<=21)
        {
            money[2]=money[2];
            printf("ola5\n");
            Derrotas[2]+=1;
        }

        if(i_player3==21 && i_dealer!=21)
        {
            money[2]= money[2]+ (0.5*aposta);
            printf("ola4\n");

        }
        if(i_player3==21 && f_dealer==21)
        {
            money[2]= money[2]+ aposta;
            printf("ola3\n");

        }
        if(i_dealer==21 && i_player3!=21)
        {
            money[2]= money[2] ;
            printf("ola2\n");

        }
        if(i_dealer==21 && f_player3==21)
        {
            money[2]= money[2] - aposta;
            printf("ola1\n");
        }

        printf("Vitorias: %d\n", Vitorias[2]);
        printf("Derrotas: %d\n", Derrotas[2]);
        printf("Empates: %d\n", Empates[2]);
    }


    /**********************************************/
    if(v4==1)
    {
        if((f_player4>f_dealer) && f_dealer<=21 && f_player4<=21)
        {
            money[3]= money[3] + (2*aposta) ;
            Vitorias[3]+=1;
            printf("ola8\n");
        }
        if((f_player4<f_dealer)&&(f_dealer<=21))
        {
            Derrotas[3]+=1;
        }
        if(f_player4>21 && f_dealer>21)
        {
            Derrotas[3]+=1;
        }

        if((f_player4==f_dealer)&&(f_dealer<=21))
        {
            money[3]= money[3]+ aposta;
            Empates[3]+=1;
            printf("ola7\n");
        }

        if((f_player4<=21) &&(f_dealer>21))
        {
            money[3]= money[3] + (2*aposta);
            printf("ola6\n");
            Vitorias[3]+=1;
        }

        if(f_player4>21 && f_dealer<=21)
        {
            money[3]=money[3];
            printf("ola5\n");
            Derrotas[3]+=1;
        }

        if(i_player4==21 && i_dealer!=21)
        {
            money[3]= money[3]+ (0.5*aposta);
            printf("ola4\n");

        }
        if(i_player4==21 && f_dealer==21)
        {
            money[3]= money[3]+ aposta;
            printf("ola3\n");

        }
        if(i_dealer==21 && i_player4!=21)
        {
            money[3]= money[3] ;
            printf("ola2\n");

        }
        if(i_dealer==21 && f_player4==21)
        {
            money[3]= money[3] - aposta;
            printf("ola1\n");
        }

        printf("Vitorias: %d\n", Vitorias[3]);
        printf("Derrotas: %d\n", Derrotas[3]);
        printf("Empates: %d\n", Empates[3]);
    }

}
/**banca_rota: retirar o jogador do jogo caso este ja nao tenha dinheiro para apostar
 * money[]: dinheiro de cada jogador;
 * aposta: aposta efectuada por cada um dos jogadores;
 * v1, v2, v3, v4 determina se o jogador pode ir a jogo ou nao
 */
void banca_rota (int aposta, int money[], int *v1, int *v2, int *v3, int *v4)
{
    if(money[0]>=aposta)           //enquanto o dinheiro do jogador for maior ou igual à aposta o jogador está válido
    {
        *v1=1;
    }
    else
    {
        *v1=0;                    //caso seja inferior o jogo fica inválido
    }

    if(money[1]>=aposta)
    {
        *v2=1;
    }
    else
    {
        *v2=0;
    }

    if(money[2]>=aposta)
    {
        *v3=1;
    }
    else
    {
        *v3=0;
    }

    if(money[3]>=aposta)
    {
        *v4=1;
    }
    else
    {
        *v4=0;
    }

}

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(int _money[], TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer,int f_player1,int f_player2,int f_player3,int f_player4,
                 int money[],int i_player1,int i_player2, int i_player3,int i_player4, int aposta)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    char name_money_str[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // renders the squares + name for each player
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );

    // renders the areas for each player: names and money too !

    if(money[0]>=aposta)
    {
        playerRect.x = 0*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "%s: %d euros || %d pontos", playerNames[0], _money[0], f_player1);
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[1]>=aposta)
    {
        playerRect.x = 1*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "%s: %d euros || %d pontos", playerNames[1], _money[1], f_player2);
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[2]>=aposta)
    {
        playerRect.x = 2*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "%s: %d euros || %d pontos", playerNames[2], _money[2], f_player3);
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[3]>=aposta)
    {
        playerRect.x = 3*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "%s: %d euros || %d pontos", playerNames[3], _money[3], f_player4);
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[0]< aposta)
    {
        playerRect.x = 0*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "Jogador Falido");
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[1]< aposta)
    {
        playerRect.x = 1*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "Jogador Falido");
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[2]< aposta)
    {
        playerRect.x = 2*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "Jogador Falido");
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(money[3]< aposta)
    {
        playerRect.x = 3*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "Jogador Falido");
        RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(f_player1>21)
    {
        playerRect.x = 0*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BUST");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(f_player2>21)
    {
        playerRect.x = 1*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BUST");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(f_player3>21)
    {
        playerRect.x = 2*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BUST");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(f_player4>21)
    {
        playerRect.x = 3*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BUST");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(i_player1==21)
    {
        playerRect.x = 0*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BLACKJACK");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(i_player2==21)
    {
        playerRect.x = 1*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BLACKJACK");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(i_player3==21)
    {
        playerRect.x = 2*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BLACKJACK");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    if(i_player4==21)
    {
        playerRect.x = 3*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str, "BLACKJACK");
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(int _house[], int _pos_house_hand, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int card, x, y;
    int div = WIDTH_WINDOW/CARD_WIDTH;

    // drawing all house cards
    for ( card = 0; card < _pos_house_hand; card++)
    {
        // calculate its position
        x = (div/2-_pos_house_hand/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        RenderCard(x, y, _house[card], _cards, _renderer);
    }
    // just one card ?: draw a card face down
    if (_pos_house_hand == 1)
    {
        x = (div/2-_pos_house_hand/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(int _player_cards[][MAX_CARD_HAND], int _pos_player_hand[], SDL_Surface **_cards, SDL_Renderer* _renderer, int n_jogador)
{
    int pos, x, y, num_player, card;
    SDL_Rect playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW);

    // for every card of every player
    for ( num_player = 0; num_player < MAX_PLAYERS; num_player++)
    {
        for ( card = 0; card < _pos_player_hand[num_player]; card++)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            pos = card % 4;
            x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
            y = (int) (0.55f*HEIGHT_WINDOW)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
            // render it !
            RenderCard(x, y, _player_cards[num_player][card], _cards, _renderer);
        }

        if(num_player==n_jogador)
        {
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
            playerRect.x = num_player*(separatorPos/4-5) + 10;
            playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
            playerRect.w = separatorPos/4-5;
            playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
            SDL_RenderDrawRect(_renderer, &playerRect);
        }
    }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);

    // destroy everything
    SDL_DestroyTexture(card_text);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[STRING_SIZE];

    // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

    return renderer;
}
