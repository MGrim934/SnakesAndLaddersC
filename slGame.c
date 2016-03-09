#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

//Snakes and Ladders Project - Procedural Programming
//Mark Grimes

//snake and ladder struct
//only one struct needed, since regardless of what they are, they will need a position and a max length
//no need to make a seperate struct type for each one! hooray
typedef struct {
	int position;
	int length;
	int end;
} sol_t; //snake or ladder sol_t

typedef struct {
	char name[20]; //only expects first names so 20 will do. 
	int position;
}player_t;

void CreateSnakes(sol_t snakes[], int size);
//dynamically assigns snakes a position and a length

void PrintSnakes(sol_t snakes[]);
//prints snake positions!

void PrintLadders(sol_t ladders[]);
//prints ladders positions
void CreateLadders(sol_t ladders[], sol_t snakes[], int size);
//dynamically assigns 7 snakes a position and length, as well as check to ensure that there is no snake in another place

void BuildPlayerList(player_t *players, int count);
//assigns player names with user input, as well as gives them their positions
void ShowPlayerList(player_t *players, int count);

void TheGame(player_t *players, sol_t snakes[], sol_t ladders[],int playerCount, int  playerIndex);
//this is where the game is played! will go inside the main function

int IsDuplicate(sol_t object, sol_t list[], int size);
//this function checks that an object that is created (snake or ladder) is not a duplicate

int RollDice();
//roll the dice from 1 - 6

int CheckVictory(int position);
//checks if a player is at 100
int CheckSnakes(int position, sol_t snakes[]); 
//checks if a player landed on a snake
int CheckLadders(int position, sol_t ladders[]); 
//checks if a player landed on a ladder

void ShowRules();
//shows rules
int GetPlayerCount();
//returns the player count

void SaveAndExitGame(int playerCount, int currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]);
//allows the player to save and exit the game, creates or overwrites a file
int LoadGame(int fileChoice, int *playerCount, int *currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]);
//loads game for a file if its there
int CheckFiles();
//says how many files are in the reference file
void removeFile(char fileName[]);
//deletes a file
void MenuRemoveFile();
//a menu that shows the user current files and how to delete them
void CreateNewReferenceFile(int fileCount, char files[20][20], char nFile[20]);
//creates a new references file
int WhatFile();
//a menu where the user chooses a file


#define MAX 7

//=======================================================================================


void main() {

	player_t *players;//structure of players
	int playerCount,loaded,fileChoice; //keeps counts

	int currentPlayer=0; //only matters if its loading in a game instead of creating one fresh

	time_t theTime;//the time!
	int menuChoice;//the choice they made
	sol_t snakes[MAX]; //holds all the snakes

	sol_t ladders[MAX];//holds all the ladders

	srand((unsigned) time(&theTime)); //makes the seed the current time

	//welcome message
	do {
		printf("===========================================\n");
		printf("Welcome To Snakes and Ladders 3000!\n");
		printf("===========================================\n");
		printf("%-14s New Game\n", "1)");
		printf("%-14s Load Game\n", "2)");
		printf("%-14s Game Rules\n", "3)");
		printf("%-14s Delete File\n", "4)");
		printf("%-14s Exit Game\n", "0)");
		printf("===========================================\n");



		//main game while loop
		//in here we will have a switch with various functions that set up the game let the user play and all that jazz
		


		scanf("%d", &menuChoice);


		switch (menuChoice) {
		case 1:
			//new game!

			// how many players?
			do {
				printf("How many players? Minimum - 2 Maximum - 6\n");
				scanf("%d", &playerCount);
			} while (playerCount > 6 || playerCount < 2);
			players = (player_t *)malloc(playerCount*sizeof(player_t)); //give the player the size the player prompted
			BuildPlayerList(players,playerCount);
			ShowPlayerList(players, playerCount);

			//generate board
			CreateSnakes(snakes, 7);
			CreateLadders(ladders, snakes, 7);


			PrintSnakes(snakes);



			PrintLadders(ladders);
			//start game
			TheGame(players, snakes, ladders,playerCount,currentPlayer);
			break;
		case 2:
			//load game
			//which game to load?

			//load players must track whose turn it is
			//load board
			fileChoice = WhatFile();
			if (fileChoice == -1) {
				break;
			}
			//if they can't find a file break out of the function
	
			printf("loading...\n");
			playerCount = GetPlayerCount(fileChoice);
			players = (player_t *)malloc(playerCount*sizeof(player_t));
			loaded=LoadGame(fileChoice, &playerCount, &currentPlayer, players, snakes, ladders);
			if (loaded > 0) {
				PrintSnakes(snakes);
				PrintLadders(ladders);

				printf("\nGame Loaded\n");

				//start game
				TheGame(players, snakes, ladders, playerCount, currentPlayer);
			}
			else {
				printf("No games to load! Maybe Try a new game...\n");
			}
			break;
		case 3:
			//show the rules
			ShowRules();
			break;

		case 4:
			printf("\nDelete a Save File\n");
			MenuRemoveFile();
			break;
		case 0:
			//quitting the game
			printf("\nThanks for Playing! Press RETURN to Exit!\n");
			break;
		default:
			printf("Invalid Input :(\n");
			break;
		}
	} while (menuChoice != 0); 
	
	getch(); //stop console window from closing


}// main
// runs the game!
//=======================================================================================


