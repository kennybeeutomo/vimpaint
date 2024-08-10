output: main.o writer.o painter.o buffer.o inputReader.o palette.o
	g++ -O3 -LC:/Programming/PDCurses-3.9/lib \
		main.o writer.o painter.o buffer.o inputReader.o palette.o \
		-o $(HOME)/bin/kenny/vimpaint.exe -lpdcurses

main.o: main.cpp
	g++ -O3 -IC:/Programming/PDCurses-3.9/include -c main.cpp

writer.o: writer.cpp writer.h
	g++ -O3 -c writer.cpp

painter.o: painter.cpp painter.h
	g++ -O3 -IC:/Programming/PDCurses-3.9/include -c painter.cpp

buffer.o: buffer.cpp buffer.h
	g++ -O3 -c buffer.cpp

inputReader.o: inputReader.cpp
	g++ -O3 -IC:/Programming/PDCurses-3.9/include -c inputReader.cpp

palette.o: palette.cpp palette.h
	g++ -O3 -c palette.cpp

clean:
	rm main.o
	rm writer.o
	rm painter.o
	rm buffer.o
	rm inputReader.o
	rm palette.o
