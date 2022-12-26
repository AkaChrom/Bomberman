#include "jeu.h"
/*
 * Lire le plateau du jeu depuis un fichier et remplir la structure star
 */
void lire_plateau(char *fichier,star_t *star) {
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
                star->plateau[i][j] = 1;
            }else{
                star->plateau[i][j] = 0;
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
 * Effacer le contenu de l'écran en mode terminal
 * Check this for more details : http://www.cplusplus.com/articles/4z18T05o/
 */
void clearScreen()
{
    int n;
    for (n = 0; n < 10; n++)
        printf( "\n\n" );
}
/*
 * Afficher le plateau sur l'écran en remplaçant 1 par '#'
 * 0 par ' ' et 2 par '*'
 */
void afficher_plateau(star_t star) {
    clearScreen();

	int i, j;
    for (i=0; i<star.lignes; i++ ){
        for (j=0; j<star.colonnes; j++ ){
            if (star.plateau[i][j] == 1)
                printf("#");
            else if (star.plateau[i][j] == 2)
                printf("B");
            else if (star.plateau[i][j] == 3)
                printf("x");
            else if (star.plateau[i][j] == 4)
                printf("o");    
            else
                printf(" ");
        }
        printf("\n");
    }

}
/*
 * Placer l'étoile dans une case choisie aléatoirement
 * en évitant les ases obstacles.
 */
void init_objects(star_t *star) {
    int i, j;

    do{
        i = rand()%(star->lignes);
        j = rand()%(star->colonnes);
    }
    while(star->plateau[i][j] != 0);

    star->posl = i;
    star->posc = j;
    star->plateau[i][j] = 2;

    int nb_obstacles =  35 + rand()%10;

    for (size_t k = 0; k < nb_obstacles; k++)
    {
        do{
            i = rand()%(star->lignes);
            j = rand()%(star->colonnes);
        }
        while(star->plateau[i][j] != 0);
        star->plateau[i][j] = 3;
    }      
    return 0;
}

/**
 * Gère les explosions
 * @param star 
 */
void explosion(star_t *star) {
    printf("\nEST VIAVANT ? : %d \n", star->is_alive);

    star->plateau[star->posl_bomb][star->posc_bomb] = 0;

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

    boolean is_blocked = FALSE;

    // explosion horizontale gauche
    for (int col = star->posc_bomb; col >= col_inf; col--)
    {
        if (is_blocked) {
            ;
        }        
        else if (star->plateau[star->posl_bomb][col] == 1) {
            is_blocked = TRUE;
        }
        else if (star->plateau[star->posl_bomb][col] == 3) {
            star->plateau[star->posl_bomb][col] = 0;
        }
        else if (star->plateau[star->posl_bomb][col] == 2) {
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
        else if (star->plateau[star->posl_bomb][col] == 1) {
            is_blocked = TRUE;
        }
        else if (star->plateau[star->posl_bomb][col] == 3) {
            star->plateau[star->posl_bomb][col] = 0;
        }
        else if (star->plateau[star->posl_bomb][col] == 2) {
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
        else if (star->plateau[lgn][star->posc_bomb] == 1) {
            is_blocked = TRUE;
        }
        else if (star->plateau[lgn][star->posc_bomb] == 3) {
            star->plateau[lgn][star->posc_bomb] = 0;
        }
        else if (star->plateau[lgn][star->posc_bomb] == 2) {
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
        else if (star->plateau[lgn][star->posc_bomb] == 1) {
            is_blocked = TRUE;
        }
        else if (star->plateau[lgn][star->posc_bomb] == 3) {
            star->plateau[lgn][star->posc_bomb] = 0;
        }
        else if (star->plateau[lgn][star->posc_bomb] == 2) {
            star->is_alive = FALSE;
        }
    }
    
    return 0;
}

void end_game(star_t *star) {

    printf("\n\nFIN DU GAME\n\n");

    return 0;
}

