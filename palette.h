#pragma once
#include "buffer.h"

class Palette {
public:
	Palette();
	Palette(std::initializer_list<chtype> _chars);
	Palette(short _size);

	void setIndex(short _index);
	short getIndex() const;

	void setCharAtIndex(short _index, chtype _char = 0);
	void setCharAtIndex(chtype _char = 0);
	chtype getCharAtIndex(short _index) const;
	chtype getCharAtIndex() const;

	const std::vector<chtype>& getPaletteData() const;

private:
	std::vector<chtype> palette;
	short index;
};
