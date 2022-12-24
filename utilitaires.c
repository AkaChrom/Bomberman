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
void placer_star(star_t *star) {
    int i, j;
    do{
        i =  rand()%(star->lignes);
        j = rand()%(star->colonnes);
    }
    while(star->plateau[i][j] == 1);

    star->posl = i;
    star->posc = j;
    star->plateau[i][j] = 2;
    return 0;
}


