all: key

key: MatrixKeypad.cpp Keypad.cpp Key.cpp socket.cpp
	g++ MatrixKeypad.cpp Keypad.cpp Key.cpp socket.cpp -o key -lwiringPi
