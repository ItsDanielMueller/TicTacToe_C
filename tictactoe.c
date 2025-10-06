#include "board.h"
#include "librarys.h"
#include "players.h"
#include "replay.h"


void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


void shuffle(int* array, int n) {
	//Fisher-Yates Shuffle. Tausch zufällige zahlen in einem vorhandenen Array.
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(&array[i], &array[j]);
	}
}


int parseArguement(int argCount, char* args[],board * field1) {

	//Checkt ob ein Argument geliefert wird.
	if (argCount < 2) {
		printf("\nArguements missing. Please refer to --help.\n");
		return 1;
	}

	//Hier werden die args[] in Kleinbuchstaben verwandelt um Groß- Kleinschreibung zu ignorieren. 
	for (int i = 0; i < argCount; i++) {
		char* p = args[i];
		while (*p) {								//While *p sagt solange der Inhalt von p nicht \0 ist soll die while loop weiterlaufen.
			*p = (char)tolower((unsigned char)*p);
			p++;									//Da p ja ein pointer ist kann man pointer arithmetics verwenden und einfach ++ machen um auf den nächsten Index zu springen.
		}
	}

	//Gibt --help aus.
	if (strcmp(args[1], "--help") == 0 && argCount <= 3) {
		printf("\n=== Guide ===");
		printf("\n.exe [Mode]\n");
		printf("\nAvailable modes : \n");
		printf("\n--pvp (Player vs Player.)");
		printf("\n--pvc (Player vs Computer.)");
		printf("\n--cvc (Computer vs Computer.)");
		printf("\n--replay\n");
		printf("\nAfter each game you can watch the replay and cou can choose to save it for later use.");
		printf("\nIf you choose to save the replay you gete 50 characters to name it.\n\n");
		return 1;
	}		
	else if(argCount>2){				//Checkt ob zuviele Argumente eingegeben worden sind.
		printf("\nToo many arguements. Please refer to --help\n");
		return 1;
	}

	//Checkt ob eingabe zu groß
	if (strlen(args[1]) > sizeof(field1->mode)) {
		printf("\nInvalid mode. Please refer to --help.\n");
		return 1;
	}
	else {
		strcpy_s(field1->mode, sizeof(field1->mode), args[1]);
	}

	if (strcmp(field1->mode, "--pvp") != 0 && strcmp(field1->mode, "--pvc") != 0 && strcmp(field1->mode, "--cvc") != 0 && strcmp(field1->mode, "--replay") != 0) {
		printf("\nInvalid mode. Please refer to --help.\n");
		return 1;
	}


	return 0;
}


int createBoard(board* field1) {

	//Fuellt Felder mit 1-9 indem es am ende inkrementiert wird.
	int fillBoards = 1;

	//Befüllt die Felder im gameBoard array. 
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			field1->gameBoard[i][j] = fillBoards + '0';
			fillBoards++;
		}
	}
	return 0;
}


int drawBoard(board* field1,player* p1, player* p2) {

	//Cleared die console.
	system("cls");
    printf("\n%s(\033[1;31m%c\033[0m) vs %s(\033[1;34m%c\033[0m)\n\n", p1->name, p1->symbol, p2->name, p2->symbol);


	//Zeichnet das Feld in die Konsole
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			char c = field1->gameBoard[i][j];

			// Färbt X rot und O blau
			if (c == 'X') {
				printf("\033[1;31m %c \033[0m", c);  // X rot
			}
			else if (c == 'O') {
				printf("\033[1;34m %c \033[0m", c);  // O blau
			}
			else {
				printf(" %c ", c);                   // Zahlen normal
			}

			// Trenner zwischen den Spalten
			if (j != 2) printf(" | ");
		}

		// Zeilen-Trenner
		if (i != 2) printf("\n---------------");
		printf("\n");
	}
}


