#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "leaderBoard.h"
#include "config.h"

LeaderBoard* newLeaderBoard(){
	LeaderBoard* LB = malloc(sizeof(LeaderBoard));
	
	if (LB == NULL){
		return NULL;
	}

	for (int i = 0; i < NB_LEADERBOARD; i++)
	{
		LB->scoreList[i] = 0;
		LB->usernames[i] = malloc(sizeof(char) * (NB_CARACTMAX_USERNAME + 1));
		strncpy(LB->usernames[i], "", NB_CARACTMAX_USERNAME);
		LB->usernames[i][NB_CARACTMAX_USERNAME] = '\0'; 
	}

	LB->nbEntry = 0;

	return LB;
}

int leaderBoard_getIndexOfUsername(LeaderBoard* LB, char* username){
	/*revoie l'index de username dans LB->usernames -1 sinon*/
	if (username == NULL){
		return -1;
	}
	for (int k = 0; k < NB_LEADERBOARD; k++){
		if (strcmp(LB->usernames[k], username) == 0){
			return k;
		}
	}
	return -1;
}


void leaderBoard_sort(LeaderBoard *LB) {
	/*trie le tableau des score en fonction des scores*/
	
	for (int i = 0; i < LB->nbEntry - 1; i++) {

		for (int j = 0; j < LB->nbEntry - i - 1; j++) {

			if (LB->scoreList[j] < LB->scoreList[j + 1]) {
				
				int tmpScore = LB->scoreList[j];
				LB->scoreList[j] = LB->scoreList[j + 1];
				LB->scoreList[j + 1] = tmpScore;

				
				char *tmpUsername = LB->usernames[j];
				LB->usernames[j] = LB->usernames[j + 1];
				LB->usernames[j + 1] = tmpUsername;
			}
		}
	}
}

void leaderBoard_addEntry(LeaderBoard* LB, int score, char* username){
	/*essaie d'ajouter au tableau des scores l'entree (username, score) */

	if (LB == NULL){
		return;
	}

	if (LB->scoreList == NULL){
		return;
	}

	if (strcmp(username, "") == 0){
		return;
	}

	// Index = -1 si le joueur n'est pas dans le LeaderBoard, sinon c'est son indice dans le leaderboard
	int Index = leaderBoard_getIndexOfUsername(LB, username);

	if (Index == -1){
		if (LB->nbEntry == NB_LEADERBOARD){
			if (LB->scoreList[LB->nbEntry - 1] <= score){

				strncpy(LB->usernames[LB->nbEntry - 1], username, NB_CARACTMAX_USERNAME);
				LB->scoreList[LB->nbEntry - 1] = score;
			}
		} else{

			strncpy(LB->usernames[LB->nbEntry], username, NB_CARACTMAX_USERNAME);
			LB->scoreList[LB->nbEntry] = score;
			LB->nbEntry++;

		}

	} else {
		LB->scoreList[Index] = score;
	}

	leaderBoard_sort(LB);
	
}

void leaderBoard_free(LeaderBoard* LB){
	/*libere la memoire de LB et des elements qu'il possede*/

	for (int i = 0; i < NB_LEADERBOARD; i++){
		free(LB->usernames[i]);
	}
	free(LB);
}