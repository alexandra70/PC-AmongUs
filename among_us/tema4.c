#include "utils.h"

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void* RotateMatrix(void* input)
{
	// TODO

	int* n;
	n = input;
	int size = *n;
	int a[size][size];
	
	int size_aux = size;
	int nr_cifre = 0;

	while (size_aux != 0) {
		nr_cifre++;
		size_aux /= 10;
	}

	char* ret = malloc(size * (size + 1) * (nr_cifre * 2 + 1));
	char* numar = malloc(nr_cifre * 2 + 1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			sprintf(numar, "%d%d\0", i+1, j+1);
			a[i][j] = atoi(numar);
		}
	}

	for (int i = 0; i < size/2; i++) {
		for (int j = i; j < size - i - 1; j++) {
			int var = a[i][j];
			a[i][j] = a[size - j - 1][i];
			a[size - j - 1][i] = a[size - i - 1][size - j - 1];
			a[size - i - 1][size - j - 1] = a[j][size - i - 1];
			a[j][size - i - 1] = var; 
		}
	}
	
	int j;
	int offset = 0;
	for (int i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			offset += sprintf(ret + offset, "%d", a[i][j]);
			if(j != size-1)
				offset += sprintf(ret + offset, " ");
			else {
				if (i != size - 1)
					offset += sprintf(ret + offset, "\n");
				else
					offset += sprintf(ret + offset, "\0");
			}
		}		
	}
	
	free(numar);

	return (void*)ret;
}

// Task 2
void *DecodeString(void *input)
{

	char* sir = input;

	int suma = atoi(sir);

	for (int i = 0; i < strlen(sir); i++) {
		if (sir[i] == '_') {
			suma += atoi(sir + i + 1);
		}
	}

	char* ret = malloc(10*sizeof(char));
	
	sprintf(ret, "%d", suma);
	return (void*)ret;
}

//Task 3
void *InvertArray(void *input)
{
	int* vector = input;

	int* ret = malloc(vector[0] * sizeof(int));
	
	if (vector[0] % 2 == 0) {
		for (int i = 0; i < vector[0]; i += 2) {
			ret[i] = vector[i + 2];
			ret[i + 1] = vector[i + 1];
		}
	}
	else {
		for (int i = 0; i < vector[0]; i++) {
			ret[i] = vector[vector[0] - i];
		}
	}
	
	//maxim de int 10 -- cifre + 1 pt spatii
	char* sir = malloc(vector[0] * sizeof(char)*11);

	int offset = 0;

	for (int i = 0; i < vector[0]; i++) {
		offset += sprintf(sir + offset, "%d ", ret[i]);
	}
	
	sir[offset - 1] = '\0';
	free(ret);
	return (void*)sir;
}


//Task 4
Player *allocPlayer()
{
	
	Player* player = (Player*)malloc(sizeof(Player));
	
	player->name = malloc(sizeof(MAX_LEN_STR_ATR));
	player->color = malloc(sizeof(MAX_LEN_STR_ATR));
	player->hat = malloc(sizeof(MAX_LEN_STR_ATR));
	player->alive = 1;
	

	return player;
}

//Task 4
Game *allocGame()
{
	Game* game = (Game*)malloc(sizeof(Game));

	game->name = malloc(sizeof(MAX_LEN_STR_ATR));

	return game;
}

