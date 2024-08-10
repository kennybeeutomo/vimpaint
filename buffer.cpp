#include "buffer.h"
#include <algorithm>

Buffer::Buffer() :
	width(0),
	height(0)
{
	bufferInit();
}

Buffer::Buffer(short _width, short _height, chtype _defaultChar) :
	width(_width),
	height(_height),
	defaultChar(_defaultChar)
{
	bufferInit();
}

void Buffer::bufferInit() {
	data = BufferVector(height, BufferRow(width, defaultChar));
}

void Buffer::setCharAt(short _row, short _col, chtype _char) {
	if (isInRangeExclusive(_row, 0, height) && isInRangeExclusive(_col, 0, width)) {
		data.at(_row).at(_col) = _char;
	}
}

void Buffer::setRow(short _row, std::wstring _rowString) {
	if (isInRangeExclusive(_row, 0, height)) {
		BufferRow bufferRowFromString = string2BufferRow(_rowString, getWidth());
		data.at(_row) = bufferRowFromString;
	}
}

void Buffer::setRow(short _row, BufferRow _bufferRow) {
	if (isInRangeExclusive(_row, 0, height)) {
		data.at(_row) = _bufferRow;
	}
}

void Buffer::mergeInto(Buffer& _buffer) const {
	short thisBufferWidth = getWidth();
	short thisBufferHeight = getHeight();
	short otherBufferWidth = _buffer.getWidth();
	short otherBufferHeight = _buffer.getHeight();

	short shorterWidth = std::min(thisBufferWidth, otherBufferWidth);
	short shorterHeight = std::min(thisBufferHeight, otherBufferHeight);

	for (int row = 0; row < shorterHeight; ++row) {
		for (int col = 0; col < shorterWidth; ++col) {
			const chtype& thisBufferChar = getCharAt(row, col);
			_buffer.setCharAt(row, col, thisBufferChar);
		}
	}
}

void Buffer::mergeFrom(Buffer& _buffer) {
	_buffer.mergeInto(*this);
}

void Buffer::resize(short _newWidth, short _newHeight) {
	if (_newWidth < 1 || _newHeight < 1) return;
	Buffer temp(_newWidth, _newHeight, defaultChar);
	mergeInto(temp);
	*this = temp;
}

const chtype& Buffer::getCharAt(short _row, short _col) const {
	return data.at(_row).at(_col);
}

const BufferRow &Buffer::getRow(short _row) const {
	return data.at(_row);
}

std::wstring Buffer::getRowAsString(short _row) const {
	const BufferRow& row = getRow(_row);
	std::wstring result = bufferRow2String(row);
	return result;
}

short Buffer::getWidth() const {
	return width;
}

short Buffer::getHeight() const {
	return height;
}

const BufferVector& Buffer::getData() const {
	return data;
}

std::wstring bufferRow2String(const BufferRow& _bufferRow) {
	std::wstring result;
	for (const chtype& c : _bufferRow)
	result.push_back(c);
	return result;
}

BufferRow string2BufferRow(const std::wstring& _string, short _bufferRowSize) {
	BufferRow result(_bufferRowSize, DEFAULTCHAR);
	for (int i = 0; i < _bufferRowSize; i++)
		result.at(i) = _string.at(i);
	return result;
}

bool isInRangeInclusive(long long _number, long long _min, long long _max) {
	return (_number >= _min && _number <= _max);
}

bool isInRangeExclusive(long long _number, long long _min, long long _max) {
	return (_number >= _min && _number < _max);
}
