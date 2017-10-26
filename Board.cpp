#include "Board.h"
#include "CustomGameWindow.h"
#include <FL/fl_ask.H>
#include <FL/Fl_JPEG_Image.H>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
//getters
vector<vector<Cell*>> Board::getVector() { return this->gameBoard; }
vector<vector<int>> Board::getHighScores() { return this->highScores; }
bool Board::getDebug() { return this->debug; }
int Board::getNumCovered() { return this->numCovered; }
int Board::getNumMines() { return this->numMines; }
Fl_Box* Board::getProgressBox() { return this->progressBox; }

//setters
void Board::setGameBoard(vector<vector<Cell*>> gameBoard) { this->gameBoard = gameBoard; }
void Board::setHighScores(vector<vector<int>> highScores) { this->highScores = highScores; }
void Board::setDebug(bool debug) { this->debug = debug; }
void Board::setNumMines(int numMines) { this->numMines = numMines; }
void Board::setNumCovered(int numCovered) { this->numCovered = numCovered; }

//non-static functions
//adds mines to the given gameboard randomly
void Board::addMines(vector<vector<Cell*>>& gameBoard, int numMines) {
	while (numMines > 0) {
		int randX = rand() % gameBoard.size();
		int randY = rand() % gameBoard.at(0).size();
		if (!gameBoard.at(randX).at(randY)->isMine() && !gameBoard.at(randX).at(randY)->isEdge()) {
			gameBoard.at(randX).at(randY)->setMine(true);
			numMines--;
		}
	}
}

//calls the reset function in each cell
void Board::clearBoard() {
	for (int i = 0; i < this->gameBoard.size(); i++) {
		for (int j = 0; j < this->gameBoard.at(i).size(); j++) {
			gameBoard.at(i).at(j)->resetCell();
		}
	}
}

//generates a vector and adds mines to it
void Board::makeVector(vector<vector<Cell*>>& gameBoard, int rows, int columns, int numMines) {
	for (int i = 0; i < rows + 2; i++) {
		vector<Cell*> tempVector;
		for (int j = 0; j < columns + 2; j++) {
			Cell* cell;
			if (i == 0 || i == rows + 1 || j == 0 || j == columns + 1) {
				cell = new Cell(40 + (i * 20), 25 + (j * 20), i, j, 20, true);
			}
			else {
				cell = new Cell(40 + (i * 20), 25 + (j * 20), i, j, 20, false);
				cell->callback(openCells, this);
			}
			tempVector.push_back(cell);
		}
		gameBoard.push_back(tempVector);
	}
	addMines(gameBoard, numMines);
}

int Board::countSurroundingMines(int x, int y) {
	if (!this->getVector().at(x).at(y)->isEdge()) {
		int sum = 0;
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (this->getVector().at(i).at(j)->isMine()) {
					sum++;
				}
			}
		}
		return sum;
	}
	return 0;
}

int Board::countSurroundingFlags(int x, int y) {
	int sum = 0;
	if (!this->getVector().at(x).at(y)->isEdge()) {
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (this->getVector().at(i).at(j)->isFlagged()) {
					sum++;
				}
			}
		}
	}
	return sum;
}

void Board::revealCell(Cell* cell) {
	if (!cell->isMine() && !cell->isOpen() && !cell->isEdge() && !cell->isFlagged()) {
		cell->setImage(this->countSurroundingMines(cell->getXVec(), cell->getYVec())); //display the number of mines touching it
		cell->setOpen(true);
		this->setNumCovered(this->getNumCovered() - 1);
		cell->redraw();
		this->checkWin();
		if (this->countSurroundingMines(cell->getXVec(), cell->getYVec()) == 0)
			this->cascade(cell);
	}
	else if (cell->isMine() && !cell->isFlagged()) {
		loseGame(cell, this);
	}
}

void Board::cascade(Cell* cell) {
	this->revealCell(this->getVector().at(cell->getXVec() - 1).at(cell->getYVec()));
	this->revealCell(this->getVector().at(cell->getXVec() + 1).at(cell->getYVec()));
	this->revealCell(this->getVector().at(cell->getXVec()).at(cell->getYVec() - 1));
	this->revealCell(this->getVector().at(cell->getXVec()).at(cell->getYVec() + 1));
	this->revealCell(this->getVector().at(cell->getXVec() - 1).at(cell->getYVec() - 1));
	this->revealCell(this->getVector().at(cell->getXVec() + 1).at(cell->getYVec() + 1)); 
	this->revealCell(this->getVector().at(cell->getXVec() + 1).at(cell->getYVec() - 1));
	this->revealCell(this->getVector().at(cell->getXVec() - 1).at(cell->getYVec() + 1));
}

