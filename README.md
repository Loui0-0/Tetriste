
# TETRISTE  
**Projet de Programmation en C**  

Louis de Domingo et Julien Amiel  

2023-2024  

## Description  
TETRISTE est un jeu de puzzle inspiré du célèbre Tetris, enrichi de mécaniques originales et d'une interface moderne. Le projet combine des défis logiques avec une gestion dynamique des pièces, des niveaux de difficulté ajustables et un système de classement. Développé en langage C, il offre deux modes d'interaction : un terminal stylisé et une interface graphique via SDL.  

## Prérequis  
- **GCC** (GNU Compiler Collection) installé sur votre système.  
- **Bibliothèques SDL2** : SDL2, SDL2_image et SDL2_ttf.  

### Installation des dépendances sous Ubuntu/Debian  
Exécutez la commande suivante :  
`sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`  

## Installation  
1. Téléchargez ou clonez le dépôt du projet.  
2. Ouvrez un terminal dans le dossier racine du projet.  

Pour compiler le projet, utilisez la commande :  
`gcc -Wall -I include -o app src/*.c \`sdl2-config --cflags --libs\` -lSDL2_image -lSDL2_ttf -lm`  

## Utilisation  
- **Mode terminal** (par défaut) : Exécutez `./app`.  
- **Mode graphique** : Ajoutez l'option `-g` avec la commande `./app -g`.  

Le jeu propose :  
- Trois niveaux de difficulté ajustant le nombre de formes, couleurs et pièces prévisualisées.  
- Un système de sauvegarde automatique des parties et du classement.  
- Une interface intuitive avec des contrôles affichés en temps réel.  

## Licence  
Ce projet est distribué sous licence [MIT](https://fr.wikipedia.org/wiki/Licence_MIT).  

## Auteurs  
- **Julien Amiel**  
- **Louis de Domingo**  
