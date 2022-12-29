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
    // nombre de caractères du plateau
    int taille_plateau = game.lignes*(game.colonnes+2)*((int) (sizeof(char)));
    // plateau sous la forme d'une chaine de caratères
    char plateau[taille_plateau];
    strcpy(plateau,"");
    // parcours des lignes du plateau 
    for (int i = 0; i < game.lignes; i++) {
        // parcours des colonnes du plateau
        for (int j = 0; j < game.colonnes; j++) {
            if (game.plateau[i][j] == WALL) {
                strcat(plateau,"#");
            }
            else if (game.plateau[i][j] == PLAYER) {
                strcat(plateau,"B");
            }
            else if (game.plateau[i][j] == OBSTACLE) {
                strcat(plateau,"x");
            }
            else if (game.plateau[i][j] == BOMB) {
                strcat(plateau,"o");    
            }
            else {
                strcat(plateau," ");
            }
        }
        strcat(plateau,"\r\n");
    }
    // affichage/envoi des scores pour chaque joueur
    for (int id = 0; id < game.nb_player; id++) {
        char score[100];  
        sprintf(&score, "%.2f", calculate_score(game.player[id]));
        send_infos(score, 100);
        printf("Joueur %d : %s   score : %s\n\n", (id+1), game.player[id].pseudo, score);
    }  
    // envoi du plateau au client
    send_infos(plateau, taille_plateau);
    // affiche le plateau
    printf(plateau);     
}