void Board::chordCell(Cell* cell) {
	for (int i = cell->getXVec() - 1; i <= cell->getXVec() + 1; i++) {
		for (int j = cell->getYVec() - 1; j <= cell->getYVec() + 1; j++) {
			if (i != cell->getXVec() || j != cell->getYVec()) {
				if (!this->getVector().at(i).at(j)->isOpen()) {
					this->revealCell(this->getVector().at(i).at(j));
				}
			}
		}
	}
}

void Board::checkWin() {
	if (this->getNumMines() == this->getNumCovered()) {
		for (int i = 0; i < this->getVector().size(); i++) {
			for (int j = 0; j < this->getVector().at(i).size(); j++) {
				this->getVector().at(i).at(j)->deactivate();
			}
		}
		this->getProgressBox()->label("You Win!");
		this->stopTimer();
		int temp = atoi(timer->label());
		this->newHighScore(this->level, temp);
	}
}

int Board::countFlags() {
	int sum = 0;
	for (int i = 0; i < this->getVector().size(); i++) {
		for (int j = 0; j < this->getVector().at(i).size(); j++) {
			if (this->getVector().at(i).at(j)->isFlagged()) {
				sum++;
			}
		}
	}
	return sum;
}

void Board::addOneMine(int i, int j) {
	int x = rand() % (this->getVector().size() - 1) + 1;
	int y = rand() % (this->getVector().at(1).size() - 1) + 1;
	if (this->getVector().at(x).at(y)->isMine() || (x == i && y == j) || this->getVector().at(x).at(y)->isEdge()) {
		this->addOneMine(i, j);

	}
	this->getVector().at(x).at(y)->setMine(true);
	if (this->getDebug()) {
		this->getVector().at(x).at(y)->debug(false);
		this->getVector().at(x).at(y)->redraw();
		this->getVector().at(i).at(j)->debug(false);
	}
}

void Board::newHighScore(char c, int time) {
	ofstream highScores;
	highScores.open("highscores.txt");
	switch (c) {
	case 'b':
		for (int i = 0; i < 3; i++) {
			if (time < this->getHighScores().at(0).at(i)) {
				this->highScores.at(0).insert(this->highScores.at(0).begin() + i, time);
				this->highScores.at(0).erase(this->highScores.at(0).begin() + 3);
				break;
			}
		}
		break;
	case 'i':
		for (int i = 0; i < 3; i++) {
			if (time < this->getHighScores().at(1).at(i)) {
				this->highScores.at(1).insert(this->highScores.at(1).begin() + i, time);
				this->highScores.at(1).erase(this->highScores.at(1).begin() + 3);
				break;
			}
		}
		break;
	case 'e':
		for (int i = 0; i < 3; i++) {
			if (time < this->getHighScores().at(2).at(i)) {
				this->highScores.at(2).insert(this->highScores.at(2).begin() + i, time);
				this->highScores.at(2).erase(this->highScores.at(2).begin() + 3);
				break;
			}
		}
		break;
	default: break;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			highScores << this->getHighScores().at(i).at(j) << " ";
		}
	}
}
//callbacks
//puts the game into and outof debug mode by calling each cells debug and flipping the boolean debug at the end
void Board::debugMode(Fl_Widget* w, void* v) {
	Board* board = static_cast<Board*>(v);
	for (int i = 0; i < board->getVector().size(); i++) {
		for (int j = 0; j < board->getVector().at(i).size(); j++) {
			board->getVector().at(i).at(j)->debug(board->getDebug());
		}
	}
	board->redraw();
	board->setDebug(!board->getDebug());

}

//calls each cells reset cell function
void Board::newBeginnerGame(Fl_Widget * b, void * v) {
	Board* board = static_cast<Board*>(v);
	board->stopTimer();
	Board* newBoard = new Board(9 * 20 + 120, 9 * 20 + 70, 10, 9, 9, 'b');
	newBoard->show();
	delete board;
}

//calls each cells reset cell function
void Board::newIntermediateGame(Fl_Widget * b, void * v) {
	Board* board = static_cast<Board*>(v);
	board->stopTimer();
	Board* newBoard = new Board(16 * 20 + 120, 16 * 20 + 70, 40, 16, 16, 'i');
	newBoard->show();
	delete board;
}

//calls each cells reset cell function
void Board::newExpertGame(Fl_Widget * b, void * v) {
	Board* board = static_cast<Board*>(v);
	board->stopTimer();
	Board* newBoard = new Board(30 * 20 + 120, 16 * 20 + 70, 99, 30, 16, 'e');
	newBoard->show();
	delete board;
}

