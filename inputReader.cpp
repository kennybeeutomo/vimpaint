#include "inputReader.h"

Keybind::Keybind(char _key, std::initializer_list<Mode> _modes, std::string _description, std::function<void()> _action) :
	key(_key),
	modes(_modes),
	description(_description),
	action(_action)
{ }

void Keybind::operator()(char _key, Mode _mode) const {
	if (key == _key) {
		for (const Mode &mode : modes) {
			if (mode == _mode) {
				action();
			}
		}
	}
}

char Keybind::getKey() const {
	return key;
}

std::string Keybind::getDescription() const {
	return description;
}

InputReader::InputReader(Painter* _painter, Writer* _writer) :
	painter(_painter),
	writer(_writer)
{}

void InputReader::inputRead() const {
	char key = getch();
	Mode mode = painter->currentMode();

	for (const Keybind& keybind : keybinds) {
		keybind(key, mode);
	}

	if (mode == Mode::Replace) {
		painter->replaceChar(key);
	}
}

void InputReader::addKeybind(Keybind _keybind) {
	keybinds.push_back(_keybind);
}

std::vector<Keybind>* InputReader::getKeybinds() {
	return &keybinds;
}
