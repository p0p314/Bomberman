//
// Created by raximex on 02/02/24.
//
#ifndef BOMBERMAN_MAINMENU_H
#define BOMBERMAN_MAINMENU_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Partie.hpp"
using namespace std;
using namespace sf;

#define Max_main_menu 4

class MainMenu
{
public:
    Text mainMenu[Max_main_menu];
    Texture Menu;
    Sprite MenuI;
    Event event;
    MainMenu(float width, float height);
    void MoveUp();
    void MoveDown();
    void setMainMenuSelected(int n);
    int MainMenuPressed()
    {
        return MainMenuSelected;
    }
    int _exit;
    bool update();
    bool jeu = false;
    void draw();
    void HandleEvents();
    ~MainMenu();
    void Run();
    RenderWindow *_windowM;

private:
    Player *joueur;
    int MainMenuSelected;
    Font font;
};

#endif // BOMBERMAN_MAINMENU_H
