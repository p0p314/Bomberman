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
	g++ -c src/game/player/Player.cpp
	g++ -c src/game/server/Server.cpp
	g++ Graphics.o Partie.o element.o Player.o Server.o MainMenu.o Bombe.o Personnage.o Ray.o TileMap.o Monde.o main.o  -o app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
	make clean

run:
	./app
