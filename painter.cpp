#include "painter.h"
#include "curses.h"
#include "writer.h"
#include <algorithm>
#include <iostream>
#include <string>

const char* mode2CString(Mode _mode) {
	const char* result;

	switch (_mode) {
		case Mode::Normal:
			result = "NORMAL";
		break;
		case Mode::Draw:
			result = "DRAW";
		break;
		case Mode::Visual:
			result = "VISUAL";
		break;
		case Mode::Replace:
			result = "REPLACE";
		break;
		case Mode::Resize:
			result = "RESIZE";
		break;
		default:
			result = "ERROR";
		break;
	}

	return result;
}

std::string mode2String(Mode _mode) {
	std::string result = mode2CString(_mode);
	return result;
}

Position::Position() :
	x(0),
	y(0)
{ }

Position::Position(short _x, short _y) :
	x(_x),
	y(_y)
{ }

Position Position::operator+(Position _position) const {
	Position result;
	result.x = x + _position.x;
	result.y = y + _position.y;
	return result;
}

Position Position::operator-(Position _position) const {
	Position result;
	result.x = x - _position.x;
	result.y = y - _position.y;
	return result;
}

Position topLeft(Position _position1, Position _position2) {
	Position result;
	result.x = std::min(_position1.x, _position2.x);
	result.y = std::min(_position1.y, _position2.y);
	return result;
}

Position bottomRight(Position _position1, Position _position2) {
	Position result;
	result.x = std::max(_position1.x, _position2.x);
	result.y = std::max(_position1.y, _position2.y);
	return result;
}

Painter::Painter(Buffer* _buffer) :
	buffer(_buffer),
	cursorPosition({0,0}),
	anchorPosition({0,0}),
	palette(Palette(PAINTER_PALETTE_SIZE)),
	mode(Mode::Normal),
	keybinds(nullptr),
	showControls(false)
{
	initPalette();
}

void Painter::display() {
	clear();
	displayBuffer();
	displayMode();
	displayPalette();
	if (showControls) displayControls();
	refresh();
}

chtype Painter::getCharAtCursor() const {
	return buffer->getCharAt(cursorPosition.y, cursorPosition.x);
}

void Painter::setCharAtCursor(chtype _ch) {
	if (currentMode() != Mode::Visual) {
		buffer->setCharAt(cursorPosition.y, cursorPosition.x, _ch);
	} else {
		for (short y = topLeftPosition().y; y <= bottomRightPosition().y; ++y) {
			for (short x = topLeftPosition().x; x <= bottomRightPosition().x; ++x) {
				buffer->setCharAt(y, x, _ch);
			}
		}
	}
}

void Painter::setCharAtCursor() {
	setCharAtCursor(palette.getCharAtIndex());
}

void Painter::replaceChar(chtype _ch) {
	if (_ch >= 32) {
		buffer->setCharAt(cursorPosition.y, cursorPosition.x, _ch);
		if (isAtStartOfLine() && _ch == 32) {
			moveCursorRight();
			updateAnchorPosition();
		} else {
			moveCursorRight();
		}
	} else if (_ch == '\n' || _ch == '\r') {
		moveCursorPosition(anchorPosition);
		moveCursorDown();
		updateAnchorPosition();
	} else if (_ch == '\b') {
		moveCursorLeft();
		buffer->setCharAt(cursorPosition.y, cursorPosition.x, DEFAULTCHAR);
		if (cursorPosition.x < anchorPosition.x) {
			updateAnchorPosition();
		}
	} else if (_ch == '\t') {
		for (int i = 0; i < 2; ++i) {
			replaceChar(' ');
		}
	}

	if (getCharAtCursor() != ' ') {
		replaceChar('\n');
	}
}

void Painter::makeBox() {
	Position tl = topLeft(anchorPosition, cursorPosition);
	Position br = bottomRight(anchorPosition, cursorPosition);

	for (int i = tl.x; i <= br.x; ++i) { buffer->setCharAt(tl.y, i, PAINTER_HORIZONTAL_CHARACTER); }
	for (int i = tl.x; i <= br.x; ++i) { buffer->setCharAt(br.y, i, PAINTER_HORIZONTAL_CHARACTER); }
	for (int i = tl.y; i <= br.y; ++i) { buffer->setCharAt(i, tl.x, PAINTER_VERTICAL_CHARACTER); }
	for (int i = tl.y; i <= br.y; ++i) { buffer->setCharAt(i, br.x, PAINTER_VERTICAL_CHARACTER); }

	buffer->setCharAt(tl.y, tl.x, PAINTER_TOPLEFT_CHARACTER);
	buffer->setCharAt(tl.y, br.x, PAINTER_TOPRIGHT_CHARACTER);
	buffer->setCharAt(br.y, tl.x, PAINTER_BOTTOMLEFT_CHARACTER);
	buffer->setCharAt(br.y, br.x, PAINTER_BOTTOMRIGHT_CHARACTER);
}

