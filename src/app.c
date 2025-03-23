#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>
#include "app.h"
#include "saveGameSys.h"
#include "leaderBoard.h"
#include "renderer.h"
#include "mathFunc.h"
#include "config.h"
#include "gameSysUI.h"
#include "tetristeGameSys.h"

const SDL_Color COLORS_UI[COLORNUM] = {
	{255, 0, 0, 255},     // Red
	{0, 255, 0, 255},     // Green
	{0, 0, 255, 255},     // Blue
	{255, 255, 0, 255},   // Yellow
	{255, 0, 255, 255},   // Magenta
	{0, 255, 255, 255}    // Cyan
};

SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, char* filename) {
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* imageText(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor, SDL_Rect* destRect){

	SDL_Surface* texteSurf = TTF_RenderText_Blended(font, text, textColor);

	if (texteSurf == NULL){
		return NULL;
	}
	
	destRect->w = texteSurf->w;
	destRect->h = texteSurf->h;

	SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, texteSurf);

	SDL_FreeSurface(texteSurf);
	return textTex;
}

App* newApp() {
	App* app = malloc(sizeof(App));
	
	if (app == NULL){
		return NULL;
	}
	
	/*initialistations relatif a SDL*/
	SDL_Window* window = SDL_CreateWindow("TETRISTE", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

	SDL_SetWindowResizable(window, SDL_FALSE);
	if (window == NULL) {
		free(app);
		return NULL;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_DestroyWindow(app->window);
		free(app);
		return NULL;
	}


	app->GSUI = newGameSysUI();
	app->shapesTextures = malloc(sizeof(SDL_Texture*) * SHAPENUM);


	if (app->shapesTextures == NULL){
		SDL_DestroyWindow(app->window);
		SDL_DestroyRenderer(app->renderer);
		free(app);
		return NULL;
	}

	char filename[256];
	for (int i = 0; i < SHAPENUM; i++){
		snprintf(filename, sizeof(filename), "assets/shapes/%d.png", i);
		app->shapesTextures[i] = loadTextureFromFile(renderer,filename);
	}

	app->buttons = malloc(sizeof(SimpleButton*) * NB_BUTTONSMAX);

	if (app->buttons == NULL){
		SDL_DestroyWindow(app->window);
		SDL_DestroyRenderer(app->renderer);
		free(app);
		return NULL;
	}

	app->nbButtons = 0;
	app->window = window;
	app->renderer = renderer;
	app->GS = NULL;
	app->LB = NULL;
	return app;
}

void app_newSimpleButton(App* app, int x, int y, char* idleFilename, char* hoveredFilename,SDL_Color colorMod , int id){
	/*ajoute un boutton a app->buttons des paramètre spécifié*/
	if (app->nbButtons == NB_BUTTONSMAX){
		return;
	}

	SimpleButton* button = malloc(sizeof(SimpleButton));
	if (button == NULL){
		return;
	}

	button->idleTexture = loadTextureFromFile(app->renderer, idleFilename);
	button->hoveredTexture = loadTextureFromFile(app->renderer, hoveredFilename);
	
	SDL_QueryTexture(button->idleTexture, NULL, NULL, &button->width, &button->height);
	
	button->x = x;
	button->y = y;

	button->id = id;
	button->colorMod = colorMod;
	button->isHovered = 0;

	app->buttons[app->nbButtons] = button;
	app->nbButtons++;
}

int app_buttons_update(App* app, int mx, int my, int isMousePressed){
	/*update les textures des buttons en function de la position de la souris 
	  et renvoie l'id d'un button si cliqué*/

	int pressedButtonID = -1;

	for (int i = 0; i < app->nbButtons; i++) {

		SimpleButton* button = app->buttons[i];

		//verify que la souris est au dessus du boutton
		if (mx >= button->x && mx < button->x + button->width &&
			my >= button->y && my < button->y + button->height) {
			button->isHovered = 1;  
		} else {
			button->isHovered = 0; 
		}

		if (isMousePressed && button->isHovered) {
			pressedButtonID = button->id;
		}
	}

	return pressedButtonID;
}

void app_buttons_draw(App* app){
	/*dessine les bouttons*/
	for (int i = 0; i < app->nbButtons; i++) {

		SimpleButton* button = app->buttons[i];
		SDL_Texture* toDrawTexture = button->isHovered ? button->hoveredTexture:
														 button->idleTexture;

		SDL_Rect destRect = {button->x, button->y, button->width, button->height};
		SDL_SetTextureColorMod(toDrawTexture, button->colorMod.r, button->colorMod.g, button->colorMod.b);
		SDL_RenderCopy(app->renderer, toDrawTexture, NULL, &destRect);
	}

	return; 
}

void app_pawnDisplayer_update(App* app){
	/*met a jour la position des pawnDisp en fonction des targets et de la position de la souris*/

	float pushRadius = 30.;
	float pushForce = 2.;

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int i = 0; i < app->GSUI->nbPawnDisp; i++){

		if (!app->GSUI->pawnsDisp[i]->hasTarget){
			continue;
		}

		app->GSUI->pawnsDisp[i]->vx = 0;
		app->GSUI->pawnsDisp[i]->vy = 0;

		float x1 = app->GSUI->pawnsDisp[i]->x;
		float y1 = app->GSUI->pawnsDisp[i]->y;

		float targetx = app->GSUI->pawnsDisp[i]->target->x;
		float targety = app->GSUI->pawnsDisp[i]->target->y;

		float distToTarget = distance(x1, y1, targetx, targety);
		float distToMouse = distance(x1, y1, mouseX, mouseY);

		float normdirToMousex = (mouseX - x1)/distToMouse;
		float normdirToMousey = (mouseY - y1)/distToMouse;

		float normdirx = (targetx - x1)/distToTarget;
		float normdiry = (targety - y1)/distToTarget;

		if (distToTarget > 1.){

			app->GSUI->pawnsDisp[i]->vx += normdirx * distToTarget/30.;
			app->GSUI->pawnsDisp[i]->vy += normdiry * distToTarget/30.;
		}

		if (distToMouse < pushRadius){
			

			app->GSUI->pawnsDisp[i]->vx += -normdirToMousex * pushForce;
			app->GSUI->pawnsDisp[i]->vy += -normdirToMousey * pushForce;


		}




		app->GSUI->pawnsDisp[i]->x += app->GSUI->pawnsDisp[i]->vx;
		app->GSUI->pawnsDisp[i]->y += app->GSUI->pawnsDisp[i]->vy;
	}
}
void app_pawnDisplayer_draw(App* app){
	/*dessine les dispPawns (les images des pions sur l'ecrans)*/
	SDL_Rect rect;

	for (int i = 0; i < app->GSUI->nbPawnDisp; i++){

		if (app->GSUI->pawnsDisp[i]->hasTarget){
			
			int isSmall = app->GSUI->pawnsDisp[i]->target->isInNextPawnList;

			rect.x = (int)app->GSUI->pawnsDisp[i]->x - 25*(1 - isSmall);
			rect.y = (int)app->GSUI->pawnsDisp[i]->y - 25*(1 - isSmall);

			rect.w = isSmall ? app->GSUI->smallPawnWidth  : app->GSUI->pawnWidth;
			rect.h = isSmall ? app->GSUI->smallPawnHeight : app->GSUI->pawnHeight;

			int colorID = app->GSUI->pawnsDisp[i]->pawn->colorId;
			int shapeID = app->GSUI->pawnsDisp[i]->pawn->shapeId;


			
			SDL_SetTextureColorMod(app->shapesTextures[shapeID], COLORS_UI[colorID].r, COLORS_UI[colorID].g, COLORS_UI[colorID].b);
			
			SDL_RenderCopy(app->renderer, app->shapesTextures[shapeID], NULL, &rect);
		}
	}
}



