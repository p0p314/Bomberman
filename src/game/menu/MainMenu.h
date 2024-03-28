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
   
    MainMenu(float width, float height);
    void MoveUp();
    void MoveDown();
    void setMainMenuSelected(int n);
    int MainMenuPressed()
    {
        return MainMenuSelected;
    }
    bool _exit;
    bool update();
    bool jeu = false;
    void draw();
    void HandleEvents();
    ~MainMenu();
    void Run();

private:
    RenderWindow *_windowM;
    Player *player;
    int MainMenuSelected;
    Font font;

    Text mainMenu[Max_main_menu];
    Texture Menu;
    Sprite MenuI;
    Event event;
};


#endif // BOMBERMAN_MAINMENU_H