void Painter::makeLine() {
	Position tl = topLeft(anchorPosition, cursorPosition);
	Position br = bottomRight(anchorPosition, cursorPosition);

	if (tl.x == br.x) {
		for (int i = tl.y; i <= br.y; ++i) {
			buffer->setCharAt(i, tl.x, PAINTER_VERTICAL_CHARACTER);
		}
	} else if (tl.y == br.y) {
		for (int i = tl.x; i <= br.x; ++i) {
			buffer->setCharAt(tl.y, i, PAINTER_HORIZONTAL_CHARACTER);
		}
	} else {
		return;
	}
}

void Painter::moveCursorPosition(Position _position) {
	if (isInRangeExclusive(_position.x, 0, buffer->getWidth()) && isInRangeExclusive(_position.y, 0, buffer->getHeight())) {
		if (currentMode() == Mode::Draw) setCharAtCursor();

		if (currentMode() != Mode::Visual && currentMode() != Mode::Replace) {
			anchorPosition = _position;
		}

		cursorPosition = _position;

		if (currentMode() == Mode::Draw) setCharAtCursor();
	}
}

void Painter::moveCursorUp() {
	moveCursorPosition(cursorPosition + Position(0, -1));
}

void Painter::moveCursorDown() {
	moveCursorPosition(cursorPosition + Position(0, 1));
}

void Painter::moveCursorLeft() {
	moveCursorPosition(cursorPosition + Position(-1, 0));
}

void Painter::moveCursorRight() {
	moveCursorPosition(cursorPosition + Position(1, 0));
}

Position Painter::getCursorPosition() const {
	return cursorPosition;
}
Position Painter::getAnchorPosition() const {
	return anchorPosition;
}

Palette& Painter::getPalette() {
	return palette;
}

Palette Painter::getPalette() const {
	return palette;
}

void Painter::setKeybinds(std::vector<Keybind>* _keybinds) {
	keybinds = _keybinds;
}

Mode Painter::currentMode() const {
	return mode;
}

void Painter::setMode(const Mode& _mode) {
	mode = _mode;

	switch (currentMode()) {
		case Mode::Draw:
			setCharAtCursor();
			break;

		case Mode::Normal:
			anchorPosition = cursorPosition;
			break;

		case Mode::Resize:
			moveCursorPosition({0,0});
			break;

		default:
			break;
	}
}

void Painter::toggleControls() {
	showControls = !showControls;
}

void Painter::yank() {
	yankedBuffer = extractFromSelection();
}

Buffer& Painter::getYankedBuffer() {
	return yankedBuffer;
}

void Painter::paste() {
	for (short n = 0; n < bottomRightPosition().y - topLeftPosition().y + 1; ++n) {
		for (short i = 0; i < yankedBuffer.getHeight(); ++i) {
			for (short j = 0; j < yankedBuffer.getWidth(); ++j) {
				buffer->setCharAt(i + topLeftPosition().y + n, j + topLeftPosition().x, yankedBuffer.getCharAt(i, j));
			}
		}
	}
}

//PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE 
void Painter::initPalette() {
	std::array<int, PAINTER_PALETTE_SIZE> defaultPalette = {
		0x2591, 0x2592, 0x2593, 0x2588,
		PAINTER_HORIZONTAL_CHARACTER, PAINTER_VERTICAL_CHARACTER, PAINTER_TOPLEFT_CHARACTER, PAINTER_TOPRIGHT_CHARACTER,
		PAINTER_BOTTOMLEFT_CHARACTER, PAINTER_BOTTOMRIGHT_CHARACTER, '.', ' '
	};
	for (int i = 0; i < defaultPalette.size(); i++) {
		palette.setCharAtIndex(i, defaultPalette[i]);
	}
}

void Painter::displayBorder(Position _topLeft, Position _bottomRight) {
	mvaddch(_topLeft.y-1, _topLeft.x-1, PAINTER_TOPLEFT_CHARACTER); //top left
	mvaddch(_topLeft.y-1, _bottomRight.x+1, PAINTER_TOPRIGHT_CHARACTER); //top right
	mvaddch(_bottomRight.y+1, _topLeft.x-1, PAINTER_BOTTOMLEFT_CHARACTER); //bottom left
	mvaddch(_bottomRight.y+1, _bottomRight.x+1, PAINTER_BOTTOMRIGHT_CHARACTER); //bottom right

	for (int i = _topLeft.x; i <= _bottomRight.x; ++i) { mvaddch(_topLeft.y-1, i, PAINTER_HORIZONTAL_CHARACTER); } //top
	for (int i = _topLeft.x; i <= _bottomRight.x; ++i) { mvaddch(_bottomRight.y+1, i, PAINTER_HORIZONTAL_CHARACTER); } //bottom
	for (int i = _topLeft.y; i <= _bottomRight.y; ++i) { mvaddch(i, _topLeft.x-1, PAINTER_VERTICAL_CHARACTER); } //left
	for (int i = _topLeft.y; i <= _bottomRight.y; ++i) { mvaddch(i, _bottomRight.x+1, PAINTER_VERTICAL_CHARACTER); } //right
}