void app_buttons_clear(App* app){
	/*vide et libere la memoire des buttons ds app->buttons*/
	for (int i = 0; i < app->nbButtons; i++){
		SDL_DestroyTexture(app->buttons[i]->idleTexture);
		SDL_DestroyTexture(app->buttons[i]->hoveredTexture);
		free(app->buttons[i]);
	}
	app->nbButtons = 0;
}

void app_main(App* app){
	/*lance le jeu en mode UI*/

	TTF_Init();
	app->GS = newGameSys();
	app->LB = newLeaderBoard();
	saveSys_loadLeaderBoard(app->LB);

	/*  
	    app_scene_menu          
		-> APP_SCENE_QUIT_ID,
		   APP_SCENE_USER_ID,
		   APP_SCENE_LOAD_ID
		  (APP_SCENE_LEAD_ID)
		
		app_scene_leaderBoard   
		-> APP_SCENE_QUIT_ID, 
		   APP_SCENE_GAME_ID

		app_scene_gameloop      
		-> APP_SCENE_QUIT_ID,
		   APP_SCENE_LEAD_ID, 
		   APP_SCENE_SAVE_ID
		
		app_scene_usernameInput 
		-> APP_SCENE_QUIT_ID, 
		   APP_SCENE_LVLS_ID

		app_scene_levels        
		-> APP_SCENE_QUIT_ID, 
		   APP_SCENE_GAME_ID
		
		app_scene_gameOver      
		->APP_SCENE_MENU_ID
	*/

	//app loop
	int sceneOut = APP_SCENE_MENU_ID;
	int running = 1;
	while (running){

		switch (sceneOut){

			case APP_SCENE_QUIT_ID:
				running = 0;
				break;
			
			case APP_SCENE_MENU_ID:
				app->GS = newGameSys();
				sceneOut = app_scene_menu(app);
				break;
			
			case APP_SCENE_LEAD_ID:
				leaderBoard_addEntry(app->LB, app->GS->score, app->GS->username);
				sceneOut = app_scene_leaderboard(app);
				break;

			case APP_SCENE_GAME_ID:
				sceneOut = app_scene_gameloop(app);
				break;

			case APP_SCENE_USER_ID:
				char newUseranme[NB_CARACTMAX_USERNAME];
				sceneOut = app_scene_usernameInput(app, newUseranme, NB_CARACTMAX_USERNAME);
				strncpy(app->GS->username, newUseranme, NB_CARACTMAX_USERNAME);	
				break;

			case APP_SCENE_LVLS_ID:
				int lvl;
				sceneOut = app_scene_levels(app, &lvl);
				gameSys_setLevel(app->GS, lvl);
				gameSys_initNext(app->GS);

				break;

			case APP_SCENE_LOAD_ID:
				if (saveSys_loadGameSysFromFile(app->GS)==0){
					sceneOut = APP_SCENE_GAME_ID;
				}else{
					sceneOut = APP_SCENE_MENU_ID;
				}
				break;

			case APP_SCENE_GAOV_ID:
				gameSys_clear(app->GS);
				sceneOut = app_scene_gameOver(app);
				break;

			case APP_SCENE_SAVE_ID:
				saveSys_writeSaveFile(app->GS);
				sceneOut = APP_SCENE_QUIT_ID;
				break;
		}
	}

	leaderBoard_addEntry(app->LB, app->GS->score, app->GS->username);
	saveSys_writeLeaderBoard(app->LB);
	TTF_Quit();
}


