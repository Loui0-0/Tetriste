#ifndef LEADER_BOARD
#define LEADER_BOARD
#include "config.h"


typedef struct{

	int nbEntry;
	int scoreList[NB_LEADERBOARD];
	char* usernames[NB_LEADERBOARD];

} LeaderBoard;

LeaderBoard* newLeaderBoard();

int leaderBoard_getIndexOfUsername(LeaderBoard* LB, char* username);
void leaderBoard_addEntry(LeaderBoard* LB, int Score, char* Username);

void leaderBoard_free(LeaderBoard* LB);

#endif //LEADER_BOARD 