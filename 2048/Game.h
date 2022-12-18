#pragma once
#include <vector>
#include <map>
class Game
{
public:
	int score;
	WCHAR* playerName;
	int **field;
	int size;
	Game(int);
	~Game();
	
	void Initialize();
	bool GenerateNumber();
	bool MakeMove(DIRECTION dir);
	void TurnFieldClockwise(int);
	bool ShiftElementsUp();
	bool CheckForEnd();
	int FindMaxPower();
	const t_array array_of_accordance[21] = {
	{0, ""},
	{1, "2"},
	{2, "4"},
	{3, "8"},
	{4, "16"},
	{5, "32"},
	{6, "64"},
	{7, "128"},
	{8, "256"},
	{9, "512"},
	{10, "1024"},
	{11, "2048"},
	{12, "4096"},
	{13, "8192"},
	{14, "16384"},
	{15, "32768"},
	{16, "65536"},
	{17, "131072"},
	{18, "262144"},
	{19, "524288"},
	{20, "1048576"},
	};
};

struct t_array {                
	short index;         //the power of 2 to store
	char str[8];		 //the string to output
};

enum DIRECTION {
	TOP,
	RIGHT, 
	BOTTOM, 
	LEFT
};