void ShowRules() {
	//simply shows the rules of the game
	printf("\n===========================================\n");
	printf("Welcome To Snakes and Ladders 3000!\n");
	printf("===========================================\n");
	printf("Welcome to Snakes and Ladders 3000!\nIt is a lot like regular Snakes And\nLadders but has the number 3000 after it.\nThere is no good reason for this.\n");
	printf("Players: 2 - 6\n");
	printf("===============Game Rules==================\n");
	printf("All players start at position 1 on the board.\n");
	printf("First to get to space 100 wins the game!\n");
	printf("Roll The Dice and move along the board.");
	printf("\nRoll a 4? Move 4 spaces.\n");
	printf("However, if you land on a snake:");
	printf("\nyou will slide down the board and have to work your way up again.\n");
	printf("If you land on a ladder:");
	printf("\nThe player may climb the ladder and take a nice shortcut to get to the top.\n");
	printf("Snakes and Ladders is a game of luck.\nSo... Good Luck!\n");
	printf("===========================================\n");
}

void CreateSnakes(sol_t snakes[], int size) {
	//fill an array with snake position and lengths


	int i;

	for (i = 0;i < size;i++) {
		//fill the array with snake objects
		do {
			snakes[i].position = (rand() % 89) + 11;


			//check that it falls with in parameters
			//snake position must not be on the first line
			//so not less than 11!
			//gets a random number between 0 and 88
			//increments it with 11, to ensure that its never less than 11
			//because it could return 0!
			//88 is the max because +11 is 99
			//stops snake from landing at 100
			snakes[i].length = ((rand() % (snakes[i].position - 10)) + 10);
			//must be less than the position
			//however minimum length is 10
			//if position is 11, the length must be 10, to move the player back to position 1. Else it would go negative, and thats bad!
			//if position is 50, 0-39 + 10, min length of 10, max length of 49 places back to get to 1
			//so we set the position as the parameter that it can go to, so if its 50, then we'll get a length from 10- 49

			//set end of snake
			snakes[i].end = snakes[i].position - snakes[i].length;

			//check for duplicate
		} while (IsDuplicate(snakes[i],snakes,i)> 0);
	
	}
}//create snakes


void CreateLadders(sol_t ladders[], sol_t snakes[], int size) {
	//fill an array with snake position and lengths
	//this function also takes in snakes, as ladders are created AFTER snakes and we need to make sure
	//that the ladder doesn't start where a snake starts OR end where a snake starts. That would be silly
	int maxPossibleLength;
	//the maximum length a ladder can be


	int i = 0;
	for (i = 0;i < size;i++) {
		//fill ladders with info
		//ladder position must be between 2 and 90
		do {
			ladders[i].position = (rand() % 89) + 2;
			//ensures it will be at least 2, and at most 90

			//length must be at least ten, most is dictated by position.
			//if the position is 50, must be at most 50
			//if the position is 60, must at most 40
			maxPossibleLength = 99 - ladders[i].position;
			if (maxPossibleLength < 10) {
				ladders[i].length = 9;
			}
			else if (maxPossibleLength == 10) {
				ladders[i].length = 10;
			}
			else {
				ladders[i].length = ((rand() % (maxPossibleLength - 9) + 10));
				//90 is max length for example
				//90-9, 81. Rand will give a number between  0 and 80 inclusive, +10
				//this means the ladder will be at least 10, at most 89 in length, since we can take a ladder to 99
				//dont want to build a ladder to 100

			}//else

			//set end point
			ladders[i].end = ladders[i].position + ladders[i].length;


			//check for dups within the current array and others!
		} while ((IsDuplicate(ladders[i], ladders, i) > 0) || (IsDuplicate(ladders[i], snakes, MAX) > 1));


	}//for
}//create ladders