int gameLoopPvp(board* field1,player *p1,player *p2) {

	int turn = 0;
	int draw = 0;
	
	//Spielschleife
	while (p1->winner != 1 && p2->winner != 1 && draw != 1) {

		if (turn % 2 == 0) {
			humanMove(p1,p1,p2,field1);		//Hier wird p1 zweimal weitergegeben da currentplayer ja p1 oder p2 sein kann. Um die Namen immer richtig einzublenden wird einfach p1,p2 auch weitergegeben
		}
		else {
			humanMove(p2,p1,p2,field1);		//Dasselbe mit p2
		}

		turn++;
		drawBoard(field1, p1, p2);
		
		if (winnerLogic(field1) == 'X') {
			p1->winner = 1;
			printf("\n\033[1;31m%s Won!\033[0m\n\n", p1->name); // Rot
			return 1;
		}
		else if (winnerLogic(field1) == 'O') {
			p2->winner = 1;
			printf("\n\033[1;34m%s Won!\033[0m\n\n", p2->name); // Blau
			return 1;
		}

		if (turn == 9) {

			draw = 1;
			
		}
	}

	printf("\n\n\033[1;32mIt's a draw.\033[0m\n");		//Green
	return 0;
}


int humanMove(player* currentPlayer, player* p1, player *p2 , board* field1) {

	int isTaken;
	char temp[5];
	char validMove;
	char errorMsg[50] = { ' ' };
	

	while (1) {

		drawBoard(field1, p1, p2);

		//Gibt fehlermeldung nach drawboard aus um das cls zu umgehen.
		if (errorMsg[0] != '\0') {
			printf("\n\n%s\n", errorMsg);
			errorMsg[0] = '\0'; 
		}

		printf("\n\nEnter your move: ");
		
		
		//Liest input ein und wertet ihn aus.
		if (fgets(temp, sizeof(temp), stdin) != NULL) {
			temp[strcspn(temp, "\n")] = '\0';
		}
		else {
			strcpy_s(errorMsg,sizeof(errorMsg),"\nInvlalid input.\n");
			continue;
		}

		if (temp[0] == '\0' || temp[1] != 0) {
			strcpy_s(errorMsg, sizeof(errorMsg), "\nPlease enter a digit between 1-9.\n");
			continue;
		}

		//Speichert die Eingabe in einen einzelnen Char.
		validMove = temp[0];
		isTaken = 0;

		//Hier wird gecheckt ob eingabe zwischen 1 und 9 ist. Man vergleicht hier zwar char (validMove) mit integers aber da integers im hintergrund mit ASCII verarbeitet werden, ist das so möglich.
		if (validMove < '1' || validMove >'9') {
			strcpy_s(errorMsg, sizeof(errorMsg), "Invalid move. Please enter 1-9.\n");
			continue;
		}

		//Hier wird das Array durchgescannt um den Input des Users zu finden und dort wird dann sein Symbol eingesetzt. Falls das Feld nicht frei ist wird der User Input auch nie gefunden -> IsTaken=1. 
		for (int i = 0; i < 3 && !isTaken; i++) {
			for (int j = 0; j < 3; j++) {
				if (field1->gameBoard[i][j] == validMove) {
					field1->gameBoard[i][j] = currentPlayer->symbol;
					isTaken = 1;  
					break;       
				}
			}
		}

		if (isTaken==0) {
			strcpy_s(errorMsg, sizeof(errorMsg), "Field already taken! Choose another.\n");
			continue;  
		}
		
		
		break;
	}
	return 0;
}


