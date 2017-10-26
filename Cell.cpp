#include"Cell.h"
#include<FL/Fl.H>
#include<FL/Fl_JPEG_Image.H>
#include<iostream>
using namespace std;

//images for the tiles in jpg form because pngs are weird.
Fl_JPEG_Image* shark = new Fl_JPEG_Image("pics/Shark.jpg");
Fl_JPEG_Image* sharkLose = new Fl_JPEG_Image("pics/mine(activated).jpg");
Fl_JPEG_Image* wrong = new Fl_JPEG_Image("pics/Bouy3.jpg");
Fl_JPEG_Image* open = new Fl_JPEG_Image("pics/OpenedBox.jpg");
Fl_JPEG_Image* questionMark = new Fl_JPEG_Image("pics/marked.jpg");
Fl_JPEG_Image* closed = new Fl_JPEG_Image("pics/UnopenedBox.jpg");
Fl_JPEG_Image* booey = new Fl_JPEG_Image("pics/Bouy2.jpg");
Fl_JPEG_Image* one = new Fl_JPEG_Image("pics/Num1.jpg");
Fl_JPEG_Image* two = new Fl_JPEG_Image("pics/Num2.jpg");
Fl_JPEG_Image* three = new Fl_JPEG_Image("pics/Num3.jpg");
Fl_JPEG_Image* four = new Fl_JPEG_Image("pics/Num4.jpg");
Fl_JPEG_Image* five = new Fl_JPEG_Image("pics/Num5.jpg");
Fl_JPEG_Image* six = new Fl_JPEG_Image("pics/Num6.jpg");
Fl_JPEG_Image* seven = new Fl_JPEG_Image("pics/num7.jpg");
Fl_JPEG_Image* eight = new Fl_JPEG_Image("pics/num8.jpg");


//constuctor calling the Fl_Button super class, setting all boolean data members to false and making the image closed.
Cell::Cell(int x, int y, int i, int j, int size, bool edge) : Fl_Button(x, y, size, size), Mine(false), Open(false), Flagged(false), Marked(false), Edge(edge), xVec(i), yVec(j), losing(false) {
	this->box(FL_NO_BOX);
	if (!this->isEdge()) {
		this->image(*closed);
	}
	this->align(-1);
	this->redraw();
}

Cell::~Cell() {

}

//setters
void Cell::setMine(bool mine) { this->Mine = mine; }
void Cell::setOpen(bool open) { this->Open = open; }
void Cell::setMarked(bool marked) { this->Marked = marked; }
void Cell::setFlagged(bool flagged) { this->Flagged = flagged; }
void Cell::setLosing(bool losing) { this->losing = losing; }
void Cell::setXVec(int x) { this->xVec = x; }
void Cell::setYVec(int y) { this->yVec = y; }

//getters
bool Cell::isOpen() { return this->Open; }
bool Cell::isMine() { return this->Mine; }
bool Cell::isFlagged() { return this->Flagged; }
bool Cell::isMarked() { return this->Marked; }
bool Cell::isEdge() { return this->Edge; }
bool Cell::isLosing() { return this->losing; }
int Cell::getXVec() { return this->xVec; }
int Cell::getYVec() { return this->yVec; }

//handler
int Cell::handle(int Event) {
	switch (Event) {
	case(FL_PUSH) : //when the mouse is pressed down on a cell
		cout << this->isMine() << endl;
		if (Fl::event_button() == FL_LEFT_MOUSE) { // if it is the left mouse
			if (!this->isFlagged()) {
				this->setMarked(false);
				this->do_callback();
			}
		}
		else if (Fl::event_button() == FL_RIGHT_MOUSE) { //if the cell is the right mouse
			if (!this->isOpen() && !this->isMarked() && !this->isFlagged() && !this->isEdge()) { // the cell is not open has no flag on it
				this->setFlagged(true);
				this->do_callback();
				this->image(*booey);
				this->redraw();
				return 1;
			}
			else if (!this->isOpen() && this->isFlagged() && !this->isEdge()) { //if the cell is not open and flagged
				this->setFlagged(false);
				this->setMarked(true);
				this->do_callback();
				this->image(*questionMark);
				this->redraw();
				return 1;
			}
			else if (!this->isOpen() && !this->isEdge()) { // if the cell is not open and question marked
				this->setMarked(false);
				this->image(*closed);
				this->redraw();
				return 1;
			}
		}
		break;
	default:
		return Fl_Widget::handle(Event);
		break;
	}
	return 0;
}

void Cell::resetCell() { // reset all boolean data members to false and reset the image
	if (!this->isEdge()) {
		this->setMine(false);
		this->setOpen(false);
		this->setFlagged(false);
		this->setMarked(false);
		this->image(*closed);
	}
}

void Cell::debug(bool debug) { // when false ever cell gets a " ", else mines are g
	if (debug || !this->isMine()) {
		this->label(" ");
	}
	else {
		this->label("x");
	}
}

void Cell::setImage(int i) {
	switch (i) {
	case 0: this->image(*open);
		break;
	case 1: this->image(*one);
		break;
	case 2: this->image(*two);
		break;
	case 3: this->image(*three);
		break;
	case 4: this->image(*four);
		break;
	case 5: this->image(*five);
		break;
	case 6: this->image(*six);
		break;
	case 7: this->image(*seven);
		break;
	case 8: this->image(*eight);
		break;
	case 9: this->image(*shark);
		break;
	case 10: this->image(*sharkLose);
		break;
	case 11: this->image(*wrong);
		break;
	default: break;
	}
}