//Task 5
Player* ReadPlayer(FILE* inputFile )
{
	Player* player = allocPlayer();
	
	fscanf(inputFile, "%s\n", player->name);

	//printf("%s\n", player->name);

	fscanf(inputFile, "%s\n", player->color);

	fscanf(inputFile, "%s\n", player->hat);

	fscanf(inputFile, "%d\n", &(player->numberOfLocations));

	player->locations = malloc(sizeof(Location) * player->numberOfLocations);
		
	int j;

	for (j = 0; j < player->numberOfLocations; j++) {

		if (j == player->numberOfLocations -1) {
			fscanf(inputFile, "(%d,%d)\n", &(player->locations[j].x), &(player->locations[j].y));
		}
		else
			fscanf(inputFile, "(%d,%d) ", &(player->locations[j].x), &(player->locations[j].y));

	}
	char* roleAsString = malloc(50);
	fscanf(inputFile, "%s\n", roleAsString);
	if (strcmp(roleAsString, "Rotator")==0)
		player->playerRole = Rotator;
	if (strcmp(roleAsString, "Decoder")==0)
		player->playerRole = Decoder;
	if (strcmp(roleAsString, "Invertor")==0)
		player->playerRole = Invertor;
	if (strcmp(roleAsString, "Impostor")==0)
		player->playerRole = Impostor;
	free(roleAsString);

	player->ability = Abilities[player->playerRole];

	return player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	Game* game = allocGame();
	fscanf(inputFile, "%s\n", game->name);

	fscanf(inputFile, "%d\n", &(game->killRange));

	fscanf(inputFile, "%d\n", &(game->numberOfCrewmates));

	game->crewmates = (Player**)malloc(game->numberOfCrewmates * sizeof(Player*));

	for (int i = 0; i < game->numberOfCrewmates; i++) {
		game->crewmates[i] = ReadPlayer(inputFile);
	}

	game->impostor = ReadPlayer(inputFile);

	return game;

}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	//printf("%s\n", player->name);
	fprintf(outputFile, "Player %s ", player->name);
	fprintf(outputFile, "with color %s, ", player->color);
	fprintf(outputFile, "hat %s and ", player->hat);	

	char *s = fromEnumtoString(player->playerRole);

	fprintf(outputFile, "role %s has entered the game.\n", s);
	
	fprintf(outputFile, "Player's locations: ");
	for (int i = 0; i < player->numberOfLocations; i++) {
		fprintf(outputFile, "(%d,%d) ", player->locations[i].x, player->locations[i].y);
	}
	fprintf(outputFile, "\n");

	free(s);

	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "	Game options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n", game->numberOfCrewmates);
	fprintf(outputFile, "\n");
	fprintf(outputFile, "	Crewmates:\n");

	int i;
	for (i = 0; i < game->numberOfCrewmates; i++) {
		WritePlayer(game->crewmates[i], outputFile);
	}
	//impostor
	fprintf(outputFile, "\n");
	fprintf(outputFile, "	Impostor:\n");
	WritePlayer(game->impostor, outputFile);

	return;
}

//Task 7
void *KillPlayer(void *input)
{
	
	char* out = (char*)malloc(300*sizeof(char));

	Game* game = input;

	int min = 10000;
	int indice_min = -1;
	int dist = 0;
	for (int i = 0; i < game->numberOfCrewmates; i++) {
		if (game->crewmates[i]->alive == 1) {
			dist = abs(game->impostor->locations[game->impostor->indexOfLocation].x - game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].x)
				+ abs(game->impostor->locations[game->impostor->indexOfLocation].y - game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].y);
			if (dist <= min) {
				indice_min = i;
				min = dist;
			}
		}
	}
	if (min <= game->killRange) {
		game->crewmates[indice_min]->alive = 0;
		sprintf(out, "Impostor %s has just killed crewmate %s from distance %d.",
			game->impostor->name, game->crewmates[indice_min]->name, min);
	} else {
		sprintf(out, "Impostor %s couldn't kill anybody.",
			game->impostor->name);
	}
	
	return (void*)out;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{


	for (int i = 0; i < game->numberOfCrewmates; i++) {
		if (game->crewmates[i]->alive == 0) {
			fprintf(outputFile, "Crewmate %s is dead.\n", game->crewmates[i]->name);
			continue;
		}

		game->crewmates[i]->indexOfLocation++;
		game->crewmates[i]->indexOfLocation = game->crewmates[i]->indexOfLocation % game->crewmates[i]->numberOfLocations;
		
		fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n",
			game->crewmates[i]->name,
			game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].x, 
			game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].y);
		char* bla = (char*)game->crewmates[i]->ability(inputMatrix[i]);
		fprintf(outputFile, "Crewmate %s's output:\n%s\n", 
			game->crewmates[i]->name, 
			bla);
		free(bla);
	
	}

	game->impostor->indexOfLocation++;
	game->impostor->indexOfLocation = game->impostor->indexOfLocation % game->impostor->numberOfLocations;

	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n",
		game->impostor->name,
		game->impostor->locations[game->impostor->indexOfLocation].x,
		game->impostor->locations[game->impostor->indexOfLocation].y);
	char* bla = (char*)game->impostor->ability((void*)game);
	fprintf(outputFile, "Impostor %s's output:\n%s\n",
		game->impostor->name,
		bla);
	free(bla);
	return;
}

// Task 9
void FreePlayer(Player *player)
{
	free(player->name);
	free(player->color);
	free(player->hat);

	free(player->locations);
	free(player);

	return;
}

// Task 9
void FreeGame(Game *game)
{
	free(game->name);
	for (int i = 0; i < game->numberOfCrewmates; i++) {
		FreePlayer(game->crewmates[i]);
	}
		
	FreePlayer(game->impostor);

	free(game->crewmates);

	free(game);

	return;
}