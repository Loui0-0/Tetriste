#TETRISTE

## Dépendances

SDL 2.0.20 SDL2_ttf SDL2_image 

## Installation des dépendances

Assurez-vous d'avoir installé les bibliothèques SDL2, SDL2_image, et SDL2_ttf sur votre système.

### Sur Ubuntu/Debian

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

## Compilation

Compilez le projet avec GCC en utilisant la commande suivante :

```bash
gcc -Wall -g -I include -o app src/*.c `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lm
```

Assurez-vous d'être dans le répertoire racine du projet avant de lancer cette commande.

## Exécution

L'application est lancé en mode terminal par defaut l'option -g
lance en mode graphique

```bash
./app -g
```