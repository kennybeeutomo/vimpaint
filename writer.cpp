#include "writer.h"
#include <iostream>
#include <string>

Writer::Writer() :
	bufferWidth(0),
	bufferHeight(0),
	bufferDefaultChar(DEFAULTCHAR)
{
	bufferInit();
}

Writer::Writer(short _bufferWidth, short _bufferHeight, chtype _defaultChar) :
	bufferWidth(_bufferWidth),
	bufferHeight(_bufferHeight),
	bufferDefaultChar(_defaultChar)
{
	bufferInit();
}

Buffer* Writer::getBuffer() {
	return &buffer;
}

void Writer::saveAs(std::string _filePath) {
	file = std::wfstream(_filePath, std::ios::out | std::ios::trunc);
	if (!file.good()) exit(1);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	for (int row = 0; row < getBuffer()->getHeight(); ++row) {
		std::wstring bufferRow = bufferRow2String(buffer.getRow(row));
		file << bufferRow << '\n';
	}
}

void Writer::loadFile(std::string _filePath, bool _resizeBufferSizeToFileSize) {
	if (_resizeBufferSizeToFileSize) {
		bufferWidth = getFileColumns(_filePath);
		bufferHeight = getFileRows(_filePath);
	}

	file = std::wfstream(_filePath, std::ios::in);
	if (!file.good()) {
		std::cerr << "Load file error\n";
		exit(1);
	}
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	bufferInit();

	for (int row = 0; row < bufferHeight; ++row) {
		std::wstring line;
		std::getline(file, line);
		for (int i = 0; i < line.size(); ++i) {
			buffer.setCharAt(row, i, line.at(i));
		}
	}
}

//PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE PRIVATE

int Writer::getFileRows(std::string _filePath) {
	file = std::wfstream(_filePath, std::ios::in);
	if (!file.good()) exit(1);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	int rows = -1;
	std::wstring line;
	while (!file.eof()) {
		std::getline(file, line);
		++rows;
	}

	return rows;
}

int Writer::getFileColumns(std::string _filePath) {
	file = std::wfstream(_filePath, std::ios::in);
	if (!file.good()) exit(1);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	unsigned long long maxColumn = 0;
	std::wstring line;
	while (!file.eof()) {
		std::getline(file, line);
		int lineLength = line.size();
		if (lineLength > maxColumn) {
			maxColumn = lineLength;
		}
	}
	return maxColumn;
}

void Writer::bufferInit() {
	buffer = Buffer(bufferWidth, bufferHeight, bufferDefaultChar);
}
