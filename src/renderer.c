#include <stdio.h>
#include <unistd.h>
#include "config.h"
#include "renderer.h"
#include "pawn.h"
#include "leaderBoard.h"



const char* COLORS[] = {
	"\x1b[38;5;1m",  // Rouge
	"\x1b[38;5;2m",  // Vert
	"\x1b[38;5;3m",  // Jaune
	"\x1b[38;5;4m",  // Bleu
	"\x1b[38;5;5m",  // Magenta
	"\x1b[38;5;6m"   // Cyan
};

const char* SHAPES[] = {
	// "▲",
	// "■",
	// "●",
	// "♠",
	// "♦",
	// "♣"

	"⯄",
	"⯂",
	"⯁",
	"⯌",
	"⯊",
	"⯋"
};

void render_displayPawnLinkedList(LinkedList* pawnList) {
	
	Node *currentNode = pawnList->firstNode;
	while (currentNode != pawnList->lastNode) {
		int ShpId = pawn_getShapeId(currentNode->pawnPtrPtr);
		int ColId = pawn_getColorId(currentNode->pawnPtrPtr);
		printf("%s%s\x1b[0m ", COLORS[ColId], SHAPES[ShpId]);
		currentNode = currentNode->next;
	}
	
	if (pawnList->lastNode){
		int ShpId = pawn_getShapeId(pawnList->lastNode->pawnPtrPtr);
		int ColId = pawn_getColorId(pawnList->lastNode->pawnPtrPtr);
		printf("\x1b[0m%s%s\x1b[0m ", COLORS[ColId], SHAPES[ShpId]);
		
	}
}

void render_displayPawnDoubleLinkedList(DoubleLinkedList* pawnList){
	Node *currentNode = pawnList->firstNode;
	while (currentNode != pawnList->lastNode) {
		int ShpId = pawn_getShapeId(currentNode->pawnPtrPtr);
		int ColId = pawn_getColorId(currentNode->pawnPtrPtr);
		printf("%s%s\x1b[0m", COLORS[ColId], SHAPES[ShpId]);
		currentNode = currentNode->next;
	}
	
	if (pawnList->lastNode){
		int ShpId = pawn_getShapeId(pawnList->lastNode->pawnPtrPtr);
		int ColId = pawn_getColorId(pawnList->lastNode->pawnPtrPtr);
		printf("\x1b[0m%s%s\x1b[0m", COLORS[ColId], SHAPES[ShpId]);
		
	}
}

void render_startMenu(GameSys* GS){
	/*affichage terminal du menu*/
	render_clearTerm();
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃              TETRISTE               ┃\n");
	printf("┠─────────────────────────────────────┨\n");
	printf("┃                ╭────────────────┬─╮ ┃\n");
	printf("┃                │    save sys    │•│ ┃\n");
	printf("┃                ├────────────────┴─┤ ┃\n");
	printf("┃                │                  │ ┃╭───────────────╮\n");
	printf("┃                │                  │ ┠┤   controles   │\n");
	printf("┃                ├─────┬────────────┤ ┃├─┬─────────────┤\n");
	printf("┃                │name │            │ ┃│0│new game     │\n");        
	printf("┃                │score│            │ ┃│1│load last    │\n");
	printf("┃                ╰─────┴────────────╯ ┃│2│LeaderBoard  │\n");
	printf("┃                                     ┃╰─┴─────────────╯\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┠───────────────────────────────────╮ ┃\n");
	printf("┃usr/                          ❯    │ ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┛\n");
	
	char filepath[256];
	snprintf(filepath, 256, "%s/%s", SAVEDIR, LASTGAMESAVEFILE);

	FILE* f = fopen(filepath, "r");
	
	render_moveCursor(19,7);
	
	if (f != NULL){

		printf("save file founded!");
		render_moveCursor(19,8);
		printf("file info:");
		char username[NB_CARACTMAX_USERNAME];
		int score;
		fscanf(f,"%s\n%d",username ,&score);
		render_moveCursor(25,10);
		printf("%s",username);
		render_moveCursor(25,11);
		printf("%d", score);
		fclose(f);
	
	} else {
	
		printf("no save found");
	}
			
	render_moveCursor(33,17);
}


