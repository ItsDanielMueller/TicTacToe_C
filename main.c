#include "board.h"
#include "librarys.h"
#include "players.h"
#include "replay.h"
#include "operatingSystem.h"



int main(int argc, char* argv[]) {


	//Seed für rand()
	srand(time(NULL));

	//Member der Structs initialisieren.
	board field1;
	player p1;
	player p2;
	Replay r1;
	r1.moveCount = 0;

	//Symbole erstellen.
	p1.symbol = 'X';
	p2.symbol = 'O';

	//Erstellt das Spielbrett
	createBoard(&field1);

	//Verarbeitet cmd input
	if (parseArguement(argc, argv, &field1) == 1) {
		exit(1);
	}

	if (strcmp(field1.mode, "--replay") == 0) {
		gameLoopReplay(&p1, &p2, &field1, &r1);
		exit(1);
	}
	else {
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
	}
	system("cls");


	//Hier wird entschieden welche gameloop gecallt werden soll.
	if (strcmp(field1.mode, "--pvp") == 0) {
		gameLoopPvp(&field1, &p1, &p2,&r1);
	}
	if (strcmp(field1.mode, "--pvc") == 0) {
		gameLoopPvC(&field1, &p1, &p2,&r1);
	}
	if (strcmp(field1.mode, "--cvc") == 0) {
		gameLoopCvc(&field1, &p1, &p2,&r1);
	}


	//Hier wird replay gehandelt
	char saveReplay[4];
	char saveFile[50];
	char fullPath[50];

	while (1) {
		
		printf("Do you want to save the replay? (Y/N): ");

		if (fgets(saveReplay, sizeof(saveReplay), stdin) != NULL) {
			saveReplay[strcspn(saveReplay, "\n")] = '\0';					//Entferne '\n'

			if (strcmp(saveReplay, "y") == 0 || strcmp(saveReplay, "Y") == 0) {
				
				while (1) {
					printf("Name the replay: ");
					if (fgets(saveFile, sizeof(saveFile), stdin) != NULL) {
						saveFile[strcspn(saveFile, "\n")] = '\0';

						//".txt" anhaengen, wenn nicht vorhanden
						if (strstr(saveFile, ".txt") == NULL) {
							snprintf(fullPath, sizeof(fullPath), "%s.txt", saveFile);
						}
						else {
							strncpy_s(fullPath, sizeof(fullPath), saveFile, sizeof(fullPath));
						}

						if (createReplay(fullPath, &r1,&p1,&p2) != 0) {
							fprintf(stderr, "Error saving replay.\n");
							exit(1);
						}
						break; 
					}
					else {
						drawBoard(&field1, &p1, &p2);
						printf("Invalid name. Maximum characters = 50.\n");
					}
				}
				break; 
			}
			else if (strcmp(saveReplay, "n") == 0 || strcmp(saveReplay, "N") == 0) {
				printf("Not saving replay.\n");
				break;
			}
			else {
				drawBoard(&field1, &p1, &p2);
				printf("Invalid input. Please enter Y or N.\n");
			}
		}
	}

	return 0;
}