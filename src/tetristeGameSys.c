#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "doubleLinkedList.h"
#include "tetristeGameSys.h"
#include "saveGameSys.h"
#include "renderer.h"
#include "pawn.h"
#include "leaderBoard.h"

GameSys* newGameSys() {
	/*revoie un pointer vers un nouveau GameSys*/

	GameSys* gameSysPtr = malloc(sizeof(GameSys));
	if (gameSysPtr == NULL){
		return NULL;
	}
	
	int nbColor = COLORNUM;
	int nbShape = SHAPENUM;
	int nbNextPawn = NB_NEXTPAWNLIST;

	
	/* On initialise tous les éléments de gameSysPtr*/

	gameSysPtr->boardGame      = newLinkedList();
	gameSysPtr->allocatedPawns = newLinkedList();
	gameSysPtr->nextPawnList   = newLinkedList();


	gameSysPtr->username  = malloc(sizeof(char) * (NB_CARACTMAX_USERNAME + 1));
	gameSysPtr->shapeList = malloc(sizeof(DoubleLinkedList *) * nbShape);
	gameSysPtr->colorList = malloc(sizeof(DoubleLinkedList *) * nbColor);

	if (gameSysPtr->shapeList == NULL || gameSysPtr->colorList == NULL || gameSysPtr->username == NULL){
		gameSys_free(gameSysPtr);
		return NULL;
	}

	for (int i = 0; i < nbShape; i++){
		gameSysPtr->shapeList[i] = newDoubleLinkedList();
	}

	for (int i = 0; i < nbColor; i++){
		gameSysPtr->colorList[i] = newDoubleLinkedList();
	}

	gameSysPtr->score = 0;
	gameSysPtr->nbColor    = nbColor;
	gameSysPtr->nbShape    = nbShape;
	gameSysPtr->nbNextPawn = nbNextPawn;
	gameSysPtr->nbMaxPawn  = NB_MAXPAWN;

	strncpy(gameSysPtr->username, "", NB_CARACTMAX_USERNAME);
	gameSysPtr->username[NB_CARACTMAX_USERNAME] = '\0';

	return gameSysPtr;
}

void gameSys_initNext(GameSys* GS){
	/*initialise la liste des prochains elements a ajouter au plateau*/

	linkedList_clear(GS->nextPawnList);
	
	for (int i = 0; i < GS->nbNextPawn; i++){
		linkedList_appendLast(GS->nextPawnList, gameSys_newRandomPawnHolder(GS));
	}
}

void gameSys_clear(GameSys* GS){
	/*reinitialise le GameSys donné en argument*/

	int nbColor = COLORNUM;
	int nbShape = SHAPENUM;
	int nbNextPawn = NB_NEXTPAWNLIST;


	linkedList_clear(GS->boardGame);
	linkedList_clear(GS->allocatedPawns);
	linkedList_clear(GS->nextPawnList);


	for (int i = 0; i < nbShape; i++){
		doubleLinkedList_clear(GS->shapeList[i]);
	}

	for (int i = 0; i < nbColor; i++){
		doubleLinkedList_clear(GS->colorList[i]);
	}

	GS->score      = 0;
	GS->nbColor    = nbColor;
	GS->nbShape    = nbShape;
	GS->nbNextPawn = nbNextPawn;
	GS->nbMaxPawn  = NB_MAXPAWN;
	
	strncpy(GS->username, "", NB_CARACTMAX_USERNAME);
	GS->username[NB_CARACTMAX_USERNAME] = '\0';
}

Pawn** gameSys_newPawnHolder(GameSys* GS, int colorID, int shapeID){
	/*renvoie un nouveau pawn**, dont les valeur sont passé en argument, alloué sur le tas, place une copie du pointer dans allocatedPawns 
	  pour pouvoir liberer la memoir ultérieurement */

	Pawn** pawnPtrPtr = newPawnHolder(colorID, shapeID);
	
	if (pawnPtrPtr == NULL){
		return NULL;
	}

	linkedList_appendLast(GS->allocatedPawns, pawnPtrPtr);
	return pawnPtrPtr;
}

