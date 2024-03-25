compile:./src/main.cpp
	g++ -c ./src/main.cpp
	g++ -c src/game/Partie.cpp
	g++ -c src/game/Graphics/Graphics.cpp 
	g++ -c src/game/elements/element.cpp
	g++ -c src/game/carte/Monde.cpp
	g++ -c src/game/menu/MainMenu.cpp
	g++ -c src/game/carte/TileMap.cpp
	g++ -c src/game/elements/bombe/Bombe.cpp
	g++ -c src/game/elements/bombe/Ray.cpp
	g++ -c src/game/elements/personnage/Personnage.cpp
	g++ Graphics.o Partie.o element.o MainMenu.o Bombe.o Personnage.o Ray.o TileMap.o Monde.o main.o  -o app -lsfml-graphics -lsfml-window -lsfml-system
	make clean

run:
	./app
