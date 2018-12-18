# Blackjack_Game
Project developed for the Programming Course at Instituto Superior Técnico. 
This game can support up to 4 human or AI players at the same time. 
The AI players have winning strategies implemented.

# Required Libraries
In order to run this game you will need the SDL2, SDL2_image and SDL2_ttf libraries.
* http://wiki.libsdl.org/APIByCategory
* https://wiki.libsdl.org/FrontPage

# Compile and run the Code
In the command line just go to your `.c` for the Midterm Submission and type the command:
 * `gcc BlackJackGUI.c -g -I/usr/local/include -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o blackjack` <br />

For the Final Submission, do the same thing and type the command:
 * `gcc <all_c_files>.c -g -I/usr/local/include -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o blackjack`
   * Replace `<all_c_files>.c` with the all the `.c` files found in the Final Submission folder separated with a space.<br />  

To execute the code in Midterm Submission just type on your console `./blackjack` <br />

To run the Final Submission type on your console `./blackjack <game_config_file>.txt <AI_config_file>.txt` <br />
 * Replace `<game_config_file>.txt` with `config_jogo.txt` and `<AI_config_file>.txt` with `ficheiro_config_EAs.txt` <br />
 
# How to play
- **Hit:** H key
- **Stand:** S key
- **Double:** D key
- **Surrender:** S key
- **New Game:** N key
- **Quit:** Q key

The `<game_config_file>.txt` contains information about the number of decks (4 to 8), the number of players (1 to 4).
For each player you must indicate the type of player (**HU:** human or **EA:** bot), the players names (max 8 characters), the initial money (10 to 500) and the initial bet.

# Results
At the end of the game a stats file is written containing the name and type of each player, the number of wins, ties and losses, final money and the casino house money balance.