/*les scene affichent dans la fenetre les differentes partie du jeu (leaderboard/ menu / ect )*/

int app_scene_menu(App* app){
	SDL_Color white = {255, 255, 255, 255};
	
	app_buttons_clear(app);

	SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
	SDL_Texture* background = loadTextureFromFile(app->renderer,"assets/backgrounds/menu.png");

	app_newSimpleButton(app, 226, 375, "assets/buttons/newgameA.png", "assets/buttons/newgameB.png", white,  		APP_SCENE_USER_ID);
	app_newSimpleButton(app, 407, 375, "assets/buttons/loadlastA.png", "assets/buttons/loadlastB.png", white, 		APP_SCENE_LOAD_ID);



	int isPressed;
	int buttonPressedID = -1;
	int mouseX, mouseY;
	int returnValue = -1;

	int running = 1;
	SDL_Event event;
	while(running){

		SDL_RenderClear(app->renderer);
		SDL_RenderCopy(app->renderer, background, NULL, &bgRect);


		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
				returnValue = APP_SCENE_QUIT_ID;

			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				isPressed = 1;
			}
		}


		SDL_GetMouseState(&mouseX, &mouseY);

		buttonPressedID = app_buttons_update(app, mouseX, mouseY, isPressed);
		app_buttons_draw(app);

		if (buttonPressedID != -1){
			returnValue = buttonPressedID;
			running = 0;
		}

		SDL_RenderPresent(app->renderer);
		isPressed = 0;
	}
	
	SDL_DestroyTexture(background);
	return returnValue;
}


