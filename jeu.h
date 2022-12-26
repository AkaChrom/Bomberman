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

enum direction {IDLE,HAUT,GAUCHE,BAS,DROITE};
typedef struct{
    int **plateau;
    int lignes; // Nombre de lignes de plateau
    int colonnes; // Nobre de colonnes de plateau
    int posl; // ligne actuelle de l'étoile
    int posc; // colonne actuelle de l'étoile
    int posl_bomb; // ligne de la bombe
    int posc_bomb; // colonne de labombe
    int n;
    int timer;
    boolean plantingBomb;
    boolean is_alive;
    enum direction direction; // direction actuelle du bomberman
} star_t;

void * lire_clavier(void *arg);
void lire_plateau(char *fichier,star_t *star);
void clearScreen();
void afficher_plateau(star_t star);
void init_objects(star_t *star);
int my_rand(int valMax);

void explosion(star_t *star);
void end_game(star_t *star);

