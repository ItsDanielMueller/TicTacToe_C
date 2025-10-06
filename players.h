#ifndef PLAYERS_H
#define PLAYERS_H

//Vor deklarieren um wiederholtes includen der header zu vermeiden.(players.h -> board.h -> players.h ->board.h etc)
struct board;

//struct fuer Spieler oder Computer
typedef struct {
	char name[50];
	char symbol;
	int winner;
}player;

//struct um computerLogic koordinaten zu speichern.
struct AiMove{
	int row;
	int col;
};


int humanMove(player* currentPlayer,player *p1,player *p2,struct board *field1);
int computerMove(player* currentPlayer, player* opponent, player* p1, player* p2, struct board* field1);
struct AiMove computerLogic(struct board* field1,player *current,player *opponent);
#endif

