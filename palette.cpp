#include "palette.h"

Palette::Palette() :
	index(0)
{ }

Palette::Palette(std::initializer_list<chtype> _chars) :
	Palette()
{
	palette = _chars;
}

Palette::Palette(short _size) :
	Palette()
{
	palette = std::vector<chtype>(_size);
}

void Palette::setIndex(short _index) {
	index = _index;
}

short Palette::getIndex() const {
	return index;
}

void Palette::setCharAtIndex(short _index, chtype _char) {
	if (_char != 0) {
		palette.at(_index) = _char;
	}
}

void Palette::setCharAtIndex(chtype _char) {
	setCharAtIndex(index, _char);
}

chtype Palette::getCharAtIndex(short _index) const {
	return palette.at(_index);
}

chtype Palette::getCharAtIndex() const {
	return getCharAtIndex(index);
}

const std::vector<chtype>& Palette::getPaletteData() const {
	return palette;
}
