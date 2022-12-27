#include "jeu.h"

// création d'un mutex
pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * lit les entrés du clavier et assigne une direction au(x) joueur(s)
 * @param player le tableau du/des joueur(s)
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
           player[0].direction = UP;
        } 
        if (GetAsyncKeyState('Q') < 0) {
           player[0].direction = LEFT;
        }
        if (GetAsyncKeyState('S') < 0) {
           player[0].direction = DOWN;
        }
        if (GetAsyncKeyState('D') < 0) {
           player[0].direction = RIGHT;
        }
        if (GetAsyncKeyState('W') < 0) {
             if (player[0].timer == 0 ) {
               player[0].planting_bomb = TRUE;
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

        // si une bombe est posée
        if (game->player[0].timer > 0) {
            if (--game->player[0].timer == 0) {
                explosion(game);
            }     
        }
        // si le joueur pose un ebombre
        if (game->player[0].planting_bomb) {
            game->player[0].posl_bomb = game->player[0].posl;
            game->player[0].posc_bomb = game->player[0].posc;
            game->player[0].timer = game->player[0].n *2;
            game->plateau[game->player[0].posl_bomb][game->player[0].posc_bomb] = BOMB;
            game->player[0].bomb_cpt++;
            game->player[0].planting_bomb = FALSE;
        } 
        // sinon si le joueur se déplace
        else if (game->player[0].direction != IDLE) {
            calculate_position(game);
        }          
        // affichage du jeu
        display_game(*game);
        // réinitialisation de la direction des joueurs
        game->player[0].direction = IDLE;
        // si les conditions de fin de parties sont réunis
        if ((game->player[0].is_alive == FALSE) || (game->nb_obstacles == 0)) {
            end_game(game);
            return 0;
        }
      
        pthread_mutex_unlock(&dmutex);
        // temporisation de 0.5 s
        Sleep(500);
    }
}

int main() {

    game_t game;
    // initialisation du joueur
    init_players(&game);
    // initialisation des threads
    pthread_t anim,keyboard;
    // initialisation de la fonction random
    srand(time(0));
    // non du fichier contenant le plateau
    char fichier[] = "plateauF.txt";
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
