#include "jeu.h"

/**
 * effacee le contenu de l'écran en mode terminal
 * cf http://www.cplusplus.com/articles/4z18T05o/
 */
void clear_screen() {

  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;
  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;
  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;
  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;
  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
}  

/**
 * affiche le jeu 
 * @param game la structure du jeu
 */
void display_game(game_t game) {
    // efface l'écran
    clear_screen();
    // affichage du score pour chaque joueur
    for (int id = 0; id < game.nb_player; id++) {
        printf("Joueur : %s   score : %.2f\n\n",game.player[id].pseudo, calculate_score(game.player[id]));
    }    
    // affichage du plateau
	int i, j;
    for (i=0; i<game.lignes; i++ ){
        for (j=0; j<game.colonnes; j++ ){
            if (game.plateau[i][j] == WALL) {
                printf("#");
            }
            else if (game.plateau[i][j] == PLAYER) {
                printf("B");
            }
            else if (game.plateau[i][j] == OBSTACLE) {
                printf("x");
            }
            else if (game.plateau[i][j] == BOMB) {
                printf("o");    
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}