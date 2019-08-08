#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>

struct {int x,y,score;} player;

int map[20][50], bestScore;

enum Map {SPACE, BIRD, BARRIER ,BORDER};

void gotoXY(int x,int y)	// for set a new x and y for console cursor position
{ 
	COORD coordinate;

	coordinate.X = x;
	coordinate.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) ,coordinate);
}

void welcome()
{
	printf("\tWelcome to the mini Flappy bird game.\n\n");
	printf("\n\tGame instructions:\n");
	printf("\n\t-> in this game your bird trying to flying between blocks in game map."\
		"\n\tyour bird falling down if don't try to fly ! Press space to move up ,wait to move down ! "\
		"\n\tRemember : be aware of BARRIER their will cause your fail ! that's all .\n\n\t-> If you want to exit press esc. \n");
	printf("\n\n\tPress any key to play game...");

	if(getch()==27)
		exit(0);

	system("cls");
}

void exit()
{

}

void faild()
{
	system("cls");
	printf("\n\n\t\tGAME OVER !\n\t\t\t YOU JUST FAILED !\n\t\t\t\tYOUR SCORE IS : %2d"\
		"\n\t\tpress enter to continue ...",player.score);
	_sleep(2000);
	fflush(stdin);
	getch();
}

void restartLevel()
{
	int i,j;

	srand(time(NULL));
	
	player.score = 0;

	for (i = 0; i < 20; i++)
		map[i][0] = map[i][49] = BORDER;

	for (i = 0; i < 50; i++)
		map[0][i] = map[19][i] = BORDER;

	for (i = 1; i < 19; i++)
		for (j = 1; j < 49; j++)
			map[i][j] = SPACE;

	player.x = 10;
	player.y = 20;

	map[player.x][player.y] = BIRD;
}

void drawMap()
{
	int i,j;

	gotoXY(0,0);

	printf("Flappy bird | SCORE : %02d ", player.score);

	for (i = 0; i < 25; i++)
		printf("%c", 177);

	printf("\n");

	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 50; j++)
			if (map[i][j] == SPACE)
				printf(" ");
			else if (map[i][j] == BARRIER)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				printf("%c",177);
			}
			else if (map[i][j] == BORDER)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("%c",177);
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				printf("*");
			}


		printf("\n");
	}
}

void play()
{
	int blockSize , i, j, newBlock = 0, key, move = 0, location=0;

	for (;;)
	{
		blockSize = rand() % 7 + 5;

		for (i = 1; i < 19; i++)
			for (j = 1; j < 48; j++)
				if (map[i][j + 1] != BIRD && map[i][j] != BIRD)
					map[i][j] = map[i][j+1];

		if ((newBlock++)%4 == 0)
		{
			for (i = 1; i < 19 ; i++)
				map[i][48] = map[19 - i][48] = BARRIER;

			for (i = blockSize; i < blockSize + 4; i++)
				map[i][48] = SPACE;
			
		}
		else
			for (i = 1; i < 10 + 1; i++)
				map[i][48] = map[19 - i][48] = SPACE;

		if (kbhit())
		{
			key = getch();

			if (key == 32)
				move = -1;
			else 
			{
				if(key == 27)
					exit();
				else
					printf("\a");
			}
		}
		else
			move = 1;

		if(player.x == 1 && move == -1)
			move = 1;

		map[player.x][player.y] = SPACE;
		player.x += move;
		if (map[player.x][player.y] == BARRIER || player.x == 19)
		{
			faild();
			break;
		}
		else
			map[player.x][player.y] = BIRD;

		if (location > 25)
		{
			if(newBlock%4 == 0)
				player.score++;
		}
		else
			location++;
		

		fflush(stdin); 

		drawMap();

		 _sleep(200);
	}
}

void Score()
{
	FILE *score;

	if ((score = fopen("score.dat","rb")) != NULL)
		fread(&bestScore, sizeof(int), 1,score);
	else
	{
		if ((score = fopen("score.dat","wb")) != NULL)
		{
			bestScore = player.score;
			fwrite(&bestScore, sizeof(int), 1, score);
		}
		else
		{
			printf("Sorry ,but can not save your record at this time .\nPress enter ...");
			fflush(stdin);
			getch();
			exit(1);
		}
	}

	if (bestScore < player.score)
	{
		bestScore = player.score;
		printf("\n\n\t\tWow ! you made a new record < %2d > \n",bestScore);
	}
	else
		printf("\n\n\t\tYour current record is < %02d > \n\n\t\tYour best record is < %02d >"
			,player.score, bestScore);
}

int main()
{
	int canExit = 1;

	welcome();

	do{
		system("cls");

		restartLevel();
		drawMap();
		
		play();

		Score();

		printf("\n\n\t\tWhould you like to play agian ? 1-yes\n\t\t\t\t\t\t2-no"\
			"\n\t\t\t\t\t\tYour Choise ( )\b\b");
		scanf("%d", &canExit);
	}while (canExit-1 == 0);
}