void PrintSnakes(sol_t snakes[]) {

	int i;
	printf("\nSnakes\n");

	for (i = 0;i < 7;i++) {
		//just prints snakes

	//	printf("Snake %d position: %d Length %d End: %d\n", (i + 1), snakes[i].position, snakes[i].length, snakes[i].end);
		printf("Snake Positioned at square: %d Tail Located at: %d\n", snakes[i].position, snakes[i].end);
	}
}

void PrintLadders(sol_t ladders[]) {

	int i;
	printf("\nLadders\n");

	for (i = 0;i < 7;i++) {
		//just prints snakes

		//printf("Ladder %d position: %d Length %d End: %d\n", (i + 1), ladders[i].position, ladders[i].length, ladders[i].end);
		printf("Ladder Positioned at square: %d Reaches Square: %d\n", ladders[i].position, ladders[i].end);
	}
}


int IsDuplicate(sol_t object, sol_t list[], int size) {
	int i;
	int duplicate = -1;
	for (i = 0;i < size;i++) {
		if (object.position == list[i].position || object.end == list[i].position) {
			//if it matches the position of a ladder or a snake start point it can't do it!
			//if an object ends where another starts, it can't do that either!
			duplicate = 1;
			//printf("Is a dupe %d %d %d", object.position, object.end, list[i].position);

			return duplicate;

		}//if

	}//for
	//returns negative int if theres no duplicate
	return duplicate;


}//method

void BuildPlayerList(player_t *players, int count) {
	int i;
	for (i = 0;i < count;i++) {

		//we need to get a name
		printf("\nEnter Player %d's Name: ", i + 1); //prompt the user for input
		scanf("%s", (players+ i)->name);
		(players + i)->position = 1; //sets the default position of one, for a new game! hurray!
	}

}

void ShowPlayerList(player_t *players, int count) {
	int i;
	for (i = 0;i < count;i++) {
		printf("Player %d: %s Position: %d\n", i+1, (players+i)->name, (players+i)->position); 
	}

}

int RollDice() {
	int roll = (rand() % 6) + 1; //rand gets 0-5, +1 to ensure the result is between 1 and 6 because standard dice and all that
	printf("You rolled a %d\n", roll);
	return roll;
}

void TheGame(player_t *players, sol_t snakes[], sol_t ladders[], int playerCount,int playerIndex) {
	int userChoice;
	int victorYN = 0;//change to 1 if a player gets to 100
	int diceRoll;
	char *save;
	int currentPlayer = playerIndex;//start at the first player if new, or wherever they left off before
	//function that runs the main game
	//player turn 1 position 1
	//roll dice?
	//save and exit
	do {
		printf("Current Player: %s Position: %d\n\n", (players + currentPlayer)->name, (players + currentPlayer)->position);
		printf("%-14s Roll Dice\n", "1)");
		printf("%-14s Save and Exit\n", "0)");
		scanf("%d", &userChoice);
		//implement a switch
		switch (userChoice) {
		case 1: //lets play!

			diceRoll = RollDice();
			//change position
			(players + currentPlayer)->position += diceRoll;
			printf("%s is now at position : %d\n\n", (players + currentPlayer)->name, (players + currentPlayer)->position);
			//check three position versus snakes, ladders, and victory!

			(players + currentPlayer)->position = CheckLadders((players + currentPlayer)->position, ladders);

			victorYN = CheckVictory((players + currentPlayer)->position); //pass an integer

			if (victorYN > 0) {
				//print out the victory
				printf("\n%s has reached position 100! Victory Achieved!\n", (players + currentPlayer)->name);
				return;
			}

			//check versus snakes

			//checks if theres a snake match
			(players + currentPlayer)->position = CheckSnakes((players + currentPlayer)->position, snakes);




			

			//we need to continuously loop through the players until they either quit or the victor acheives  victory
			if (currentPlayer == (playerCount - 1)) {
				currentPlayer = 0;
				//if we go back to player 1, we want to print out the current status of all the players, as well as the locations of the snakes and ladders
				//just incase they forgot!
				printf("\n===========================================\n");
				printf("Snakes and Ladders 3000!\nCurrent Positions\n");
				printf("===========================================\n");
				ShowPlayerList(players, playerCount);
				PrintSnakes(snakes);
				PrintLadders(ladders);
			}
			else {
				currentPlayer++;
			}//if the current player is at the cap we must reset to 0 else we must keep going

			break;
		case 0:
			
		
			SaveAndExitGame(playerCount, currentPlayer, players, snakes, ladders);//creates and writes out to file

			break;
		default:
			printf("\nInavlid Input - Try again!\n");
		}
		
	} while (userChoice != 0);
	


	




}

