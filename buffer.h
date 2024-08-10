#pragma once
#include <vector>
#include <string>
#include <iostream>

#define DEFAULTCHAR ' '

typedef unsigned long chtype;
typedef std::vector< std::vector<chtype> > BufferVector;
typedef std::vector<chtype> BufferRow;

std::wstring bufferRow2String(const BufferRow& _bufferRow);
BufferRow string2BufferRow(const std::wstring& _string, short _bufferRowSize);

bool isInRangeInclusive(long long _number, long long _min, long long _max);
bool isInRangeExclusive(long long _number, long long _min, long long _max);

class Buffer {
public:
	Buffer();
	Buffer(short _width, short _height, chtype _defaultChar = DEFAULTCHAR);

	void bufferInit();

	void setCharAt(short _row, short _col, chtype _char);
	void setRow(short _row, std::wstring _rowString);
	void setRow(short _row, BufferRow _bufferRow);

	void mergeInto(Buffer& _buffer) const; //merges the content of this Buffer into _buffer.
	void mergeFrom(Buffer& _buffer); //merges the content of _buffer into this Buffer.
	void resize(short _newWidth, short _newHeight);

	const chtype& getCharAt(short _row, short _col) const;
	const BufferRow& getRow(short _row) const;
	std::wstring getRowAsString(short _row) const;

	short getWidth() const;
	short getHeight() const;
	const BufferVector& getData() const;

private:
	short width;
	short height;
	BufferVector data;
	chtype defaultChar;
};
