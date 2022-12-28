#include "jeu.h"

// création d'un mutex
pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * lit les entrés du clavier et assigne une direction au(x) joueur(s)
 * @param player le tableau du/des joueur(s)
 * @return void* 
 */
 void *read_keyboard(player_t *player){

    while(1) {       

        pthread_mutex_lock(&dmutex);     

        // Joueur 1
        if (GetAsyncKeyState(VK_UP) < 0) {
            player[0].direction = UP;
        } 
        if (GetAsyncKeyState(VK_LEFT) < 0) {
            player[0].direction = LEFT;
        }
        if (GetAsyncKeyState(VK_DOWN) < 0) {
            player[0].direction = DOWN;
        }
        if (GetAsyncKeyState(VK_RIGHT) < 0) {
            player[0].direction = RIGHT;
        }
        if (GetAsyncKeyState(VK_SPACE) < 0) {
            if (player[0].timer == 0 ) {
                player[0].planting_bomb = TRUE;
            }            
        }       

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
            if (player[1].timer == 0 ) {
                player[1].planting_bomb = TRUE;
            }    
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

        for (int id = 0; id < game->nb_player; id++) {
            // si une bombe est posée
            if (game->player[id].timer > 0) {
                if (--game->player[id].timer == 0) {
                    explosion(game, id);           
                }     
            }
            // si le joueur pose un ebombre
            if (game->player[id].planting_bomb) {
                game->player[id].posl_bomb = game->player[id].posl;
                game->player[id].posc_bomb = game->player[id].posc;
                game->player[id].timer = game->player[id].n *2;
                game->plateau[game->player[id].posl_bomb][game->player[id].posc_bomb] = BOMB;
                game->player[id].bomb_cpt++;
                game->player[id].planting_bomb = FALSE;
            } 
            // sinon si le joueur se déplace
            else if (game->player[id].direction != IDLE) {
                calculate_position(game, id);
            }  
            // réinitialisation de la direction du joueur
            game->player[id].direction = IDLE;

            // si les conditions de fin de parties sont réunis (mode 1 joueur)  
            if ((game->nb_obstacles == 0) && (game->nb_player == 1)) {
                display_game(*game);
                end_game(game);
                return 0;
            }
            // sinon si les conditions de fin de parties sont réunis (mode 2 joueurs)
            else if ((game->player[id].is_alive == FALSE)) {
                display_game(*game);
                end_game(game);
                return 0;
            }
        }                        
        // affichage du jeu
        display_game(*game); 
      
        pthread_mutex_unlock(&dmutex);
        // temporisation de 0.5 s
        Sleep(500);
    }
}

int main() {

    test_net();

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
    // initialise les objets sur le plateau
    init_objects(&game);
    // lancement des threads
    pthread_create(&anim,NULL,update_game,&game);
    pthread_create(&keyboard,NULL,read_keyboard,game.player);
    pthread_join(anim,NULL);
	return 0;
}
