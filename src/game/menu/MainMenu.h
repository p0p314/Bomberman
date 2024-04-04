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


class MainMenu
{
public:
    MainMenu(float width, float height); /*Constructeur du menu
                                          *
                                          *
                                          */

    void MoveUp();
    /*Fonction pour se déplacer dans le menu grâce aux flèches directionnelle
     *
     *
     *
     */
    void MoveDown();

    void setMainMenuSelected(int n); /*Setter qui change
                                      * la valeur de la variable MainMenuSelected
                                      *
                                      */

    int MainMenuPressed()
    {
        return MainMenuSelected;
    }

    bool _exit;
    bool update();
    bool jeu = false;

    void draw(); /*Fonction d'affichage du menu.
                  *
                  *
                  */

    void HandleEvents(); /*Fonction qui fait appel aux
                          * fonctions MoveUp et MoveDown cette
                          * fonction gère les évènements sur le menu
                          */

    ~MainMenu(); /*Destructeur
                  *
                  *
                  */

    void Run(); /*Fonction qui fait
                 * tourner le menu Toutes
                 * les fonctions principales sont lancés ici.
                 *
                 */

private:
    RenderWindow *_windowM;
    Player *player;
    int MainMenuSelected = 0;
    Font font;
    
    int Max_main_menu = 4;

    Text mainMenu[4];
    Texture Menu;
    Sprite MenuI;
    Event event;
};


#endif // BOMBERMAN_MAINMENU_H
