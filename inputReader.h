#pragma once
#include "buffer.h"
#include "painter.h"
#include "writer.h"
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

class Painter;
enum class Mode;

class Keybind {
public:
	Keybind(char _key, std::initializer_list<Mode> _modes, std::string _description, std::function<void()> _action);
	void operator()(char _key, Mode _mode) const; 
	// Executes action if the right key is passed and on the right mode

	char getKey() const;
	std::string getDescription() const;
private:
	char key;
	std::vector<Mode> modes;
	std::string description;
	std::function<void()> action;
};

class InputReader {
public:
	InputReader(Painter* _painter, Writer* _writer);
	void inputRead() const;
	void addKeybind(Keybind _keybind);
	std::vector<Keybind>* getKeybinds();

private:
	Painter* painter;
	Writer* writer;
	std::vector<Keybind> keybinds;
};