//calls each cells reset cell function
void Board::newCustomGameWindow(Fl_Widget * w, void * v) {
	CustomGameWindow* window = new CustomGameWindow(300, 150);
	Fl_Button* ok = new Fl_Button(240, 100, 50, 40, "OK");
	Fl_Button* cancel = new Fl_Button(10, 100, 50, 40, "Cancel");
	Board* board = static_cast<Board*>(v);
	board->stopTimer();
	window->setBoard(board);
	cancel->callback(quit, window);
	ok->callback((Fl_Callback*)newCustomGame, window);
	window->show();
}

void Board::newCustomGame(Fl_Widget * b, void * v) {
	CustomGameWindow* window = static_cast<CustomGameWindow*>(v);
	int rows = atoi(window->getInputr()->value());
	int cols = atoi(window->getInputc()->value());
	int mines = atoi(window->getInputm()->value());
	if (rows > 16 || rows < 4) {
		fl_alert("Number of rows must be between 4 and 16");
	}
	else if (cols > 30 || cols < 4) {
		fl_alert("Number of columns must be between 4 and 30");
	}
	else if (mines >= (rows*cols) || mines < 1) {
		fl_alert("There can not be more mines than cells and there must be atleast one mine");
	}
	else{
		Board* newBoard = new Board(rows * 20 + 120, cols * 20 + 70, mines, rows, cols, 'c');
		Board* oldBoard = window->getBoard();
		newBoard->show();
		delete oldBoard;
		delete window;
	}
}

void Board::quit(Fl_Widget * w, void * v) {
	Fl_Window* window = static_cast<Fl_Window*>(v);
	delete window;
}

void Board::openCells(Fl_Widget* w, void* v) {
	Board* board = static_cast<Board*>(v);
	Cell* cell = static_cast<Cell*>(w);
	if ((board->timerIsRunning()) == false) {
		if (cell->isMine()) {
			cell->setMine(false);
			board->addOneMine(cell->getXVec(), cell->getYVec());
		}
		board->startTimer();
		board->getProgressBox()->label("In Progress");
	}
	if (cell->isMine() && !cell->isEdge() && !cell->isFlagged()) {
		loseGame(cell, board);
	}
	else if (!cell->isFlagged() && !cell->isMarked()) {
		int surrounding = board->countSurroundingMines(cell->getXVec(), cell->getYVec());
		if (!cell->isOpen() && !cell->isEdge() && !cell->isFlagged()) { //if the cell is not open
			board->revealCell(cell);
		}
		else if (cell->isOpen() && !cell->isFlagged()) {
			if (surrounding == board->countSurroundingFlags(cell->getXVec(), cell->getYVec()) && surrounding != 0) {
				board->chordCell(cell);
			}
		}
	}
	else {
		int numFlags = board->countFlags();
		board->counter->copy_label((to_string(board->getNumMines() - numFlags)).c_str());
	}
}

void Board::loseGame(Fl_Widget* w, void* v) {
	Board* board = static_cast<Board*>(v);
	Cell* cell = static_cast<Cell*>(w);
	if (!cell->isFlagged() && !cell->isMarked()) {
		cell->setLosing(true);
		cell->setImage(10);
		cell->redraw();
		board->stopTimer();
		board->getProgressBox()->label("You Lose!");
		for (int i = 0; i < board->getVector().size(); i++) {
			for (int j = 0; j < board->getVector().at(i).size(); j++) {
				if (board->getVector().at(i).at(j)->isMine() && !board->getVector().at(i).at(j)->isFlagged() && !board->getVector().at(i).at(j)->isLosing()) {
					board->getVector().at(i).at(j)->setImage(9);
					board->getVector().at(i).at(j)->redraw();
				}
				else if (!board->getVector().at(i).at(j)->isMine() && board->getVector().at(i).at(j)->isFlagged()) {
					board->getVector().at(i).at(j)->setImage(11);
					board->getVector().at(i).at(j)->redraw();
				}
				board->getVector().at(i).at(j)->deactivate();
			}
		}
	}
	else {
		int numFlags = board->countFlags();
		board->counter->copy_label((to_string(board->getNumMines() - numFlags)).c_str());
	}
}

