#ifndef GAMESYS_UI
#define GAMESYS_UI

#include "pawn.h"

typedef struct {
	/*un target est toujours associé a un pawn du jeu*/

	//pos
	float x, y;

	//indentifie
	Pawn* pawn;

	int hasDisplayer;
	int isInNextPawnList;
	
} TargetPawn;


typedef struct {
	/*un PawnDisplayer represente la piece de jeu sur le plateau*/

	//pos
	float x;
	float y;
	
	//vel
	float vx;
	float vy;

	//indentifie
	Pawn* pawn;

	TargetPawn* target;

	int hasTarget;

} PawnDisplayer;

typedef struct {
	int totalMAXPAWN; //NB_MAXPAWN + NB_NEXTPAWNLIST
	int nbTarget;
	int nbPawnDisp;
	TargetPawn** 	targetsPawns;
	PawnDisplayer** pawnsDisp;

	int pawnWidth;
	int pawnHeight;
	int smallPawnWidth;
	int smallPawnHeight;

} gameSysUI;


gameSysUI* newGameSysUI();
TargetPawn* newTargetPawn(float x, float y, Pawn* pawn, int isInNextPawnList);

void gameSysUI_TargetsClear(gameSysUI* GSUI);
void gameSysUI_updateTargets(gameSysUI* GSUI, GameSys* GS);
void gameSysUI_update(gameSysUI* GSUI, GameSys* GS);


void gameSysUI_free(gameSysUI* GSUI);
#endif