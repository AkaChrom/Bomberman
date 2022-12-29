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

    boolean end_of_game = FALSE;

    while(1) {

        pthread_mutex_lock(&dmutex);

        // récupération de la commande du client
        game->player[1].direction = atoi(recv_infos(5));
        // si le joeur 2 souhaite planter la bombe
        char is_planting; 
        sprintf(&is_planting, "%s",  recv_infos(5));
        if (is_planting == '1') {
            if (game->player[1].timer == 0 ) {
                game->player[1].planting_bomb = TRUE;
            }   
        }        
        // traitement des joeurs
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
            // sinon si les conditions de fin de parties sont réunis (mode 2 joueurs)
            if ((game->player[id].is_alive == FALSE)) {
                end_of_game = TRUE;    
            }
        }
        // fin du jeu
        if (end_of_game == TRUE) {
            // prévient que le jeux est fini
            send_infos("0", 5);
            end_game(game);
            return 0;
        }        
        // prévient que le jeux continue
        send_infos("1", 5);
        // affichage du jeu
        display_game(*game); 
      
        pthread_mutex_unlock(&dmutex);
        // temporisation de 0.5 s
        Sleep(500);
    }
}

int main() {

    // initialisation du server
    start_server();
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
    // initialise les objets sur le plateau
    init_objects(&game);
    // lancement des threads
    pthread_create(&anim,NULL,update_game,&game);
    pthread_create(&keyboard,NULL,read_keyboard,game.player);
    pthread_join(anim,NULL);
	return 0;
}