int winnerLogic(board *field1) {
	
	//Hier werden alle Reihen gecheckt ob sie die gleichen Symbole enthalten.
	for (int i = 0; i < 3; i++) {
		if ((field1->gameBoard[i][0] == 'X' || field1->gameBoard[i][0] == 'O') && field1->gameBoard[i][0] == field1->gameBoard[i][1] && field1->gameBoard[i][1] == field1->gameBoard[i][2]) {
			return field1->gameBoard[i][0];
		}
	}

	//Hier werden alle Spalten gecheckt ob sie die gleichen Symbole enhtalten.
	for (int i = 0; i < 3; i++) {
		if ((field1->gameBoard[0][i] == 'X' || field1->gameBoard[0][i] == 'O') && field1->gameBoard[0][i] == field1->gameBoard[1][i] && field1->gameBoard[1][i] == field1->gameBoard[2][i]) {
			return field1->gameBoard[0][i];
		}
	}

	//Hier wird eine Diagonale gecheckt ob alle Symbole gleich sind.
	if ((field1->gameBoard[0][0] == 'X' || field1->gameBoard[0][0] == 'O') && field1->gameBoard[0][0] == field1->gameBoard[1][1] && field1->gameBoard[1][1] == field1->gameBoard[2][2]) {
		return field1->gameBoard[0][0];
	}

	//Hier wird die andere Diagonale gecheckt ob alle Symbole gleich sind.
	if ((field1->gameBoard[0][2] == 'X' || field1->gameBoard[0][2] == 'O') && field1->gameBoard[0][2] == field1->gameBoard[1][1] && field1->gameBoard[1][1] == field1->gameBoard[2][0]) {
		return field1->gameBoard[0][2];
	}
	
	//Wenn nichts zutrifft return man einfach ein leeres Symbol.
	return ' ';
}


int gameLoopPvC(board *field1,player *p1, player *p2) {

	int turn = 0;
	int draw = 0;

	//Spielschleife
	while (p1->winner != 1 && p2->winner != 1 && draw != 1) {

		if (turn % 2 == 0) {
			humanMove(p1, p1, p2, field1);		//Hier wird p1 zweimal weitergegeben da currentplayer ja p1 oder p2 sein kann. Um die Namen immer richtig einzublenden wird einfach p1,p2 auch weitergegeben
		}
		else {
			computerMove(p2,p1, p1, p2, field1);		//Dasselbe mit p2
		}

		turn++;
		drawBoard(field1, p1, p2);

		if (winnerLogic(field1) == 'X') {
			p1->winner = 1;
			printf("\n\033[1;31m%s Won!\033[0m\n\n", p1->name); // Rot
			return 1;
		}
		else if (winnerLogic(field1) == 'O') {
			p2->winner = 1;
			printf("\n\033[1;34m%s Won!\033[0m\n\n", p2->name); // Blau
			return 1;
		}

		if (turn == 9) {

			draw = 1;

		}
	}

	printf("\n\n\033[1;32mIt's a draw.\033[0m\n");		//Green
	return 0;


}


int computerMove(player* currentPlayer,player *opponent, player* p1, player* p2, board* field1) {

	
	char computerMsg[50] = { ' ' };

		drawBoard(field1, p1, p2);

		printf("\n\nEnter your move: ");

		//Speichert den return struct von computerLogic in einen lokalen member aiChoice.
		struct AiMove aiChoice = computerLogic(field1,currentPlayer,opponent);

		if (aiChoice.row != -1) {
			field1->gameBoard[aiChoice.row][aiChoice.col] = currentPlayer->symbol;
			return 0;
		}

		//Erstellt ein Array mit allen eingabemoeglichkeiten. Dieses Array wird dann geshuffelt. Dadurch ensteht eine random auswahl ohne duplizierung.
		int randomNumbers[] = { 1,2,3,4,5,6,7,8,9 };

		//Hier wird array geshuffled
		shuffle(randomNumbers, 9);

		int isTaken = 0;

		//Hier wird das Array durchgescannt um den Input des Users zu finden und dort wird dann sein Symbol eingesetzt. Falls das Feld nicht frei ist wird der User Input auch nie gefunden -> IsTaken=1. 
		for (int k = 0; k < 9 &&isTaken!=1; k++) {
			int validMove = randomNumbers[k];
			for (int i = 0; i < 3 && !isTaken; i++) {
				for (int j = 0; j < 3; j++) {
					if (field1->gameBoard[i][j] == validMove + '0') {
						field1->gameBoard[i][j] = currentPlayer->symbol;
						isTaken = 1;
						break;
					}
				}
			}
		}

	return 0;
}


struct AiMove computerLogic(board* field1,player *current,player *opponent) {

	struct AiMove move;
	move.row = -1;
	move.col = -1;

