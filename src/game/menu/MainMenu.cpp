//
// Created by raximex on 02/02/24.
//

#include "MainMenu.h"

MainMenu::MainMenu(float width, float height) {
    player = new Player();
    ContextSettings settings;
    settings.antialiasingLevel = 16;
    _windowM = new RenderWindow();
    _windowM->create(sf::VideoMode(width, height), "BOMBERMAN", sf::Style::Default, settings); // Création de la fenêtre
    _windowM->setFramerateLimit(30);

    if (!font.loadFromFile("assets/fonts/BondiBraveDemoRegular.ttf"))
        cout<< "Pas d'image ici \n";

    if (!Menu.loadFromFile("assets/MenuFinale.png"))
        cout << "Pas d'image menu";

    MenuI.setTexture(Menu);

    // Bouton Rejoindre partie

    mainMenu[0].setFont(font);
    mainMenu[0].setFillColor(Color::White);
    mainMenu[0].setString("Rejoindre une Partie");
    mainMenu[0].setCharacterSize(50);
    mainMenu[0].setPosition(170, 290);
    // Bouton Créer une partie
    mainMenu[1].setFont(font);
    mainMenu[1].setFillColor(Color::White);
    mainMenu[1].setString("CREER UNE PARTIE");
    mainMenu[1].setCharacterSize(50);
    mainMenu[1].setPosition(170, 350);
    // Bouton Score
    mainMenu[2].setFont(font);
    mainMenu[2].setFillColor(Color::White);
    mainMenu[2].setString("SCORE");
    mainMenu[2].setCharacterSize(50);
    mainMenu[2].setPosition(170, 410);
    // Bouton Paramètres
    mainMenu[3].setFont(font);
    mainMenu[3].setFillColor(Color::White);
    mainMenu[3].setString("PARAMETRES");
    mainMenu[3].setCharacterSize(50);
    mainMenu[3].setPosition(170, 470);

    MainMenuSelected = 0;
}
MainMenu::~MainMenu() { delete _windowM; delete player; };

void MainMenu::Run()
{
    _exit = false;
    while (!_exit)
    {
        draw();
        HandleEvents();
    }
    _windowM->close();
}

void MainMenu::HandleEvents() // Fonction qui gère les évèvenements du menu;
{
    while (_windowM->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            _exit = true;
        if (sf::Keyboard::isKeyPressed(Keyboard::Up))
        {
            MoveUp();
        }
        if (sf::Keyboard::isKeyPressed(Keyboard::Down))
            MoveDown();
        if (MainMenuSelected == 0 && sf::Keyboard::isKeyPressed(Keyboard::Enter))
        {
            Partie *partie = new Partie(_windowM, player, sf::IpAddress("127.0.0.1")); // Rejoindre partie;
            _exit = !partie->Run();
        }
        if (MainMenuSelected == 1 && sf::Keyboard::isKeyPressed(Keyboard::Enter))
        {
            Partie *partie = new Partie(_windowM, player); // Ici le joueur décide de jouer, ceci déclenche donc la création de la classe Partie.
            _exit = !partie->Run();
                                     // Execution de la méthode Run de la classe Partie qui renvoi un booléen
            std::cout << "sortie du run, _exit est a : "<< _exit << std::endl;
            //delete partie;
            return;
        }
        if (MainMenuSelected == 2)
        {
        }
        if (MainMenuSelected == 3)
        {
        }
    }
}
void MainMenu::draw() // Affichage du menu.
{
    _windowM->draw(MenuI);
    for (int i = 0; i < Max_main_menu; i++)
        _windowM->draw(mainMenu[i]);
    _windowM->display();
}

void MainMenu::setMainMenuSelected(int n)
{
    MainMenuSelected=n;
}

void MainMenu::MoveUp()
{ // Fonction pour la sélection du menu.
    if(MainMenuSelected - 1 >= -1) {
        mainMenu[MainMenuSelected].setFillColor(Color::White);
        MainMenuSelected--;
        if (MainMenuSelected == -1)
            MainMenuSelected = 3;
        mainMenu[MainMenuSelected].setFillColor(Color::Magenta);
    }
}
void MainMenu::MoveDown()
{ // Fonction pour la sélection du menu.
    if(MainMenuSelected +1 <=Max_main_menu)
    {
        mainMenu[MainMenuSelected].setFillColor(Color::White);
        MainMenuSelected++;
        if(MainMenuSelected==4)
        {
            MainMenuSelected=0;
        }
        mainMenu[MainMenuSelected].setFillColor(Color::Magenta);
    }
}