Pawn** gameSys_newRandomPawnHolder(GameSys* GS){
	/*renvoie un nouveau pawn**, dont les valuers sont generées aleatoirement, alloué sur le tas,
	  place une copie du pointer dans allocatedPawns 
	  pour pouvoir liberer la memoir ultérieurement */

	int rndColorID = rand() % GS->nbColor;
	int rndShapeID = rand() % GS->nbShape;

	Pawn** pawnPtrPtr = gameSys_newPawnHolder(GS, rndColorID, rndShapeID);
	
	if (pawnPtrPtr == NULL){
		return NULL;
	}

	return pawnPtrPtr;
}

void gameSys_displayDebug(GameSys* GS){
	if (GS->username != NULL){
		printf("username: %s\n", GS->username);
	}
	
	printf("GameBoard: \n");
	render_displayPawnLinkedList(GS->boardGame);
	printf("\n");

	for (int i = 0; i < GS->nbShape; i++) {
		printf("Shape list %d:\n", i);
		render_displayPawnDoubleLinkedList(GS->shapeList[i]);
		printf("\n");
	}

	for (int i = 0; i < GS->nbColor; i++) {
		printf("Color list %d:\n", i);
		render_displayPawnDoubleLinkedList(GS->colorList[i]);
		printf("\n");
	}

	printf("next five:\n");
	render_displayPawnLinkedList(GS->nextPawnList);
	printf("\n");
}

void gameSys_setLevel(GameSys* GS, int lvl){
	/*met a jour le nombre de couleur ou de formes disponibles en fonction du niveau*/

	switch (lvl){
	case 0:
		GS->nbColor = 5;
		GS->nbShape = 5;
		GS->nbNextPawn = 5;
		return;

	case 1:
		GS->nbColor = 6;
		GS->nbShape = 5;
		GS->nbNextPawn = 3;
		return;

	case 2:
		GS->nbColor = 6;
		GS->nbShape = 6;
		GS->nbNextPawn = 1;
		return;
	}
}

void gameSys_addPawnLeft(GameSys *GS, Pawn **pawnPtrPtr) {
	/*ajoute un pawn a gauche du plateau ainsi qu'aux sous listes correspondantes*/

	linkedList_appendFirst(GS->boardGame, pawnPtrPtr);

	doubleLinkedList_appendFirst(GS->shapeList[pawn_getShapeId(pawnPtrPtr)],pawnPtrPtr);

	doubleLinkedList_appendFirst(GS->colorList[pawn_getColorId(pawnPtrPtr)],pawnPtrPtr);
}

void gameSys_addPawnRight(GameSys *GS, Pawn **pawnPtrPtr) {
	/*ajoute un pawn a droite du plateau ainsi qu'aux sous listes correspondantes*/

	linkedList_appendLast(GS->boardGame, pawnPtrPtr);

	doubleLinkedList_appendLast(GS->shapeList[pawn_getShapeId(pawnPtrPtr)], pawnPtrPtr);

	doubleLinkedList_appendLast(GS->colorList[pawn_getColorId(pawnPtrPtr)], pawnPtrPtr);
}

void gameSys_play(GameSys* GS, int move, int ShapeColorID){
	/*permet de jouer un coup ajout g/d et decalage formes/couleurs*/

	if (GS->nextPawnList->size == 0){
		gameSys_initNext(GS);
	}

	if (move == 0) {
		if (GS->boardGame->size <= GS->nbMaxPawn){
	    	gameSys_addPawnLeft(GS, gameSys_getNextPawn(GS));
		}
	} else if (move == 1) {
		if (GS->boardGame->size <= GS->nbMaxPawn){
		    gameSys_addPawnRight(GS, gameSys_getNextPawn(GS));
		}
	} else if (move == 2) {
	    gameSys_rotateDoubleLinkedListLeftShape(GS, ShapeColorID);

	} else if (move == 3) {
	    gameSys_rotateDoubleLinkedListLeftColor(GS, ShapeColorID);

	}

	LinkedList* pawnToDelete = gameSys_checkGameBoard(GS);

	int nbDeletionInRow = 0;
	while (pawnToDelete->size > 0){
		
		nbDeletionInRow += 2;
		GS->score += 10*nbDeletionInRow;

		while (pawnToDelete->firstNode){
			gameSys_remove(GS, linkedList_pop(pawnToDelete));
		}

		linkedList_free(pawnToDelete);
		pawnToDelete = gameSys_checkGameBoard(GS);
	}

	linkedList_free(pawnToDelete);
}

int gameSys_isGameOver(GameSys* GS){
	return GS->boardGame->size > GS->nbMaxPawn;
}