int CheckVictory(int playerPos) {
	int victoryYN;
	if (playerPos >= 100) {
		victoryYN = 1; //hurray they win
	}
	else {
		victoryYN = 0; //keep playing
	}

	return victoryYN;
}

int CheckSnakes(int position, sol_t snakes[]) {
	//check the players position against each snake position
	int pos = position; 
	int i;
	for (i = 0;i < MAX;i++) {
		if (position == snakes[i].position) {
			printf("You landed on a Snake at Position: %d and slid down the board...\nNew Position %d\n",snakes[i].position,snakes[i].end);
			pos = snakes[i].end; //move them down to the location

		}
		else {
			//printf("Test code : No Snake %d\n", i);
		}
	}//for
	return pos; //if the player doesn't land on a snake, it will return the same number so the players position won't change!
	//if they did land on a snake, the players new position will be wherever the snake ends

}


int CheckLadders(int position, sol_t ladders[]) {
	//check the players position against each snake position
	int pos = position;
	int i;
	for (i = 0;i < MAX;i++) {
		if (position == ladders[i].position) {
			printf("You landed on a Ladder at Position: %d and climbed up the board...\nNew Position %d\n", ladders[i].position, ladders[i].end);
			pos = ladders[i].end; //move them down to the location

		}
		else {
			//printf("Test code : No Ladder %d\n", i);
		}
	}//for
	return pos; 

}


void SaveAndExitGame(int playerCount, int currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]) {

	//read in reference, store in array, write out to new file, k

	char fileSave[20] = "";
	int overWriteQ,overA;
	char files[20][20]; //maximum of 20 saves
	FILE *reference = fopen("reference.txt", "r");//opens a reference file to read
	int fileCount,i;

	char end[] = ".txt"; // for appending

	FILE *file;// create a file

	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount); //gets the number of files stored in the reference file
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}

	}
	else {
		fileCount = 0; //if it can't open the file, files are none!
	}



	//present user with the option overwrite previous save
	//or create a new one
	printf("\n===========================================\n");
	printf("Saving Snakes and Ladders 3000!\n");
	printf("===========================================\n");
	printf("Would you Like to Save Before Quitting?\n");
	printf("%10s New Save File\n", "1)");
	printf("%10s Overwrite Existing File\n", "2)");
	printf("%10s Quit Without Saving\n", "0)");
	scanf("%d", &overWriteQ);
	switch (overWriteQ) {
	case 1:
		//new save hurray
		//buffer
		printf("Name The file: (one word only, please)");
		scanf("%s", fileSave);
		strcat(fileSave, end);
		//appends .txt to the file
	
		break;
	case 2:
		//overwrite
		
		//present list of saves


		//pick one
		//make that the fileSave

		if (fileCount > 0) {
			for (i = 0;i < fileCount;i++) {
				printf("%d) %s\n", (i + 1), files[i]);
			}
			do {
				printf("Choose a File to OverWrite: (based off the number assigned):\n");
				scanf("%d", &overA);
				overA--; //decrement to ensure correct association with array
			} while (overA<0 || overA>fileCount);
			strcpy(fileSave, files[overA]);
			printf("You have chosen to overwrite %s\n", fileSave);
		}
		else {
			printf("Error, no files to overwrite!\n");
			return; //breaks out
		}
		break;
		case 0:
			printf("quitting without saving\n");
			return;
			//exit without saving
		break;
		default:
			printf("Invalid Choice - Exiting without saving \n");
			break;
	}//end of switch

	




	if(reference!=NULL)
	fclose(reference);

	CreateNewReferenceFile((fileCount), files, fileSave); 
	//creates a new reference file with the previous list of files anda  new one

	file = fopen(fileSave, "w"); //opens the file the user wanted to create to write


	//now we must write out to the file
	//first we will write out the players as well as the current index

	fprintf(file, "%d\n", playerCount);
	fprintf(file, "%d\n", currentPlayer);
	//player names and position
	for (i = 0;i < playerCount;i++) {

		fprintf(file, "%s %d\n", (players + i)->name, (players + i)->position); //writes out the player name and position
	}
	//then he will write out the snakes and ladders positions

	//always 7 snakes and ladders
	for (i = 0;i < MAX;i++) {

		fprintf(file, "%d %d %d\n", (snakes + i)->position, (snakes + i)->length, (snakes + i)->end);
	}

	for (i = 0;i < MAX;i++) {

		fprintf(file, "%d %d %d\n", (ladders + i)->position, (ladders + i)->length, (ladders + i)->end);
	}
	fprintf(file,"EOF");
	fclose(file);//closes the file


}

