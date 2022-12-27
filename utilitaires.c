#include "jeu.h"
/*
 * Lire le plateau du jeu depuis un fichier et remplir la structure star
 */
void read_board_game(char *fichier,star_t *star) {
	FILE *f;
	char str[100];
	int i,j;
	char ch;
    star->lignes = 0;
	f = fopen(fichier,"r");
    if (f == NULL){
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
	while (fgets(str,sizeof(str),f) != NULL){
		str[strlen(str)-1] = '\0';
		star->colonnes = strlen(str);
		star->lignes++;
	}
	fclose(f);
	star->plateau = (int **)malloc(star->lignes*sizeof(int *));
	for (i=0; i < star->lignes;i++){
		star->plateau[i] = (int *)malloc(star->colonnes*sizeof(int));
	}
	f = fopen(fichier,"r");
	i = 0; j = 0;
	while ((ch = getc(f)) != EOF) {
        if (ch != '\n') {
            if (ch == '#') {
                star->plateau[i][j] = WALL;
            }else{
                star->plateau[i][j] = EMPTY;
            }
            j++;
            if (j == star->colonnes) {
                j = 0;
                i++;
            }
        }
	}
}


/*
 * Placer l'étoile dans une case choisie aléatoirement
 * en évitant les ases obstacles.
 */
void init_objects(star_t *star) {
    int i, j;
    // recherche des coordonées d'un emplacement libre
    do{
        i = rand()%(star->lignes);
        j = rand()%(star->colonnes);
    }
    while(star->plateau[i][j] != 0);
    // placement du bomberman
    star->posl = i;
    star->posc = j;
    star->plateau[i][j] = PLAYER;

    // calcul du nombre d'obstacle
    int nb_obstacles =  35 + rand()%10;
    // placement des obstacles
    for (size_t k = 0; k < nb_obstacles; k++)
    {
        do{
            i = rand()%(star->lignes);
            j = rand()%(star->colonnes);
        }
        while(star->plateau[i][j] != EMPTY);
        star->plateau[i][j] = OBSTACLE;
    }      
    return 0;
}


/**
 * 
 * @param star 
 * @return float le score calculé
 */
float calculate_score(star_t star) {
    float score = (star.bomb_cpt == 0) ? 0 :  ((float) (star.obstacle_cpt) / (float)(star.bomb_cpt*star.n));
    return score;
}

/**
 * Gère les explosions
 * @param star 
 */
void explosion(star_t *star) {

    // fait disparaître la bombre
    star->plateau[star->posl_bomb][star->posc_bomb] = 0;

    /**
     * calcule des bornes délimitant le rayon d'action de
     * la bombe en prenant en compte la taille du plateau
     */

    // bornes des colonnes
    int col_inf, col_sup;
    col_inf = star->posc_bomb - star->n;
    col_sup = star->posc_bomb + star->n;
    // respect des indices du plateau
    if (col_inf < 0) {
        col_inf = 0;
    }
    if (col_sup > star->colonnes-1) {
        col_sup = star->colonnes-1;
    }
    // bornes des lignes
    int lgn_inf, lgn_sup;
    lgn_inf = star->posl_bomb - star->n;
    lgn_sup = star->posl_bomb + star->n;
    // respect des indices du plateau
    if (lgn_inf < 0) {
        lgn_inf = 0;
    }
    if (lgn_sup > star->lignes-1) {
        lgn_sup = star->lignes-1;
    }

    /**
     * destruction des obets dans le rayon de d'action tout 
     * en prenant en compte les murs qui bloquent l'exlosion
     */

    boolean is_blocked = FALSE;
    // explosion horizontale gauche
    for (int col = star->posc_bomb; col >= col_inf; col--)
    {
        if (is_blocked) {
            ;
        }        
        else if (star->plateau[star->posl_bomb][col] == WALL) {
            is_blocked = TRUE;
        }
        else if (star->plateau[star->posl_bomb][col] == OBSTACLE) {
            star->plateau[star->posl_bomb][col] = EMPTY;
            star->obstacle_cpt++;
        }
        else if ((star->posl_bomb == star->posl) && (star->posc_bomb == col)) {
            star->is_alive = FALSE;
        }
    }
    is_blocked = FALSE;
    // explosion horizontale droite
    for (int col = star->posc_bomb+1; col <= col_sup; col++)
    {
        if (is_blocked) {
            ;
        }        
        else if (star->plateau[star->posl_bomb][col] == WALL) {
            is_blocked = TRUE;
        }
        else if (star->plateau[star->posl_bomb][col] == OBSTACLE) {
            star->plateau[star->posl_bomb][col] = EMPTY;
            star->obstacle_cpt++;
        }
        else if ((star->posl_bomb == star->posl) && (star->posc_bomb == col)) {
            star->is_alive = FALSE;
        }
    }
    is_blocked = FALSE;
    // explosion verticale haut
    for (int lgn = star->posl_bomb-1; lgn >= lgn_inf; lgn--)
    {
        if (is_blocked) {
            ;
        }        
        else if (star->plateau[lgn][star->posc_bomb] == WALL) {
            is_blocked = TRUE;
        }
        else if (star->plateau[lgn][star->posc_bomb] == OBSTACLE) {
            star->plateau[lgn][star->posc_bomb] = EMPTY;
            star->obstacle_cpt++;
        }
        else if ((lgn == star->posl) && (star->posc_bomb == star->posc)) {
            star->is_alive = FALSE;
        }
    }
    is_blocked = FALSE;
    // explosion verticale bas
    for (int lgn = star->posl_bomb+1; lgn <= lgn_sup; lgn++)
    {
        if (is_blocked) {
            ;
        }        
        else if (star->plateau[lgn][star->posc_bomb] == WALL) {
            is_blocked = TRUE;
        }
        else if (star->plateau[lgn][star->posc_bomb] == OBSTACLE) {
            star->plateau[lgn][star->posc_bomb] = EMPTY;
            star->obstacle_cpt++;
        }
        else if ((lgn == star->posl) && (star->posc_bomb == star->posc)) {
            star->is_alive = FALSE;
        }
    }    
    return 0;
}

void end_game(star_t *star) {

    printf("\n\nPARTIE TERMINÉ\n\n");

    // creation d'un pointeur pour le fichier
    FILE *fptr;
    // ouverture du fichier en mode "append"
    fptr = fopen("Scores.txt","a");  
    // récupération de la date actuelle
    time_t t;   // not a primitive datatype
    time(&t);   
    // ajout du score
    fprintf(fptr,"%s | %.2f | %s",star->pseudo,calculate_score(*star),ctime(&t));
    // fermeture du fichier
    fclose(fptr);

    return 0;
}

