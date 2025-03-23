#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubleLinkedList.h"
#include "saveGameSys.h"
#include "leaderBoard.h"
#include "config.h"



int saveSys_loadGameSysFromFile(GameSys* GS){
	/*charge le contenue du jeu depuis SAVEDIR/LASTGAMESAVEFILE*/

	char filepath[256];
	snprintf(filepath, 256, "%s/%s", SAVEDIR, LASTGAMESAVEFILE);


	FILE* f = fopen(filepath, "r");
	
	if (f == NULL){ 
		return -1;
	}

	fseek (f, 0, SEEK_END);
    int fileSize = ftell(f);

    if (0 == fileSize) {
        fclose(f);
        return -1;
    }
	fseek (f, 0, SEEK_SET);


	int score;
	int nbColor;
	int nbShape;
  	char username[NB_CARACTMAX_USERNAME+1];
	
	fscanf(f, "%s\n%d\n%d\n%d\n", username, &score, &nbColor, &nbShape);
    
	strncpy(GS->username, username, NB_CARACTMAX_USERNAME);

	GS->score = score;
	GS->nbColor = nbColor;
	GS->nbShape = nbShape;


	// REMPLIR LE BOARDGAME
	int size;

	int ColorID;
	int ShapeID;
	char line[NB_CARACTMAX_LINE];


	char* outc = fgets(line, NB_CARACTMAX_LINE, f);

	if(outc==NULL){
		fclose(f);
		return -1;
	}

	size = strtol(line, NULL, 10);
	
	for(int i = 0; i < size; i++){

		outc = fgets(line, NB_CARACTMAX_LINE, f);
		
		if(outc == NULL){
			fclose(f);
			return -1;
		}

		ColorID = strtol(line, NULL, 10);

		outc = fgets(line, NB_CARACTMAX_LINE, f);

		if(outc == NULL){
			fclose(f);
			return -1;
		}

		ShapeID = strtol(line, NULL, 10);

		gameSys_addPawnRight(GS, gameSys_newPawnHolder(GS, ColorID, ShapeID));
	}

	// REMPLIR LA NEXTPAWNLIST

	linkedList_clear(GS->nextPawnList);

	for(int i = 0; i < NB_NEXTPAWNLIST; i++){
		outc = fgets(line, sizeof(line), f);

		if(outc == NULL){
			fclose(f);
			return -1;
		}

		ColorID = strtol(line, NULL, 10);

		outc = fgets(line, sizeof(line), f);
		
		if(outc == NULL){
			fclose(f);
			return -1;
		}

		ShapeID = strtol(line, NULL, 10);

		linkedList_appendLast(GS->nextPawnList, gameSys_newPawnHolder(GS, ColorID, ShapeID));
	}
	GS->nbNextPawn = GS->nextPawnList->size;
	fclose(f);
	return 0;
}

int saveSys_writeSaveFile(GameSys* GS){
	/*sauvegarde le contenue du jeu dans SAVEDIR/LASTGAMESAVEFILE*/


	char filepath[NB_CARACTMAX_LINE];
	snprintf(filepath, NB_CARACTMAX_LINE, "%s/%s", SAVEDIR, LASTGAMESAVEFILE);

	FILE *fichier = fopen(filepath, "w");
	if (fichier == NULL){
		return -1;
	}

    fprintf(fichier, "%s\n", GS->username);
	fprintf(fichier, "%d\n", GS->score);
	fprintf(fichier, "%d\n", GS->nbColor);
	fprintf(fichier, "%d\n", GS->nbShape);


	int size = GS->boardGame->size;
	fprintf(fichier, "%d\n", size);
	
	if (size > 0){
		Node* currentNode = GS->boardGame->firstNode;

		do {
			fprintf(fichier, "%d\n", pawn_getColorId(currentNode->pawnPtrPtr));
			fprintf(fichier, "%d\n", pawn_getShapeId(currentNode->pawnPtrPtr));
			currentNode = currentNode->next;
		} while(currentNode != GS->boardGame->firstNode);
	}

	if (NB_NEXTPAWNLIST > 0){
		Node* currentNode = GS->nextPawnList->firstNode;

		do {
			fprintf(fichier, "%d\n", pawn_getColorId(currentNode->pawnPtrPtr));
			fprintf(fichier, "%d\n", pawn_getShapeId(currentNode->pawnPtrPtr));
			currentNode = currentNode->next;

		} while(currentNode != GS->nextPawnList->firstNode);
	}
	fclose(fichier);
	return 0;
}

int saveSys_loadLeaderBoard(LeaderBoard* LB){
	/*charge le tableau des score depuis SAVEDIR/LEADERBOARDFILENAME*/

	char filepath[NB_CARACTMAX_LINE];
	snprintf(filepath, NB_CARACTMAX_LINE, "%s/%s", SAVEDIR, LEADERBOARDFILENAME);

	FILE *f = fopen(filepath, "r");

	if (f == NULL){  // On vérifie si le fichier est rempli ou non: s'il n'est pas rempli il s'agit d'une première partie
		return -1;
	}

	// Remplir le LeaderBoard

	char username[NB_CARACTMAX_LINE];
	int userscore;
	char* outc;
	char line[NB_CARACTMAX_LINE];

	for(int i = 0; i < NB_LEADERBOARD; i++){

		outc = fgets(username, NB_CARACTMAX_LINE, f);
		
		if(outc == NULL){
			fclose(f);
			return -1;
		}
		
		outc = fgets(line, NB_CARACTMAX_LINE, f);

		if (outc == NULL){
			fclose(f);
			return -1;
		}

		if (strcmp(username,NULL_USERNAME) == 0){
			break;
		}

		username[strlen(username) - 1] = '\0';

		userscore = strtol(line, NULL, 10);


		//la memoire de Usernames est deja alloué
		strncpy(LB->usernames[i], username, NB_CARACTMAX_USERNAME);
		LB->scoreList[i] = userscore;
		LB->nbEntry++;
	}

	fclose(f);
	return 0;
}

int saveSys_writeLeaderBoard(LeaderBoard* LB){
	/*sauvegarde le tableau des score dans SAVEDIR/LEADERBOARDFILENAME*/


	char filepath[NB_CARACTMAX_LINE];
	snprintf(filepath, NB_CARACTMAX_LINE, "%s/%s", SAVEDIR, LEADERBOARDFILENAME);

	FILE *f = fopen(filepath, "w");
	if (f == NULL){
		return -1;
	}

	for (int i = 0; i < LB->nbEntry; i++){
		
		fprintf(f, "%s\n", LB->usernames[i]);
		fprintf(f, "%d\n", LB->scoreList[i]);
	}

	fclose(f);
	return 0;
}

