#ifndef REPLAY_H
#define REPLAY_H


struct board;
struct player;

typedef struct  {

int replayMoves[10];
int moveCount;
char player1[51];
char player2[51];

}Replay;

int gameLoopReplay(struct player* p1,struct player* p2, struct board* field1, Replay* replay);
int createReplay(char saveFile[], Replay* replay1,struct player *p1,struct player *p2);
void delay(int seconds);
int scanReplay(const char* fullPath, Replay* replay);
int loadReplayFromUser(Replay* replay,struct player *p1,struct player *p2);
int makeMove(struct board* field, struct player* currentPlayer, int move);

#endif 