int app_scene_leaderboard(App* app){
	SDL_Color black = {0, 0, 0, 255};
	SDL_Color white = {255, 255, 255, 255};
	
	app_buttons_clear(app);
	
	TTF_Font* font = TTF_OpenFont("assets/fonts/BigBlueTerm.ttf", 14);
	if (font == NULL){
		printf("ERROR loading font\n");
		return APP_SCENE_QUIT_ID;
	}


	SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
	SDL_Texture* background = loadTextureFromFile(app->renderer,"assets/backgrounds/leaderboard.png");

	app_newSimpleButton(app, 700, 70, "assets/buttons/resumeA.png", "assets/buttons/resumeB.png", white, APP_SCENE_GAME_ID);

	SDL_Texture* entryTexture;

	int isPressed = 0;
	int buttonPressedID = -1;
	int mouseX, mouseY;
	int returnValue = -1;
	SDL_Event event;

	int running = 1;
	while(running){
		SDL_RenderClear(app->renderer);
		SDL_RenderCopy(app->renderer, background, NULL, &bgRect);
		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
				returnValue = APP_SCENE_QUIT_ID;

			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				isPressed = 1;
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		buttonPressedID = app_buttons_update(app, mouseX, mouseY, isPressed);

		if (buttonPressedID != -1){
			returnValue = buttonPressedID;
			running = 0;
		}
 
		app_buttons_draw(app);

		int x = 245;
		int y = 105;
		SDL_Rect dstRect;

		for (int i = 0; i < app->LB->nbEntry; i++){
			
			if (i < 3){
				y = 105 + i*30;
			} else {
				y = 190 + (i - 3)*20;
			}

			char text[] = "............................................";
			snprintf(text, strlen(app->LB->usernames[i]) + 1, "%s", app->LB->usernames[i]);
			text[strlen(app->LB->usernames[i])] = '.';
			snprintf((text+40), 4, "%d", app->LB->scoreList[i]);

			dstRect.x = x;
			dstRect.y = y;
			
			entryTexture = imageText(app->renderer, font, text, black, &dstRect);
			
			SDL_RenderCopy(app->renderer, entryTexture, NULL, &dstRect);
			SDL_DestroyTexture(entryTexture);
		}


		SDL_RenderPresent(app->renderer);
		isPressed = 0;
	}

	return returnValue;
}

int app_scene_usernameInput(App* app, char* username, int sizeUsername){

	SDL_Color black = {0, 0, 0, 255};
	
	app_buttons_clear(app);
	
	TTF_Font* font = TTF_OpenFont("assets/fonts/BigBlueTerm.ttf", 50);
	if (font == NULL){
		printf("ERROR loading font\n");
		return APP_SCENE_QUIT_ID;
	}

	SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
	SDL_Texture* background = loadTextureFromFile(app->renderer,"assets/backgrounds/userinput.png");

	

	int isPressed = 0;
	int buttonPressedID = -1;
	int mouseX, mouseY;
	int returnValue = -1;
	SDL_Event event;

	char text[NB_CARACTMAX_USERNAME] = "";
	SDL_StartTextInput();
	
	int running = 1;
	while(running){

		SDL_RenderClear(app->renderer);
		SDL_RenderCopy(app->renderer, background, NULL, &bgRect);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
				returnValue = APP_SCENE_QUIT_ID;

			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				isPressed = 1;

			} else if (event.type == SDL_TEXTINPUT){
				if (strlen(text) < sizeUsername){
					strcat(text, event.text.text);
				}

			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_BACKSPACE) {
					int sizeText = strlen(text);
					if (sizeText > 0){
						text[sizeText-1] = '\0';
					}
				} else if (event.key.keysym.sym == SDLK_RETURN) {
					returnValue = APP_SCENE_LVLS_ID;
					running = 0;
				}
			}
		}
	


		SDL_GetMouseState(&mouseX, &mouseY);

		buttonPressedID = app_buttons_update(app, mouseX, mouseY, isPressed);

		if (buttonPressedID != -1){
			returnValue = buttonPressedID;
			running = 0;
		}

		app_buttons_draw(app);

		SDL_Rect dstRect;
		SDL_Texture* textTex = imageText(app->renderer, font, text, black, &dstRect);

		if (textTex != NULL){
			dstRect.x = (WIDTH  - dstRect.w) / 2;
			dstRect.y = 220;
		}

		SDL_RenderCopy(app->renderer, textTex, NULL, &dstRect);

		SDL_RenderPresent(app->renderer);
		SDL_DestroyTexture(textTex);
		isPressed = 0;
	}

	TTF_CloseFont(font);
	SDL_StopTextInput();
	
	strncpy(username, text, sizeUsername);
	
	SDL_DestroyTexture(background);

	return returnValue;
}

