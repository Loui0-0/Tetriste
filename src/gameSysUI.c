#include <SDL.h>
#include "app.h"
#include "config.h"
#include "doubleLinkedList.h"
#include "tetristeGameSys.h"

gameSysUI* newGameSysUI(){
	gameSysUI* GSUI = malloc(sizeof(gameSysUI));
	if (GSUI == NULL){
		return NULL;
	}

	GSUI->totalMAXPAWN = NB_MAXPAWN + NB_NEXTPAWNLIST;

	GSUI->targetsPawns = malloc(sizeof(TargetPawn*) * GSUI->totalMAXPAWN);
	GSUI->pawnsDisp    = malloc(sizeof(PawnDisplayer*) * GSUI->totalMAXPAWN);
	

	if (GSUI->targetsPawns == NULL){
		free(GSUI);
		return NULL;
	}


	for (int i = 0; i < GSUI->totalMAXPAWN; i++)
	{
		GSUI->targetsPawns[i] = NULL;
		GSUI->pawnsDisp[i]    = NULL;
	}

	GSUI->pawnWidth  = 50;
	GSUI->pawnHeight = 50;
	GSUI->smallPawnWidth  = 35;
	GSUI->smallPawnHeight = 35;

	GSUI->nbTarget   = 0;
	GSUI->nbPawnDisp = 0;
	return GSUI;
}

TargetPawn* newTargetPawn(float x, float y, Pawn* pawn, int isInNextPawnList){
	TargetPawn* target = malloc(sizeof(TargetPawn));
	if (target == NULL){
		return NULL;
	}

	target->hasDisplayer = 0;
	target->x = x;
	target->y = y;
	target->pawn = pawn;
	target->isInNextPawnList = isInNextPawnList;
	return target;
}

PawnDisplayer* newPawnsDisp(float x, float y, TargetPawn* target){
	PawnDisplayer* pawnDisp = malloc(sizeof(PawnDisplayer));
	if (pawnDisp == NULL){
		return NULL;
	} 
	pawnDisp->x = x;
	pawnDisp->y = y;

	pawnDisp->vx = 0;
	pawnDisp->vy = 0;

	pawnDisp->target = target;
	pawnDisp->hasTarget = 1;
	pawnDisp->pawn = target->pawn;

	return pawnDisp;
}

void gameSysUI_TargetsClear(gameSysUI* GSUI){
	for (int i = 0; i < GSUI->totalMAXPAWN; i++)
	{
		if (GSUI->targetsPawns[i] != NULL){
			// free(GSUI->targetsPawns[i]);
			GSUI->targetsPawns[i] = NULL;
		}

	}
	GSUI->nbTarget = 0;
}

void gameSysUI_LinkedListToTargets(gameSysUI* GSUI, LinkedList* list, float minX, float maxX, float y, int isSmall){
	/*transforme une list chainé de pawn en tableau de cible et l'ajoute a GSUI->targetsPawns*/
 
	if (list->size == 0 || (GSUI->nbTarget + list->size > NB_NEXTPAWNLIST + NB_MAXPAWN)){
		return;
	}

	float sepX = (maxX-minX)/list->size;


	Node* currentNode = list->firstNode;	
	for (int i = 0; i < list->size; i++)
	{
		float x = minX + sepX*i + sepX/2.;

		GSUI->targetsPawns[GSUI->nbTarget] = newTargetPawn(x, y, *currentNode->pawnPtrPtr, isSmall);
		GSUI->nbTarget++;
		currentNode = currentNode->next;
	}
}

void gameSysUI_updateTargets(gameSysUI* GSUI, GameSys* GS){
	/*vide les cible et reremplit avec les nouvelles valeurs de boardGame et nextPawnList*/

	gameSysUI_TargetsClear(GSUI);
	gameSysUI_LinkedListToTargets(GSUI, GS->boardGame, 50, WIDTH - 50, HEIGHT / 2, 0);
	gameSysUI_LinkedListToTargets(GSUI, GS->nextPawnList, 634, 806, 105, 1);
}

void pawnDisplayer_setTarget(PawnDisplayer* pawnDisp, TargetPawn* target){
	/*lie un Pawndisplayer et une target*/
	pawnDisp->target = target;
	pawnDisp->hasTarget = 1;
	target->hasDisplayer = 1;
}

void gameSysUI_removeUnLinkedDisp(gameSysUI* GSUI){
	/*nettoie pawnsDisp des displayer n'ayant plus de cible a atteindre*/
	int newSize = 0;

	for (int i = 0; i < GSUI->nbPawnDisp; i++){

		if (GSUI->pawnsDisp[i]->hasTarget){
			GSUI->pawnsDisp[newSize] = GSUI->pawnsDisp[i];
			newSize++;
		} else {
			free(GSUI->pawnsDisp[i]);
		}
	}
	GSUI->nbPawnDisp = newSize;
}

void gameSysUI_updatePawnsDisp(gameSysUI* GSUI){
	/*
		met a jour les cible des pawnDisp et en crée de nouvelles si neccessaire
		si aucune cible n'est trouvé marque le pawnDisp comme tel (hasTarget)
	*/

			
	for (int i = 0; i < GSUI->nbPawnDisp; i++){
		GSUI->pawnsDisp[i]->hasTarget = 0;
		
		for (int j = 0; j < GSUI->nbTarget; j++){
			if (GSUI->pawnsDisp[i]->pawn == GSUI->targetsPawns[j]->pawn && !GSUI->targetsPawns[j]->hasDisplayer){
				pawnDisplayer_setTarget(GSUI->pawnsDisp[i], GSUI->targetsPawns[j]);
			}
		 }
	}
	
	for (int i = 0; i < GSUI->nbTarget; i++){
		if (!GSUI->targetsPawns[i]->hasDisplayer){
			GSUI->pawnsDisp[GSUI->nbPawnDisp] = newPawnsDisp(WIDTH, 0, GSUI->targetsPawns[i]);
			GSUI->nbPawnDisp++;
		}
	}
}


void gameSysUI_update(gameSysUI* GSUI, GameSys* GS){
	/*met a jour les cible et les displayer*/
	gameSysUI_updateTargets(GSUI, GS);
	gameSysUI_updatePawnsDisp(GSUI);
	gameSysUI_removeUnLinkedDisp(GSUI);

}

void gameSysUI_free(gameSysUI* GSUI){
	/*lebere la memoire de GSUI et des elements qu'il contien*/
	for (int i = 0; i < GSUI->nbTarget; i++)
	{
		free(GSUI->targetsPawns[i]);
	}
	
	for (int i = 0; i < GSUI->nbPawnDisp; i++)
	{
		free(GSUI->pawnsDisp[i]);
	}
	

	free(GSUI->targetsPawns);
	free(GSUI->pawnsDisp);

	free(GSUI);
}