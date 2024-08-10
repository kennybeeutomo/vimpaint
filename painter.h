#pragma once
#include <array>
#include <curses.h>
#include <string>
#include <vector>
#include "palette.h"
#include "buffer.h"
#include "inputReader.h"

#define PAINTER_PALETTE_SIZE 12
#define PAINTER_HORIZONTAL_CHARACTER 0x2500
#define PAINTER_VERTICAL_CHARACTER 0x2502
#define PAINTER_TOPLEFT_CHARACTER 0x250C
#define PAINTER_TOPRIGHT_CHARACTER 0x2510
#define PAINTER_BOTTOMLEFT_CHARACTER 0x2514
#define PAINTER_BOTTOMRIGHT_CHARACTER 0x2518

class Keybind;

enum class Mode {Normal, Visual, Replace, Draw, Resize};

const char* mode2CString(Mode _mode);
std::string mode2String(Mode _mode);

struct Position {
	short x;
	short y;
	Position();
	Position(short _x, short _y);
	Position operator+(Position _position) const;
	Position operator-(Position _position) const;
};

Position topLeft(Position _position1, Position _position2);
Position bottomRight(Position _position1, Position _position2);

class Painter {
public:
	Painter(Buffer* _buffer);
	void display();

	chtype getCharAtCursor() const;
	void setCharAtCursor(chtype _ch);
	void setCharAtCursor();

	void replaceChar(chtype _ch);

	void makeBox();
	void makeLine();

	void moveCursorPosition(Position _position);
	void moveCursorUp();
	void moveCursorDown();
	void moveCursorLeft();
	void moveCursorRight();

	Position getCursorPosition() const;
	Position getAnchorPosition() const;

	Palette& getPalette();
	Palette getPalette() const;

	void setKeybinds(std::vector<Keybind>* _keybinds);

	Mode currentMode() const;
	void setMode(const Mode& _mode);

	void toggleControls();

	void yank();
	Buffer& getYankedBuffer();
	void paste();

private:
	Buffer* buffer;
	Buffer yankedBuffer;
	Position cursorPosition;
	Position anchorPosition;

	Palette palette;
	Mode mode;
	std::vector<Keybind>* keybinds;

	Position modePosition;
	Position palettePosition;
	Position controlsPosition;
	
	bool showControls;

	void initPalette();
	void displayBorder(Position _topLeft, Position _bottomRight);
	void displayBuffer();
	void displayMode();
	void displayPalette();
	void displayControls();

	void updateAnchorPosition(); //Places anchor at cursor
	bool isAtStartOfLine() const; //If cursor is at start of line
	Position topLeftPosition() const;
	Position bottomRightPosition() const;

	Buffer extractFromSelection() const;
};
