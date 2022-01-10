all: key

OBJECTS = MatrixKeypad.o Keypad.o Key.o socket.o http_request.o

LIBS = -lwiringPi -lwiringPiDev -lcurl

key: MatrixKeypad.cpp Keypad.cpp Key.cpp socket.cpp functions.cpp
	g++ MatrixKeypad.cpp Keypad.cpp Key.cpp http_request.cpp functions.cpp -o key $(LIBS)

test: lcd_test.cpp
	g++ lcd_test.cpp -o test $(LIBS)