	//Gewinnen check
	// Reihen check
	for (int i = 0; i < 3; i++) {
		if (field1->gameBoard[i][0] == current->symbol &&
			field1->gameBoard[i][1] == current->symbol &&
			field1->gameBoard[i][2] != current->symbol &&
			field1->gameBoard[i][2] != opponent->symbol) {
			move.row = i;
			move.col = 2;
			return move;
		}
		if (field1->gameBoard[i][0] == current->symbol &&
			field1->gameBoard[i][2] == current->symbol &&
			field1->gameBoard[i][1] != current->symbol &&
			field1->gameBoard[i][1] != opponent->symbol) {
			move.row = i;
			move.col = 1;
			return move;
		}
		if (field1->gameBoard[i][1] == current->symbol &&
			field1->gameBoard[i][2] == current->symbol &&
			field1->gameBoard[i][0] != current->symbol &&
			field1->gameBoard[i][0] != opponent->symbol) {
			move.row = i;
			move.col = 0;
			return move;
		}
	}

	// Spalten check
	for (int i = 0; i < 3; i++) {

		if (field1->gameBoard[0][i] == current->symbol &&
			field1->gameBoard[1][i] == current->symbol &&
			field1->gameBoard[2][i] != current->symbol &&
			field1->gameBoard[2][i] != opponent->symbol) {
			move.row = 2;
			move.col = i;
			return move;
		}
		if (field1->gameBoard[0][i] == current->symbol &&
			field1->gameBoard[2][i] == current->symbol &&
			field1->gameBoard[1][i] != current->symbol &&
			field1->gameBoard[1][i] != opponent->symbol) {
			move.row = 1;
			move.col = i;
			return move;
		}
		if (field1->gameBoard[1][i] == current->symbol &&
			field1->gameBoard[2][i] == current->symbol &&
			field1->gameBoard[0][i] != current->symbol &&
			field1->gameBoard[0][i] != opponent->symbol) {
			move.row = 0;
			move.col = i;
			return move;
		}
	}

	// Diagonalen check
	char a = field1->gameBoard[0][0];
	char b = field1->gameBoard[1][1];
	char c = field1->gameBoard[2][2];

	if (a == current->symbol && b == current->symbol && c != current->symbol && c != opponent->symbol) {
		move.row = 2; move.col = 2; return move;
	}
	if (a == current->symbol && c == current->symbol && b != current->symbol && b != opponent->symbol) {
		move.row = 1; move.col = 1; return move;
	}
	if (b == current->symbol && c == current->symbol && a != current->symbol && a != opponent->symbol) {
		move.row = 0; move.col = 0; return move;
	}

	//Nebendiagonale prüfen 
	a = field1->gameBoard[0][2];
	b = field1->gameBoard[1][1];
	c = field1->gameBoard[2][0];

	if (a == current->symbol && b == current->symbol && c != current->symbol && c != opponent->symbol) {
		move.row = 2; move.col = 0; return move;
	}
	if (a == current->symbol && c == current->symbol && b != current->symbol && b != opponent->symbol) {
		move.row = 1; move.col = 1; return move;
	}
	if (b == current->symbol && c == current->symbol && a != current->symbol && a != opponent->symbol) {
		move.row = 0; move.col = 2; return move;
	}


	//Blockieren check
	//Reihen check
	for (int i = 0; i < 3; i++) {
		if (field1->gameBoard[i][0] == opponent->symbol &&
			field1->gameBoard[i][1] == opponent->symbol &&
			field1->gameBoard[i][2] != opponent->symbol &&
			field1->gameBoard[i][2] != current->symbol) {
			move.row = i;
			move.col = 2;
			return move;
		}
		if (field1->gameBoard[i][0] == opponent->symbol &&
			field1->gameBoard[i][2] == opponent->symbol &&
			field1->gameBoard[i][1] != opponent->symbol &&
			field1->gameBoard[i][1] != current->symbol) {
			move.row = i;
			move.col = 1;
			return move;
		}
		if (field1->gameBoard[i][1] == opponent->symbol &&
			field1->gameBoard[i][2] == opponent->symbol &&
			field1->gameBoard[i][0] != opponent->symbol &&
			field1->gameBoard[i][0] != current->symbol) {
			move.row = i;
			move.col = 0;
			return move;
		}
	}

