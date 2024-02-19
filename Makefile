compile:./src/main.cpp
	g++ -c ./src/main.cpp
	g++ -c ./src/game/menu/MainMenu.cpp
	g++ -c ./src/game/Graphics/Graphics.cpp
	g++ Graphics.o MainMenu.o main.o  -o app -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app