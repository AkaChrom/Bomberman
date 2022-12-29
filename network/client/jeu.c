#include "jeu.h"

// création d'un mutex
pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * lit les entrés du clavier et assigne une direction au joueur
 * @param player le tableau des joueurs
 * @return void* 
 */
 void *read_keyboard(player_t *player){

    while(1) {       

        pthread_mutex_lock(&dmutex);    

        // Joueur 2
        if (GetAsyncKeyState('Z') < 0) {
            player[1].direction = UP;
        } 
        if (GetAsyncKeyState('Q') < 0) {
            player[1].direction = LEFT;
        }
        if (GetAsyncKeyState('S') < 0) {
            player[1].direction = DOWN;
        }
        if (GetAsyncKeyState('D') < 0) {
            player[1].direction = RIGHT;
        }
        if (GetAsyncKeyState('W') < 0) {
            player[1].planting_bomb = TRUE;
        }

        pthread_mutex_unlock(&dmutex);     
        Sleep(1);
    }
 }

/**
 * met à jour le jeu toutes les 500 ms
 * @param game la structure du jeu
 * @return void* 
 */
void *update_game(game_t *game) {

    while(1) {

        pthread_mutex_lock(&dmutex);

        // récupération de la direction du joueur sous forme de string
        char direction[5];  
        sprintf(&direction, "%d", game->player[1].direction);
        // réinitialisation de la direction du joueur
        game->player[1].direction = IDLE;
        // envoi de la direction au server
        send_infos(direction, 5);

        // informe si le joueur souhaite planter une bombe
        boolean planting = game->player[1].planting_bomb;       
        // remise à FALSE de planting_bomb du joueur
        game->player[1].planting_bomb = FALSE;
        // envoi l'information si le joueur souhaite planter une bombre
        if (planting == TRUE) {
           send_infos("1", 5);
        } else {
            send_infos("0", 5);
        }
        
        // prévient que le jeux est fini
        char end_of_game; 
        sprintf(&end_of_game, "%s",  recv_infos(5));
        // fin du jeu
        if(end_of_game == '0') {
            end_game(game);
            return 0;
        }
        // affichage du jeu
        display_game(*game); 

        pthread_mutex_unlock(&dmutex);
        // temporisation de 0.5 s
        Sleep(500);
    }
}

int main() {
    // initialisation du client
    start_client();
    // initialisation de la structure du jeu
    game_t game;
    // initialisation du joueur
    init_players(&game);
    // initialisation des threads
    pthread_t anim,keyboard;
    // initialisation de la fonction random
    srand(time(0));
    // non du fichier contenant le plateau
    char fichier[] = "plateau.txt";
    // initialise le plateau du jeu
	read_board_game(&fichier,&game);
    // lancement des threads
    pthread_create(&anim,NULL,update_game,&game);
    pthread_create(&keyboard,NULL,read_keyboard,game.player);
    pthread_join(anim,NULL);
	return 0;
}