void gameSys_remove(GameSys* GS, Pawn** pawnPtrPtr){
	/*supprime un pawn du jeu du plateau ainsi que de ses sous listes correspondantes*/

	int shapeID = pawn_getShapeId(pawnPtrPtr);
	int colorID = pawn_getColorId(pawnPtrPtr);
	
	doubleLinkedList_remove(GS->shapeList[shapeID], *pawnPtrPtr);
	doubleLinkedList_remove(GS->colorList[colorID], *pawnPtrPtr);

	linkedList_remove(GS->boardGame, *pawnPtrPtr);
}

LinkedList* gameSys_checkGameBoard(GameSys* GS){
	/*renvoie une liste chainé des pawn** a supprimé du plateau
	  les pawn** a supprimmer sont les suite de plus de 3 elements partageants la meme couleur ou la meme forme
	*/

	if (GS == NULL){
		return newLinkedList();
	}

	if (GS->boardGame == NULL) {
		return newLinkedList();
	}

	if (GS->boardGame->size == 0){
		return newLinkedList();
	}



	LinkedList* shapeToDelete = newLinkedList();
	LinkedList* colorToDelete = newLinkedList();

	//definissent quel est id couleur/forme de la suite en train de se consruire
	int shapeIDtoDelete = -1;
	int colorIDtoDelete = -1;
	
	//une fois qu'une suite de 3 elements de meme couleurs (resp. formes) est trouvé on arrete la recherche pour les couleurs (resp. formes) 
	int shapeLocked = 0;
	int colorLocked = 0;


	Node* currentNode = GS->boardGame->firstNode;
	do {

		int currentShapeID = pawn_getShapeId(currentNode->pawnPtrPtr);
		int currentColorID = pawn_getColorId(currentNode->pawnPtrPtr);

		if (shapeToDelete->size == 0){
			shapeIDtoDelete = currentShapeID;

		} else if (shapeToDelete->size > 0 && shapeToDelete->size < 3  && currentShapeID != shapeIDtoDelete){
			linkedList_clear(shapeToDelete);
			shapeIDtoDelete = currentShapeID;

		} else if (shapeToDelete->size >= 3 && currentShapeID != shapeIDtoDelete){
			shapeLocked = 1;
		}


		
		if (colorToDelete->size == 0){
			colorIDtoDelete = currentColorID;

		} else if (colorToDelete->size > 0 && colorToDelete->size < 3  && currentColorID != colorIDtoDelete){
			linkedList_clear(colorToDelete);
			colorIDtoDelete = currentColorID;

		} else if (colorToDelete->size >= 3 && currentColorID != colorIDtoDelete){
			colorLocked = 1;
		}

		
		if (!colorLocked){
			linkedList_appendLast(colorToDelete, currentNode->pawnPtrPtr);
		}
		
		if (!shapeLocked){
			linkedList_appendLast(shapeToDelete, currentNode->pawnPtrPtr);
		}

		currentNode = currentNode->next;

	} while (currentNode != GS->boardGame->firstNode);


	//on revoie l'union des liste de couleur et de formes

	LinkedList* pawnToDelete = newLinkedList();

	if (shapeToDelete->size >= 3){
		while (shapeToDelete->firstNode){
			linkedList_appendLast(pawnToDelete, linkedList_pop(shapeToDelete));
		}
	}
	
	if (colorToDelete->size >= 3){
		while (colorToDelete->firstNode){
			Pawn** tempPawn = linkedList_pop(colorToDelete);
			if (!linkedList_isElem(pawnToDelete, tempPawn)){
				linkedList_appendLast(pawnToDelete, tempPawn);
			}
		}
	}

	linkedList_free(colorToDelete);
	linkedList_free(shapeToDelete);
	return pawnToDelete;
}

void gameSys_rotateDoubleLinkedListLeftShape(GameSys* GS, int shapeID){
	/*effectue un decalage a gauche des pawn dans plateau de l'id forme specifé*/

	if (shapeID >= GS->nbShape){
		return;
	}
	if (GS->shapeList[shapeID]->size == 0){
		return;
	}
	
	Pawn* lastPawnToAssign = *GS->shapeList[shapeID]->firstNode->pawnPtrPtr;	
	
	Node* currentNode = GS->shapeList[shapeID]->firstNode;

	while (currentNode != GS->shapeList[shapeID]->lastNode)
	{
		*currentNode->pawnPtrPtr = *currentNode->next->pawnPtrPtr;
		currentNode = currentNode->next;
	}
	
	*GS->shapeList[shapeID]->lastNode->pawnPtrPtr = lastPawnToAssign;

	gameSys_colorListReCompute(GS);
}

