#ifndef APP
#define APP

#include <SDL.h>
#include "tetristeGameSys.h"
#include "leaderBoard.h"

#include "gameSysUI.h"

typedef struct {
	/*represente un buttons permettant de simple interfaces graphiques*/

	int x, y;
	int width, height;
	
	int isHovered;

	SDL_Color colorMod;
	SDL_Texture* idleTexture;
	SDL_Texture* hoveredTexture;

	int id;
} SimpleButton;

typedef struct {
	/*envellope SDL2 dans une structure en y ajoutant des
	 fonctionalité propre a de l'UI basique et TETRISTE*/


	SDL_Window* window;
	SDL_Renderer* renderer;
	
	gameSysUI* GSUI;
	GameSys* GS;
	LeaderBoard* LB;
	
	int nbButtons;
	SimpleButton** buttons;
	
	SDL_Texture** shapesTextures;

} App;



App* newApp();

SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, char* filename) ;

void app_newSimpleButton(App* app, int x, int y, char* idleFilename, char* hoveredFilename,SDL_Color colorMod , int id);

int app_buttons_update(App* app, int mx, int my, int isMousePressed);

void app_buttons_draw(App* app);

void app_displayTargets(App* app);

void app_pawnDisplayer_draw(App* app);

void app_pawnDisplayer_update(App* app);

void app_buttons_clear(App* app);

void app_main(App* app);

int app_scene_leaderboard(App* app);

int app_scene_levels(App* app, int* lvl);

int app_scene_menu(App* app);

int app_scene_usernameInput(App* app, char* username, int sizeUsername);

int app_scene_gameloop(App* app);

int app_scene_gameOver(App* app);


void app_quit(App* app);


#endif

