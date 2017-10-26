#pragma once
#include <Windows.h>
#include<FL/Fl_Window.H>;
#include<FL/Fl_Input.H>;
#include"Board.h"

class CustomGameWindow : public Fl_Window {
private:
	int numMines;
	int numRows;
	int numColumns;
	Board* board;
	Fl_Int_Input* inputr;
	Fl_Int_Input* inputc;
	Fl_Int_Input* inputm;
public:
	//constructor
	CustomGameWindow(int width, int heigth);

	//getters
	Board* getBoard();
	Fl_Int_Input* getInputr();
	Fl_Int_Input* getInputc();
	Fl_Int_Input* getInputm();

	//setter
	void setBoard(Board* board);

};