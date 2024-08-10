#include "inputReader.h"
#include <iostream>

int main(int argc, char** argv) {
	initscr();
	noecho();

	std::string file = "C:/Users/Kenny/projects-old/vimpaint/file.txt";

	Writer writer;
	Buffer* buffer = writer.getBuffer();
	writer.loadFile(file, true);
	Painter painter(buffer);

	bool running = true;
	bool save = false;

	InputReader inputReader(&painter, &writer);
	{
		// Quitting and Saving
		inputReader.addKeybind(Keybind('`', {Mode::Normal}, "Quit without saving", [&]() { running = false; }));
		inputReader.addKeybind(Keybind('~', {Mode::Normal}, "Save and quit", [&]() { running = false; save = true; }));

		// Cursor Movement
		inputReader.addKeybind(Keybind('h', {Mode::Normal, Mode::Draw, Mode::Visual}, "Moves cursor left", [&]() { painter.moveCursorLeft(); }));
		inputReader.addKeybind(Keybind('j', {Mode::Normal, Mode::Draw, Mode::Visual}, "Moves cursor down", [&]() { painter.moveCursorDown(); }));
		inputReader.addKeybind(Keybind('k', {Mode::Normal, Mode::Draw, Mode::Visual}, "Moves cursor up", [&]() { painter.moveCursorUp(); }));
		inputReader.addKeybind(Keybind('l', {Mode::Normal, Mode::Draw, Mode::Visual}, "Moves cursor right", [&]() { painter.moveCursorRight(); }));

		// Normal
		inputReader.addKeybind(Keybind('x', {Mode::Normal, Mode::Draw, Mode::Visual}, "Delete character", [&]() { painter.setCharAtCursor(' '); }));

		// Resize
		inputReader.addKeybind(
			Keybind('h', {Mode::Resize}, "Resizes buffer left", [&]() { buffer->resize(buffer->getWidth() - 1, buffer->getHeight()); }));
		inputReader.addKeybind(
			Keybind('j', {Mode::Resize}, "Resizes buffer down", [&]() { buffer->resize(buffer->getWidth(), buffer->getHeight() + 1); }));
		inputReader.addKeybind(
			Keybind('k', {Mode::Resize}, "Resizes buffer up", [&]() { buffer->resize(buffer->getWidth(), buffer->getHeight() - 1); }));
		inputReader.addKeybind(
			Keybind('l', {Mode::Resize}, "Resizes buffer right", [&]() { buffer->resize(buffer->getWidth() + 1, buffer->getHeight()); }));

		// Visual
		inputReader.addKeybind(
			Keybind(' ', {Mode::Normal, Mode::Visual}, "Puts the selected character at cursor", [&]() { painter.setCharAtCursor(); }));
		inputReader.addKeybind(Keybind( 'b', {Mode::Visual}, "Make a box",
			[&]() {
				if (painter.getAnchorPosition().x != painter.getCursorPosition().x &&
					painter.getAnchorPosition().y != painter.getCursorPosition().y) {
					painter.makeBox();
				} else {
					painter.makeLine();
				}
			}));
		inputReader.addKeybind(Keybind('y', {Mode::Normal, Mode::Visual}, "Yank selection", [&]() { painter.yank(); }));
		inputReader.addKeybind(Keybind('p', {Mode::Normal, Mode::Visual}, "Paste yanked characters", [&]() { painter.paste(); }));

		// Draw
		inputReader.addKeybind(Keybind('p', {Mode::Draw}, "", [&]() { ;}));

		// Modes
		inputReader.addKeybind(Keybind(';', {Mode::Normal}, "Activates Draw mode", [&]() { painter.setMode(Mode::Draw); }));
		inputReader.addKeybind(Keybind(
			27, {Mode::Normal, Mode::Draw, Mode::Visual, Mode::Replace, Mode::Resize}, "Activates Normal mode",
			[&]() { painter.setMode(Mode::Normal); }));
		inputReader.addKeybind(Keybind('m', {Mode::Normal}, "Activates Resize mode", [&]() { painter.setMode(Mode::Resize); }));
		inputReader.addKeybind(Keybind('v', {Mode::Normal}, "Activates Visual mode", [&]() { painter.setMode(Mode::Visual); }));
		inputReader.addKeybind(Keybind('r', {Mode::Normal}, "Activates Replace mode", [&]() { painter.setMode(Mode::Replace); }));

		// Palette Selection
		inputReader.addKeybind(Keybind(
			'Q', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 0 in the palette", [&]() { painter.getPalette().setIndex(0); }));
		inputReader.addKeybind(Keybind(
			'W', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 1 in the palette", [&]() { painter.getPalette().setIndex(1); }));
		inputReader.addKeybind(Keybind(
			'E', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 2 in the palette", [&]() { painter.getPalette().setIndex(2); }));
		inputReader.addKeybind(Keybind(
			'R', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 3 in the palette", [&]() { painter.getPalette().setIndex(3); }));
		inputReader.addKeybind(Keybind(
			'A', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 4 in the palette", [&]() { painter.getPalette().setIndex(4); }));
		inputReader.addKeybind(Keybind(
			'S', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 5 in the palette", [&]() { painter.getPalette().setIndex(5); }));
		inputReader.addKeybind(Keybind(
			'D', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 6 in the palette", [&]() { painter.getPalette().setIndex(6); }));
		inputReader.addKeybind(Keybind(
			'F', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 7 in the palette", [&]() { painter.getPalette().setIndex(7); }));
		inputReader.addKeybind(Keybind(
			'Z', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 8 in the palette", [&]() { painter.getPalette().setIndex(8); }));
		inputReader.addKeybind(Keybind(
			'X', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 9 in the palette", [&]() { painter.getPalette().setIndex(9); }));
		inputReader.addKeybind(Keybind(
			'C', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 10 in the palette", [&]() { painter.getPalette().setIndex(10); }));
		inputReader.addKeybind(Keybind(
			'V', {Mode::Normal, Mode::Draw, Mode::Visual}, "Selects character 11 in the palette", [&]() { painter.getPalette().setIndex(11); }));

		inputReader.addKeybind(Keybind('c', {Mode::Normal}, "Show controls", [&]() { painter.toggleControls(); }));
	}
	painter.setKeybinds(inputReader.getKeybinds());

	while (running) {
		painter.display();
		napms(1);
		inputReader.inputRead();
	}

	if (save) {
		writer.saveAs(file);
	}

	endwin();
}
