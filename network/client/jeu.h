#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include <windows.h>
#include <conio.h>

#include <winsock2.h>

// port d'écoute
#define PORT 777
// port d'envoi
#define PORT_D 666

#define TRUE 1
#define FALSE 0

// objets du plateau
#define EMPTY 0
#define WALL 1
#define PLAYER 2
#define OBSTACLE 3
#define BOMB 4

enum direction {IDLE,UP,LEFT,DOWN,RIGHT};

/**
 * structure qui représente un joueur
 */
typedef struct{   
    // pseudo du joueur
    char pseudo[20];
    // position du joueur
    int posl;
    int posc;
    // position de la bombe
    int posl_bomb;
    int posc_bomb;
    // rayon de l'explosion
    int n;
    // temps avant que la bombe explose
    int timer;
    // nombre de bombes posées
    int bomb_cpt;
    // nombre d'obstacles détruits
    int obstacle_cpt;
    // vrai si le joueur vient de planter une bombe
    boolean planting_bomb;
    // vrai si le joueur est en vie
    boolean is_alive;
    // direction du joueur
    enum direction direction; // direction actuelle du bomberman
} player_t;


/**
 * structure qui représente le jeu
 */
typedef struct{
    // plateau du jeu
    int **plateau;
    // nombre de lignes de plateau
    int lignes;
    // nombre de colonnes de plateau
    int colonnes;
    // nombre d'obstacles sur le plateau
    int nb_obstacles;
    // nombre de joueurs
    int nb_player;
    // joueurs
    player_t *player;
} game_t;


// utilitaire.c

void init_players(game_t *game);
void read_board_game(char *fichier,game_t *game);

// affichage.c

void clear_screen();
void display_game(game_t game);

// traitement.c

void end_game(game_t *game);

// network.c

void start_server();
void stop_server();
void send_infos(char* msg, int len);
char* recv_infos(int len);


