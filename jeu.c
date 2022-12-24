#include "jeu.h"

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;


//Sous windows utiliser cette vevrsion de lire_clavier. Mettez la fonction lire_clavier
// ci-dessus en commentaire
//Pour windows https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/kbhit?view=vs-2019
 void *lire_clavier(void *arg){
    star_t * star = (star_t *)arg;
    while(1) {
       
        pthread_mutex_lock(&dmutex);
        
        // Joueur 1
        if (GetAsyncKeyState(VK_UP) < 0) {
            star->direction = HAUT;
        } 
        if (GetAsyncKeyState(VK_LEFT) < 0) {
            star->direction = GAUCHE;
        }
        if (GetAsyncKeyState(VK_DOWN) < 0) {
            star->direction = BAS;
        }
        if (GetAsyncKeyState(VK_RIGHT) < 0) {
            star->direction = DROITE;
        }
        if (GetAsyncKeyState(VK_SPACE) < 0) {
            star->plantingBomb = TRUE;
        }

        // Joueur 2
        if (GetAsyncKeyState('Z') < 0) {
            star->direction = HAUT;
        } 
        if (GetAsyncKeyState('Q') < 0) {
            star->direction = GAUCHE;
        }
        if (GetAsyncKeyState('S') < 0) {
            star->direction = BAS;
        }
        if (GetAsyncKeyState('D') < 0) {
            star->direction = DROITE;
        }
        if (GetAsyncKeyState('W') < 0) {
            star->plantingBomb = TRUE;
        }

        pthread_mutex_unlock(&dmutex);     
        Sleep(1);
    }
 }


void calculer_direction(star_t *star) {
                
        star->plateau[star->posl][star->posc] = 0;
        switch ( star->direction )
        {
            case HAUT:
                if (star->plateau[star->posl-1][star->posc] != 1){                
                    star->posl --;
                }
                break;
            case BAS:
                if (star->plateau[star->posl+1][star->posc] != 1){
                     star->posl ++;
                }               
                break;
            case GAUCHE:
                if (star->plateau[star->posl][star->posc-1] != 1){
                    if (star->posc == 0) {
                        star->posc = star->colonnes-1;
                    } else {
                        star->posc --;
                    }                    
                }                
                break;
            case DROITE:
                if (star->plateau[star->posl][star->posc+1] != 1){
                    if (star->posc == 24) {
                        star->posc = 0;
                    } else {
                        star->posc ++;
                    }
                }                
                break;
        }   
        star->plateau[star->posl][star->posc] = 2;
        //printf("\n LIGNE : %d \n",star->posl);
        //printf("\n COLLONE : %d \n",star->posc);
}


void *deplacer_star(void *arg) {
    star_t * star = (star_t *)arg;
    while(1) {
        pthread_mutex_lock(&dmutex);
        calculer_direction(star);
        afficher_plateau(*star);
        star->direction = IDLE;
        pthread_mutex_unlock(&dmutex);
        Sleep(500);
    }
}

int main() {
    star_t star;
    star.direction = IDLE;
    pthread_t anim,clavier;
    srand(time(0));

    char fichier[] = "plateau.txt";

	lire_plateau(&fichier,&star);
    placer_star(&star);

    pthread_create(&anim,NULL,deplacer_star,&star);
    pthread_create(&clavier,NULL,lire_clavier,&star);
    pthread_join(anim,NULL);
	return 0;
}
