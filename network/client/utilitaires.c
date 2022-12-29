#include "jeu.h"

/**
 * initialise le(s) joueur(s)
 * @param game la structure du jeu
 */
void init_players(game_t *game) {  

    clear_screen();
    // initialisation du nombre de joueur à 2
    game->nb_player = 2;
    // allocation de l'espace mémoire
    game->player = (player_t *)malloc(game->nb_player*sizeof(player_t));

    // initialisation du joueur 1
    printf("\n ---- JOUEUR HOST ---- \n");   
    // récupération du pseudo du joueur 2
    printf("\nPseudo :  ");       
    snprintf(&game->player[0].pseudo, 20, "%s", recv_infos(20));
    printf("%s",game->player[0].pseudo);
    // récupération du rayon de l'explosion
    printf("\nRayon de l'explosion (> 0) : "); 
    // atoi : String/ASCII -> int 
    game->player[1].n = atoi(recv_infos(5));
    printf("%d\n",game->player[1].n);
    
    // initialisation du joueur 2
    printf("\n ---- JOUEUR GUEST ---- \n");
    // choisir le pseudo du joueur
    printf("\nPseudo :  ");
    scanf("%s",&game->player[1].pseudo);
    // envoi du pseudo
    send_infos(game->player[1].pseudo, 20);
    // Choisir le rayon de l'explosion
    do
    {
        printf("Rayon de l'explosion (> 0) : ");
        scanf("%d",&game->player[1].n);
    }
    while (game->player[1].n < 1);
    // envoi du rayon de l'explosion
    char n[5];  
    sprintf(&n, "%d", game->player[1].n);
    send_infos(n, 5);  
    
    // pour tous les joueurs
    for (int id = 0; id < game->nb_player; id++) {
        // initialises les autres paramètres par défaut   
        game->player[id].timer = 0;
        game->player[id].bomb_cpt = 0;
        game->player[id].obstacle_cpt = 0;
        game->player[id].planting_bomb = FALSE;
        game->player[id].is_alive = TRUE;
        game->player[id].direction = IDLE;
    }
    // effet de chargement (pour le style)
    printf("\n\n > LANCEMENT DE LA PARTIE ");  
    for (int k = 0; k < 3; k++)
    {
        printf(".");
        Sleep(500);
    }    
    clear_screen();
}

/**
 * lit le plateau du jeu depuis un fichier et remplit la structure du jeu
 * @param file non du fichier contenant le plateau
 * @param game la structure du jeu
 */
void read_board_game(char *file, game_t *game) {
    // pointeur du fichier
	FILE *f;
	char str[100];
	int i,j;
	char ch;
    game->lignes = 0;
    // ouverture du fichier
	f = fopen(file,"r");
    if (f == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",file);
        exit(-1);
    }
    // récupération de la taille de plateau
	while (fgets(str,sizeof(str),f) != NULL){
		str[strlen(str)-1] = '\0';
		game->colonnes = strlen(str);
		game->lignes++;
	}
    // fermeture du ficher
	fclose(f);
}