void Board::highScoresPage(Fl_Widget* w, void* v) {
	Board* board = static_cast<Board*>(v);
	Fl_Window* scores = new Fl_Window(250, 150, "High Scores");
	Fl_Box* beginerLabel = new Fl_Box(20, 20, 50, 50, "Beginner");
	Fl_Box* beginer1 = new Fl_Box(20, 50, 50, 50, " ");
	Fl_Box* beginer2 = new Fl_Box(20, 80, 50, 50, " ");
	Fl_Box* beginer3 = new Fl_Box(20, 110, 50, 50, " ");
	beginer1->copy_label(to_string(board->getHighScores().at(0).at(0)).c_str());
	beginer2->copy_label(to_string(board->getHighScores().at(0).at(1)).c_str());
	beginer3->copy_label(to_string(board->getHighScores().at(0).at(2)).c_str());
	Fl_Box* interlabel = new Fl_Box(100, 20, 50, 50, "Intermediate");
	Fl_Box* inter1 = new Fl_Box(100, 50, 50, 50, " ");
	Fl_Box* inter2= new Fl_Box(100, 80, 50, 50, " ");
	Fl_Box* inter3 = new Fl_Box(100, 110, 50, 50, " ");
	inter1->copy_label(to_string(board->getHighScores().at(1).at(0)).c_str());
	inter2->copy_label(to_string(board->getHighScores().at(1).at(1)).c_str());
	inter3->copy_label(to_string(board->getHighScores().at(1).at(2)).c_str());
	Fl_Box* expertLabel = new Fl_Box(180, 20, 50, 50, "Expert");
	Fl_Box* expert1 = new Fl_Box(180, 50, 50, 50, " ");
	Fl_Box* expert2 = new Fl_Box(180, 80, 50, 50, " ");
	Fl_Box* expert3 = new Fl_Box(180, 110, 50, 50, " ");
	expert1->copy_label(to_string(board->getHighScores().at(2).at(0)).c_str());
	expert2->copy_label(to_string(board->getHighScores().at(2).at(1)).c_str());
	expert3->copy_label(to_string(board->getHighScores().at(2).at(2)).c_str());
	scores->show();
}

void Board::timerCallback(void* data) {
	Board* board = (Board*)data;
	board->incrementTimer();
	Fl::repeat_timeout(1.0, timerCallback, data);
}

void Board::aboutPageCallback(Fl_Widget* w, void* v) {
	Fl_Window* aboutPage = new Fl_Window(0, 0, 700, 700, "The about page");
	Fl_Box* groupPic = new Fl_Box(0, 0, 700, 700);
	Fl_JPEG_Image* pic = new Fl_JPEG_Image("pics/FinalGroupPic.jpg");
	groupPic->image(pic->copy(700,700));
	aboutPage->show();
}

void Board::incrementTimer() {
	int temp = atoi(timer->label());
	temp += 1;
	timer->copy_label(to_string(temp).c_str());
	timer->redraw();
}

void Board::startTimer() {
	Fl::add_timeout(1.0, timerCallback, this);
	timerRunning = true;
}

void Board::stopTimer() {
	Fl::remove_timeout(timerCallback);
	timerRunning = false;
}

bool Board::timerIsRunning() {
	return timerRunning;
}

Board::Board(int width, int heigth, int numMines, int rows, int columns, char level) : Fl_Window(width, heigth, "MineSweeper"), numCovered(rows * columns), numMines(numMines), debug(false), level(level) {

	makeVector(this->gameBoard, rows, columns, numMines);
	timer = new Fl_Box(20, heigth-35, 20, 40, "0");
	Fl_Box* timerLabel = new Fl_Box(20, heigth-55, 20, 40, "Timer");
	Fl_Box* counterLabel = new Fl_Box(width - 40, heigth - 55, 20, 40, "Mines");
	counter = new Fl_Box(width - 40, heigth - 35, 20, 40, "0");
	counter->copy_label(to_string(numMines).c_str());
	this->progressBox = new Fl_Box(((rows * 20 + 50) - 65) / 2, heigth - 27, 130, 30, "New Game");

	ifstream highScoresText = ifstream("highscores.txt");
	while (!highScoresText.eof()) {
		for (int i = 0; i < 3; i++) {
			vector<int> tempVec;
			for (int j = 0; j < 3; j++) {
				int tempInt;
				highScoresText >> tempInt;
				tempVec.push_back(tempInt);
			}
			highScores.push_back(tempVec);
		}
	}

	cout << highScores.at(0).at(0);


	Fl_Menu_Bar* bar = new Fl_Menu_Bar(0, 0, width, 30);
	bar->box(FL_THIN_UP_BOX);
	bar->add("&New Game/&Beginner", "^b", newBeginnerGame, this);
	bar->add("&New Game/&Intermediate", "^i", newIntermediateGame, this);
	bar->add("&New Game/&Expert", "^e", newExpertGame, this);
	bar->add("&New Game/&Custom", "^c", newCustomGameWindow, this);
	bar->add("&Info/&About", "^a", aboutPageCallback, this);
	bar->add("&Info/&High Scores", "^h", highScoresPage, this);
	bar->add("&Debug Mode", "^d", debugMode, this);
	bar->add("&Quit", "^q", quit, this);

}