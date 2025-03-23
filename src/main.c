#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include "app.h"
#include "leaderBoard.h"
#include "tetristeGameSys.h"




int main(int argc, char* argv[]) {

	srand(time(NULL));

	if (argc > 1 && strcmp(argv[1], "-g") == 0) {

		//initialise SDL
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			SDL_Quit();
			return -1;
		}
		
		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" );

		App* app = newApp();

		if (app == NULL){
			return -1;
		}

		app_main(app);
		app_quit(app);

	} else {
		gameSys_gameLoopTerminal(6, 6);
	}

	return 0;
}