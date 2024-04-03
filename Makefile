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
	g++ -c src/game/player/Debouncer.cpp
	g++ -c src/game/lobby/Lobby.cpp
	g++ -c src/game/GUI/Gui.cpp
	g++ Graphics.o Partie.o element.o Lobby.o Player.o MainMenu.o Bombe.o Debouncer.o Personnage.o Ray.o TileMap.o Monde.o main.o Gui.o  -o app -lsfml-graphics 	-lsfml-audio -lsfml-window -lsfml-system -lsfml-network
	make clean

run:
	./app
