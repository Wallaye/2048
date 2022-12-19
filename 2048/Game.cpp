#include "pch.h"
#include "Game.h"
#include <random>

Game::Game(int size, WCHAR* name) {
	lstrcpyW(playerName, name);
	this->size = size;
	this->canMakeMove = true;
	this->IsGame = true;
	Initialize();
}
Game::~Game() {};

void Game::Initialize() {
	for (int i = 0; i < size; i++) {
		std::vector<int> temp(size);
		for (auto el : temp) {
			el = 0;
		};
		this->field.push_back(temp);
	}
	GenerateNumber();
	GenerateNumber();
}

bool Game::GenerateNumber() {
	int result = rand() % 20;                               // generate 2 or 4
	if (result < 18) {
		result = 1;
	}
	else {
		result = 2;
	}
	int posi, posj;
	int count = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (field[i][j] == 0) {
				if (count == 0) {
					posi = i;
					posj = j;
				}
				count++;
				if ((rand() % 3) == 1) {                     //creates a random position
					posi = i;
					posj = j;
				}
			}
		}
	}
	if (count > 0) {                                         
		field[posi][posj] = result;
		return true;
	}
	else return false;
}

bool Game::MakeMove(DIRECTION dir) {
	bool success = false;
	switch (dir) {
	case TOP:
		success = ShiftElementsUp();
		break;
	case LEFT:
		TurnFieldClockwise(field, 1);
		success = ShiftElementsUp();
		TurnFieldClockwise(field, 3);
		break;
	case BOTTOM:
		TurnFieldClockwise(field, 2);
		success = ShiftElementsUp();
		TurnFieldClockwise(field, 2);
		break;
	case RIGHT:
		TurnFieldClockwise(field, 3);
		success = ShiftElementsUp();
		TurnFieldClockwise(field, 1);
		break;
	}
	return success;
}

void Game::TurnFieldClockwise(std::vector<std::vector<int>> &field, int count) {
	int buffer;
	for (int k = 0; k < count; k++) {
		for (int i = 0; i < size / 2; i++)
			for (int j = i; j < size - 1 - i; j++)
			{
				buffer = field[i][j];
				field[i][j] = field[size - j - 1][i];
				field[size - j - 1][i] = field[size - i - 1][size - j - 1];
				field[size - i - 1][size - j - 1] = field[j][size - i - 1];
				field[j][size - i - 1] = buffer;
			}
	}
}

bool Game::CheckForEnd() {
	std::vector<std::vector<int>> arr;
	std::vector<int> temp;
	for (int i = 0; i < size; i++) {
		std::vector<int> temp(size);
		for (int j = 0; j < size; j++) {
			temp[j] = field[i][j];
		}
		arr.push_back(temp);
	}
	bool can_continue = false;
	for (int k = 0; k < 4 && !can_continue; k++) {
		for (int i = 0; i < size && !can_continue; i++) {
			for (int j = 0; j < size && !can_continue; j++) {
				if (arr[i][j] == 0) can_continue = true;
			}
		}
		if (!can_continue) {
			for (int i = 0; i < size && !can_continue; i++) {
				for (int j = 0; j < size - 1 && !can_continue; j++) {
					if (arr[j][i] == arr[j + 1][i]) {
						can_continue = true;
					}
				}
			}
		}
		TurnFieldClockwise(arr, 1);
	}
	return can_continue;
}

int Game::FindMaxPower() {
	int maxPower = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (maxPower < field[i][j]) {
				maxPower = field[i][j];
			}
		}
	}
	return maxPower;
}

bool Game::ShiftElementsUp() {
	bool success = false;
	for (int i = 0; i < size; i++) {                                              //check every cell
		for (int j = 0; j < size - 1; j++) {
			if (field[j][i] == 0) {                                                 //if found null cell
				for (int k = j + 1; k < size; k++) if (field[k][i] != 0)           //search in column for non null cell
				{                                                                //shift up if non null cell is found
					field[j][i] = field[k][i];
					field[k][i] = 0;
					success = true;
					break;
				}
			}
		}
	}
	//trying to merge
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (field[j][i] == field[j + 1][i] && field[j][i] != 0) {
				score += atoi(array_of_accordance[field[j][i]].str);
				field[j][i]++;
				field[j + 1][i] = 0;
				success = true;
			}
		}
	}
	//secondary merge
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (field[j][i] == 0) {
				for (int k = j; k < size; k++) {
					if (field[k][i] != 0) {
						score += atoi(array_of_accordance[field[j][i]].str);
						field[j][i] = field[k][i];
						field[k][i] = 0;
						break;
					}
				}
			}
		}
	}
	return success;
}