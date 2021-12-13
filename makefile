all: key

OBJECTS = MatrixKeypad.o Keypad.o Key.cpp socket.cpp

key: MatrixKeypad.cpp Keypad.cpp Key.cpp socket.cpp
	g++ MatrixKeypad.cpp Keypad.cpp Key.cpp socket.cpp -o key -lwiringPi
