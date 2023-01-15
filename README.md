# Bomberman

C'est un jeu bomberman en c sous Windows.

**Lancer le jeu :**
+ installer gcc
+ ouvrir un terminal PowerShell dans le dossier du jeu
+ entrer la commande suivante

```
if ($?) { rm *.o } ; if ($?) { gcc -o ./jeu *.c -lwsock32 } ; if ($?) { ./jeu }
```