int app_scene_levels(App* app, int* lvl){
	SDL_Color white = {255, 255, 255, 255};
	
	app_buttons_clear(app);

	SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
	SDL_Texture* background = loadTextureFromFile(app->renderer, "assets/backgrounds/selectdiff.png");
	
	app_newSimpleButton(app, 300, 120, "assets/buttons/cepasdurA.png", "assets/buttons/cepasdurB.png", white, 0);
	app_newSimpleButton(app, 290, 210, "assets/buttons/cunpeuplusdurA.png", "assets/buttons/cunpeuplusdurB.png", white, 1);
	app_newSimpleButton(app, 300, 300, "assets/buttons/cetredurA.png", "assets/buttons/cetredurB.png", white, 2);

	SDL_Event event;

	int returnValue = 0;
	int isPressed = 0;
	int mouseX, mouseY;
	int buttonPressedID = -1;

	int running = 1;
	while (running){
		SDL_RenderClear(app->renderer);
		SDL_RenderCopy(app->renderer, background, NULL, &bgRect);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
				returnValue = APP_SCENE_QUIT_ID;
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				isPressed = 1;
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		buttonPressedID = app_buttons_update(app, mouseX, mouseY, isPressed);

		if (buttonPressedID != -1){
			*lvl = buttonPressedID;
			returnValue = APP_SCENE_GAME_ID;
			running = 0;
		}


		app_buttons_draw(app);
		SDL_RenderPresent(app->renderer);
	}

	SDL_DestroyTexture(background);
	return returnValue;
}

int app_scene_gameOver(App* app){
	SDL_Color white = {255, 255, 255, 255};
	
	app_buttons_clear(app);


	SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
	SDL_Texture* background = loadTextureFromFile(app->renderer,"assets/backgrounds/gameover.png");

	app_newSimpleButton(app, 320, 230, "assets/buttons/menuA.png", "assets/buttons/menuB.png", white, APP_SCENE_MENU_ID);

	int isPressed;
	int buttonPressedID = -1;
	int mouseX, mouseY;
	int returnValue = -1;

	int running = 1;
	SDL_Event event;
	while(running){

		SDL_RenderClear(app->renderer);
		SDL_RenderCopy(app->renderer, background, NULL, &bgRect);


		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
				returnValue = APP_SCENE_QUIT_ID;

			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				isPressed = 1;
			}
		}


		SDL_GetMouseState(&mouseX, &mouseY);

		buttonPressedID = app_buttons_update(app, mouseX, mouseY, isPressed);
		app_buttons_draw(app);

		if (buttonPressedID != -1){
			returnValue = buttonPressedID;
			running = 0;
		}

		SDL_RenderPresent(app->renderer);
		isPressed = 0;
	}
	
	SDL_DestroyTexture(background);
	return returnValue;
}

