#include "board.h"
#include "librarys.h"
#include "players.h"
#include "replay.h"



int main(int argc, char* argv[]) {

	//Member erstellen um replay zu speichern
	Replay replay1;

	//Seed für rand()
	srand(time(NULL));

	//Member der Structs initialisieren.
	board field1;
	player p1;
	player p2;

	//Symbole erstellen.
	p1.symbol = 'X';
	p2.symbol = 'O';

	//Verarbeitet cmd input
	if (parseArguement(argc, argv,&field1) == 1) {
		exit(1);
	}

	//Liest Spielernamen ein und checkt eingabe. Danach wird es ins Struct "player" gespeichert.
	while (1) {
		printf("\nPlayer 1 name: ");
		if (fgets(p1.name, sizeof(p1.name), stdin) != NULL) {
			p1.name[strcspn(p1.name, "\n")] = '\0';
			break;
		}
		else {
			printf("Invalid name. Maximum characters = 50.\n");
		}
	}
	while (1) {
		printf("\nPlayer 2 name: ");
		if (fgets(p2.name, sizeof(p2.name), stdin) != NULL) {
			p2.name[strcspn(p2.name, "\n")] = '\0';
			break;
		}
		else {
			printf("Invalid name. Maximum characters = 50.\n");
		}
	}
	system("cls");

	//Erstellt das Spielbrett
	createBoard(&field1);

	//Hier wird entschieden welche gameloop gecallt werden soll.
	if (strcmp(field1.mode, "--pvp") == 0) {
		gameLoopPvp(&field1, &p1, &p2);
	}
	if (strcmp(field1.mode, "--pvc") == 0) {
		gameLoopPvC(&field1, &p1, &p2);
	}
	if (strcmp(field1.mode, "--cvc") == 0) {
		gameLoopCvc(&field1, &p1, &p2);
	}


	return 0;
}