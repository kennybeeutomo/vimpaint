#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <codecvt>
#include "buffer.h"

#define DEFAULTCHAR ' '

class Writer {
public:
	Writer();
	Writer(short _bufferWidth, short _bufferHeight, chtype _defaultChar = DEFAULTCHAR);

	Buffer* getBuffer();
	void saveAs(std::string _filePath);
	void loadFile(std::string _filePath, bool _resizeBufferSizeToFileSize = true);

private:
	short bufferWidth;
	short bufferHeight;
	Buffer buffer;
	chtype bufferDefaultChar;
	std::wfstream file;

	void bufferInit();
	int getFileRows(std::string _filePath);
	int getFileColumns(std::string _filePath);
};
