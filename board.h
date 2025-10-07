#ifndef BOARD_H
#define BOARD_H
#include "players.h"
#include "replay.h"

typedef struct {

	char gameBoard[3][3];
	char mode[10];

}board;



int createBoard(board* field1);
int drawBoard(board* field1,player *p1, player *p2);
int parseArguement(int argCount, char* args[],board *field1);
int gameLoopPvp(board *field1,player *p1,player *p2,Replay *r1);
int gameLoopPvC(board* field1, player* p1, player* p2,Replay *r1);
int gameLoopCvc(board* field1, player* p1, player* p2,Replay *r1);
#endif