void render_usernameInput(){
	/*affichage terminal de l'entrée utilisateur*/
	render_clearTerm();
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃              TETRISTE               ┃\n");
	printf("┠─────────────────────────────────────┨\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃╭────────────────────────────────╮   ┃\n");
	printf("┃│enter username ↓↓↓↓↓ max size   │   ┃\n");
	printf("┠┴────────────────────────────────┴─╮ ┃\n");
	printf("┃usr/                               │ ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┛\n");
	render_moveCursor(33,15);
	printf("%d\n", NB_CARACTMAX_USERNAME);
	render_moveCursor(6,17);
	
}

void render_level(GameSys* GS){
	/*affichage terminal du choix de niveau*/
	render_clearTerm();
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃              TETRISTE               ┃\n");
	printf("┠─────────────────────────────────────┨\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃╭───────────────╮\n");
	printf("┃                                     ┠┤     level     │\n");
	printf("┃                                     ┃├─┬─────────────┤\n");
	printf("┃                                     ┃│1│cépasdur     │\n");
	printf("┃                                     ┃│2│céunpeupludur│\n");
	printf("┃                                     ┃│3│cétrédur     │\n");
	printf("┃                                     ┃╰─┴─────────────╯\n");
	printf("┃                                     ┃\n");
	printf("┃╭──────────────────╮                 ┃\n");
	printf("┃│enter level ↓↓↓↓↓ │                 ┃\n");
	printf("┠┴──────────────────┴───────────────╮ ┃\n");
	printf("┃usr/                         ❯     │ ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┛\n");
	
	render_moveCursor(6,17);
	printf("%s/level/",GS->username);
	render_moveCursor(32,17);

}

void render_leaderBoard(LeaderBoard* LB){
	/*affichage terminal du tableau des score*/
	render_clearTerm();
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃              TETRISTE               ┃\n");
	printf("┠─────────────────────────────────────┨\n");
	printf("┃╭───────────────────────────────────╮┃\n");
	printf("┃│           🏆LEADERBOARD🏆         │┃\n");
	printf("┃├──┬──────────────────────────┬─────┤┃\n");
	printf("┃│1.│..........................│     │┃\n");
	printf("┃│2.│..........................│     │┃\n");
	printf("┃│3.│..........................│     │┃\n");
	printf("┃│4.│..........................│     │┃\n");
	printf("┃│5.│..........................│     │┃\n");
	printf("┃│6.│..........................│     │┃\n");
	printf("┃│7.│..........................│     │┃\n");
	printf("┃│8.│..........................│     │┃\n");
	printf("┃│9.│..........................│     │┃\n");
	printf("┃│10│..........................│     │┃\n");
	printf("┃╰──┴──────────────────────────┴─────╯┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");


	for (int i = 0; i < LB->nbEntry && i < 10; ++i)
	{	

		render_moveCursor(6,7+i);
		printf("(%s)", LB->usernames[i]);

		render_moveCursor(33,7+i);
		printf("%d", LB->scoreList[i]);
	}

	render_moveCursor(0,50);
	
}


