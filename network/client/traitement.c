#include "jeu.h"

/**
 * met fin au jeu 
 * @param game la structure du jeu
 */
void end_game(game_t *game) {

    printf("\n\n---- PARTIE TERMINEE ----\n\n");
    // création du message de fin
    char msg[100];
    // réception du message
    snprintf(&msg, 100, "%s", recv_infos(100));
    // affichage du message
    printf(msg);
    // stop le serveur
    stop_server();
}