//a method to read in a file, hurray
int LoadGame(int fileChoice, int *playerCount, int *currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]) {
	//int playerCount, currentPlayer;

	//open reference file
	//fill array
	//close
	//show in loop
	//get choice
	//open file based off choice
	FILE *reference = fopen("reference.txt", "r");
	char fileName[20];
	FILE *file;

	int i;
	
	int fileCount;
	char files[20][20];


	//fill up the array

	if (reference != NULL) {
	fscanf(reference, "%d", &fileCount); //gets the number of files stored in the reference file
	for (i = 0;i < fileCount;i++) {
		fscanf(reference, "%s", files[i]);
		//read in each file name
	}
	fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return -1; //break out of function
	}

	
	
	if (fileCount > 0) {
		
		strcpy(fileName, files[fileChoice]);
		printf("You have chosen to open %s\n", fileName);

		file = fopen(fileName, "r"); //reads in the file
		printf("File Opened\n");

		//make sure it aint empty
		if (file != NULL) {
			//read in the player count
			fscanf(file, "%d", playerCount);
				
			fscanf(file, "%d", currentPlayer); 

			for (i = 0; i < *playerCount;i++) {
				//read in each player
				fscanf(file, "%s", (players + i)->name);
				fscanf(file, "%d", &(players + i)->position);

			}
			ShowPlayerList(players, *playerCount);
			//now read in the snake locations
			for (i = 0;i < MAX;i++) {

				fscanf(file, "%d %d %d", &(snakes + i)->position, &(snakes + i)->length, &(snakes + i)->end);
			
			}
			for (i = 0;i < MAX;i++) {

				fscanf(file, "%d %d %d", &(ladders + i)->position, &(ladders + i)->length, &(ladders + i)->end);
				
			}
		}//if
		fclose(file);
	}//if filecount is greater than 1
	

	return 1; 

}


int WhatFile() {


	char fileName[20];

	int i, count, userChoice;
	int fileChoice;
	int fileCount;
	char files[20][20];
	FILE *file;
	FILE *reference = fopen("reference.txt", "r");

	//fill up the array

	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount); //gets the number of files stored in the reference file
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}
		fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return -1; //break out of function
	}


	for (i = 0;i <fileCount;i++) {
		printf("%d) %s\n", (i + 1), files[i]);
	}
	if (fileCount > 0) {
		do {
			printf("Choose a File to Open: (based off the number assigned):\n");
			scanf("%d", &fileChoice);
			fileChoice--; //decrement to ensure correct association with array
		} while (fileChoice<0 || fileChoice>fileCount);
		strcpy(fileName, files[fileChoice]);
		//printf("You have chosen to open %s\n", fileName);


		file = fopen(fileName, "r"); //reads in the file
		if (file != NULL) {
			printf("Accessing File...\n");
			
			fclose(file);
			return fileChoice; //returns the index of the file in the array

			
		}
		else {
			printf("Error opening File");
			return -1; //if theres an error
		}
	}

}//whatfile

