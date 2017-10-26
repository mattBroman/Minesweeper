#pragma once
#include <Windows.h>
#include<FL/Fl_Button.H>
#include<vector>

class Cell : public Fl_Button {

private:
	bool Mine;
	bool Open;
	bool Flagged;
	bool Marked;
	bool Edge;
	bool losing;
	int xVec;
	int yVec;

public:
	//constuctor
	Cell(int x, int y, int i, int j, int size, bool edge);
	~Cell();

	//getters
	bool isMine();
	bool isOpen();
	bool isFlagged();
	bool isMarked();
	bool isEdge();
	bool isLosing();
	int getXVec();
	int getYVec();

	//setters
	void setOpen(bool open);
	void setMine(bool mine);
	void setFlagged(bool flagged);
	void setMarked(bool marked);
	void setLosing(bool lose);
	void setXVec(int x);
	void setYVec(int y);

	//handler & callbacks
	int handle(int Event);
	void resetCell();
	void debug(bool debug);
	void setImage(int i);
};