	// Spalten check
	for (int i = 0; i < 3; i++) {
		if (field1->gameBoard[0][i] == opponent->symbol &&
			field1->gameBoard[1][i] == opponent->symbol &&
			field1->gameBoard[2][i] != opponent->symbol &&
			field1->gameBoard[2][i] != current->symbol) {
			move.row = 2;
			move.col = i;
			return move;
		}
		if (field1->gameBoard[0][i] == opponent->symbol &&
			field1->gameBoard[2][i] == opponent->symbol &&
			field1->gameBoard[1][i] != opponent->symbol &&
			field1->gameBoard[1][i] != current->symbol) {
			move.row = 1;
			move.col = i;
			return move;
		}
		if (field1->gameBoard[1][i] == opponent->symbol &&
			field1->gameBoard[2][i] == opponent->symbol &&
			field1->gameBoard[0][i] != opponent->symbol &&
			field1->gameBoard[0][i] != current->symbol) {
			move.row = 0;
			move.col = i;
			return move;
		}
	}

	// Diagonalen check
	a = field1->gameBoard[0][0];
	b = field1->gameBoard[1][1];
	c = field1->gameBoard[2][2];

	if (a == opponent->symbol && b == opponent->symbol && c != opponent->symbol && c != current->symbol) {
		move.row = 2; move.col = 2; return move;
	}
	if (a == opponent->symbol && c == opponent->symbol && b != opponent->symbol && b != current->symbol) {
		move.row = 1; move.col = 1; return move;
	}
	if (b == opponent->symbol && c == opponent->symbol && a != opponent->symbol && a != current->symbol) {
		move.row = 0; move.col = 0; return move;
	}

	//Nebendiagonale prüfen 
	a = field1->gameBoard[0][2];
	b = field1->gameBoard[1][1];
	c = field1->gameBoard[2][0];

	if (a == opponent->symbol && b == opponent->symbol && c != opponent->symbol && c != current->symbol) {
		move.row = 2; move.col = 0; return move;
	}
	if (a == opponent->symbol && c == opponent->symbol && b != opponent->symbol && b != current->symbol) {
		move.row = 1; move.col = 1; return move;
	}
	if (b == opponent->symbol && c == opponent->symbol && a != opponent->symbol && a != current->symbol) {
		move.row = 0; move.col = 2; return move;
	}

	return move; // -1,-1 wenn nichts gefunden
}


int gameLoopCvc(board* field1, player* p1, player* p2) {


	int turn = 0;
	int draw = 0;

	//Spielschleife
	while (p1->winner != 1 && p2->winner != 1 && draw != 1) {

		if (turn % 2 == 0) {
			computerMove(p1,p2, p1, p2, field1);//Hier wird p1 zweimal weitergegeben da currentplayer ja p1 oder p2 sein kann. Um die Namen immer richtig einzublenden wird einfach p1,p2 auch weitergegeben
			Sleep(1000);
		}
		else {
			computerMove(p2,p1, p1, p2, field1);		//Dasselbe mit p2
			Sleep(1000);
		}

		turn++;
		drawBoard(field1, p1, p2);

		if (winnerLogic(field1) == 'X') {
			p1->winner = 1;
			printf("\n\033[1;31m%s Won!\033[0m\n\n", p1->name); // Rot
			return 1;
		}
		else if (winnerLogic(field1) == 'O') {
			p2->winner = 1;
			printf("\n\033[1;34m%s Won!\033[0m\n\n", p2->name); // Blau
			return 1;
		}

		if (turn == 9) {

			draw = 1;

		}
	}
	printf("\n\n\033[1;32mIt's a draw.\033[0m\n");
	return 0;

}


int createReplay(char saveFile[],Replay *replay1) {

	FILE* fptr1;
	errno_t err;

	err = fopen_s(&fptr1,saveFile,"w");

	if (err != 0 || fptr1 == NULL) {
		printf("\nKeine Datei gefunden.\n");
		return 1;
	}

	for (int i = 0; i < replay1->moveCount; i++) {
		fprintf(fptr1, "%d", replay1->replayMoves[i]);
	}

	fclose(fptr1);
	return 0;
}