
# TETRISTE  
2023-2024  

## Prérequis  
- **GCC** (GNU Compiler Collection) installé sur votre système.  
- **Bibliothèques SDL2** : SDL2, SDL2_image et SDL2_ttf.  

### Installation des dépendances sous Ubuntu/Debian  
Exécutez la commande suivante :  
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`  
```
## Installation  
1. Téléchargez ou clonez le dépôt du projet.  
2. Ouvrez un terminal dans le dossier racine du projet.  

Pour compiler le projet, utilisez la commande :  
```bash
gcc -Wall -I include -o app src/*.c \`sdl2-config --cflags --libs\` -lSDL2_image -lSDL2_ttf -lm`  
```
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