void gameSys_rotateDoubleLinkedListLeftColor(GameSys* GS, int colorID){
	/*effectue un decalage a gauche des pawn dans plateau de l'id couleur specifé*/

	
	if (colorID >= GS->nbColor){
		return;
	}

	if (GS->colorList[colorID]->size == 0){
		return;
	}

	Pawn* lastPawnToAssign = *GS->colorList[colorID]->firstNode->pawnPtrPtr;
	
	Node* currentNode = GS->colorList[colorID]->firstNode;

	while (currentNode != GS->colorList[colorID]->lastNode){

		*currentNode->pawnPtrPtr = *currentNode->next->pawnPtrPtr;
		currentNode = currentNode->next;
	}

	*GS->colorList[colorID]->lastNode->pawnPtrPtr = lastPawnToAssign;
	gameSys_shapeListReCompute(GS);
}

void gameSys_shapeListReCompute(GameSys* GS){
	/*calcule toutes les sous listes formes a partir du plateau*/

	for (int i = 0; i < GS->nbShape; i++)
	{
		doubleLinkedList_clear(GS->shapeList[i]);
	}

	if (GS->boardGame->size == 0){
		return;
	}

	Node* currentNode = GS->boardGame->firstNode;

	do{
		int shapeID = pawn_getShapeId(currentNode->pawnPtrPtr);
		doubleLinkedList_appendLast(GS->shapeList[shapeID], currentNode->pawnPtrPtr);
		currentNode = currentNode->next;
		
	} while (currentNode != GS->boardGame->firstNode);
}

void gameSys_colorListReCompute(GameSys* GS){
	/*calcule toutes les sous listes couleurs a partir du plateau*/
	
	for (int i = 0; i < GS->nbColor; i++)
	{
		doubleLinkedList_clear(GS->colorList[i]);
	}

	if (GS->boardGame->size == 0){
		return;
	}

	Node* currentNode = GS->boardGame->firstNode;

	do{
		int colorID = pawn_getColorId(currentNode->pawnPtrPtr);
		doubleLinkedList_appendLast(GS->colorList[colorID], currentNode->pawnPtrPtr);

		currentNode = currentNode->next;
	} while (currentNode != GS->boardGame->firstNode);
}

void gameSys_free(GameSys* GS) {
	/*libere la memoire du jeu ainsi que tous les pawns precedement alloué*/

	if (GS == NULL) {
		return;
	}

	if (GS->colorList != NULL) {
		for (int i = 0; i < COLORNUM; i++) {
			doubleLinkedList_free(GS->colorList[i]);
		}
		free(GS->colorList);
	}
	
	if (GS->shapeList != NULL) {
		for (int i = 0; i < SHAPENUM; i++) {
				doubleLinkedList_free(GS->shapeList[i]);
			}
		free(GS->shapeList);
	}

	if (GS->nextPawnList != NULL){
		linkedList_free(GS->nextPawnList);
	}

	if (GS->boardGame != NULL) {
		linkedList_free(GS->boardGame);
	}

	if (GS->username != NULL){
		free(GS->username);
	}

	if (GS->allocatedPawns != NULL){
		while (GS->allocatedPawns->firstNode != NULL){
			pawnHolder_free(linkedList_pop(GS->allocatedPawns));
		}
		linkedList_free(GS->allocatedPawns);
	}

	free(GS);
}

Pawn** gameSys_getNextPawn(GameSys* GS){
	/*revoie le premier element de nextPawnList soit le prochain element a ajouter au plateau*/

	Pawn** nextP = linkedList_pop(GS->nextPawnList);
	linkedList_appendLast(GS->nextPawnList, gameSys_newRandomPawnHolder(GS));
	return nextP;
}

int getUserInput(char* input, int maxLength) {
	/*recupere l'input utilisateur de maniere plus robuste que scanf("%c...*/

	if (fgets(input, maxLength+1, stdin) == NULL) {
		return -1;
	}

	int inputLength = strlen(input);
	if (inputLength > 0 && input[inputLength - 1] == '\n') {
		input[inputLength - 1] = '\0';
	} else {
		
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
	}

	return 0;
}

