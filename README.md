# Bomberman

C'est un jeu bomberman en c.

(Marche sous Windows).

**Lancer le jeu :**
+ installer gcc
+ ouvrir un terminal dans le dossier du jeu
+ entrer la commande suivante
```
if ($?) { rm *.o } ; if ($?) { gcc -o ./jeu *.c -lwsock32 } ; if ($?) { ./jeu }
```