int app_scene_gameloop(App* app){
	SDL_Color white = {255, 255, 255, 255};
	SDL_Color black = {0, 0, 0, 255};

	TTF_Font* font = TTF_OpenFont("assets/fonts/BigBlueTerm.ttf", 20);
	if (font == NULL){
		printf("ERROR loading font\n");
		return APP_SCENE_QUIT_ID;
	}

	app_buttons_clear(app);

	SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
	SDL_Texture* background = loadTextureFromFile(app->renderer,"assets/backgrounds/game.png");

	app_newSimpleButton(app, 1, 215, "assets/buttons/leftA.png","assets/buttons/leftB.png",  white, 0);
	app_newSimpleButton(app, WIDTH - 52, 215, "assets/buttons/rightA.png","assets/buttons/rightB.png",  white, 1);

	app_newSimpleButton(app, 12, 87, "assets/buttons/quitA.png", "assets/buttons/quitB.png", white, 2);
	app_newSimpleButton(app, 12, 137, "assets/buttons/leaderboardA.png", "assets/buttons/leaderboardB.png", white, 3);

	char filenameidle[256];
	char filenamehover[256];

	for (int i = 0; i < app->GS->nbShape; i++)	{
		snprintf(filenameidle, sizeof(filenameidle), "assets/buttons/%d.png", i);
		snprintf(filenamehover, sizeof(filenamehover), "assets/buttons/h%d.png", i);
		int x = 20  + (i%3)*70;
		int y = 300 + 65*(i >= 3);
		app_newSimpleButton(app, x, y, filenameidle, filenamehover, white, 20 + i);
	}

	for (int i = 0; i < app->GS->nbColor; i++)	{
		int x = WIDTH - (i%3 + 1)*70;
		int y = 300 + 65*(i >= 3);
		SDL_Color color = COLORS_UI[i];
		app_newSimpleButton(app, x, y, "assets/buttons/colortemplate.png",
									   "assets/buttons/hovercolortemplate.png",
										color, 30 + i);
	}

	gameSysUI_update(app->GSUI, app->GS);

	char scoreString[5] = "";
	snprintf(scoreString, 5, "%d", app->GS->score);
	SDL_Rect scoreDstRect = {30, 35, 0, 0};
	SDL_Texture* scoreTex = imageText(app->renderer, font, scoreString, black, &scoreDstRect);
	

	SDL_Rect usernameDstRect = {325, 340, 0, 0};
	SDL_Texture* usernameTex = imageText(app->renderer, font, app->GS->username, black, &usernameDstRect);

	


	int isPressed;
	int buttonPressedID = -1;
	int mouseX, mouseY;
	SDL_Event event;
	int returnValue;


	int running = 1;
	while(running){

		SDL_RenderClear(app->renderer);
		SDL_RenderCopy(app->renderer, background, NULL, &bgRect);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				returnValue = APP_SCENE_QUIT_ID;
				running = 0;

			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				isPressed = 1;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_s) {
					

					gameSys_play(app->GS, rand()%2+2,rand()%6);
					gameSysUI_update(app->GSUI, app->GS);

				}
			} 
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		buttonPressedID = app_buttons_update(app, mouseX, mouseY, isPressed);

		app_pawnDisplayer_update(app);

		if (buttonPressedID == 0) {
			gameSys_play(app->GS, 0, -1);

		} else if (buttonPressedID == 1) {
			gameSys_play(app->GS, 1, -1);

		} else if (buttonPressedID == 2) {
			returnValue = APP_SCENE_SAVE_ID;
			running = 0;

		} else if (buttonPressedID == 3) {
			returnValue = APP_SCENE_LEAD_ID;
			running = 0;

		} else if (buttonPressedID >= 20 && buttonPressedID < 30) {
			gameSys_play(app->GS, 2, buttonPressedID - 20);


		} else if (buttonPressedID >= 30 && buttonPressedID < 40) {
			gameSys_play(app->GS, 3, buttonPressedID - 30);

		}
				

		if (buttonPressedID != -1){
			
			gameSysUI_update(app->GSUI, app->GS);
			snprintf(scoreString, 5, "%d", app->GS->score);
			scoreTex = imageText(app->renderer, font, scoreString, black, &scoreDstRect);

			if (gameSys_isGameOver(app->GS)){
				returnValue = APP_SCENE_GAOV_ID;
				running = 0;
			}

		}
		


		SDL_RenderCopy(app->renderer, scoreTex, NULL, &scoreDstRect);
		SDL_RenderCopy(app->renderer, usernameTex, NULL, &usernameDstRect);

		app_buttons_draw(app);
		app_pawnDisplayer_draw(app);

		SDL_RenderPresent(app->renderer);
		isPressed = 0;
	}

	SDL_DestroyTexture(background);
	return returnValue;
}

void app_quit(App* app){
	/*lebere la memoire de app et des objets que app possede*/
	if (app->GS != NULL){
		gameSys_free(app->GS);
	}

	if (app->LB != NULL){
		leaderBoard_free(app->LB);
	}

	for (int i = 0; i < SHAPENUM; ++i)
	{
		SDL_DestroyTexture(app->shapesTextures[i]);
	}

	for (int i = 0; i < app->nbButtons; ++i)
	{

		SDL_DestroyTexture(app->buttons[i]->idleTexture);
		SDL_DestroyTexture(app->buttons[i]->hoveredTexture);
		free(app->buttons[i]);
	}
	free(app->buttons);

	gameSysUI_free(app->GSUI);
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	
	free(app);
	SDL_Quit();
}