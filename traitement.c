#include "jeu.h"

/**
 * calcule la nouvelle position des/du joueur(s)
 * @param game la structure du jeu
 */
void calculate_position(game_t *game) {

    /* si le joueur vient de placer la bombe, 
     * on ne remplace pas son ancienne position par du vide */
    if (game->plateau[game->player[0].posl][game->player[0].posc] != BOMB) {
        game->plateau[game->player[0].posl][game->player[0].posc] = EMPTY;
    }   
    // modification de la position du joueur
    switch ( game->player[0].direction )
    {
        case UP:
            if (game->plateau[game->player[0].posl-1][game->player[0].posc] == EMPTY){                
                game->player[0].posl --;
            }
            break;
        case DOWN:
            if (game->plateau[game->player[0].posl+1][game->player[0].posc] == EMPTY){
                    game->player[0].posl ++;
            }               
            break;
        case LEFT:
            if (game->plateau[game->player[0].posl][game->player[0].posc-1] == EMPTY){
                if (game->player[0].posc == 0) {
                    game->player[0].posc = game->colonnes-1;
                } else {
                    game->player[0].posc --;
                }                    
            }                
            break;
        case RIGHT:
            if (game->plateau[game->player[0].posl][game->player[0].posc+1] == EMPTY){
                if (game->player[0].posc == game->colonnes-1) {
                    game->player[0].posc = 0;
                } else {
                    game->player[0].posc ++;
                }
            }                
            break;
    }   
    // ajout de la nouvelle position du joueur sur le plateau
    game->plateau[game->player[0].posl][game->player[0].posc] = PLAYER;        
}


/**
 * met fin au jeu 
 * @param game la structure du jeu
 */
void end_game(game_t *game) {

    printf("\n\nPARTIE TERMINEE\n\n");
    
    if (!game->player[0].is_alive) {
        printf("Vous avez perdu\n\n");
        return 0;
    }
    
    // creation d'un pointeur pour le fichier
    FILE *fptr;
    // ouverture du fichier en mode "append"
    fptr = fopen("Scores.txt","a");  
    // récupération de la date actuelle
    time_t t;   // not a primitive datatype
    time(&t);   
    // ajout du score
    fprintf(fptr,"%s | %.2f | %s",game->player[0].pseudo,calculate_score(game->player[0]),ctime(&t));
    // fermeture du fichier
    fclose(fptr);

    return 0;
}

/**
 * gère l'effet de l'explosion sur une case donnée
 * @param game la structure du jeu
 * @param i la ligne de la case
 * @param j la colonne de la case
 * @return FALSE si un mur à été percuté, TRUE sinon
 */
boolean compute_explosion(game_t **game, int i, int j) {
    // 
    if ((*game)->plateau[i][j] == WALL) {
        return TRUE;
    }
    if ((*game)->plateau[i][j] == OBSTACLE) {
        (*game)->plateau[i][j] = EMPTY;
        (*game)->player[0].obstacle_cpt++;
        (*game)->nb_obstacles--;
        return FALSE;
    }
    if ((i == (*game)->player[0].posl) && (j == (*game)->player[0].posc)) {
        (*game)->player[0].is_alive = FALSE;
        return FALSE;
    }
    return FALSE;
}



/**
 * gère les explosions
 * @param game la structure du jeu
 */
void explosion(game_t *game) {

    // fait disparaître la bombre
    game->plateau[game->player[0].posl_bomb][game->player[0].posc_bomb] = 0;

    /**
     * calcule des bornes délimitant le rayon d'action de
     * la bombe en prenant en compte la taille du plateau
     */

    // bornes des colonnes
    int col_inf, col_sup;
    col_inf = game->player[0].posc_bomb - game->player[0].n;
    col_sup = game->player[0].posc_bomb + game->player[0].n;
    // respect des indices du plateau
    if (col_inf < 0) {
        col_inf = 0;
    }
    if (col_sup > game->colonnes-1) {
        col_sup = game->colonnes-1;
    }
    // bornes des lignes
    int lgn_inf, lgn_sup;
    lgn_inf = game->player[0].posl_bomb - game->player[0].n;
    lgn_sup = game->player[0].posl_bomb + game->player[0].n;
    // respect des indices du plateau
    if (lgn_inf < 0) {
        lgn_inf = 0;
    }
    if (lgn_sup > game->lignes-1) {
        lgn_sup = game->lignes-1;
    }

    /**
     * destruction des obets dans le rayon de d'action tout 
     * en prenant en compte les murs qui bloquent l'exlosion
     */

    boolean is_blocked = FALSE;
    // explosion horizontale gauche (+ à la postion de la bombre)
    for (int col = game->player[0].posc_bomb; col >= col_inf; col--)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, game->player[0].posl_bomb, col);
        }  
    }
    is_blocked = FALSE;
    // explosion horizontale droite
    for (int col = game->player[0].posc_bomb+1; col <= col_sup; col++)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, game->player[0].posl_bomb, col);
        }    
    }
    is_blocked = FALSE;
    // explosion verticale haut
    for (int lgn = game->player[0].posl_bomb-1; lgn >= lgn_inf; lgn--)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, lgn, game->player[0].posc_bomb);
        }    
    }
    is_blocked = FALSE;
    // explosion verticale bas  
    for (int lgn = game->player[0].posl_bomb+1; lgn <= lgn_sup; lgn++)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, lgn, game->player[0].posc_bomb);
        }              
    }  
    return 0;
}