int GetPlayerCount(int fileChoice) {

	char fileName[20];

	int i, count, userChoice;
	int fileCount;
	char files[20][20];
	FILE *file;
	FILE *reference = fopen("reference.txt", "r");
	
//fill up the array

	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount); //gets the number of files stored in the reference file
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}
		fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return -1; //break out of function
	}


	
		strcpy(fileName, files[fileChoice]);
		//assigns the filename based on the index
		//printf("You have chosen to open %s\n", fileName);

		
		file = fopen(fileName, "r"); //reads in the file
		if (file != NULL) {
			//printf("File Opened\n");
			fscanf(file, "%d", &count);
			fclose(file);
			return count; //returns the player count

			fclose(file);
		}


}//get player count




int CheckFiles() {
	//open file
	FILE *file;
	int fileCount=0;
	//scrape the count off the top
	//close file
	file = fopen("reference.txt", "r");
	if (file != NULL) {
		fscanf(file, "%d", &fileCount);
		fclose(file);
	}
	return fileCount; //returns the number of files according to the reference file

}

void CreateNewReferenceFile(int fileCount, char files[20][20],char nFile[20]) {
	
	FILE *ref;
	int i;
	int sameQ,sameA;
	sameA = 0;
	
	
	removeFile("reference.txt");
	
	//removes the existing file
	

	for (i = 0;i < fileCount; i++) {
		//we must string compare each
		sameQ = strcmp(files[i], nFile);
		if (sameQ == 0) {
			sameA = 1;
		}
	}

	ref = fopen("reference.txt", "w");
	if (sameA!=1) {

		fprintf(ref, "%d", (fileCount+1));
		//if we didn't find a file name in the array thats the same as the one the user wants to save
		//we add to the total file count, because a new file is going to be added

	}
	else {
		fprintf(ref, "%d", fileCount);
		//print out the filecount
		//we are over writing, so filecount stays the same
	}

	

	for (i = 0;i < (fileCount);i++) {
		//minus 1 as fileCount adds the new file
		fprintf(ref, "\n%s",files[i]);
	}
	//now add the new file
	//check to see if the file already exists

	if (sameA != 1) {
		fprintf(ref, "\n%s", nFile);
		//we add the file name to the end
	}
	
	fclose(ref);
	printf("\nReference file has been updated to reflect changes\n");
}

void removeFile(char fileName[]) {
	int i;
	i=remove(fileName);
	if (i == 0) {
		printf("\n%s File Deleted\n",fileName);
	}
	else {
		printf("\ncannot find  %s\n", fileName);
	}

}//remove file test

void MenuRemoveFile() {
	//show reference files
	//which do you want to delete

	//removefile


	char fileName[20];

	int i;
	int fileChoice;
	int fileCount;
	char files[20][20];
	
	FILE *reference = fopen("reference.txt", "r");

	//fill up the array

	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount); //gets the number of files stored in the reference file
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}
		fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return; //break out of function
	}


	for (i = 0;i <fileCount;i++) {
		printf("%d) %s\n", (i + 1), files[i]);
	}
	if (fileCount > 0) {
		do {
			printf("Choose a File to Delete: (based off the number assigned):\n");
			scanf("%d", &fileChoice);
			fileChoice--; //decrement to ensure correct association with array
		} while (fileChoice<0 || fileChoice>fileCount);
		strcpy(fileName, files[fileChoice]);
		printf("You have chosen to delete %s\n", fileName);
		removeFile(fileName);

	}

	//remove the reference file
	//print it out again without the deleted file
	removeFile("reference.txt");
	if ((fileCount - 1) > 0) {
		//this if statement is here to prevent the creating of a blank reference file
		//resulting in an error if one tries to load it. Since the file will exist but be empty
		reference = fopen("reference.txt", "w");

		//now create file
		//write out the number
		fprintf(reference, "%d", (fileCount - 1));
		//need to remove 1 from the count, to reflect less files being present!
		//now write out the rest

		for (i = 0;i < fileCount;i++) {
			//if its the one we removed, dont write it out, otherwise go ahead
			if (strcmp(fileName, files[i]) == 0) {
				//do nothing
				//printf("No printy %s", fileName);
				//test code
			}
			else {
				//printf("\n%s printy yes! \n", files[i]);
				//test code to ensure working as intended

				fprintf(reference, "\n%s", files[i]);
			}
		}

		fclose(reference);
	}//if theres still a file to write out

}


