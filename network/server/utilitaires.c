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
    // choisir le pseudo du joueur
    printf("\nPseudo :  ");
    scanf("%s",&game->player[0].pseudo);
    // envoi du pseudo
    send_infos(game->player[0].pseudo, 20);
    // Choisir le rayon de l'explosion
    do
    {
        printf("Rayon de l'explosion (> 0) : ");
        scanf("%d",&game->player[0].n);
    }
    while (game->player[0].n < 1);
    // envoi du rayon de l'explosion
    char n[5];  
    sprintf(&n, "%d", game->player[0].n);
    send_infos(n, 5);

    // initialisation du joueur 2
    printf("\n ---- JOUEUR GUEST ---- \n");   
    // récupération du pseudo du joueur 2
    printf("\nPseudo :  ");       
    snprintf(&game->player[1].pseudo, 20, "%s", recv_infos(20));
    printf("%s",game->player[1].pseudo);
    // récupération du rayon de l'explosion
    printf("\nRayon de l'explosion (> 0) : "); 
    // atoi : String/ASCII -> int 
    game->player[1].n = atoi(recv_infos(5));
    printf("%d",game->player[1].n);

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
    // allocation de l'espace mémoire du plateau
	game->plateau = (int **)malloc(game->lignes*sizeof(int *));
	for (i=0; i < game->lignes;i++){
		game->plateau[i] = (int *)malloc(game->colonnes*sizeof(int));
	}
    // ouverture du fichier
	f = fopen(file,"r");
    // initialisation du plateau
	i = 0; j = 0;
	while ((ch = getc(f)) != EOF) {
        if (ch != '\n') {
            if (ch == '#') {
                game->plateau[i][j] = WALL;
            }else{
                game->plateau[i][j] = EMPTY;
            }
            j++;
            if (j == game->colonnes) {
                j = 0;
                i++;
            }
        }
	}
    // fermeture du ficher
    fclose(f);
}

/**
 * place les objets aléatoirement sur le plateau
 * @param game la structure du jeu
 */
void init_objects(game_t *game) {

    int i, j;
    // traitements des/du joueur(s)
    for (int id = 0; id < game->nb_player; id++) {
        // recherche des coordonées d'un emplacement libre
        do{
            i = rand()%(game->lignes);
            j = rand()%(game->colonnes);
        }
        while(game->plateau[i][j] != 0);
        // placement du bomberman
        game->player[id].posl = i;
        game->player[id].posc = j;
        game->plateau[i][j] = PLAYER;
    }   
    // calcul du nombre d'obstacle
    game->nb_obstacles = 20 + rand()%10;
    // placement des obstacles
    for (int k = 1; k <  game->nb_obstacles; k++)
    {
        do{
            i = rand()%(game->lignes-2) + 1;
            j = rand()%(game->colonnes-2) + 1;
        }
        while(game->plateau[i][j] != EMPTY);
        game->plateau[i][j] = OBSTACLE;
    }      
}

/**
 * calcule le score d'un joueur
 * @param player la structure d'un joueur
 * @return le score calculé
 */
float calculate_score(player_t player) {
    return (player.bomb_cpt == 0) ? 0.f :  ((float) (player.obstacle_cpt) / (float)(player.bomb_cpt*player.n));
}


