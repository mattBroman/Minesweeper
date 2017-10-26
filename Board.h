#pragma once
#include <Windows.h>
#include<FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Timer.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>
#include<vector>
#include"Cell.h"

class Board : public Fl_Window {

private:
	std::vector<std::vector<Cell* >> gameBoard;
	std::vector<std::vector<int>> highScores;
	int numCovered;
	int numMines;
	bool debug;
	bool timerRunning;
	Fl_Box* timer;
	Fl_Box* progressBox;
	Fl_Box* counter;
	char level;

public:
	//constuctor
	Board(int width, int heigth, int numMines, int rows, int columns, char level);

	//getters
	std::vector<std::vector <Cell*>> getVector();
	std::vector<std::vector<int>> getHighScores();
	bool getDebug();
	int getNumCovered();
	int getNumMines();
	Fl_Box* getProgressBox();

	//setters
	void setGameBoard(std::vector<std::vector<Cell*>> gameBoard);
	void setHighScores(std::vector<std::vector<int>> highScores);
	void setDebug(bool debug);
	void setNumCovered(int numCovered);
	void setNumMines(int numMines);

	//non-static functions
	void makeVector(std::vector<std::vector<Cell*>>& gameBoard, int rows, int columns, int numMines);
	void clearBoard();
	void addMines(std::vector<std::vector<Cell*>>& gameBoard, int numMines);
	int countSurroundingMines(int x, int y);
	int countSurroundingFlags(int x, int y);
	void revealCell(Cell* cell);
	void chordCell(Cell* cell);
	void cascade(Cell* cell);
	int countFlags();
	void addOneMine(int i, int j);
	void incrementTimer();
	void startTimer();
	void stopTimer();
	bool timerIsRunning();
	void checkWin();
	void newHighScore(char c, int time);

	//callbacks
	static void newBeginnerGame(Fl_Widget* b, void* v);
	static void newIntermediateGame(Fl_Widget* b, void* v);
	static void newExpertGame(Fl_Widget* b, void* v);
	static void newCustomGame(Fl_Widget* b, void* v);
	static void newCustomGameWindow(Fl_Widget* w, void* v);
	static void debugMode(Fl_Widget* b, void* v);
	static void openCells(Fl_Widget* w, void* v);
	static void quit(Fl_Widget* w, void* v);
	static void loseGame(Fl_Widget* w, void* v);
	static void timerCallback(void* data);
	static void aboutPageCallback(Fl_Widget* w, void* v);
	static void highScoresPage(Fl_Widget* w, void* v);
};