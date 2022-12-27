#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include <windows.h>
#include <conio.h>

#define TRUE 1
#define FALSE 0

// objets du plateau
#define EMPTY 0
#define WALL 1
#define PLAYER 2
#define OBSTACLE 3
#define BOMB 4

enum direction {IDLE,UP,LEFT,DOWN,RIGHT};

typedef struct{
    // plateau
    int **plateau;
    int lignes; // Nombre de lignes de plateau
    int colonnes; // Nobre de colonnes de plateau
    // position
    int posl; // ligne actuelle de l'étoile
    int posc; // colonne actuelle de l'étoile
    int posl_bomb; // ligne de la bombe
    int posc_bomb; // colonne de la bombe
    int timer;
    int n;
    int bomb_cpt;
    int obstacle_cpt;
    char pseudo[20];
    boolean plantingBomb;
    boolean is_alive;
    enum direction direction; // direction actuelle du bomberman
} star_t;

void * read_keyboard(void *arg);
void read_board_game(char *fichier,star_t *star);
void clear_screen();
void display_game(star_t star);
void init_objects(star_t *star);
int my_rand(int valMax);

void explosion(star_t *star);
void end_game(star_t *star);