int getUserInputInt(int* inputInt, int maxLength) {
	/*recupere l'input utilisateur d'un int de maniere plus robuste que scanf("%d...*/

	char input[maxLength];
	if (fgets(input, maxLength+1, stdin) == NULL) {
		return -1;
	}

	int inputLength = strlen(input);
	if (inputLength > 0 && input[inputLength - 1] == '\n') {
		input[inputLength - 1] = '\0';
	} else {

		int c;
		while ((c = getchar()) != '\n' && c != EOF);
	}

	char* endptr;
	int out = strtol(input, &endptr, 10);
	if (endptr == input){
		return -1;
	}

	*inputInt = out;

	return 0;
}

void gameSys_menuTerm(GameSys* GS, LeaderBoard* LB){
	/*interface terminal du menu*/


	/*
      menu --(0|1)--> jeu
	 |	  |
	LB<--(2)
		
	*/
	char option[100];
	char tmp[1];

	*option = '2'; 
	while (*option == '2'){

		render_clearTerm();
		render_startMenu(GS);
		getUserInput(option, 100);
		if (*option == '2'){

			render_clearTerm();
			
			render_leaderBoard(LB);
			getUserInput(tmp, 1);
		}
	}
	
	while (*option != '1' && *option != '0'){
		render_clearTerm();
		render_startMenu(GS);
		getUserInput(option, 100);

	}

	if (*option == '1'){
		gameSys_InitTerminal(GS, 1);
	}
	else if (*option == '0'){
		gameSys_InitTerminal(GS, 0);
	}
}

void gameSys_InitTerminal(GameSys* GS, int continueLast){
	/*interface terminal du menu de la creation ou non d'une nouvelle partie*/
	
	int out;

	if (continueLast == 1){
		out = saveSys_loadGameSysFromFile(GS);
		if (out != 0){
			printf("erreur lors du chargement de la partie precedente\n");
		}
	}
	
	if (continueLast == 0){
		char Newusername[10000]; //pas bien
		render_clearTerm();
		render_usernameInput();
		
		getUserInput(Newusername, NB_CARACTMAX_USERNAME);
		printf("%s\n", Newusername);
		strncpy(GS->username, Newusername, NB_CARACTMAX_USERNAME);

		render_clearTerm();
		render_level(GS);
		
		int selectedlvl = -1;
		int out = -1;

		while ((selectedlvl < 1 || selectedlvl > 3) && out != 0){
			out = getUserInputInt(&selectedlvl, 10);	
		}
		gameSys_setLevel(GS, selectedlvl - 1);
		gameSys_initNext(GS);
	}
}

void gameSys_gameLoopTerminal(int nbShape, int nbColor){
	/*interface de jeu dans le terminal*/
	
	GameSys* GS = newGameSys();
	LeaderBoard* LB = newLeaderBoard();
	
	saveSys_loadLeaderBoard(LB);

	render_clearTerm();
	gameSys_menuTerm(GS, LB);
	
	int isGameLost = 0;
	int running = 1;

	while (running){

		int moveID = -1;
		int rotateID = -1;
		int out = -1;

		render_clearTerm();
		render_game(GS);
		
		while ((moveID < 0 || moveID > 4) && out != 0){
			out = getUserInputInt(&moveID, 10);	
		}

		if (moveID == 2 || moveID == 3) {
			
			int currentMaxID = (moveID == 2) ? nbColor : nbShape;
		
			render_clearTerm();
			render_gameRotate(GS, moveID == 2 ? 1 : 0);

			out = -1;

			while ((rotateID < 0 || rotateID >= currentMaxID) && out != 0){
				out = getUserInputInt(&rotateID, 1);
			}

		}

		if (moveID == 4){
			running = 0;
		} else {
			gameSys_play(GS, moveID, rotateID);
		}

		if (gameSys_isGameOver(GS)){
			isGameLost = 1;
			running = 0;
		}
	}
	
	render_clearTerm();

	if (!isGameLost){
		saveSys_writeSaveFile(GS);
		printf("partie automatiquement sauvegardé\n");
	}

	leaderBoard_addEntry(LB,  GS->score,  GS->username);
	
	saveSys_writeLeaderBoard(LB);
	
	leaderBoard_free(LB);
	gameSys_free(GS);
}