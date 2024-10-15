#pragma once

typedef struct playerStep
{
	int row;
	int coloumn;
}playerStep;

typedef enum screen {
	boardZone,
	playZone,
	infoZone,
	settingZone,
	menuZone,
	prePlayZone,
	chooseLvlZone,
	winZone
} screenMode;


typedef void (*lvl_ptr)(int*, int*, int, int, char**);

int Game(playerStep* humStep, playerStep* compStep, int field_size, char who_first, int len_of_win_line, int difficulty_lvl, char** upload_or_free_board, GLFWwindow* window);

void humanMove(int* humRow, int* humcoloumn, int field_size, char** upload_or_free_board, playerStep humStep);

lvl_ptr whatLvl(int difficulty_lvl);
int inputProtection(int* humRow, int* humColumn, int sizeR, int sizeC, char* str_humRow, char* str_humColumn, int field_size);
void computerMoveLvlEasy(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
int findPlace(int r, int c, int* compRow, int* compColumn, int field_size, char** upload_or_free_board);

void computerMoveLvlMed(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlMed_Small(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlMed_Med(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlMed_Big(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);

void computerMoveLvlHard(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlHard_Small(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlHard_Med(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlHard_Big(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);

void computerMoveLvlVeryHard(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlVeryHard_Small(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlVeryHard_Med(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);
void computerMoveLvlVeryHard_Big(int* compRow, int* compColumn, int field_size, int len_of_win_line, char** upload_or_free_board);

void checkAttacks(int* max_i, int* max_j, int* max_heckStr, int* potential, char player, int field_size, int len_of_win_line, char** upload_or_free_board);

int checkBoards(int compRow, int compColumn, int max_heckStr_comp, int field_size, int len_of_win_line, char** upload_or_free_board);

void deleteFictitious(int field_size, char** upload_or_free_board);

int checkFreeSpaces(int field_size, char** upload_or_free_board);

int checkLines(int checkStr, int row, int column, char player, int field_size, int len_of_win_line, char** upload_or_free_board);

char checkWinner(int row, int column, char player, int field_size, int len_of_win_line, char** upload_or_free_board);

char** readFile(char** upload_or_free_boardF, int* field_size, char* who_first, int* len_of_win_line, int* difficulty_lvl);

void writeToFile(char** upload_or_free_board, int field_size, char who_first, int len_of_win_line, int difficulty_lvl);

void drawBoard(GLFWwindow* window, playerStep* humStep, playerStep* compStep);


