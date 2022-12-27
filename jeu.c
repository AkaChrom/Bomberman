#include "jeu.h"

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * 
 * @param arg 
 * @return void* 
 */
 void *read_keyboard(void *arg){
    star_t * star = (star_t *)arg;
    while(1) {
       
        pthread_mutex_lock(&dmutex);
        
        // Joueur 1
        if (GetAsyncKeyState(VK_UP) < 0) {
            star->direction = UP;
        } 
        if (GetAsyncKeyState(VK_LEFT) < 0) {
            star->direction = LEFT;
        }
        if (GetAsyncKeyState(VK_DOWN) < 0) {
            star->direction = DOWN;
        }
        if (GetAsyncKeyState(VK_RIGHT) < 0) {
            star->direction = RIGHT;
        }
        if (GetAsyncKeyState(VK_SPACE) < 0) {
            if (star->timer == 0 ) {
                star->plantingBomb = TRUE;
            }            
        }

        // Joueur 2
        if (GetAsyncKeyState('Z') < 0) {
            star->direction = UP;
        } 
        if (GetAsyncKeyState('Q') < 0) {
            star->direction = LEFT;
        }
        if (GetAsyncKeyState('S') < 0) {
            star->direction = DOWN;
        }
        if (GetAsyncKeyState('D') < 0) {
            star->direction = RIGHT;
        }
        if (GetAsyncKeyState('W') < 0) {
             if (star->timer == 0 ) {
                star->plantingBomb = TRUE;
            }    
        }

        pthread_mutex_unlock(&dmutex);     
        Sleep(1);
    }
 }


void calculate_direction(star_t *star) {

    if (star->plateau[star->posl][star->posc] != BOMB) {
        star->plateau[star->posl][star->posc] = EMPTY;
    }   

    switch ( star->direction )
    {
        case UP:
            if (star->plateau[star->posl-1][star->posc] == EMPTY){                
                star->posl --;
            }
            break;
        case DOWN:
            if (star->plateau[star->posl+1][star->posc] == EMPTY){
                    star->posl ++;
            }               
            break;
        case LEFT:
            if (star->plateau[star->posl][star->posc-1] == EMPTY){
                if (star->posc == 0) {
                    star->posc = star->colonnes-1;
                } else {
                    star->posc --;
                }                    
            }                
            break;
        case RIGHT:
            if (star->plateau[star->posl][star->posc+1] == EMPTY){
                if (star->posc == star->colonnes-1) {
                    star->posc = 0;
                } else {
                    star->posc ++;
                }
            }                
            break;
    }   

    star->plateau[star->posl][star->posc] = PLAYER;        
}


void *update_game(void *arg) {
    star_t * star = (star_t *)arg;
    while(1) {
        pthread_mutex_lock(&dmutex);

        if (star->timer > 0) {
            if (--star->timer == 0) {
                explosion(star);
            }     
        }
        
        if (star->plantingBomb) {
            star->posl_bomb = star->posl;
            star->posc_bomb = star->posc;
            star->timer = star->n *2;
            star->plateau[star->posl_bomb][star->posc_bomb] = BOMB;
            star->bomb_cpt++;
            star->plantingBomb = FALSE;
        } 
        else if (star->direction != IDLE) {
            calculate_direction(star);
        }          
    
        display_game(*star);
        star->direction = IDLE;

        if (star->is_alive == FALSE) {
            end_game(star);
            return 0;
        }
      
        pthread_mutex_unlock(&dmutex);

        Sleep(500);
    }
}

int main() {

    clear_screen();

    star_t star;
    star.direction = IDLE;
    star.n = 3;
    star.timer = 0;
    star.plantingBomb = FALSE;
    star.is_alive = TRUE;
    star.bomb_cpt = 0;
    star.obstacle_cpt = 0;

    printf("Choisir un pseudo : ");
    scanf("%s",star.pseudo);

    pthread_t anim,clavier;
    srand(time(0));

    char fichier[] = "plateauF.txt";

	read_board_game(&fichier,&star);
    init_objects(&star);

    pthread_create(&anim,NULL,update_game,&star);
    pthread_create(&clavier,NULL,read_keyboard,&star);
    pthread_join(anim,NULL);
	return 0;
}
