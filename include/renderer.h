#ifndef RENDERER_H
#define RENDERER_H
#include "config.h"
#include "leaderBoard.h"
#include "tetristeGameSys.h"

void render_leaderBoard(LeaderBoard* LB);
void render_usernameInput();

void render_gameRotate(GameSys* GS, int shapeOrColor);
void render_startMenu(GameSys* GS);
void render_game(GameSys* GS);
void render_level(GameSys* GS);

void render_displayPawnLinkedList(LinkedList* pawnList);
void render_displayPawnDoubleLinkedList(DoubleLinkedList* pawnList);
void render_displayPawn(Pawn** PawnPtrPtr);

void render_moveCursor(int x, int y);
void render_clearTerm();

#endif //RENDERER_H	