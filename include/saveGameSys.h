#ifndef SAVEGAMESYS_H
#define SAVEGAMESYS_H

#include "tetristeGameSys.h"
#include "leaderBoard.h"

int saveSys_loadGameSysFromFile(GameSys* GS);
int saveSys_writeSaveFile(GameSys* GS);

int saveSys_loadLeaderBoard(LeaderBoard* LB);
int saveSys_writeLeaderBoard(LeaderBoard* LB);

/* LE FICHIER EST ECRIT COMME SUIT:

ligne   CurrentUsername
ligne 1 CurrentScore;
ligne 2 int nbColor;
ligne 3 int nbShape;

LEADERBOARD (ligne 4 - 23 Username and Score)

ligne 4 Username 1
ligne 5 Score 1
ligne 6 Username 2
ligne 7 Score 2 
...


BOARDGAME

Ligne 24 Size of LinkedList
Ligne 25 ColorID Pawn 1
Ligne 26 ShapeID Pawn 1
Ligne 27 ColorID Pawn 2
Ligne 28 ShapeID Pawn 2
...

NEXTPAWNLIST

ligne 29 ColorID NextPawn 1
ligne 30 ShapeID NextPawn 1
ligne 31 ColorID NextPawn 2
ligne 32 ShapeID NextPawn 2
...

*/

#endif // SAVEGAMESYS_H