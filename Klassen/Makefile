all: compile link main remove

compile:
	g++ -I src/include -c main.cpp -o main.o
	g++ -I src/include -c Gui.cpp -o Gui.o
	g++ -I src/include -c Engine.cpp -o Engine.o
	g++ -I src/include -c Scanner.cpp -o Scanner.o

link:
	g++ -o main main.o Gui.o Engine.o Scanner.o -L src/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
main:
	main
