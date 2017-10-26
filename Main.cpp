#include <Windows.h>
#include<FL/Fl_Window.H>
#include<FL/Fl_Widget.H>
#include<FL/Fl_Button.H>
#include<FL/Fl.H>
#include<ctime>
#include<math.h>
#include"Board.h"
#include"Cell.h"
using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));
	Board* board = new Board(16 * 20 + 120, 16 * 20 + 70, 40, 16, 16, 'i');
	board->show(argc, argv);
	Fl::run();
}