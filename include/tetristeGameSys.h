#ifndef TETRISTE_GAMESYS_H
#define TETRISTE_GAMESYS_H
#include "config.h"
#include "doubleLinkedList.h"
#include "leaderBoard.h"


typedef struct{
  /* Structure principale du jeux avec une linkedliste modelisant le tableau de jeux 
  et deux doublelinkedlistes qui lient les pièces en fonction de leur couleur ou de leur forme */
  
  char* username;
  int score;
  
  int nbColor;
  int nbShape;
  int nbNextPawn;
  int nbMaxPawn;

  DoubleLinkedList** shapeList;
  DoubleLinkedList** colorList;

  LinkedList* boardGame;
  LinkedList* nextPawnList;
  LinkedList* allocatedPawns;


}GameSys;


GameSys* newGameSys();

Pawn** gameSys_newPawnHolder(GameSys* GS, int colorID, int shapeID);

Pawn** gameSys_newRandomPawnHolder(GameSys* GS);

void gameSys_displayDebug(GameSys* GS);

void gameSys_addPawnToBoardDebug(GameSys *GS, Pawn **pawnPtrPtr);

void ShowGameSys(GameSys *GameSysPtr);

void gameSys_free(GameSys *GS);

void gameSys_addPawnLeft(GameSys *GS, Pawn **pawnPtrPtr);

void gameSys_addPawnRight(GameSys *GS, Pawn **pawnPtrPtr);

void gameSys_setLevel(GameSys* GS, int lvl); 

void gameSys_play(GameSys* GS, int move, int ShapeColorID);

void gameSys_initNext(GameSys* GS);

void gameSys_clear(GameSys* GS);

int gameSys_isGameOver(GameSys* GS);

void gameSys_rotateDoubleLinkedListLeftShape(GameSys* GS, int shapeID);

void gameSys_rotateDoubleLinkedListLeftColor(GameSys* GS, int colorID);

void gameSys_shapeListReCompute(GameSys* GS);

void gameSys_colorListReCompute(GameSys* GS);

Pawn** gameSys_getNextPawn(GameSys* GS);

void gameSys_remove(GameSys* GS, Pawn** PawnPtrPtr);

LinkedList* gameSys_checkGameBoard(GameSys* GS);

void gameSys_menuTerm(GameSys* GS, LeaderBoard* LB);

void gameSys_InitTerminal(GameSys* GS, int newGame);

void gameSys_gameLoopTerminal(int nbShape, int nbColor);
#endif //TETRISTE_GAMESYS_H