void Painter::displayBuffer() {
	for (short y = 0; y < buffer->getHeight(); ++y) {
		for (short x = 0; x < buffer->getWidth(); ++x) {
			mvaddch(y, x, buffer->getCharAt(y, x));
		}
	}

	switch (currentMode()) {
		case Mode::Visual: {

			for (short y = topLeftPosition().y; y <= bottomRightPosition().y; ++y) {
				for (short x = topLeftPosition().x; x <= bottomRightPosition().x; ++x) {
					mvaddch(y, x, buffer->getCharAt(y, x) | A_REVERSE);
				}
			}
			break;
		}

		case Mode::Replace: {
			mvaddch(cursorPosition.y, cursorPosition.x, buffer->getCharAt(cursorPosition.y, cursorPosition.x) | A_UNDERLINE);
			break;
		}

		default: {
			mvaddch(cursorPosition.y, cursorPosition.x, buffer->getCharAt(cursorPosition.y, cursorPosition.x) | A_REVERSE);
			break;
		}
	}

	displayBorder({0, 0}, Position(buffer->getWidth()-1, buffer->getHeight()-1));
}

void Painter::displayMode() {
	modePosition.x = 0;
	modePosition.y = buffer->getHeight();
	const char* modeName = mode2CString(currentMode());
	mvprintw(modePosition.y, modePosition.x, modeName);
}

void Painter::displayPalette() {
	palettePosition.x = 2;
	palettePosition.y = buffer->getHeight()+3;
	short paletteColumns = 4;
	short paletteRows = PAINTER_PALETTE_SIZE/paletteColumns;

	for (int i = 0; i < paletteRows; ++i) {
		for (int j = 0; j < paletteColumns; ++j) {
			short paletteIndex = i*4 + j;
			short x = palettePosition.x + j*2;
			short y = palettePosition.y + i*2;

			if (getPalette().getIndex() == paletteIndex) {
				mvaddch(y-1, x, PAINTER_HORIZONTAL_CHARACTER);
				mvaddch(y-1, x+1, PAINTER_TOPRIGHT_CHARACTER);
				mvaddch(y, x+1, PAINTER_VERTICAL_CHARACTER);
				mvaddch(y+1, x+1, PAINTER_BOTTOMRIGHT_CHARACTER);
				mvaddch(y+1, x, PAINTER_HORIZONTAL_CHARACTER);
				mvaddch(y+1, x-1, PAINTER_BOTTOMLEFT_CHARACTER);
				mvaddch(y, x-1, PAINTER_VERTICAL_CHARACTER);
				mvaddch(y-1, x-1, PAINTER_TOPLEFT_CHARACTER);
			}

			mvaddch(y, x, getPalette().getPaletteData().at(paletteIndex));
		}
	}

	displayBorder(palettePosition + Position(-1, -1), palettePosition + Position(7,5));
}

void Painter::displayControls() {
	controlsPosition.x = buffer->getWidth()+2;
	controlsPosition.y = 1;
	int i = 0;

	for (const Keybind& keybind : *keybinds) {
		mvprintw(controlsPosition.y + i++, controlsPosition.x, "%c: %s", keybind.getKey(), keybind.getDescription().c_str());
	}

	displayBorder(controlsPosition, controlsPosition + Position(40, keybinds->size()-1));
}

void Painter::updateAnchorPosition() {
	anchorPosition = cursorPosition;
}

bool Painter::isAtStartOfLine() const {
	return cursorPosition.x == anchorPosition.x;
}

Position Painter::topLeftPosition() const {
	return topLeft(anchorPosition, cursorPosition);
}

Position Painter::bottomRightPosition() const {
	return bottomRight(anchorPosition, cursorPosition);
}

Buffer Painter::extractFromSelection() const {
	short width = bottomRightPosition().x - topLeftPosition().x + 1;
	short height = bottomRightPosition().y - topLeftPosition().y + 1;
	Buffer result(width, height, DEFAULTCHAR);

	for (short i = 0; i < height; ++i) {
		for (short j = 0; j < width; ++j) {
			result.setCharAt(i, j, buffer->getCharAt(i + topLeftPosition().y, j + topLeftPosition().x));
		}
	}

	return result;
}