void render_gameRotate(GameSys* GS, int shapeOrColor){
	/*affichage terminal du jeu en mode decalage (fenetre ouverte sur le coté)*/

	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓╭─────────────╮\n");
	printf("┃              TETRISTE               ┠┤%s = %d │ %s▇\x1b[0m = %d│\n", SHAPES[0], 0, COLORS[0], 0);
	printf("┠────┬─────────────────────┬──────────┨│%s = %d │ %s▇\x1b[0m = %d│\n", SHAPES[1], 1, COLORS[1], 1);
	printf("┃    │                     │╭─╮       ┃│%s = %d │ %s▇\x1b[0m = %d│\n", SHAPES[2], 2, COLORS[2], 2);
	printf("┠────╯                     ││▲│■ ● ♠ ♦┃│%s = %d │ %s▇\x1b[0m = %d│\n", SHAPES[3], 3, COLORS[3], 3);
	printf("┃                          │╰─╯       ┃│%s = %d │ %s▇\x1b[0m = %d│\n", SHAPES[4], 4, COLORS[4], 4);
	printf("┃                          └──────────┨│%s = %d │ %s▇\x1b[0m = %d│\n", SHAPES[5], 5, COLORS[5], 5);
	printf("┃                                     ┃╰─────────────╯\n");
	printf("┃                                     ┃╭───────────────╮\n");
	printf("┃                                     ┠┤   controles   │\n");
	printf("┃                                     ┃├─┬─────────────┤\n");
	printf("┃                                     ┃│n│rotate nth id│\n");   
	printf("┃                                     ┃╰─┴─────────────╯\n");                 
	printf("┃   ❱                             ❰   ┃\n");                 
	printf("┃                                     ┃\n");                 
	printf("┠───────────────────────────────────╮ ┃\n");                 
	printf("┃usr/                         ❯     │ ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┛\n");
	
	render_moveCursor(2,4);
	printf("%d", GS->score);
	

	Node* currentNode = GS->nextPawnList->firstNode;
	for (int i = 0; i < NB_NEXTPAWNLIST; i++){
		render_moveCursor(30 + i*2,5);
		render_displayPawn(currentNode->pawnPtrPtr);
		currentNode = currentNode->next;
	}

	int size = GS->boardGame->size;
	
	currentNode = GS->boardGame->firstNode;
	if (size > 0){
		render_moveCursor(20 - size,13);
		printf("╭\n");
		render_moveCursor(20 - size,15);
		printf("╰\n");

		for (int i = 0; i < size; i++){
			render_moveCursor(7 + 14 - size + i*2,14);
			render_displayPawn(currentNode->pawnPtrPtr);
			currentNode = currentNode->next;
		}
		render_moveCursor(20 + size,13);
		printf("╮\n");
		render_moveCursor(20 + size,15);
		printf("╯\n");
	}
	render_moveCursor(6,17);
	printf("%s/move/rotate%s",GS->username, shapeOrColor ? "Shape" : "Color");
	
	render_moveCursor(32,17);
}
void render_game(GameSys* GS){
	/*affichage terminal du jeu*/
	
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃              TETRISTE               ┃\n");
	printf("┠────┬─────────────────────┬──────────┨\n");
	printf("┃    │                     │╭─╮       ┃\n");
	printf("┠────╯                     ││ │       ┃\n");
	printf("┃                          │╰─╯       ┃\n");
	printf("┃                          └──────────┨\n");
	printf("┃                                     ┃\n");
	printf("┃                                     ┃╭───────────────╮\n");
	printf("┃                                     ┠┤   controles   │\n");
	printf("┃                                     ┃├─┬─────────────┤\n");
	printf("┃                                     ┃│0│add left     │\n");
	printf("┃╭ ╮                               ╭ ╮┃│1│add right    │\n");
	printf("┃│0│❱                             ❰│1│┃│2│rotate shape │\n");
	printf("┃╰ ╯                               ╰ ╯┃│3│rotate color │\n");
	printf("┠───────────────────────────────────╮ ┃│4│save and quit│\n");
	printf("┃usr/                         ❯     │ ┃╰─┴─────────────╯\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┛\n");

	
	render_moveCursor(2,4);
	printf("%d", GS->score);
	
	Node* currentNode = GS->nextPawnList->firstNode;
	for (int i = 0; i < GS->nbNextPawn; i++){

		render_moveCursor(30 + i*2,5);

		render_displayPawn(currentNode->pawnPtrPtr);
		
		currentNode = currentNode->next;
	}

	int size = GS->boardGame->size;
	
	currentNode = GS->boardGame->firstNode;
	if (size > 0){
		render_moveCursor(20 - size,13);
		printf("╭\n");
		render_moveCursor(20 - size,15);
		printf("╰\n");

		for (int i = 0; i < size; i++){
			render_moveCursor(7 + 14 - size + i*2,14);
			render_displayPawn(currentNode->pawnPtrPtr);
			currentNode = currentNode->next;
		}
		render_moveCursor(20 + size,13);
		printf("╮\n");
		render_moveCursor(20 + size,15);
		printf("╯\n");
	}
	render_moveCursor(6,17);
	printf("%s/move",GS->username);
	
	render_moveCursor(32,17);
}



void render_displayPawn(Pawn** pawnPtrPtr){
	/*affiche un pawn dans le terminal en fonction de sa couleur et de sa forme*/
	int ShpId = pawn_getShapeId(pawnPtrPtr);
	int ColId = pawn_getColorId(pawnPtrPtr);
	printf("\x1b[0m%s%s\x1b[0m", COLORS[ColId], SHAPES[ShpId]);

}

