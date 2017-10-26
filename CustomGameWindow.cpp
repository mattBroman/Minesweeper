#include"CustomGameWindow.h"

CustomGameWindow::CustomGameWindow(int width, int heigth) : Fl_Window(width, heigth, "Custom Game"), numRows(0), numColumns(0), numMines(0), board(nullptr) {
	this->inputr = new Fl_Int_Input(160, 0, 100, 30, "Enter # rows (4-16):");
	this->inputc = new Fl_Int_Input(160, 25, 100, 30, "Enter # columns (4-30):");
	this->inputm = new Fl_Int_Input(160, 50, 100, 30, "Enter # mines:");
}
//setters
void CustomGameWindow::setBoard(Board* board) { this->board = board; }
//getters
Board* CustomGameWindow::getBoard() { return this->board; }
Fl_Int_Input* CustomGameWindow::getInputr() { return this->inputr; }
Fl_Int_Input* CustomGameWindow::getInputc() { return this->inputc; }
Fl_Int_Input* CustomGameWindow::getInputm() { return this->inputm; }