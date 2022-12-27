#include "jeu.h"

/*
 * Effacer le contenu de l'écran en mode terminal
 * Check this for more details : http://www.cplusplus.com/articles/4z18T05o/
 */
void clear_screen()
  {
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
  
/*
 * Afficher le plateau sur l'écran en remplaçant 1 par '#'
 * 0 par ' ' et 2 par '*'
 */
void display_game(star_t star) {
    // efface l'écran
    clear_screen();
    // affichage du score
    printf("Joueur : %s   score : %.2f\n\n",star.pseudo, calculate_score(star));
    // affichage du plateau
	int i, j;
    for (i=0; i<star.lignes; i++ ){
        for (j=0; j<star.colonnes; j++ ){
            if (star.plateau[i][j] == WALL)
                printf("#");
            else if (star.plateau[i][j] == PLAYER)
                printf("B");
            else if (star.plateau[i][j] == OBSTACLE)
                printf("x");
            else if (star.plateau[i][j] == BOMB)
                printf("o");    
            else
                printf(" ");
        }
        printf("\n");
    }
}