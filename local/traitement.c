#include "jeu.h"

/**
 * calcule la nouvelle position des/du joueur(s)
 * @param game la structure du jeu
 * @param id le numéro du joueur
 */
void calculate_position(game_t *game, int id) {

    //printf("\n DEPLACEMENT de %d",id);
//
    //printf("\n DIRECTION : %d",game->player[id].direction);
//

    /* si le joueur vient de placer la bombe, 
     * on ne remplace pas son ancienne position par du vide */
    if (game->plateau[game->player[id].posl][game->player[id].posc] != BOMB) {
        game->plateau[game->player[id].posl][game->player[id].posc] = EMPTY;
    }   
    // modification de la position du joueur
    switch ( game->player[id].direction )
    {
        case UP:
            if (game->plateau[game->player[id].posl-1][game->player[id].posc] == EMPTY){                
                game->player[id].posl --;
            }
            break;
        case DOWN:
            if (game->plateau[game->player[id].posl+1][game->player[id].posc] == EMPTY){
                    game->player[id].posl ++;
            }               
            break;
        case LEFT:
            if (game->player[id].posc == 0) {
                game->player[id].posc = (game->colonnes)-1;
            }            
            else if (game->plateau[game->player[id].posl][game->player[id].posc-1] == EMPTY){
                game->player[id].posc --;                                 
            }                
            break;
        case RIGHT:
            if (game->player[id].posc == game->colonnes-1) {
                game->player[id].posc = 0;
            }  
            else if (game->plateau[game->player[id].posl][game->player[id].posc+1] == EMPTY){
                game->player[id].posc ++;                
            }                
            break;
    }   
    // ajout de la nouvelle position du joueur sur le plateau
    if (game->plateau[game->player[id].posl][game->player[id].posc] != BOMB) {
        game->plateau[game->player[id].posl][game->player[id].posc] = PLAYER; 
    }              
}

/**
 * met fin au jeu 
 * @param game la structure du jeu
 */
void end_game(game_t *game) {

    printf("\n\n---- PARTIE TERMINEE ----\n\n");
    
    if (game->nb_player == 2) {
        if ((!game->player[0].is_alive) && (game->player[1].is_alive)) {
            printf(" >> VICTOIRE DE %s <<\n\n",game->player[1].pseudo);
        }   
        else if ((game->player[0].is_alive) && (!game->player[1].is_alive)) {
            printf(" >> VICTOIRE DE %s <<\n\n",game->player[0].pseudo);
        }
        else {
            printf(" >> EGALITE <<\n\n");
        }
        return 0;
    }


    if (!game->player[0].is_alive) {
        printf(" >> VOUS AVEZ PERDU !! <<\n\n");
        return 0;
    }

    printf(" >> VOUS AVEZ GAGNE !! <<\n\n");
    
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
}

/**
 * gère l'effet de l'explosion sur une case donnée
 * @param game la structure du jeu
 * @param id le numéro du joueur
 * @param i la ligne de la case
 * @param j la colonne de la case
 * @return FALSE si un mur à été percuté, TRUE sinon
 */
boolean compute_explosion(game_t **game, int id, int i, int j) {
    // s'il y a un mur, l'xploison de ce côté s'arrête
    if ((*game)->plateau[i][j] == WALL) {
        return TRUE;
    }
    // s'il y a un obstacle, on l'enlève et on met à jour les compteurs
    if ((*game)->plateau[i][j] == OBSTACLE) {
        (*game)->plateau[i][j] = EMPTY;
        (*game)->player[id].obstacle_cpt++;
        (*game)->nb_obstacles--;
        return FALSE;
    }
    // s'il y a un joueur, on le marque mort
    for (int num = 0; num < (*game)->nb_player; num++) {
        if ((i == (*game)->player[num].posl) && (j == (*game)->player[num].posc)) {
            (*game)->player[num].is_alive = FALSE;
        }
    }
    return FALSE;
}

/**
 * gère les explosions
 * @param game la structure du jeu
 * @param id le numéro du joueur
 */
void explosion(game_t *game, int id) {

    // fait disparaître la bombre
    game->plateau[game->player[id].posl_bomb][game->player[id].posc_bomb] = 0;

    /*
     * calcule des bornes délimitant le rayon d'action de
     * la bombe en prenant en compte la taille du plateau
     */

    // bornes des colonnes
    int col_inf, col_sup;
    col_inf = game->player[id].posc_bomb - game->player[id].n;
    col_sup = game->player[id].posc_bomb + game->player[id].n;
    // respect des indices du plateau
    if (col_inf < 0) {
        col_inf = 0;
    }
    if (col_sup > game->colonnes-1) {
        col_sup = game->colonnes-1;
    }
    // bornes des lignes
    int lgn_inf, lgn_sup;
    lgn_inf = game->player[id].posl_bomb - game->player[id].n;
    lgn_sup = game->player[id].posl_bomb + game->player[id].n;
    // respect des indices du plateau
    if (lgn_inf < 0) {
        lgn_inf = 0;
    }
    if (lgn_sup > game->lignes-1) {
        lgn_sup = game->lignes-1;
    }

    /*
     * destruction des obets dans le rayon de d'action tout 
     * en prenant en compte les murs qui bloquent l'exlosion
     */

    boolean is_blocked = FALSE;
    // explosion horizontale gauche (+ à la postion de la bombre)
    for (int col = game->player[id].posc_bomb; col >= col_inf; col--)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, id, game->player[id].posl_bomb, col);
        }  
    }
    is_blocked = FALSE;
    // explosion horizontale droite
    for (int col = game->player[id].posc_bomb+1; col <= col_sup; col++)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, id, game->player[id].posl_bomb, col);
        }    
    }
    is_blocked = FALSE;
    // explosion verticale haut
    for (int lgn = game->player[id].posl_bomb-1; lgn >= lgn_inf; lgn--)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, id, lgn, game->player[id].posc_bomb);
        }    
    }
    is_blocked = FALSE;
    // explosion verticale bas  
    for (int lgn = game->player[id].posl_bomb+1; lgn <= lgn_sup; lgn++)
    {
        if (!is_blocked) {
            is_blocked = compute_explosion(&game, id, lgn, game->player[id].posc_bomb);
        }              
    }  
}

